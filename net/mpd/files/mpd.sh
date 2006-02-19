#!/bin/sh
#
# $FreeBSD: ports/net/mpd/files/mpd.sh,v 1.3 2006/01/07 06:29:53 dougb Exp $
#
# PROVIDE: mpd
# REQUIRE: NETWORKING syslogd
# KEYWORD: FreeBSD
#
# Add the following line to /etc/rc.conf to enable mpd:
#
# mpd_enable="YES"
#

mpd_flags="${mpd_flags:--b}"
mpd_enable="${mpd_enable-NO}"

. %%RC_SUBR%%

name=mpd
rcvar=`set_rcvar`

prefix=%%PREFIX%%
procname=${prefix}/sbin/mpd
pidfile=/var/run/mpd.pid
required_files="${prefix}/etc/mpd/mpd.conf ${prefix}/etc/mpd/mpd.links"
command="${prefix}/sbin/mpd"

load_rc_config ${name}
run_rc_command "$1"
