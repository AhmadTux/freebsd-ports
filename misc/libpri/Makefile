# New ports collection makefile for:	libpri
# Date created:				25 Oct 2004
# Whom:					Alexander Timoshenko <gonzo@univ.kiev.ua>
#
# $FreeBSD$
#

PORTNAME=	libpri
PORTVERSION=	1.4.12
CATEGORIES=	misc
MASTER_SITES=	http://downloads.asterisk.org/pub/telephony/libpri/releases/

MAINTAINER=	fjoe@FreeBSD.org
COMMENT=	A C implementation of the Primary Rate ISDN specification

USE_GMAKE=	yes
USE_LDCONFIG=	yes
PLIST_FILES=	include/libpri.h lib/libpri.a lib/libpri.so \
		lib/libpri.so.1

ONLY_FOR_ARCHS=	amd64 i386 sparc64

.include <bsd.port.mk>
