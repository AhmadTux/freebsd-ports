
$FreeBSD: ports/net/linphone/files/patch-lpc10-1.5::lpcini.c,v 1.1 2003/01/19 21:01:51 sobomax Exp $

--- lpc10-1.5/lpcini.c	2002/12/30 08:26:21	1.1
+++ lpc10-1.5/lpcini.c	2002/12/30 08:28:01
@@ -32,9 +32,9 @@
 	-lf2c -lm   (in that order)
 */
 
-#include "f2c.h"
+#include <stdlib.h>
 
-#include <malloc.h>
+#include "f2c.h"
 
 /* Common Block Declarations */
 
