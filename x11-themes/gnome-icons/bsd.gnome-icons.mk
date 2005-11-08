# New ports collection makefile for:	Gnome iconset
# Date created:				29 Feb 2004
# Whom:					Tom McLaughlin <tmclaugh@sdf.lonestar.org>
#
# $FreeBSD: ports/x11-themes/gnome-icons/bsd.gnome-icons.mk,v 1.2 2004/08/11 22:01:56 pav Exp $
#

# Port logic gratuitously stolen from x11-themes/kde-icons-noia by
# lioux@.

PKGNAMEPREFIX=	gnome-icons-

NO_BUILD=	yes
USE_SIZE=	yes

REASON=		Themes may contain artwork not done by the author. \
		Keep FreeBSD safe if theme author violated copyrights.

USE_X_PREFIX=	yes
PLIST=		${WRKDIR}/plist

WANT_GNOME=	yes
INSTALLS_ICONS=	yes

pre-install:
	@${RM} -rf ${PLIST}

	@cd ${WRKDIR} && ${FIND} * ! -type d | ${SORT} >> ${PLIST}; \
	${SED} -i "" -e "s:^:share/icons/:" ${PLIST}; \
	${FIND} * -type d ! -empty | ${SORT} -r | \
		${SED} -e "s:^:@dirrm share/icons/:" | \
		${GREP} / >> ${PLIST}; \
	${ECHO} "@unexec /bin/rmdir %D/share/icons 2> /dev/null || \
		/usr/bin/true" >> ${PLIST}

do-install:
	cd ${WRKDIR} && ${FIND} * -type d ! -empty \
		-exec ${MKDIR} -m 0755 \
		${PREFIX}/share/icons/"{}" \;
	cd ${WRKDIR} && ${FIND} * ! -type d ! -name 'plist' \
		-exec ${INSTALL_DATA} ${WRKDIR}/"{}" \
		${PREFIX}/share/icons/"{}" \;
