# New ports collection makefile for:	ghostess
# Date created:		2007-01-13
# Whom:			trasz <trasz@pin.if.uz.zgora.pl>
#
# $FreeBSD$
#

PORTNAME=	ghostess
PORTVERSION=	20120105
CATEGORIES=	audio
MASTER_SITES=	http://www.smbolton.com/linux/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Graphical DSSI host with JACK MIDI support

LICENSE=	GPLv2 BSD
LICENSE_COMB=	multi

BUILD_DEPENDS=	${LOCALBASE}/include/dssi.h:${PORTSDIR}/audio/dssi \
		${LOCALBASE}/include/ladspa.h:${PORTSDIR}/audio/ladspa \
		jackit>=0.109.2:${PORTSDIR}/audio/jack
# Between 0.109.0 and earlier versions there was an api change in JACK.
# However, the library version number was not changed.  The line above
# is supposed to work around that.
LIB_DEPENDS=	dssialsacompat.0:${PORTSDIR}/audio/libdssialsacompat \
		lo.7:${PORTSDIR}/audio/liblo \
		jack.0:${PORTSDIR}/audio/jack

USE_BZIP2=	yes
USE_GNOME=	gtk20
USE_GMAKE=	yes
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-jackmidi
MAKE_JOBS_SAFE=	yes

MAN1=		ghostess.1
PLIST_FILES=	bin/ghostess bin/ghostess_universal_gui

CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

post-patch:
	@${REINPLACE_CMD} -e \
		'/LDADD/s| -ldl | |' ${WRKSRC}/src/Makefile.in

.include <bsd.port.mk>
