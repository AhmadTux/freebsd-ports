# New ports collection makefile for: xye
# Date created:        3 May 2006
# Whom:                Robert Gogolok <gogo@cs.uni-sb.de>
#
# $FreeBSD$
#

PORTNAME=	xye
PORTVERSION=	0.9.1
PORTREVISION=	5
CATEGORIES=	games
MASTER_SITES=	SF

MAINTAINER=	gogo@cs.uni-sb.de
COMMENT=	A puzzle game for X

USE_SDL=	sdl image ttf
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-docdir=${PREFIX}/share/doc
USE_GMAKE=	yes
MAKE_JOBS_SAFE=	yes

.ifdef NOPORTDOCS
post-patch:
	@${REINPLACE_CMD} '/install-data-am:/s/install-doceDATA//' \
		${WRKSRC}/Makefile.in
.endif

.include <bsd.port.mk>
