--- transupp.c.orig	Sat Aug  9 20:15:26 1997
+++ transupp.c	Fri Jun  4 15:25:29 2004
@@ -1,7 +1,7 @@
 /*
  * transupp.c
  *
- * Copyright (C) 1997, Thomas G. Lane.
+ * Copyright (C) 1997-2001, Thomas G. Lane.
  * This file is part of the Independent JPEG Group's software.
  * For conditions of distribution and use, see the accompanying README file.
  *
@@ -20,6 +20,7 @@
 #include "jinclude.h"
 #include "jpeglib.h"
 #include "transupp.h"		/* My own external interface */
+#include <ctype.h>		/* to declare isdigit() */
 
 
 #if TRANSFORMS_SUPPORTED
@@ -28,7 +29,8 @@
  * Lossless image transformation routines.  These routines work on DCT
  * coefficient arrays and thus do not require any lossy decompression
  * or recompression of the image.
- * Thanks to Guido Vollbeding for the initial design and code of this feature.
+ * Thanks to Guido Vollbeding for the initial design and code of this feature,
+ * and to Ben Jackson for introducing the cropping feature.
  *
  * Horizontal flipping is done in-place, using a single top-to-bottom
  * pass through the virtual source array.  It will thus be much the
@@ -42,6 +44,13 @@
  * arrays for most of the transforms.  That could result in much thrashing
  * if the image is larger than main memory.
  *
+ * If cropping or trimming is involved, the destination arrays may be smaller
+ * than the source arrays.  Note it is not possible to do horizontal flip
+ * in-place when a nonzero Y crop offset is specified, since we'd have to move
+ * data from one block row to another but the virtual array manager doesn't
+ * guarantee we can touch more than one row at a time.  So in that case,
+ * we have to use a separate destination array.
+ *
  * Some notes about the operating environment of the individual transform
  * routines:
  * 1. Both the source and destination virtual arrays are allocated from the
@@ -54,20 +63,366 @@
  *    and we may as well take that as the effective iMCU size.
  * 4. When "trim" is in effect, the destination's dimensions will be the
  *    trimmed values but the source's will be untrimmed.
- * 5. All the routines assume that the source and destination buffers are
+ * 5. When "crop" is in effect, the destination's dimensions will be the
+ *    cropped values but the source's will be uncropped.  Each transform
+ *    routine is responsible for picking up source data starting at the
+ *    correct X and Y offset for the crop region.  (The X and Y offsets
+ *    passed to the transform routines are measured in iMCU blocks of the
+ *    destination.)
+ * 6. All the routines assume that the source and destination buffers are
  *    padded out to a full iMCU boundary.  This is true, although for the
  *    source buffer it is an undocumented property of jdcoefct.c.
- * Notes 2,3,4 boil down to this: generally we should use the destination's
- * dimensions and ignore the source's.
  */
 
 
+/* Drop code may be used with or without virtual memory adaptation code.
+ * This code has some dependencies on internal library behavior, so you
+ * may choose to disable it.  For example, it doesn't make a difference
+ * if you only use jmemnobs anyway.
+ */
+#ifndef DROP_REQUEST_FROM_SRC
+#define DROP_REQUEST_FROM_SRC 1		/* 0 disables adaptation */
+#endif
+
+
+#if DROP_REQUEST_FROM_SRC
+/* Force jpeg_read_coefficients to request
+ * the virtual coefficient arrays from
+ * the source decompression object.
+ */
+METHODDEF(jvirt_barray_ptr)
+drop_request_virt_barray (j_common_ptr cinfo, int pool_id, boolean pre_zero,
+			  JDIMENSION blocksperrow, JDIMENSION numrows,
+			  JDIMENSION maxaccess)
+{
+  j_decompress_ptr srcinfo = (j_decompress_ptr) cinfo->client_data;
+
+  return (*srcinfo->mem->request_virt_barray)
+	  ((j_common_ptr) srcinfo, pool_id, pre_zero,
+	   blocksperrow, numrows, maxaccess);
+}
+
+
+/* Force jpeg_read_coefficients to return
+ * after requesting and before accessing
+ * the virtual coefficient arrays.
+ */
+METHODDEF(int)
+drop_consume_input (j_decompress_ptr cinfo)
+{
+  return JPEG_SUSPENDED;
+}
+
+
+METHODDEF(void)
+drop_start_input_pass (j_decompress_ptr cinfo)
+{
+  cinfo->inputctl->consume_input = drop_consume_input;
+}
+
+
 LOCAL(void)
