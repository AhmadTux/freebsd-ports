#!/bin/sh
# $FreeBSD: ports/net/dictd/files/dictd.sh,v 1.2 2001/01/28 21:23:04 clive Exp $

SOCKSTAT=/usr/bin/sockstat
GREP=/usr/bin/grep
AWK=/usr/bin/awk
ECHO=/bin/echo
CAT=/bin/cat
KILL=/bin/kill
RM=/bin/rm

DICTD=%%PREFIX%%/sbin/dictd

# DICTD_OPTIONS="-put -command_line -options -for -dictd -here"
DICTD_OPTIONS=""

DICTD_PID_FILE=/var/run/dictd.pid

case "$1" in
	start)
		if [ -x $DICTD ]; then
			${ECHO} -n " dictd"
			$DICTD $DICTD_OPTIONS
			${ECHO} `${SOCKSTAT} | ${GREP} dictd | ${AWK} '{print $3}'` > ${DICTD_PID_FILE}
		else
			${ECHO} "dictd.sh: cannot find $DICTD or it's not executable"
		fi
		;;

	stop)
		if [ ! -f $DICTD_PID_FILE ]; then
			exit 0
		fi
		dictdpid=`${CAT} $DICTD_PID_FILE`
		if [ "$dictdpid" -gt 0 ]; then
			${ECHO} -n " dictd"
			${KILL} -15 $dictdpid 2>&1 > /dev/null
		fi
		${RM} -f $DICTD_PID_FILE
		;;
	*)
		echo "Usage: dictd.sh { start | stop }"
		;;
esac
