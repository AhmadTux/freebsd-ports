
$FreeBSD: ports/net/linphone/files/patch-oRTP::src::scheduler.h,v 1.1 2003/01/19 21:01:51 sobomax Exp $

--- oRTP/src/scheduler.h	2002/12/30 08:15:08	1.1
+++ oRTP/src/scheduler.h	2002/12/30 08:15:16
@@ -20,6 +20,8 @@
 #ifndef SCHEDULER_H
 #define SCHEDULER_H
 
+#include <pthread.h>
+
 #include "rtpsession.h"
 #include "rtptimer.h"
 #include "sessionset.h"