-do_flip_h (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
-	   jvirt_barray_ptr *src_coef_arrays)
-/* Horizontal flip; done in-place, so no separate dest array is required */
+drop_request_from_src (j_decompress_ptr dropinfo, j_decompress_ptr srcinfo)
+{
+  void *save_client_data;
+  JMETHOD(jvirt_barray_ptr, save_request_virt_barray,
+	  (j_common_ptr cinfo, int pool_id, boolean pre_zero,
+	   JDIMENSION blocksperrow, JDIMENSION numrows, JDIMENSION maxaccess));
+  JMETHOD(void, save_start_input_pass, (j_decompress_ptr cinfo));
+
+  /* Set custom method pointers, save original pointers */
+  save_client_data = dropinfo->client_data;
+  dropinfo->client_data = (void *) srcinfo;
+  save_request_virt_barray = dropinfo->mem->request_virt_barray;
+  dropinfo->mem->request_virt_barray = drop_request_virt_barray;
+  save_start_input_pass = dropinfo->inputctl->start_input_pass;
+  dropinfo->inputctl->start_input_pass = drop_start_input_pass;
+
+  /* Execute only initialization part.
+   * Requested coefficient arrays will be realized later by the srcinfo object.
+   * Next call to the same function will then do the actual data reading.
+   * NB: since we request the coefficient arrays from another object,
+   * the inherent realization call is effectively a no-op.
+   */
+  (void) jpeg_read_coefficients(dropinfo);
+
+  /* Reset method pointers */
+  dropinfo->client_data = save_client_data;
+  dropinfo->mem->request_virt_barray = save_request_virt_barray;
+  dropinfo->inputctl->start_input_pass = save_start_input_pass;
+  /* Do input initialization for first scan now,
+   * which also resets the consume_input method.
+   */
+  (*save_start_input_pass)(dropinfo);
+}
+#endif /* DROP_REQUEST_FROM_SRC */
+
+
+LOCAL(void)
+dequant_comp (j_decompress_ptr cinfo, jpeg_component_info *compptr,
+	      jvirt_barray_ptr coef_array, JQUANT_TBL *qtblptr1)
+{
+  JDIMENSION blk_x, blk_y;
+  int offset_y, k;
+  JQUANT_TBL *qtblptr;
+  JBLOCKARRAY buffer;
+  JBLOCKROW block;
+  JCOEFPTR ptr;
+
+  qtblptr = compptr->quant_table;
+  for (blk_y = 0; blk_y < compptr->height_in_blocks;
+       blk_y += compptr->v_samp_factor) {
+    buffer = (*cinfo->mem->access_virt_barray)
+      ((j_common_ptr) cinfo, coef_array, blk_y,
+       (JDIMENSION) compptr->v_samp_factor, TRUE);
+    for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+      block = buffer[offset_y];
+      for (blk_x = 0; blk_x < compptr->width_in_blocks; blk_x++) {
+	ptr = block[blk_x];
+	for (k = 0; k < DCTSIZE2; k++)
+	  if (qtblptr->quantval[k] != qtblptr1->quantval[k])
+	    ptr[k] *= qtblptr->quantval[k] / qtblptr1->quantval[k];
+      }
+    }
+  }
+}
+
+
+LOCAL(void)
+requant_comp (j_decompress_ptr cinfo, jpeg_component_info *compptr,
+	      jvirt_barray_ptr coef_array, JQUANT_TBL *qtblptr1)
+{
+  JDIMENSION blk_x, blk_y;
+  int offset_y, k;
+  JQUANT_TBL *qtblptr;
+  JBLOCKARRAY buffer;
+  JBLOCKROW block;
+  JCOEFPTR ptr;
+  JCOEF temp, qval;
+
+  qtblptr = compptr->quant_table;
+  for (blk_y = 0; blk_y < compptr->height_in_blocks;
+       blk_y += compptr->v_samp_factor) {
+    buffer = (*cinfo->mem->access_virt_barray)
+      ((j_common_ptr) cinfo, coef_array, blk_y,
+       (JDIMENSION) compptr->v_samp_factor, TRUE);
+    for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+      block = buffer[offset_y];
+      for (blk_x = 0; blk_x < compptr->width_in_blocks; blk_x++) {
+	ptr = block[blk_x];
+	for (k = 0; k < DCTSIZE2; k++) {
+	  temp = qtblptr->quantval[k];
+	  qval = qtblptr1->quantval[k];
+	  if (temp != qval) {
+	    temp *= ptr[k];
+	    /* The following quantization code is a copy from jcdctmgr.c */
+#ifdef FAST_DIVIDE
+#define DIVIDE_BY(a,b)	a /= b
+#else
+#define DIVIDE_BY(a,b)	if (a >= b) a /= b; else a = 0
+#endif
+	    if (temp < 0) {
+	      temp = -temp;
+	      temp += qval>>1;	/* for rounding */
+	      DIVIDE_BY(temp, qval);
+	      temp = -temp;
+	    } else {
+	      temp += qval>>1;	/* for rounding */
+	      DIVIDE_BY(temp, qval);
+	    }
+	    ptr[k] = temp;
+	  }
+	}
+      }
+    }
+  }
+}
+
+
+/* Calculate largest common denominator with Euklid's algorithm.
+ */
+LOCAL(JCOEF)
+largest_common_denominator(JCOEF a, JCOEF b)
+{
+  JCOEF c;
+
+  do {
+    c = a % b;
+    a = b;
+    b = c;
+  } while (c);
+
+  return a;
+}
+
+
+LOCAL(void)
+adjust_quant(j_decompress_ptr srcinfo, jvirt_barray_ptr *src_coef_arrays,
+	     j_decompress_ptr dropinfo, jvirt_barray_ptr *drop_coef_arrays,
+	     boolean trim, j_compress_ptr dstinfo)
+{
+  jpeg_component_info *compptr1, *compptr2;
+  JQUANT_TBL *qtblptr1, *qtblptr2, *qtblptr3;
+  int ci, k;
+
+  for (ci = 0; ci < dstinfo->num_components &&
+	       ci < dropinfo->num_components; ci++) {
+    compptr1 = srcinfo->comp_info + ci;
+    compptr2 = dropinfo->comp_info + ci;
+    qtblptr1 = compptr1->quant_table;
+    qtblptr2 = compptr2->quant_table;
+    for (k = 0; k < DCTSIZE2; k++) {
+      if (qtblptr1->quantval[k] != qtblptr2->quantval[k]) {
+	if (trim)
+	  requant_comp(dropinfo, compptr2, drop_coef_arrays[ci], qtblptr1);
+	else {
+	  qtblptr3 = dstinfo->quant_tbl_ptrs[compptr1->quant_tbl_no];
+	  for (k = 0; k < DCTSIZE2; k++)
+	    if (qtblptr1->quantval[k] != qtblptr2->quantval[k])
+	      qtblptr3->quantval[k] = largest_common_denominator
+		(qtblptr1->quantval[k], qtblptr2->quantval[k]);
+	  dequant_comp(srcinfo, compptr1, src_coef_arrays[ci], qtblptr3);
+	  dequant_comp(dropinfo, compptr2, drop_coef_arrays[ci], qtblptr3);
+	}
+	break;
+      }
+    }
+  }
+}
+
+
+LOCAL(void)
+do_drop (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	 JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
+	 jvirt_barray_ptr *src_coef_arrays,
+	 j_decompress_ptr dropinfo, jvirt_barray_ptr *drop_coef_arrays,
+	 JDIMENSION drop_width, JDIMENSION drop_height)
+/* Drop.  If the dropinfo component number is smaller than the destination's,
+ * we fill in the remaining components with zero.  This provides the feature
+ * of dropping grayscale into (arbitrarily sampled) color images.
+ */
+{
+  JDIMENSION comp_width, comp_height;
+  JDIMENSION blk_y, x_drop_blocks, y_drop_blocks;
+  int ci, offset_y;
+  JBLOCKARRAY src_buffer, dst_buffer;
+  jpeg_component_info *compptr;
+
+  for (ci = 0; ci < dstinfo->num_components; ci++) {
+    compptr = dstinfo->comp_info + ci;
+    comp_width = drop_width * compptr->h_samp_factor;
+    comp_height = drop_height * compptr->v_samp_factor;
+    x_drop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_drop_blocks = y_crop_offset * compptr->v_samp_factor;
+    for (blk_y = 0; blk_y < comp_height; blk_y += compptr->v_samp_factor) {
+      dst_buffer = (*srcinfo->mem->access_virt_barray)
+	((j_common_ptr) srcinfo, src_coef_arrays[ci], blk_y + y_drop_blocks,
+	 (JDIMENSION) compptr->v_samp_factor, TRUE);
+      if (ci < dropinfo->num_components) {
+	src_buffer = (*srcinfo->mem->access_virt_barray)
+	  ((j_common_ptr) srcinfo, drop_coef_arrays[ci], blk_y,
+	   (JDIMENSION) compptr->v_samp_factor, FALSE);
+	for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+	  jcopy_block_row(src_buffer[offset_y],
+			  dst_buffer[offset_y] + x_drop_blocks,
+			  comp_width);
+	}
+      } else {
+	for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+	  jzero_far(dst_buffer[offset_y] + x_drop_blocks,
+		    comp_width * SIZEOF(JBLOCK));
+	} 	
+      }
+    }
+  }
+}
+
+
+LOCAL(void)
+do_crop (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	 JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
+	 jvirt_barray_ptr *src_coef_arrays,
+	 jvirt_barray_ptr *dst_coef_arrays)
+/* Crop.  This is only used when no rotate/flip is requested with the crop.
+ * Extension: If the destination size is larger than the source, we fill in
+ * the extra area with zero (neutral gray).  Note we also have to zero partial
+ * iMCUs at the right and bottom edge of the source image area in this case.
+ */
+{
+  JDIMENSION MCU_cols, MCU_rows, comp_width, comp_height;
+  JDIMENSION dst_blk_y, x_crop_blocks, y_crop_blocks;
+  int ci, offset_y;
+  JBLOCKARRAY src_buffer, dst_buffer;
+  jpeg_component_info *compptr;
+
+  MCU_cols = srcinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
+  MCU_rows = srcinfo->image_height / (dstinfo->max_v_samp_factor * DCTSIZE);
+
+  for (ci = 0; ci < dstinfo->num_components; ci++) {
+    compptr = dstinfo->comp_info + ci;
+    comp_width = MCU_cols * compptr->h_samp_factor;
+    comp_height = MCU_rows * compptr->v_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
+    for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
+	 dst_blk_y += compptr->v_samp_factor) {
+      dst_buffer = (*srcinfo->mem->access_virt_barray)
+	((j_common_ptr) srcinfo, dst_coef_arrays[ci], dst_blk_y,
+	 (JDIMENSION) compptr->v_samp_factor, TRUE);
+      if (dstinfo->image_height > srcinfo->image_height) {
+	if (dst_blk_y < y_crop_blocks ||
+	    dst_blk_y >= comp_height + y_crop_blocks) {
+	  for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+	    jzero_far(dst_buffer[offset_y],
+		      compptr->width_in_blocks * SIZEOF(JBLOCK));
+	  }
+	  continue;
+	}
+	src_buffer = (*srcinfo->mem->access_virt_barray)
+	  ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	   dst_blk_y - y_crop_blocks,
+	   (JDIMENSION) compptr->v_samp_factor, FALSE);
+      } else {
+	src_buffer = (*srcinfo->mem->access_virt_barray)
+	  ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	   dst_blk_y + y_crop_blocks,
+	   (JDIMENSION) compptr->v_samp_factor, FALSE);
+      }
+      for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+	if (dstinfo->image_width > srcinfo->image_width) {
+	  if (x_crop_blocks > 0) {
+	    jzero_far(dst_buffer[offset_y],
+		      x_crop_blocks * SIZEOF(JBLOCK));
+	  }
+	  jcopy_block_row(src_buffer[offset_y],
+			  dst_buffer[offset_y] + x_crop_blocks,
+			  comp_width);
+	  if (compptr->width_in_blocks > comp_width + x_crop_blocks) {
+	    jzero_far(dst_buffer[offset_y] +
+			comp_width + x_crop_blocks,
+		      (compptr->width_in_blocks -
+			comp_width - x_crop_blocks) * SIZEOF(JBLOCK));
+	  }
+	} else {
+	  jcopy_block_row(src_buffer[offset_y] + x_crop_blocks,
+			  dst_buffer[offset_y],
+			  compptr->width_in_blocks);
+	}
+      }
+    }
+  }
+}
+
+
+LOCAL(void)
+do_flip_h_no_crop (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+		   JDIMENSION x_crop_offset,
+		   jvirt_barray_ptr *src_coef_arrays)
+/* Horizontal flip; done in-place, so no separate dest array is required.
+ * NB: this only works when y_crop_offset is zero.
+ */
 {
-  JDIMENSION MCU_cols, comp_width, blk_x, blk_y;
+  JDIMENSION MCU_cols, comp_width, blk_x, blk_y, x_crop_blocks;
   int ci, k, offset_y;
   JBLOCKARRAY buffer;
   JCOEFPTR ptr1, ptr2;
@@ -79,17 +434,19 @@
    * mirroring by changing the signs of odd-numbered columns.
    * Partial iMCUs at the right edge are left untouched.
    */
-  MCU_cols = dstinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
+  MCU_cols = srcinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
 
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
     comp_width = MCU_cols * compptr->h_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
     for (blk_y = 0; blk_y < compptr->height_in_blocks;
 	 blk_y += compptr->v_samp_factor) {
       buffer = (*srcinfo->mem->access_virt_barray)
 	((j_common_ptr) srcinfo, src_coef_arrays[ci], blk_y,
 	 (JDIMENSION) compptr->v_samp_factor, TRUE);
       for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+	/* Do the mirroring */
 	for (blk_x = 0; blk_x * 2 < comp_width; blk_x++) {
 	  ptr1 = buffer[offset_y][blk_x];
 	  ptr2 = buffer[offset_y][comp_width - blk_x - 1];
@@ -105,6 +462,79 @@
 	    *ptr2++ = -temp1;
 	  }
 	}
+	if (x_crop_blocks > 0) {
+	  /* Now left-justify the portion of the data to be kept.
+	   * We can't use a single jcopy_block_row() call because that routine
+	   * depends on memcpy(), whose behavior is unspecified for overlapping
+	   * source and destination areas.  Sigh.
+	   */
+	  for (blk_x = 0; blk_x < compptr->width_in_blocks; blk_x++) {
+	    jcopy_block_row(buffer[offset_y] + blk_x + x_crop_blocks,
+			    buffer[offset_y] + blk_x,
+			    (JDIMENSION) 1);
+	  }
+	}
+      }
+    }
+  }
+}
+
+
+LOCAL(void)
+do_flip_h (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	   JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
+	   jvirt_barray_ptr *src_coef_arrays,
+	   jvirt_barray_ptr *dst_coef_arrays)
+/* Horizontal flip in general cropping case */
+{
+  JDIMENSION MCU_cols, comp_width, dst_blk_x, dst_blk_y;
+  JDIMENSION x_crop_blocks, y_crop_blocks;
+  int ci, k, offset_y;
+  JBLOCKARRAY src_buffer, dst_buffer;
+  JBLOCKROW src_row_ptr, dst_row_ptr;
+  JCOEFPTR src_ptr, dst_ptr;
+  jpeg_component_info *compptr;
+
+  /* Here we must output into a separate array because we can't touch
+   * different rows of a single virtual array simultaneously.  Otherwise,
+   * this is essentially the same as the routine above.
+   */
+  MCU_cols = srcinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
+
+  for (ci = 0; ci < dstinfo->num_components; ci++) {
+    compptr = dstinfo->comp_info + ci;
+    comp_width = MCU_cols * compptr->h_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
+    for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
+	 dst_blk_y += compptr->v_samp_factor) {
+      dst_buffer = (*srcinfo->mem->access_virt_barray)
+	((j_common_ptr) srcinfo, dst_coef_arrays[ci], dst_blk_y,
+	 (JDIMENSION) compptr->v_samp_factor, TRUE);
+      src_buffer = (*srcinfo->mem->access_virt_barray)
+	((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	 dst_blk_y + y_crop_blocks,
+	 (JDIMENSION) compptr->v_samp_factor, FALSE);
+      for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
+	dst_row_ptr = dst_buffer[offset_y];
+	src_row_ptr = src_buffer[offset_y];
+	for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks; dst_blk_x++) {
+	  if (x_crop_blocks + dst_blk_x < comp_width) {
+	    /* Do the mirrorable blocks */
+	    dst_ptr = dst_row_ptr[dst_blk_x];
+	    src_ptr = src_row_ptr[comp_width - x_crop_blocks - dst_blk_x - 1];
+	    /* this unrolled loop doesn't need to know which row it's on... */
+	    for (k = 0; k < DCTSIZE2; k += 2) {
+	      *dst_ptr++ = *src_ptr++;	 /* copy even column */
+	      *dst_ptr++ = - *src_ptr++; /* copy odd column with sign change */
+	    }
+	  } else {
+	    /* Copy last partial block(s) verbatim */
+	    jcopy_block_row(src_row_ptr + dst_blk_x + x_crop_blocks,
+			    dst_row_ptr + dst_blk_x,
+			    (JDIMENSION) 1);
+	  }
+	}
       }
     }
   }
