
$FreeBSD: ports/www/oops/files/patch-src_modules_transparent.c,v 1.1 2005/05/20 17:53:56 pav Exp $

--- src/modules/transparent.c.orig
+++ src/modules/transparent.c
@@ -214,7 +214,7 @@
 	natLookup.nl_outip = rq->client_sa.sin_addr;
 	natLookup.nl_flags = IPN_TCP;
 	if (natfd < 0) {
-	    natfd = open(IPL_NAT, O_RDONLY, 0);
+	    natfd = open(IPNAT_NAME, O_RDONLY, 0);
 	    if (natfd < 0) {
 		my_xlog(OOPS_LOG_HTTP|OOPS_LOG_DBG|OOPS_LOG_SEVERE, "transparent/redir(): NAT open failed: %m\n");
 		goto notdone;
@@ -270,7 +270,7 @@
 	natLookup.nl_outip = rq->client_sa.sin_addr;
 	natLookup.nl_flags = IPN_TCP;
 	if (natfd < 0) {
-	    natfd = open(IPL_NAT, O_RDONLY, 0);
+	    natfd = open(IPNAT_NAME, O_RDONLY, 0);
 	    if (natfd < 0) {
 		my_xlog(OOPS_LOG_HTTP|OOPS_LOG_DBG|OOPS_LOG_SEVERE, "transparent/redir(): NAT open failed: %m\n");
 		goto notdone;
