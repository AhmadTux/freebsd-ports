# New ports collection makefile for:	www/joomla
# Date created:				Sat Out 15, 2005
# Whom:					Francisco Cabrita <include@npf.pt.freebsd.org>
#
# $FreeBSD$

PORTNAME=	joomla
PORTVERSION=	1.7.5
CATEGORIES=	www
MASTER_SITES=	http://downloads.joomlacode.org/frsrelease/7/2/0/72049/
DISTNAME=	Joomla_${PORTVERSION}-Stable-Full_Package

MAINTAINER=	wen@FreeBSD.org
COMMENT=	A dynamic web content management system (CMS)

CONFLICTS=	joomla-2.*.*

NO_BUILD=	yes
FETCH_ARGS=	-apR
USE_ZIP=	yes
USE_MYSQL=	yes
USE_PHP=	mysql pcre xml zlib session gd pdf json mbstring \
		simplexml
WANT_PHP_WEB=	yes
PKGMESSAGE=	${WRKDIR}/pkg-message
SUB_FILES=	pkg-message

do-extract:
	@${MKDIR} ${WRKSRC}
	@${TAR} -yxf ${DISTDIR}/${DISTNAME}${EXTRACT_SUFX} -C ${WRKSRC}

do-install:
	@${MKDIR} ${WWWDIR}
	@cd ${WRKSRC} && \
		${FIND} . -type d -exec ${MKDIR} ${WWWDIR}/{} \; \
			-exec ${CHOWN} ${WWWOWN}:${WWWGRP} ${WWWDIR}/{} \;
	@cd ${WRKSRC} && \
		${FIND} . \! -type d -exec ${INSTALL_DATA} {} ${WWWDIR}/{} \; \
			-exec ${CHOWN} ${WWWOWN}:${WWWGRP} ${WWWDIR}/{} \;

post-install:
	@${CAT} ${PKGMESSAGE}

.include <bsd.port.mk>
