#!/bin/sh
#
# $FreeBSD: ports/security/clamav/files/clamav-clamd.sh,v 1.3 2004/04/19 15:21:33 eik Exp $
#

# PROVIDE: clamd
# REQUIRE: LOGIN
# BEFORE: mail
# KEYWORD: FreeBSD shutdown

#
# Add the following lines to /etc/rc.conf to enable clamd:
#
#clamav_clamd_enable="YES"
#
# See clamd(8) for flags
#

. %%RC_SUBR%%

name=clamav_clamd
rcvar=`set_rcvar`

command=%%PREFIX%%/sbin/clamd
pidfile=/var/run/clamav/clamd.pid
required_dirs=%%DATADIR%%
required_files=%%PREFIX%%/etc/clamd.conf

# read settings, set default values
load_rc_config "$name"
: ${clamav_clamd_enable="NO"}
: ${clamav_clamd_flags=""}
: ${clamav_clamd_socket="%%CLAMAV_CLAMD_SOCKET%%"}

run_rc_command "$1"
