#!/bin/sh
# $FreeBSD: ports/mail/nullmailer/files/nullmail.sh,v 1.5 2004/10/12 09:09:24 clive Exp $
#
# PROVIDE: nullmailer
# REQUIRE: DAEMON
# KEYWORD: FreeBSD
#
# Add the following line to /etc/rc.conf to enable nullmailer:
#
# nullmailer_enable="YES"
#

. %%RC_SUBR%%

name=nullmailer
rcvar=`set_rcvar`
nullmailer_user=nullmail
command=%%PREFIX%%/sbin/nullmailer-send
command_args=" 2>&1 | /usr/bin/logger -i -p local5.info -t nullmail &"
load_rc_config $name
run_rc_command "$1"

