# ex:ts=8
# Ports collection makefile for:	csmash
# Date created:			Nov 11, 2003
# Whom:				ijliao
#
# $FreeBSD$
#

PORTNAME=	csmash
PORTVERSION=	0.6.6
PORTREVISION=	16
CATEGORIES=	games
MASTER_SITES=	SF/cannonsmash/CannonSmash/${PORTVERSION}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	A 3D tabletennis game

USE_GETTEXT=	yes
USE_SDL=	image mixer sdl
USE_GL=		yes
USE_GNOME=	gtk20
USE_GMAKE=	yes
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include -I${LOCALBASE}/include/SDL
LDFLAGS+=	-L${LOCALBASE}/lib

post-patch:
	@${FIND} ${WRKSRC} -name "Makefile.in" | ${XARGS} ${REINPLACE_CMD} -e \
		's|/games/|/|g'
	@${REINPLACE_CMD} -e 's|BallView::||' ${WRKSRC}/BallView.h
	@${REINPLACE_CMD} -e 's|== 3|>= 3|' ${WRKSRC}/matrix

.include <bsd.port.mk>
