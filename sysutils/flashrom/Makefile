# New ports collection makefile for:	flashrom
# Date created:				06 May 2009
# Whom:					Alexander Logvinov <ports@logvinov.com>
#
# $FreeBSD$
#

PORTNAME=	flashrom
PORTVERSION=	0.9.4
CATEGORIES=	sysutils
MASTER_SITES=	http://download.flashrom.org/releases/

MAINTAINER=	avl@FreeBSD.org
COMMENT=	A utility for reading, writing, verifying and erasing flash ROM chips

LIB_DEPENDS=	pci.3:${PORTSDIR}/devel/libpci

OPTIONS=	DMIDECODE	"Use dmidecode to gather DMI information"	on \
		FTDI		"Enable external FT2232SPI flasher support"	off

USE_BZIP2=	yes
USE_GMAKE=	yes

ONLY_FOR_ARCHS=	amd64 i386

PORTDOCS=	ChangeLog COPYING README
PLIST_FILES=	bin/${PORTNAME}

LICENSE=	GPLv2
LICENSE_FILE=	${WRKSRC}/COPYING

.if !defined(NO_INSTALL_MANPAGES)
MAN8=		${PORTNAME}.8
.endif

.include <bsd.port.options.mk>

.if !defined(WITHOUT_DMIDECODE)
RUN_DEPENDS=	dmidecode:${PORTSDIR}/sysutils/dmidecode
.endif

.if defined(WITH_FTDI)
USE_GNOME=	pkgconfig
LIB_DEPENDS+=	ftdi.18:${PORTSDIR}/devel/libftdi
.endif

post-patch:
.if defined(WITH_FTDI)
	@${REINPLACE_CMD} -e 's|-lusb")|-lusb") -lz|' ${WRKSRC}/Makefile
.else
	@${REINPLACE_CMD} -e 's|2_SPI ?= yes|2_SPI ?= no|' ${WRKSRC}/Makefile
.endif

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/${PORTNAME} ${PREFIX}/bin
.if !defined(NO_INSTALL_MANPAGES)
	${MKDIR} ${MANPREFIX}/man/man8
	${INSTALL_MAN} ${WRKSRC}/${MAN8} ${MANPREFIX}/man/man8
.endif
.if !defined(NOPORTDOCS)
	${MKDIR} ${DOCSDIR}
.for i in ${PORTDOCS}
	${INSTALL_DATA} ${WRKSRC}/${i} ${DOCSDIR}
.endfor
.endif

.include <bsd.port.mk>
