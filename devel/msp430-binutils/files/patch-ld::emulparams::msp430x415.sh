--- ld/emulparams/msp430x415.sh.orig	Mon Mar  1 16:23:31 2004
+++ ld/emulparams/msp430x415.sh	Mon Mar  1 16:23:22 2004
@@ -0,0 +1,15 @@
+ARCH=msp:41
+MACHINE=
+SCRIPT_NAME=elf32msp430
+OUTPUT_FORMAT="elf32-msp430"
+MAXPAGESIZE=1
+EMBEDDED=yes
+TEMPLATE_NAME=generic
+
+ROM_START=0xc000
+ROM_SIZE=0x3fe0
+RAM_START=0x0200
+RAM_SIZE=512
+
+STACK=0x400
+
