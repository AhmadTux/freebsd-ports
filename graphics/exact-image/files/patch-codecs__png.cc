--- codecs/png.cc.orig	2009-04-27 19:52:17.000000000 +0200
+++ codecs/png.cc	2010-03-29 15:07:59.000000000 +0200
@@ -71,7 +71,7 @@
   /* Allocate/initialize the memory for image information.  REQUIRED. */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
-    png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
+    png_destroy_read_struct(&png_ptr, NULL, NULL);
     return 0;
   }
   
@@ -82,7 +82,7 @@
   
   if (setjmp(png_jmpbuf(png_ptr))) {
     /* Free all of the memory associated with the png_ptr and info_ptr */
-    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
+    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
     /* If we get here, we had a problem reading the file */
     return 0;
   }
@@ -99,7 +99,7 @@
   png_read_info (png_ptr, info_ptr);
   
   png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
-		&interlace_type, int_p_NULL, int_p_NULL);
+		&interlace_type, NULL, NULL);
   
   image.w = width;
   image.h = height;
@@ -132,7 +132,7 @@
 #if 0 // no longer needed
   /* Expand grayscale images to the full 8 bits from 2, or 4 bits/pixel */
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth > 1 && bit_depth < 8) {
-    png_set_gray_1_2_4_to_8(png_ptr);
+    png_set_expand_gray_1_2_4_to_8(png_ptr);
     image.bps = 8;
   }
 #endif  
@@ -196,11 +196,11 @@
   for (int pass = 0; pass < number_passes; ++pass)
     for (unsigned int y = 0; y < height; ++y) {
       row_pointers[0] = image.getRawData() + y * stride;
-      png_read_rows(png_ptr, row_pointers, png_bytepp_NULL, 1);
+      png_read_rows(png_ptr, row_pointers, NULL, 1);
     }
   
   /* clean up after the read, and free any memory allocated - REQUIRED */
-  png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
+  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   
   /* that's it */
   return true;
@@ -224,7 +224,7 @@
   /* Allocate/initialize the memory for image information.  REQUIRED. */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
-    png_destroy_write_struct(&png_ptr, png_infopp_NULL);
+    png_destroy_write_struct(&png_ptr, NULL);
     return false;
   }
   
@@ -244,7 +244,6 @@
   else if (quality > Z_BEST_COMPRESSION) quality = Z_BEST_COMPRESSION;
   png_set_compression_level(png_ptr, quality);
   
-  png_info_init (info_ptr);
   
   /* Set up our STL stream output control */ 
   png_set_write_fn (png_ptr, stream, &stdstream_write_data, &stdstream_flush_data);
