# New ports collection makefile for:	libclaw
# Date created:		28 Mar 2008
# Whom:			Dmitry Marakasov <amdmi3@amdmi3.ru>
#
# $FreeBSD$
#

PORTNAME=	libclaw
PORTVERSION=	1.5.3
PORTREVISION=	4
CATEGORIES=	devel
MASTER_SITES=	SF

MAINTAINER=	amdmi3@FreeBSD.org
COMMENT=	C++ Library Absolutely Wonderful

LIB_DEPENDS=	jpeg.11:${PORTSDIR}/graphics/jpeg \
		png.6:${PORTSDIR}/graphics/png

USE_BZIP2=	yes

USE_CMAKE=	yes
CMAKE_ARGS=	-DCLAW_CMAKE_MODULES_INSTALL_PATH="${PREFIX}/lib/claw"
USE_LDCONFIG=	yes

post-patch:
	@${REINPLACE_CMD} -e '/INSTALL.*COPYING/ d' ${WRKSRC}/CMakeLists.txt

.include <bsd.port.mk>
