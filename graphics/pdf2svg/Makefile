# New ports collection makefile for:	pdf2svg
# Date created:		Mar 13 2009
# Whom:			martin.dieringer@gmx.de
#
# $FreeBSD$

PORTNAME=	pdf2svg
PORTVERSION=	0.2.1
PORTREVISION=	5
CATEGORIES=	graphics
MASTER_SITES=	http://www.cityinthesky.co.uk/_media/opensource/

MAINTAINER=	martin.dieringer@gmx.de
COMMENT=	Convert PDF to SVG

LIB_DEPENDS=	poppler-glib:${PORTSDIR}/graphics/poppler-glib \
		cairo:${PORTSDIR}/graphics/cairo \
		gtk:${PORTSDIR}/x11-toolkits/gtk20

GNU_CONFIGURE=	yes

PLIST_FILES=	bin/pdf2svg

.include <bsd.port.mk>
