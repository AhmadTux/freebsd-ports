# New ports collection makefile for:	cd-discid
# Date created:			July 29, 2002
# Whom:				Munish Chopra <mchopra@engmail.uwaterloo.ca>
#
# $FreeBSD$
#

PORTNAME=	cd-discid
PORTVERSION=	0.9
CATEGORIES=	audio
MASTER_SITES=	http://lly.org/~rcw/cd-discid/
DISTNAME=	${PORTNAME}_${PORTVERSION}.orig

MAINTAINER=	hcoyote@ghostar.org
COMMENT=	Backend utility to retrieve CDDB discid information

WRKSRC=		${WRKDIR}/${PKGNAME}

MAKE_ARGS=	prefix=${PREFIX} mandir=${PREFIX}/man/man1

MAN1=	cd-discid.1
PLIST_FILES=	bin/cd-discid

.include <bsd.port.mk>
