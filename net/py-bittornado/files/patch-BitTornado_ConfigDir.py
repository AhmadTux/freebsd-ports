
$FreeBSD: ports/net/py-bittornado/files/patch-BitTornado_ConfigDir.py,v 1.1 2004/05/25 13:47:28 pav Exp $

--- BitTornado/ConfigDir.py.orig	Sun Jul 11 04:57:30 2004
+++ BitTornado/ConfigDir.py	Thu Jul 15 12:03:50 2004
@@ -19,7 +19,7 @@
     realpath = os.path.realpath
 except:
     realpath = lambda x:x
-OLDICONPATH = os.path.abspath(os.path.dirname(realpath(sys.argv[0])))
+OLDICONPATH="%%PREFIX%%/share/BitTornado"
 
 DIRNAME = '.'+product_name
 