@@ -113,11 +543,13 @@
 
 LOCAL(void)
 do_flip_v (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	   JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
 	   jvirt_barray_ptr *src_coef_arrays,
 	   jvirt_barray_ptr *dst_coef_arrays)
 /* Vertical flip */
 {
   JDIMENSION MCU_rows, comp_height, dst_blk_x, dst_blk_y;
+  JDIMENSION x_crop_blocks, y_crop_blocks;
   int ci, i, j, offset_y;
   JBLOCKARRAY src_buffer, dst_buffer;
   JBLOCKROW src_row_ptr, dst_row_ptr;
@@ -131,33 +563,38 @@
    * of odd-numbered rows.
    * Partial iMCUs at the bottom edge are copied verbatim.
    */
-  MCU_rows = dstinfo->image_height / (dstinfo->max_v_samp_factor * DCTSIZE);
+  MCU_rows = srcinfo->image_height / (dstinfo->max_v_samp_factor * DCTSIZE);
 
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
     comp_height = MCU_rows * compptr->v_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
     for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
 	 dst_blk_y += compptr->v_samp_factor) {
       dst_buffer = (*srcinfo->mem->access_virt_barray)
 	((j_common_ptr) srcinfo, dst_coef_arrays[ci], dst_blk_y,
 	 (JDIMENSION) compptr->v_samp_factor, TRUE);
-      if (dst_blk_y < comp_height) {
+      if (y_crop_blocks + dst_blk_y < comp_height) {
 	/* Row is within the mirrorable area. */
 	src_buffer = (*srcinfo->mem->access_virt_barray)
 	  ((j_common_ptr) srcinfo, src_coef_arrays[ci],
-	   comp_height - dst_blk_y - (JDIMENSION) compptr->v_samp_factor,
+	   comp_height - y_crop_blocks - dst_blk_y -
+	   (JDIMENSION) compptr->v_samp_factor,
 	   (JDIMENSION) compptr->v_samp_factor, FALSE);
       } else {
 	/* Bottom-edge blocks will be copied verbatim. */
 	src_buffer = (*srcinfo->mem->access_virt_barray)
-	  ((j_common_ptr) srcinfo, src_coef_arrays[ci], dst_blk_y,
+	  ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	   dst_blk_y + y_crop_blocks,
 	   (JDIMENSION) compptr->v_samp_factor, FALSE);
       }
       for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
-	if (dst_blk_y < comp_height) {
+	if (y_crop_blocks + dst_blk_y < comp_height) {
 	  /* Row is within the mirrorable area. */
 	  dst_row_ptr = dst_buffer[offset_y];
 	  src_row_ptr = src_buffer[compptr->v_samp_factor - offset_y - 1];
+	  src_row_ptr += x_crop_blocks;
 	  for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks;
 	       dst_blk_x++) {
 	    dst_ptr = dst_row_ptr[dst_blk_x];
@@ -173,7 +610,8 @@
 	  }
 	} else {
 	  /* Just copy row verbatim. */
-	  jcopy_block_row(src_buffer[offset_y], dst_buffer[offset_y],
+	  jcopy_block_row(src_buffer[offset_y] + x_crop_blocks,
+			  dst_buffer[offset_y],
 			  compptr->width_in_blocks);
 	}
       }
@@ -184,11 +622,12 @@
 
 LOCAL(void)
 do_transpose (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	      JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
 	      jvirt_barray_ptr *src_coef_arrays,
 	      jvirt_barray_ptr *dst_coef_arrays)
 /* Transpose source into destination */
 {
-  JDIMENSION dst_blk_x, dst_blk_y;
+  JDIMENSION dst_blk_x, dst_blk_y, x_crop_blocks, y_crop_blocks;
   int ci, i, j, offset_x, offset_y;
   JBLOCKARRAY src_buffer, dst_buffer;
   JCOEFPTR src_ptr, dst_ptr;
@@ -201,6 +640,8 @@
    */
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
     for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
 	 dst_blk_y += compptr->v_samp_factor) {
       dst_buffer = (*srcinfo->mem->access_virt_barray)
@@ -210,11 +651,12 @@
 	for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks;
 	     dst_blk_x += compptr->h_samp_factor) {
 	  src_buffer = (*srcinfo->mem->access_virt_barray)
-	    ((j_common_ptr) srcinfo, src_coef_arrays[ci], dst_blk_x,
+	    ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	     dst_blk_x + x_crop_blocks,
 	     (JDIMENSION) compptr->h_samp_factor, FALSE);
 	  for (offset_x = 0; offset_x < compptr->h_samp_factor; offset_x++) {
-	    src_ptr = src_buffer[offset_x][dst_blk_y + offset_y];
 	    dst_ptr = dst_buffer[offset_y][dst_blk_x + offset_x];
+	    src_ptr = src_buffer[offset_x][dst_blk_y + offset_y + y_crop_blocks];
 	    for (i = 0; i < DCTSIZE; i++)
 	      for (j = 0; j < DCTSIZE; j++)
 		dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -228,6 +670,7 @@
 
 LOCAL(void)
 do_rot_90 (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	   JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
 	   jvirt_barray_ptr *src_coef_arrays,
 	   jvirt_barray_ptr *dst_coef_arrays)
 /* 90 degree rotation is equivalent to
@@ -237,6 +680,7 @@
  */
 {
   JDIMENSION MCU_cols, comp_width, dst_blk_x, dst_blk_y;
+  JDIMENSION x_crop_blocks, y_crop_blocks;
   int ci, i, j, offset_x, offset_y;
   JBLOCKARRAY src_buffer, dst_buffer;
   JCOEFPTR src_ptr, dst_ptr;
@@ -246,11 +690,13 @@
    * at the (output) right edge properly.  They just get transposed and
    * not mirrored.
    */
-  MCU_cols = dstinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
+  MCU_cols = srcinfo->image_height / (dstinfo->max_h_samp_factor * DCTSIZE);
 
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
     comp_width = MCU_cols * compptr->h_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
     for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
 	 dst_blk_y += compptr->v_samp_factor) {
       dst_buffer = (*srcinfo->mem->access_virt_barray)
@@ -259,15 +705,26 @@
       for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
 	for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks;
 	     dst_blk_x += compptr->h_samp_factor) {
-	  src_buffer = (*srcinfo->mem->access_virt_barray)
-	    ((j_common_ptr) srcinfo, src_coef_arrays[ci], dst_blk_x,
-	     (JDIMENSION) compptr->h_samp_factor, FALSE);
+	  if (x_crop_blocks + dst_blk_x < comp_width) {
+	    /* Block is within the mirrorable area. */
+	    src_buffer = (*srcinfo->mem->access_virt_barray)
+	      ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	       comp_width - x_crop_blocks - dst_blk_x -
+	       (JDIMENSION) compptr->h_samp_factor,
+	       (JDIMENSION) compptr->h_samp_factor, FALSE);
+	  } else {
+	    /* Edge blocks are transposed but not mirrored. */
+	    src_buffer = (*srcinfo->mem->access_virt_barray)
+	      ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	       dst_blk_x + x_crop_blocks,
+	       (JDIMENSION) compptr->h_samp_factor, FALSE);
+	  }
 	  for (offset_x = 0; offset_x < compptr->h_samp_factor; offset_x++) {
-	    src_ptr = src_buffer[offset_x][dst_blk_y + offset_y];
-	    if (dst_blk_x < comp_width) {
+	    dst_ptr = dst_buffer[offset_y][dst_blk_x + offset_x];
+	    if (x_crop_blocks + dst_blk_x < comp_width) {
 	      /* Block is within the mirrorable area. */
-	      dst_ptr = dst_buffer[offset_y]
-		[comp_width - dst_blk_x - offset_x - 1];
+	      src_ptr = src_buffer[compptr->h_samp_factor - offset_x - 1]
+		[dst_blk_y + offset_y + y_crop_blocks];
 	      for (i = 0; i < DCTSIZE; i++) {
 		for (j = 0; j < DCTSIZE; j++)
 		  dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -277,7 +734,8 @@
 	      }
 	    } else {
 	      /* Edge blocks are transposed but not mirrored. */
-	      dst_ptr = dst_buffer[offset_y][dst_blk_x + offset_x];
+	      src_ptr = src_buffer[offset_x]
+		[dst_blk_y + offset_y + y_crop_blocks];
 	      for (i = 0; i < DCTSIZE; i++)
 		for (j = 0; j < DCTSIZE; j++)
 		  dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -292,6 +750,7 @@
 
 LOCAL(void)
 do_rot_270 (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	    JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
 	    jvirt_barray_ptr *src_coef_arrays,
 	    jvirt_barray_ptr *dst_coef_arrays)
 /* 270 degree rotation is equivalent to
@@ -301,6 +760,7 @@
  */
 {
   JDIMENSION MCU_rows, comp_height, dst_blk_x, dst_blk_y;
+  JDIMENSION x_crop_blocks, y_crop_blocks;
   int ci, i, j, offset_x, offset_y;
   JBLOCKARRAY src_buffer, dst_buffer;
   JCOEFPTR src_ptr, dst_ptr;
@@ -310,11 +770,13 @@
    * at the (output) bottom edge properly.  They just get transposed and
    * not mirrored.
    */
-  MCU_rows = dstinfo->image_height / (dstinfo->max_v_samp_factor * DCTSIZE);
+  MCU_rows = srcinfo->image_width / (dstinfo->max_v_samp_factor * DCTSIZE);
 
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
     comp_height = MCU_rows * compptr->v_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
     for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
 	 dst_blk_y += compptr->v_samp_factor) {
       dst_buffer = (*srcinfo->mem->access_virt_barray)
@@ -324,14 +786,15 @@
 	for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks;
 	     dst_blk_x += compptr->h_samp_factor) {
 	  src_buffer = (*srcinfo->mem->access_virt_barray)
-	    ((j_common_ptr) srcinfo, src_coef_arrays[ci], dst_blk_x,
+	    ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	     dst_blk_x + x_crop_blocks,
 	     (JDIMENSION) compptr->h_samp_factor, FALSE);
 	  for (offset_x = 0; offset_x < compptr->h_samp_factor; offset_x++) {
 	    dst_ptr = dst_buffer[offset_y][dst_blk_x + offset_x];
-	    if (dst_blk_y < comp_height) {
+	    if (y_crop_blocks + dst_blk_y < comp_height) {
 	      /* Block is within the mirrorable area. */
 	      src_ptr = src_buffer[offset_x]
-		[comp_height - dst_blk_y - offset_y - 1];
+		[comp_height - y_crop_blocks - dst_blk_y - offset_y - 1];
 	      for (i = 0; i < DCTSIZE; i++) {
 		for (j = 0; j < DCTSIZE; j++) {
 		  dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -341,7 +804,8 @@
 	      }
 	    } else {
 	      /* Edge blocks are transposed but not mirrored. */
-	      src_ptr = src_buffer[offset_x][dst_blk_y + offset_y];
+	      src_ptr = src_buffer[offset_x]
+		[dst_blk_y + offset_y + y_crop_blocks];
 	      for (i = 0; i < DCTSIZE; i++)
 		for (j = 0; j < DCTSIZE; j++)
 		  dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -356,6 +820,7 @@
 
 LOCAL(void)
 do_rot_180 (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	    JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
 	    jvirt_barray_ptr *src_coef_arrays,
 	    jvirt_barray_ptr *dst_coef_arrays)
 /* 180 degree rotation is equivalent to
@@ -365,89 +830,93 @@
  */
 {
   JDIMENSION MCU_cols, MCU_rows, comp_width, comp_height, dst_blk_x, dst_blk_y;
+  JDIMENSION x_crop_blocks, y_crop_blocks;
   int ci, i, j, offset_y;
   JBLOCKARRAY src_buffer, dst_buffer;
   JBLOCKROW src_row_ptr, dst_row_ptr;
   JCOEFPTR src_ptr, dst_ptr;
   jpeg_component_info *compptr;
 
-  MCU_cols = dstinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
-  MCU_rows = dstinfo->image_height / (dstinfo->max_v_samp_factor * DCTSIZE);
+  MCU_cols = srcinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
+  MCU_rows = srcinfo->image_height / (dstinfo->max_v_samp_factor * DCTSIZE);
 
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
     comp_width = MCU_cols * compptr->h_samp_factor;
     comp_height = MCU_rows * compptr->v_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
     for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
 	 dst_blk_y += compptr->v_samp_factor) {
       dst_buffer = (*srcinfo->mem->access_virt_barray)
 	((j_common_ptr) srcinfo, dst_coef_arrays[ci], dst_blk_y,
 	 (JDIMENSION) compptr->v_samp_factor, TRUE);
-      if (dst_blk_y < comp_height) {
+      if (y_crop_blocks + dst_blk_y < comp_height) {
 	/* Row is within the vertically mirrorable area. */
 	src_buffer = (*srcinfo->mem->access_virt_barray)
 	  ((j_common_ptr) srcinfo, src_coef_arrays[ci],
-	   comp_height - dst_blk_y - (JDIMENSION) compptr->v_samp_factor,
+	   comp_height - y_crop_blocks - dst_blk_y -
+	   (JDIMENSION) compptr->v_samp_factor,
 	   (JDIMENSION) compptr->v_samp_factor, FALSE);
       } else {
 	/* Bottom-edge rows are only mirrored horizontally. */
 	src_buffer = (*srcinfo->mem->access_virt_barray)
-	  ((j_common_ptr) srcinfo, src_coef_arrays[ci], dst_blk_y,
+	  ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	   dst_blk_y + y_crop_blocks,
 	   (JDIMENSION) compptr->v_samp_factor, FALSE);
       }
       for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
-	if (dst_blk_y < comp_height) {
+	dst_row_ptr = dst_buffer[offset_y];
+	if (y_crop_blocks + dst_blk_y < comp_height) {
 	  /* Row is within the mirrorable area. */
-	  dst_row_ptr = dst_buffer[offset_y];
 	  src_row_ptr = src_buffer[compptr->v_samp_factor - offset_y - 1];
-	  /* Process the blocks that can be mirrored both ways. */
-	  for (dst_blk_x = 0; dst_blk_x < comp_width; dst_blk_x++) {
+	  for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks; dst_blk_x++) {
 	    dst_ptr = dst_row_ptr[dst_blk_x];
-	    src_ptr = src_row_ptr[comp_width - dst_blk_x - 1];
-	    for (i = 0; i < DCTSIZE; i += 2) {
-	      /* For even row, negate every odd column. */
-	      for (j = 0; j < DCTSIZE; j += 2) {
-		*dst_ptr++ = *src_ptr++;
-		*dst_ptr++ = - *src_ptr++;
+	    if (x_crop_blocks + dst_blk_x < comp_width) {
+	      /* Process the blocks that can be mirrored both ways. */
+	      src_ptr = src_row_ptr[comp_width - x_crop_blocks - dst_blk_x - 1];
+	      for (i = 0; i < DCTSIZE; i += 2) {
+		/* For even row, negate every odd column. */
+		for (j = 0; j < DCTSIZE; j += 2) {
+		  *dst_ptr++ = *src_ptr++;
+		  *dst_ptr++ = - *src_ptr++;
+		}
+		/* For odd row, negate every even column. */
+		for (j = 0; j < DCTSIZE; j += 2) {
+		  *dst_ptr++ = - *src_ptr++;
+		  *dst_ptr++ = *src_ptr++;
+		}
 	      }
-	      /* For odd row, negate every even column. */
-	      for (j = 0; j < DCTSIZE; j += 2) {
-		*dst_ptr++ = - *src_ptr++;
-		*dst_ptr++ = *src_ptr++;
+	    } else {
+	      /* Any remaining right-edge blocks are only mirrored vertically. */
+	      src_ptr = src_row_ptr[x_crop_blocks + dst_blk_x];
+	      for (i = 0; i < DCTSIZE; i += 2) {
+		for (j = 0; j < DCTSIZE; j++)
+		  *dst_ptr++ = *src_ptr++;
+		for (j = 0; j < DCTSIZE; j++)
+		  *dst_ptr++ = - *src_ptr++;
 	      }
 	    }
 	  }
-	  /* Any remaining right-edge blocks are only mirrored vertically. */
-	  for (; dst_blk_x < compptr->width_in_blocks; dst_blk_x++) {
-	    dst_ptr = dst_row_ptr[dst_blk_x];
-	    src_ptr = src_row_ptr[dst_blk_x];
-	    for (i = 0; i < DCTSIZE; i += 2) {
-	      for (j = 0; j < DCTSIZE; j++)
-		*dst_ptr++ = *src_ptr++;
-	      for (j = 0; j < DCTSIZE; j++)
-		*dst_ptr++ = - *src_ptr++;
-	    }
-	  }
 	} else {
 	  /* Remaining rows are just mirrored horizontally. */
-	  dst_row_ptr = dst_buffer[offset_y];
 	  src_row_ptr = src_buffer[offset_y];
-	  /* Process the blocks that can be mirrored. */
-	  for (dst_blk_x = 0; dst_blk_x < comp_width; dst_blk_x++) {
-	    dst_ptr = dst_row_ptr[dst_blk_x];
-	    src_ptr = src_row_ptr[comp_width - dst_blk_x - 1];
-	    for (i = 0; i < DCTSIZE2; i += 2) {
-	      *dst_ptr++ = *src_ptr++;
-	      *dst_ptr++ = - *src_ptr++;
+	  for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks; dst_blk_x++) {
+	    if (x_crop_blocks + dst_blk_x < comp_width) {
+	      /* Process the blocks that can be mirrored. */
+	      dst_ptr = dst_row_ptr[dst_blk_x];
+	      src_ptr = src_row_ptr[comp_width - x_crop_blocks - dst_blk_x - 1];
+	      for (i = 0; i < DCTSIZE2; i += 2) {
+		*dst_ptr++ = *src_ptr++;
+		*dst_ptr++ = - *src_ptr++;
+	      }
+	    } else {
+	      /* Any remaining right-edge blocks are only copied. */
+	      jcopy_block_row(src_row_ptr + dst_blk_x + x_crop_blocks,
+			      dst_row_ptr + dst_blk_x,
+			      (JDIMENSION) 1);
 	    }
 	  }
-	  /* Any remaining right-edge blocks are only copied. */
-	  for (; dst_blk_x < compptr->width_in_blocks; dst_blk_x++) {
-	    dst_ptr = dst_row_ptr[dst_blk_x];
-	    src_ptr = src_row_ptr[dst_blk_x];
-	    for (i = 0; i < DCTSIZE2; i++)
-	      *dst_ptr++ = *src_ptr++;
-	  }
 	}
       }
     }
@@ -457,6 +926,7 @@
 
 LOCAL(void)
 do_transverse (j_decompress_ptr srcinfo, j_compress_ptr dstinfo,
+	       JDIMENSION x_crop_offset, JDIMENSION y_crop_offset,
 	       jvirt_barray_ptr *src_coef_arrays,
 	       jvirt_barray_ptr *dst_coef_arrays)
 /* Transverse transpose is equivalent to
@@ -470,18 +940,21 @@
  */
 {
   JDIMENSION MCU_cols, MCU_rows, comp_width, comp_height, dst_blk_x, dst_blk_y;
+  JDIMENSION x_crop_blocks, y_crop_blocks;
   int ci, i, j, offset_x, offset_y;
   JBLOCKARRAY src_buffer, dst_buffer;
   JCOEFPTR src_ptr, dst_ptr;
   jpeg_component_info *compptr;
 
-  MCU_cols = dstinfo->image_width / (dstinfo->max_h_samp_factor * DCTSIZE);
-  MCU_rows = dstinfo->image_height / (dstinfo->max_v_samp_factor * DCTSIZE);
+  MCU_cols = srcinfo->image_height / (dstinfo->max_h_samp_factor * DCTSIZE);
+  MCU_rows = srcinfo->image_width / (dstinfo->max_v_samp_factor * DCTSIZE);
 
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
     comp_width = MCU_cols * compptr->h_samp_factor;
     comp_height = MCU_rows * compptr->v_samp_factor;
+    x_crop_blocks = x_crop_offset * compptr->h_samp_factor;
+    y_crop_blocks = y_crop_offset * compptr->v_samp_factor;
     for (dst_blk_y = 0; dst_blk_y < compptr->height_in_blocks;
 	 dst_blk_y += compptr->v_samp_factor) {
       dst_buffer = (*srcinfo->mem->access_virt_barray)
@@ -490,17 +963,26 @@
       for (offset_y = 0; offset_y < compptr->v_samp_factor; offset_y++) {
 	for (dst_blk_x = 0; dst_blk_x < compptr->width_in_blocks;
 	     dst_blk_x += compptr->h_samp_factor) {
-	  src_buffer = (*srcinfo->mem->access_virt_barray)
-	    ((j_common_ptr) srcinfo, src_coef_arrays[ci], dst_blk_x,
-	     (JDIMENSION) compptr->h_samp_factor, FALSE);
+	  if (x_crop_blocks + dst_blk_x < comp_width) {
+	    /* Block is within the mirrorable area. */
+	    src_buffer = (*srcinfo->mem->access_virt_barray)
+	      ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	       comp_width - x_crop_blocks - dst_blk_x -
+	       (JDIMENSION) compptr->h_samp_factor,
+	       (JDIMENSION) compptr->h_samp_factor, FALSE);
+	  } else {
+	    src_buffer = (*srcinfo->mem->access_virt_barray)
+	      ((j_common_ptr) srcinfo, src_coef_arrays[ci],
+	       dst_blk_x + x_crop_blocks,
+	       (JDIMENSION) compptr->h_samp_factor, FALSE);
+	  }
 	  for (offset_x = 0; offset_x < compptr->h_samp_factor; offset_x++) {
-	    if (dst_blk_y < comp_height) {
-	      src_ptr = src_buffer[offset_x]
-		[comp_height - dst_blk_y - offset_y - 1];
-	      if (dst_blk_x < comp_width) {
+	    dst_ptr = dst_buffer[offset_y][dst_blk_x + offset_x];
+	    if (y_crop_blocks + dst_blk_y < comp_height) {
+	      if (x_crop_blocks + dst_blk_x < comp_width) {
 		/* Block is within the mirrorable area. */
-		dst_ptr = dst_buffer[offset_y]
-		  [comp_width - dst_blk_x - offset_x - 1];
+		src_ptr = src_buffer[compptr->h_samp_factor - offset_x - 1]
+		  [comp_height - y_crop_blocks - dst_blk_y - offset_y - 1];
 		for (i = 0; i < DCTSIZE; i++) {
 		  for (j = 0; j < DCTSIZE; j++) {
 		    dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -516,7 +998,8 @@
 		}
 	      } else {
 		/* Right-edge blocks are mirrored in y only */
-		dst_ptr = dst_buffer[offset_y][dst_blk_x + offset_x];
+		src_ptr = src_buffer[offset_x]
+		  [comp_height - y_crop_blocks - dst_blk_y - offset_y - 1];
 		for (i = 0; i < DCTSIZE; i++) {
 		  for (j = 0; j < DCTSIZE; j++) {
 		    dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -526,11 +1009,10 @@
 		}
 	      }
 	    } else {
-	      src_ptr = src_buffer[offset_x][dst_blk_y + offset_y];
-	      if (dst_blk_x < comp_width) {
+	      if (x_crop_blocks + dst_blk_x < comp_width) {
 		/* Bottom-edge blocks are mirrored in x only */
-		dst_ptr = dst_buffer[offset_y]
-		  [comp_width - dst_blk_x - offset_x - 1];
+		src_ptr = src_buffer[compptr->h_samp_factor - offset_x - 1]
+		  [dst_blk_y + offset_y + y_crop_blocks];
 		for (i = 0; i < DCTSIZE; i++) {
 		  for (j = 0; j < DCTSIZE; j++)
 		    dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -540,7 +1022,8 @@
 		}
 	      } else {
 		/* At lower right corner, just transpose, no mirroring */
-		dst_ptr = dst_buffer[offset_y][dst_blk_x + offset_x];
+		src_ptr = src_buffer[offset_x]
+		  [dst_blk_y + offset_y + y_crop_blocks];
 		for (i = 0; i < DCTSIZE; i++)
 		  for (j = 0; j < DCTSIZE; j++)
 		    dst_ptr[j*DCTSIZE+i] = src_ptr[i*DCTSIZE+j];
@@ -554,8 +1037,116 @@
 }
 
 
+/* Parse an unsigned integer: subroutine for jtransform_parse_crop_spec.
+ * Returns TRUE if valid integer found, FALSE if not.
+ * *strptr is advanced over the digit string, and *result is set to its value.
+ */
+
+LOCAL(boolean)
+jt_read_integer (const char ** strptr, JDIMENSION * result)
+{
+  const char * ptr = *strptr;
+  JDIMENSION val = 0;
+
+  for (; isdigit(*ptr); ptr++) {
+    val = val * 10 + (JDIMENSION) (*ptr - '0');
+  }
+  *result = val;
+  if (ptr == *strptr)
+    return FALSE;		/* oops, no digits */
+  *strptr = ptr;
+  return TRUE;
+}
+
+
+/* Parse a crop specification (written in X11 geometry style).
+ * The routine returns TRUE if the spec string is valid, FALSE if not.
+ *
+ * The crop spec string should have the format
+ *	<width>x<height>{+-}<xoffset>{+-}<yoffset>
+ * where width, height, xoffset, and yoffset are unsigned integers.
+ * Each of the elements can be omitted to indicate a default value.
+ * (A weakness of this style is that it is not possible to omit xoffset
+ * while specifying yoffset, since they look alike.)
+ *
+ * This code is loosely based on XParseGeometry from the X11 distribution.
+ */
+
+GLOBAL(boolean)
+jtransform_parse_crop_spec (jpeg_transform_info *info, const char *spec)
+{
+  info->crop = FALSE;
+  info->crop_width_set = JCROP_UNSET;
+  info->crop_height_set = JCROP_UNSET;
+  info->crop_xoffset_set = JCROP_UNSET;
+  info->crop_yoffset_set = JCROP_UNSET;
+
+  if (isdigit(*spec)) {
+    /* fetch width */
+    if (! jt_read_integer(&spec, &info->crop_width))
+      return FALSE;
+    info->crop_width_set = JCROP_POS;
+  }
+  if (*spec == 'x' || *spec == 'X') {	
+    /* fetch height */
+    spec++;
+    if (! jt_read_integer(&spec, &info->crop_height))
+      return FALSE;
+    info->crop_height_set = JCROP_POS;
+  }
+  if (*spec == '+' || *spec == '-') {
+    /* fetch xoffset */
+    info->crop_xoffset_set = (*spec == '-') ? JCROP_NEG : JCROP_POS;
+    spec++;
+    if (! jt_read_integer(&spec, &info->crop_xoffset))
+      return FALSE;
+  }
+  if (*spec == '+' || *spec == '-') {
+    /* fetch yoffset */
+    info->crop_yoffset_set = (*spec == '-') ? JCROP_NEG : JCROP_POS;
+    spec++;
+    if (! jt_read_integer(&spec, &info->crop_yoffset))
+      return FALSE;
+  }
+  /* We had better have gotten to the end of the string. */
+  if (*spec != '\0')
+    return FALSE;
+  info->crop = TRUE;
+  return TRUE;
+}
+
+
+/* Trim off any partial iMCUs on the indicated destination edge */
+
+LOCAL(void)
+trim_right_edge (jpeg_transform_info *info, JDIMENSION full_width)
+{
+  JDIMENSION MCU_cols;
+
+  MCU_cols = info->output_width / (info->max_h_samp_factor * DCTSIZE);
+  if (MCU_cols > 0 && info->x_crop_offset + MCU_cols ==
+      full_width / (info->max_h_samp_factor * DCTSIZE))
+    info->output_width = MCU_cols * (info->max_h_samp_factor * DCTSIZE);
+}
+
+LOCAL(void)
+trim_bottom_edge (jpeg_transform_info *info, JDIMENSION full_height)
+{
+  JDIMENSION MCU_rows;
+
+  MCU_rows = info->output_height / (info->max_v_samp_factor * DCTSIZE);
+  if (MCU_rows > 0 && info->y_crop_offset + MCU_rows ==
+      full_height / (info->max_v_samp_factor * DCTSIZE))
+    info->output_height = MCU_rows * (info->max_v_samp_factor * DCTSIZE);
+}
+
+
 /* Request any required workspace.
  *
+ * This routine figures out the size that the output image will be
+ * (which implies that all the transform parameters must be set before
+ * it is called).
+ *
  * We allocate the workspace virtual arrays from the source decompression
  * object, so that all the arrays (both the original data and the workspace)
  * will be taken into account while making memory management decisions.
@@ -569,9 +1160,13 @@
 			      jpeg_transform_info *info)
 {
   jvirt_barray_ptr *coef_arrays = NULL;
+  boolean need_workspace, transpose_it;
   jpeg_component_info *compptr;
-  int ci;
+  JDIMENSION xoffset, yoffset, dtemp, width_in_iMCUs, height_in_iMCUs;
+  JDIMENSION width_in_blocks, height_in_blocks;
+  int itemp, ci, h_samp_factor, v_samp_factor;
 
+  /* Determine number of components in output image */
   if (info->force_grayscale &&
       srcinfo->jpeg_color_space == JCS_YCbCr &&
       srcinfo->num_components == 3) {
@@ -581,55 +1176,267 @@
     /* Process all the components */
     info->num_components = srcinfo->num_components;
   }
+  /* If there is only one output component, force the iMCU size to be 1;
+   * else use the source iMCU size.  (This allows us to do the right thing
+   * when reducing color to grayscale, and also provides a handy way of
+   * cleaning up "funny" grayscale images whose sampling factors are not 1x1.)
+   */
 
   switch (info->transform) {
+  case JXFORM_TRANSPOSE:
+  case JXFORM_TRANSVERSE:
+  case JXFORM_ROT_90:
+  case JXFORM_ROT_270:
+    info->output_width = srcinfo->image_height;
+    info->output_height = srcinfo->image_width;
+    if (info->num_components == 1) {
+      info->max_h_samp_factor = 1;
+      info->max_v_samp_factor = 1;
+    } else {
+      info->max_h_samp_factor = srcinfo->max_v_samp_factor;
+      info->max_v_samp_factor = srcinfo->max_h_samp_factor;
+    }
+    break;
+  default:
+    info->output_width = srcinfo->image_width;
+    info->output_height = srcinfo->image_height;
+    if (info->num_components == 1) {
+      info->max_h_samp_factor = 1;
+      info->max_v_samp_factor = 1;
+    } else {
+      info->max_h_samp_factor = srcinfo->max_h_samp_factor;
+      info->max_v_samp_factor = srcinfo->max_v_samp_factor;
+    }
+    break;
+  }
+
+  /* If cropping has been requested, compute the crop area's position and
+   * dimensions, ensuring that its upper left corner falls at an iMCU boundary.
+   */
+  if (info->crop) {
+    /* Insert default values for unset crop parameters */
+    if (info->crop_xoffset_set == JCROP_UNSET)
+      info->crop_xoffset = 0;	/* default to +0 */
+    if (info->crop_yoffset_set == JCROP_UNSET)
+      info->crop_yoffset = 0;	/* default to +0 */
+    if (info->crop_width_set == JCROP_UNSET) {
+      if (info->crop_xoffset >= info->output_width)
+	ERREXIT(srcinfo, JERR_BAD_CROP_SPEC);
+      info->crop_width = info->output_width - info->crop_xoffset;
+    } else {
+      /* Check for crop extension */
+      if (info->crop_width > info->output_width) {
+	/* Crop extension does not work when transforming! */
+	if (info->transform != JXFORM_NONE ||
+	    info->crop_xoffset >= info->crop_width ||
+	    info->crop_xoffset > info->crop_width - info->output_width)
+	  ERREXIT(srcinfo, JERR_BAD_CROP_SPEC);
+      } else {
+	if (info->crop_xoffset >= info->output_width ||
+	    info->crop_width <= 0 ||
+	    info->crop_xoffset > info->output_width - info->crop_width)
+	  ERREXIT(srcinfo, JERR_BAD_CROP_SPEC);
+      }
+    }
+    if (info->crop_height_set == JCROP_UNSET) {
+      if (info->crop_yoffset >= info->output_height)
+	ERREXIT(srcinfo, JERR_BAD_CROP_SPEC);
+      info->crop_height = info->output_height - info->crop_yoffset;
+    } else {
+      /* Check for crop extension */
+      if (info->crop_height > info->output_height) {
+	/* Crop extension does not work when transforming! */
+	if (info->transform != JXFORM_NONE ||
+	    info->crop_yoffset >= info->crop_height ||
+	    info->crop_yoffset > info->crop_height - info->output_height)
+	  ERREXIT(srcinfo, JERR_BAD_CROP_SPEC);
+      } else {
+	if (info->crop_yoffset >= info->output_height ||
+	    info->crop_height <= 0 ||
+	    info->crop_yoffset > info->output_height - info->crop_height)
+	  ERREXIT(srcinfo, JERR_BAD_CROP_SPEC);
+      }
+    }
+    /* Convert negative crop offsets into regular offsets */
+    if (info->crop_xoffset_set == JCROP_NEG) {
+      if (info->crop_width > info->output_width)
+	xoffset = info->crop_width - info->output_width - info->crop_xoffset;
+      else
+	xoffset = info->output_width - info->crop_width - info->crop_xoffset;
+    } else
+      xoffset = info->crop_xoffset;
+    if (info->crop_yoffset_set == JCROP_NEG) {
+      if (info->crop_height > info->output_height)
+	yoffset = info->crop_height - info->output_height - info->crop_yoffset;
+      else
+	yoffset = info->output_height - info->crop_height - info->crop_yoffset;
+    } else
+      yoffset = info->crop_yoffset;
+    /* Now adjust so that upper left corner falls at an iMCU boundary */
+    if (info->transform == JXFORM_DROP) {
+      /* Ensure the effective drop region will not exceed the requested */
+      itemp = info->max_h_samp_factor * DCTSIZE;
+      dtemp = itemp - 1 - ((xoffset + itemp - 1) % itemp);
+      xoffset += dtemp;
+      if (info->crop_width > dtemp)
+	info->drop_width = (info->crop_width - dtemp) / itemp;
+      else
+	info->drop_width = 0;
+      itemp = info->max_v_samp_factor * DCTSIZE;
+      dtemp = itemp - 1 - ((yoffset + itemp - 1) % itemp);
+      yoffset += dtemp;
+      if (info->crop_height > dtemp)
+	info->drop_height = (info->crop_height - dtemp) / itemp;
+      else
+	info->drop_height = 0;
+      /* Check if sampling factors match for dropping */
+      if (info->drop_width != 0 && info->drop_height != 0)
+	for (ci = 0; ci < info->num_components &&
+		     ci < info->drop_ptr->num_components; ci++) {
+	  if (info->drop_ptr->comp_info[ci].h_samp_factor *
+	      srcinfo->max_h_samp_factor !=
+	      srcinfo->comp_info[ci].h_samp_factor *
+	      info->drop_ptr->max_h_samp_factor)
+	    ERREXIT6(srcinfo, JERR_BAD_DROP_SAMPLING, ci,
+	      info->drop_ptr->comp_info[ci].h_samp_factor,
+	      info->drop_ptr->max_h_samp_factor,
+	      srcinfo->comp_info[ci].h_samp_factor,
+	      srcinfo->max_h_samp_factor, 'h');
+	  if (info->drop_ptr->comp_info[ci].v_samp_factor *
+	      srcinfo->max_v_samp_factor !=
+	      srcinfo->comp_info[ci].v_samp_factor *
+	      info->drop_ptr->max_v_samp_factor)
+	    ERREXIT6(srcinfo, JERR_BAD_DROP_SAMPLING, ci,
+	      info->drop_ptr->comp_info[ci].v_samp_factor,
+	      info->drop_ptr->max_v_samp_factor,
+	      srcinfo->comp_info[ci].v_samp_factor,
+	      srcinfo->max_v_samp_factor, 'v');
+	}
+    } else {
+      /* Ensure the effective crop region will cover the requested */
+      if (info->crop_width > info->output_width)
+	info->output_width = info->crop_width;
+      else
+	info->output_width =
+	  info->crop_width + (xoffset % (info->max_h_samp_factor * DCTSIZE));
+      if (info->crop_height > info->output_height)
+	info->output_height = info->crop_height;
+      else
+	info->output_height =
+	  info->crop_height + (yoffset % (info->max_v_samp_factor * DCTSIZE));
+    }
+    /* Save x/y offsets measured in iMCUs */
+    info->x_crop_offset = xoffset / (info->max_h_samp_factor * DCTSIZE);
+    info->y_crop_offset = yoffset / (info->max_v_samp_factor * DCTSIZE);
+  } else {
+    info->x_crop_offset = 0;
+    info->y_crop_offset = 0;
+  }
+
+  /* Figure out whether we need workspace arrays,
+   * and if so whether they are transposed relative to the source.
+   */
+  need_workspace = FALSE;
+  transpose_it = FALSE;
+  switch (info->transform) {
   case JXFORM_NONE:
+    if (info->x_crop_offset != 0 || info->y_crop_offset != 0 ||
+	info->output_width > srcinfo->image_width ||
+	info->output_height > srcinfo->image_height)
+      need_workspace = TRUE;
+    /* No workspace needed if neither cropping nor transforming */
+    break;
   case JXFORM_FLIP_H:
-    /* Don't need a workspace array */
+    if (info->trim)
+      trim_right_edge(info, srcinfo->image_width);
+    if (info->y_crop_offset != 0)
+      need_workspace = TRUE;
+    /* do_flip_h_no_crop doesn't need a workspace array */
     break;
   case JXFORM_FLIP_V:
-  case JXFORM_ROT_180:
-    /* Need workspace arrays having same dimensions as source image.
-     * Note that we allocate arrays padded out to the next iMCU boundary,
-     * so that transform routines need not worry about missing edge blocks.
-     */
-    coef_arrays = (jvirt_barray_ptr *)
-      (*srcinfo->mem->alloc_small) ((j_common_ptr) srcinfo, JPOOL_IMAGE,
-	SIZEOF(jvirt_barray_ptr) * info->num_components);
-    for (ci = 0; ci < info->num_components; ci++) {
-      compptr = srcinfo->comp_info + ci;
-      coef_arrays[ci] = (*srcinfo->mem->request_virt_barray)
-	((j_common_ptr) srcinfo, JPOOL_IMAGE, FALSE,
-	 (JDIMENSION) jround_up((long) compptr->width_in_blocks,
-				(long) compptr->h_samp_factor),
-	 (JDIMENSION) jround_up((long) compptr->height_in_blocks,
-				(long) compptr->v_samp_factor),
-	 (JDIMENSION) compptr->v_samp_factor);
-    }
+    if (info->trim)
+      trim_bottom_edge(info, srcinfo->image_height);
+    /* Need workspace arrays having same dimensions as source image. */
+    need_workspace = TRUE;
     break;
   case JXFORM_TRANSPOSE:
+    /* transpose does NOT have to trim anything */
+    /* Need workspace arrays having transposed dimensions. */
+    need_workspace = TRUE;
+    transpose_it = TRUE;
+    break;
   case JXFORM_TRANSVERSE:
+    if (info->trim) {
+      trim_right_edge(info, srcinfo->image_height);
+      trim_bottom_edge(info, srcinfo->image_width);
+    }
+    /* Need workspace arrays having transposed dimensions. */
+    need_workspace = TRUE;
+    transpose_it = TRUE;
+    break;
   case JXFORM_ROT_90:
+    if (info->trim)
+      trim_right_edge(info, srcinfo->image_height);
+    /* Need workspace arrays having transposed dimensions. */
+    need_workspace = TRUE;
+    transpose_it = TRUE;
+    break;
+  case JXFORM_ROT_180:
+    if (info->trim) {
+      trim_right_edge(info, srcinfo->image_width);
+      trim_bottom_edge(info, srcinfo->image_height);
+    }
+    /* Need workspace arrays having same dimensions as source image. */
+    need_workspace = TRUE;
+    break;
   case JXFORM_ROT_270:
-    /* Need workspace arrays having transposed dimensions.
-     * Note that we allocate arrays padded out to the next iMCU boundary,
-     * so that transform routines need not worry about missing edge blocks.
-     */
+    if (info->trim)
+      trim_bottom_edge(info, srcinfo->image_width);
+    /* Need workspace arrays having transposed dimensions. */
+    need_workspace = TRUE;
+    transpose_it = TRUE;
+    break;
+  case JXFORM_DROP:
+#if DROP_REQUEST_FROM_SRC
+    drop_request_from_src(info->drop_ptr, srcinfo);
+#endif
+    break;
+  }
+
+  /* Allocate workspace if needed.
+   * Note that we allocate arrays padded out to the next iMCU boundary,
+   * so that transform routines need not worry about missing edge blocks.
+   */
+  if (need_workspace) {
     coef_arrays = (jvirt_barray_ptr *)
       (*srcinfo->mem->alloc_small) ((j_common_ptr) srcinfo, JPOOL_IMAGE,
-	SIZEOF(jvirt_barray_ptr) * info->num_components);
+		SIZEOF(jvirt_barray_ptr) * info->num_components);
+    width_in_iMCUs = (JDIMENSION)
+      jdiv_round_up((long) info->output_width,
+		    (long) (info->max_h_samp_factor * DCTSIZE));
+    height_in_iMCUs = (JDIMENSION)
+      jdiv_round_up((long) info->output_height,
+		    (long) (info->max_v_samp_factor * DCTSIZE));
     for (ci = 0; ci < info->num_components; ci++) {
       compptr = srcinfo->comp_info + ci;
+      if (info->num_components == 1) {
+	/* we're going to force samp factors to 1x1 in this case */
+	h_samp_factor = v_samp_factor = 1;
+      } else if (transpose_it) {
+	h_samp_factor = compptr->v_samp_factor;
+	v_samp_factor = compptr->h_samp_factor;
+      } else {
+	h_samp_factor = compptr->h_samp_factor;
+	v_samp_factor = compptr->v_samp_factor;
+      }
+      width_in_blocks = width_in_iMCUs * h_samp_factor;
+      height_in_blocks = height_in_iMCUs * v_samp_factor;
       coef_arrays[ci] = (*srcinfo->mem->request_virt_barray)
 	((j_common_ptr) srcinfo, JPOOL_IMAGE, FALSE,
-	 (JDIMENSION) jround_up((long) compptr->height_in_blocks,
-				(long) compptr->v_samp_factor),
-	 (JDIMENSION) jround_up((long) compptr->width_in_blocks,
-				(long) compptr->h_samp_factor),
-	 (JDIMENSION) compptr->h_samp_factor);
+	 width_in_blocks, height_in_blocks, (JDIMENSION) v_samp_factor);
     }
-    break;
   }
+
   info->workspace_coef_arrays = coef_arrays;
 }
 
@@ -642,14 +1449,8 @@
   int tblno, i, j, ci, itemp;
   jpeg_component_info *compptr;
   JQUANT_TBL *qtblptr;
-  JDIMENSION dtemp;
   UINT16 qtemp;
 
-  /* Transpose basic image dimensions */
-  dtemp = dstinfo->image_width;
-  dstinfo->image_width = dstinfo->image_height;
-  dstinfo->image_height = dtemp;
-
   /* Transpose sampling factors */
   for (ci = 0; ci < dstinfo->num_components; ci++) {
     compptr = dstinfo->comp_info + ci;
@@ -674,46 +1475,159 @@
 }
 
 
-/* Trim off any partial iMCUs on the indicated destination edge */
+/* Adjust Exif image parameters.
+ *
+ * We try to adjust the Tags ExifImageWidth and ExifImageHeight if possible.
+ */
 
 LOCAL(void)
-trim_right_edge (j_compress_ptr dstinfo)
+adjust_exif_parameters (JOCTET FAR * data, unsigned int length,
+			JDIMENSION new_width, JDIMENSION new_height)
 {
-  int ci, max_h_samp_factor;
-  JDIMENSION MCU_cols;
+  boolean is_motorola; /* Flag for byte order */
+  unsigned int number_of_tags, tagnum;
+  unsigned int firstoffset, offset;
+  JDIMENSION new_value;
+
+  if (length < 12) return; /* Length of an IFD entry */
+
+  /* Discover byte order */
+  if (GETJOCTET(data[0]) == 0x49 && GETJOCTET(data[1]) == 0x49)
+    is_motorola = FALSE;
+  else if (GETJOCTET(data[0]) == 0x4D && GETJOCTET(data[1]) == 0x4D)
+    is_motorola = TRUE;
+  else
+    return;
+
+  /* Check Tag Mark */
+  if (is_motorola) {
+    if (GETJOCTET(data[2]) != 0) return;
+    if (GETJOCTET(data[3]) != 0x2A) return;
+  } else {
+    if (GETJOCTET(data[3]) != 0) return;
+    if (GETJOCTET(data[2]) != 0x2A) return;
+  }
 
-  /* We have to compute max_h_samp_factor ourselves,
-   * because it hasn't been set yet in the destination
-   * (and we don't want to use the source's value).
-   */
-  max_h_samp_factor = 1;
-  for (ci = 0; ci < dstinfo->num_components; ci++) {
-    int h_samp_factor = dstinfo->comp_info[ci].h_samp_factor;
-    max_h_samp_factor = MAX(max_h_samp_factor, h_samp_factor);
+  /* Get first IFD offset (offset to IFD0) */
+  if (is_motorola) {
+    if (GETJOCTET(data[4]) != 0) return;
+    if (GETJOCTET(data[5]) != 0) return;
+    firstoffset = GETJOCTET(data[6]);
+    firstoffset <<= 8;
+    firstoffset += GETJOCTET(data[7]);
+  } else {
+    if (GETJOCTET(data[7]) != 0) return;
+    if (GETJOCTET(data[6]) != 0) return;
+    firstoffset = GETJOCTET(data[5]);
+    firstoffset <<= 8;
+    firstoffset += GETJOCTET(data[4]);
   }
-  MCU_cols = dstinfo->image_width / (max_h_samp_factor * DCTSIZE);
-  if (MCU_cols > 0)		/* can't trim to 0 pixels */
-    dstinfo->image_width = MCU_cols * (max_h_samp_factor * DCTSIZE);
-}
+  if (firstoffset > length - 2) return; /* check end of data segment */
 
-LOCAL(void)
-trim_bottom_edge (j_compress_ptr dstinfo)
-{
-  int ci, max_v_samp_factor;
-  JDIMENSION MCU_rows;
+  /* Get the number of directory entries contained in this IFD */
+  if (is_motorola) {
+    number_of_tags = GETJOCTET(data[firstoffset]);
+    number_of_tags <<= 8;
+    number_of_tags += GETJOCTET(data[firstoffset+1]);
+  } else {
+    number_of_tags = GETJOCTET(data[firstoffset+1]);
+    number_of_tags <<= 8;
+    number_of_tags += GETJOCTET(data[firstoffset]);
+  }
+  if (number_of_tags == 0) return;
+  firstoffset += 2;
+
+  /* Search for ExifSubIFD offset Tag in IFD0 */
+  for (;;) {
+    if (firstoffset > length - 12) return; /* check end of data segment */
+    /* Get Tag number */
+    if (is_motorola) {
+      tagnum = GETJOCTET(data[firstoffset]);
+      tagnum <<= 8;
+      tagnum += GETJOCTET(data[firstoffset+1]);
+    } else {
+      tagnum = GETJOCTET(data[firstoffset+1]);
+      tagnum <<= 8;
+      tagnum += GETJOCTET(data[firstoffset]);
+    }
+    if (tagnum == 0x8769) break; /* found ExifSubIFD offset Tag */
+    if (--number_of_tags == 0) return;
+    firstoffset += 12;
+  }
+
+  /* Get the ExifSubIFD offset */
+  if (is_motorola) {
+    if (GETJOCTET(data[firstoffset+8]) != 0) return;
+    if (GETJOCTET(data[firstoffset+9]) != 0) return;
+    offset = GETJOCTET(data[firstoffset+10]);
+    offset <<= 8;
+    offset += GETJOCTET(data[firstoffset+11]);
+  } else {
+    if (GETJOCTET(data[firstoffset+11]) != 0) return;
+    if (GETJOCTET(data[firstoffset+10]) != 0) return;
+    offset = GETJOCTET(data[firstoffset+9]);
+    offset <<= 8;
+    offset += GETJOCTET(data[firstoffset+8]);
+  }
+  if (offset > length - 2) return; /* check end of data segment */
 
-  /* We have to compute max_v_samp_factor ourselves,
-   * because it hasn't been set yet in the destination
-   * (and we don't want to use the source's value).
-   */
-  max_v_samp_factor = 1;
-  for (ci = 0; ci < dstinfo->num_components; ci++) {
-    int v_samp_factor = dstinfo->comp_info[ci].v_samp_factor;
-    max_v_samp_factor = MAX(max_v_samp_factor, v_samp_factor);
-  }
-  MCU_rows = dstinfo->image_height / (max_v_samp_factor * DCTSIZE);
-  if (MCU_rows > 0)		/* can't trim to 0 pixels */
-    dstinfo->image_height = MCU_rows * (max_v_samp_factor * DCTSIZE);
+  /* Get the number of directory entries contained in this SubIFD */
+  if (is_motorola) {
+    number_of_tags = GETJOCTET(data[offset]);
+    number_of_tags <<= 8;
+    number_of_tags += GETJOCTET(data[offset+1]);
+  } else {
+    number_of_tags = GETJOCTET(data[offset+1]);
+    number_of_tags <<= 8;
+    number_of_tags += GETJOCTET(data[offset]);
+  }
+  if (number_of_tags < 2) return;
+  offset += 2;
+
+  /* Search for ExifImageWidth and ExifImageHeight Tags in this SubIFD */
+  do {
+    if (offset > length - 12) return; /* check end of data segment */
+    /* Get Tag number */
+    if (is_motorola) {
+      tagnum = GETJOCTET(data[offset]);
+      tagnum <<= 8;
+      tagnum += GETJOCTET(data[offset+1]);
+    } else {
+      tagnum = GETJOCTET(data[offset+1]);
+      tagnum <<= 8;
+      tagnum += GETJOCTET(data[offset]);
+    }
+    if (tagnum == 0xA002 || tagnum == 0xA003) {
+      if (tagnum == 0xA002)
+	new_value = new_width; /* ExifImageWidth Tag */
+      else
+	new_value = new_height; /* ExifImageHeight Tag */
+      if (is_motorola) {
+	data[offset+2] = 0; /* Format = unsigned long (4 octets) */
+	data[offset+3] = 4;
+	data[offset+4] = 0; /* Number Of Components = 1 */
+	data[offset+5] = 0;
+	data[offset+6] = 0;
+	data[offset+7] = 1;
+	data[offset+8] = 0;
+	data[offset+9] = 0;
+	data[offset+10] = (JOCTET)((new_value >> 8) & 0xFF);
+	data[offset+11] = (JOCTET)(new_value & 0xFF);
+      } else {
+	data[offset+2] = 4; /* Format = unsigned long (4 octets) */
+	data[offset+3] = 0;
+	data[offset+4] = 1; /* Number Of Components = 1 */
+	data[offset+5] = 0;
+	data[offset+6] = 0;
+	data[offset+7] = 0;
+	data[offset+8] = (JOCTET)(new_value & 0xFF);
+	data[offset+9] = (JOCTET)((new_value >> 8) & 0xFF);
+	data[offset+10] = 0;
+	data[offset+11] = 0;
+      }
+    }
+    offset += 12;
+  } while (--number_of_tags);
 }
 
 
@@ -736,18 +1650,22 @@
 {
   /* If force-to-grayscale is requested, adjust destination parameters */
   if (info->force_grayscale) {
-    /* We use jpeg_set_colorspace to make sure subsidiary settings get fixed
-     * properly.  Among other things, the target h_samp_factor & v_samp_factor
-     * will get set to 1, which typically won't match the source.
-     * In fact we do this even if the source is already grayscale; that
-     * provides an easy way of coercing a grayscale JPEG with funny sampling
-     * factors to the customary 1,1.  (Some decoders fail on other factors.)
+    /* First, ensure we have YCbCr or grayscale data, and that the source's
+     * Y channel is full resolution.  (No reasonable person would make Y
+     * be less than full resolution, so actually coping with that case
+     * isn't worth extra code space.  But we check it to avoid crashing.)
      */
-    if ((dstinfo->jpeg_color_space == JCS_YCbCr &&
-	 dstinfo->num_components == 3) ||
-	(dstinfo->jpeg_color_space == JCS_GRAYSCALE &&
-	 dstinfo->num_components == 1)) {
-      /* We have to preserve the source's quantization table number. */
+    if (((dstinfo->jpeg_color_space == JCS_YCbCr &&
+	  dstinfo->num_components == 3) ||
+	 (dstinfo->jpeg_color_space == JCS_GRAYSCALE &&
+	  dstinfo->num_components == 1)) &&
+	srcinfo->comp_info[0].h_samp_factor == srcinfo->max_h_samp_factor &&
+	srcinfo->comp_info[0].v_samp_factor == srcinfo->max_v_samp_factor) {
+      /* We use jpeg_set_colorspace to make sure subsidiary settings get fixed
+       * properly.  Among other things, it sets the target h_samp_factor &
+       * v_samp_factor to 1, which typically won't match the source.
+       * We have to preserve the source's quantization table number, however.
+       */
       int sv_quant_tbl_no = dstinfo->comp_info[0].quant_tbl_no;
       jpeg_set_colorspace(dstinfo, JCS_GRAYSCALE);
       dstinfo->comp_info[0].quant_tbl_no = sv_quant_tbl_no;
@@ -755,48 +1673,54 @@
       /* Sorry, can't do it */
       ERREXIT(dstinfo, JERR_CONVERSION_NOTIMPL);
     }
+  } else if (info->num_components == 1) {
+    /* For a single-component source, we force the destination sampling factors
+     * to 1x1, with or without force_grayscale.  This is useful because some
+     * decoders choke on grayscale images with other sampling factors.
+     */
+    dstinfo->comp_info[0].h_samp_factor = 1;
+    dstinfo->comp_info[0].v_samp_factor = 1;
   }
 
-  /* Correct the destination's image dimensions etc if necessary */
+  /* Correct the destination's image dimensions etc as necessary
+   * for crop and rotate/flip operations.
+   */
+  dstinfo->image_width = info->output_width;
+  dstinfo->image_height = info->output_height;
   switch (info->transform) {
-  case JXFORM_NONE:
-    /* Nothing to do */
-    break;
-  case JXFORM_FLIP_H:
-    if (info->trim)
-      trim_right_edge(dstinfo);
-    break;
-  case JXFORM_FLIP_V:
-    if (info->trim)
-      trim_bottom_edge(dstinfo);
-    break;
   case JXFORM_TRANSPOSE:
-    transpose_critical_parameters(dstinfo);
-    /* transpose does NOT have to trim anything */
-    break;
   case JXFORM_TRANSVERSE:
-    transpose_critical_parameters(dstinfo);
-    if (info->trim) {
-      trim_right_edge(dstinfo);
-      trim_bottom_edge(dstinfo);
-    }
-    break;
   case JXFORM_ROT_90:
-    transpose_critical_parameters(dstinfo);
-    if (info->trim)
-      trim_right_edge(dstinfo);
-    break;
-  case JXFORM_ROT_180:
-    if (info->trim) {
-      trim_right_edge(dstinfo);
-      trim_bottom_edge(dstinfo);
-    }
-    break;
   case JXFORM_ROT_270:
     transpose_critical_parameters(dstinfo);
-    if (info->trim)
-      trim_bottom_edge(dstinfo);
     break;
+  case JXFORM_DROP:
+    if (info->drop_width != 0 && info->drop_height != 0)
+      adjust_quant(srcinfo, src_coef_arrays,
+		   info->drop_ptr, info->drop_coef_arrays,
+		   info->trim, dstinfo);
+    break;
+  }
+
+  /* Adjust Exif properties */
+  if (srcinfo->marker_list != NULL &&
+      srcinfo->marker_list->marker == JPEG_APP0+1 &&
+      srcinfo->marker_list->data_length >= 6 &&
+      GETJOCTET(srcinfo->marker_list->data[0]) == 0x45 &&
+      GETJOCTET(srcinfo->marker_list->data[1]) == 0x78 &&
+      GETJOCTET(srcinfo->marker_list->data[2]) == 0x69 &&
+      GETJOCTET(srcinfo->marker_list->data[3]) == 0x66 &&
+      GETJOCTET(srcinfo->marker_list->data[4]) == 0 &&
+      GETJOCTET(srcinfo->marker_list->data[5]) == 0) {
+    /* Suppress output of JFIF marker */
+    dstinfo->write_JFIF_header = FALSE;
+    /* Adjust Exif image parameters */
+    if (dstinfo->image_width != srcinfo->image_width ||
+	dstinfo->image_height != srcinfo->image_height)
+      /* Align data segment to start of TIFF structure for parsing */
+      adjust_exif_parameters(srcinfo->marker_list->data + 6,
+	srcinfo->marker_list->data_length - 6,
+	dstinfo->image_width, dstinfo->image_height);
   }
 
   /* Return the appropriate output data set */
@@ -816,38 +1740,114 @@
  */
 
 GLOBAL(void)
-jtransform_execute_transformation (j_decompress_ptr srcinfo,
-				   j_compress_ptr dstinfo,
-				   jvirt_barray_ptr *src_coef_arrays,
-				   jpeg_transform_info *info)
+jtransform_execute_transform (j_decompress_ptr srcinfo,
+			      j_compress_ptr dstinfo,
+			      jvirt_barray_ptr *src_coef_arrays,
+			      jpeg_transform_info *info)
 {
   jvirt_barray_ptr *dst_coef_arrays = info->workspace_coef_arrays;
 
+  /* Note: conditions tested here should match those in switch statement
+   * in jtransform_request_workspace()
+   */
   switch (info->transform) {
   case JXFORM_NONE:
+    if (info->x_crop_offset != 0 || info->y_crop_offset != 0 ||
+	info->output_width > srcinfo->image_width ||
+	info->output_height > srcinfo->image_height)
+      do_crop(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+	      src_coef_arrays, dst_coef_arrays);
     break;
   case JXFORM_FLIP_H:
-    do_flip_h(srcinfo, dstinfo, src_coef_arrays);
+    if (info->y_crop_offset != 0)
+      do_flip_h(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+		src_coef_arrays, dst_coef_arrays);
+    else
+      do_flip_h_no_crop(srcinfo, dstinfo, info->x_crop_offset,
+			src_coef_arrays);
     break;
   case JXFORM_FLIP_V:
-    do_flip_v(srcinfo, dstinfo, src_coef_arrays, dst_coef_arrays);
+    do_flip_v(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+	      src_coef_arrays, dst_coef_arrays);
     break;
   case JXFORM_TRANSPOSE:
-    do_transpose(srcinfo, dstinfo, src_coef_arrays, dst_coef_arrays);
+    do_transpose(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+		 src_coef_arrays, dst_coef_arrays);
     break;
   case JXFORM_TRANSVERSE:
-    do_transverse(srcinfo, dstinfo, src_coef_arrays, dst_coef_arrays);
+    do_transverse(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+		  src_coef_arrays, dst_coef_arrays);
     break;
   case JXFORM_ROT_90:
-    do_rot_90(srcinfo, dstinfo, src_coef_arrays, dst_coef_arrays);
+    do_rot_90(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+	      src_coef_arrays, dst_coef_arrays);
     break;
   case JXFORM_ROT_180:
-    do_rot_180(srcinfo, dstinfo, src_coef_arrays, dst_coef_arrays);
+    do_rot_180(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+	       src_coef_arrays, dst_coef_arrays);
+    break;
+  case JXFORM_ROT_270:
+    do_rot_270(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+	       src_coef_arrays, dst_coef_arrays);
     break;
+  case JXFORM_DROP:
+    if (info->drop_width != 0 && info->drop_height != 0)
+      do_drop(srcinfo, dstinfo, info->x_crop_offset, info->y_crop_offset,
+	      src_coef_arrays, info->drop_ptr, info->drop_coef_arrays,
+	      info->drop_width, info->drop_height);
+    break;
+  }
+}
+
+/* jtransform_perfect_transform
+ *
+ * Determine whether lossless transformation is perfectly
+ * possible for a specified image and transformation.
+ *
+ * Inputs:
+ *   image_width, image_height: source image dimensions.
+ *   MCU_width, MCU_height: pixel dimensions of MCU.
+ *   transform: transformation identifier.
+ * Parameter sources from initialized jpeg_struct
+ * (after reading source header):
+ *   image_width = cinfo.image_width
+ *   image_height = cinfo.image_height
+ *   MCU_width = cinfo.max_h_samp_factor * DCTSIZE
+ *   MCU_height = cinfo.max_v_samp_factor * DCTSIZE
+ * Result:
+ *   TRUE = perfect transformation possible
+ *   FALSE = perfect transformation not possible
+ *           (may use custom action then)
+ */
+
+GLOBAL(boolean)
+jtransform_perfect_transform(JDIMENSION image_width, JDIMENSION image_height,
+			     int MCU_width, int MCU_height,
+			     JXFORM_CODE transform)
+{
+  boolean result = TRUE; /* initialize TRUE */
+
+  switch (transform) {
+  case JXFORM_FLIP_H:
   case JXFORM_ROT_270:
-    do_rot_270(srcinfo, dstinfo, src_coef_arrays, dst_coef_arrays);
+    if (image_width % (JDIMENSION) MCU_width)
+      result = FALSE;
+    break;
+  case JXFORM_FLIP_V:
+  case JXFORM_ROT_90:
+    if (image_height % (JDIMENSION) MCU_height)
+      result = FALSE;
+    break;
+  case JXFORM_TRANSVERSE:
+  case JXFORM_ROT_180:
+    if (image_width % (JDIMENSION) MCU_width)
+      result = FALSE;
+    if (image_height % (JDIMENSION) MCU_height)
+      result = FALSE;
     break;
   }
+
+  return result;
 }
 
 #endif /* TRANSFORMS_SUPPORTED */
