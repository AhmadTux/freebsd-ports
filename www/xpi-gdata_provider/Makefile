# New ports collection makefile for:	xpi-gdata_provider
# Date created:			21 Nov 2011
# Whom:				Alex Dupre <ale@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	gdata_provider
PORTVERSION=	0.9
PORTREVISION=	2
DISTVERSIONSUFFIX=	-sm+tb
CATEGORIES=	www deskutils

MAINTAINER=	ale@FreeBSD.org
COMMENT=	Allows bidirectional access to Google Calendar

XPI_DISTNAME=	provider_for_google_calendar
XPI_NUM=	4631
XPI_ID=		{a62ef8ec-5fdc-40c2-873c-223b8a6925cc}

post-patch:
	@${REINPLACE_CMD} -i "" "s/8\.\*/10.*/;s/2\.5\.\*/2.7.*/;s/1\.0\.\*/1.2.*/" \
		${WRKSRC}/install.rdf

.include "../xpi-adblock/Makefile.xpi"
.include <bsd.port.mk>
