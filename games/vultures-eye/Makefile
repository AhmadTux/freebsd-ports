# New ports collection makefile for:	vultures-eye
# Date created:				13 April 2006
# Whom:					Leland Wang <llwang@infor.org>
#
# $FreeBSD$
#

PORTNAME=	vultures
PORTVERSION=	2.1.0
PORTREVISION=	9
CATEGORIES=	games
MASTER_SITES=	http://www.darkarts.co.za/projects/vultures/attachment/wiki/downloads/${PORTVERSION}/
PKGNAMESUFFIX=	-eye
DISTFILES=	${DISTNAME}-full${EXTRACT_SUFX}?format=raw

MAINTAINER=	llwang@infor.org
COMMENT=	A graphical interface for NetHack 3.4.3 forked from Falcon's Eye

LIB_DEPENDS=	png.6:${PORTSDIR}/graphics/png

LATEST_LINK=	${PORTNAME}${PKGNAMESUFFIX}

USE_GMAKE=	yes
USE_SDL=	sdl mixer ttf
USE_BZIP2=	yes
WRKSRC=		${WRKDIR}/${DISTNAME}/nethack
HAS_CONFIGURE=	yes
CONFIGURE_WRKSRC=	${WRKSRC}/sys/unix
CONFIGURE_SCRIPT=	setup.sh
DATADIR=	${PREFIX}/share/${PORTNAME}${PKGNAMESUFFIX}
MAKE_ENV+=	DATADIR="${DATADIR}" MAKE="${GMAKE}"
MAKE_JOBS_UNSAFE=	yes

post-patch:
	@${REINPLACE_CMD} -e \
		's|png_infopp_NULL|NULL|g' \
		${WRKSRC}/win/vultures/util/tiletrans.c

pre-configure:
	@${CHMOD} 755 ${CONFIGURE_WRKSRC}/${CONFIGURE_SCRIPT}

.include <bsd.port.mk>
