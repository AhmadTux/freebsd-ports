# New ports collection makefile for:	avr-gcc
# Date created:		29 Nov 2000
# Whom:			Joerg Wunsch <joerg@freebsd.org>
#
# $FreeBSD$
#

PORTNAME=	gcc-3
PORTVERSION=	3.4.6
PORTREVISION=	1
CATEGORIES=	devel
MASTER_SITES=	${MASTER_SITE_GCC}
MASTER_SITES+=	http://people.freebsd.org/~joerg/:local
MASTER_SITE_SUBDIR=	releases/${DISTNAME}
PKGNAMEPREFIX=	avr-
DISTNAME=	gcc-${DISTVERSION}
DISTFILES=	gcc-core-${PORTVERSION}${EXTRACT_SUFX} \
		gcc-g++-${PORTVERSION}${EXTRACT_SUFX}
EXTRACT_ONLY=	gcc-core-${PORTVERSION}${EXTRACT_SUFX} \
		gcc-g++-${PORTVERSION}${EXTRACT_SUFX}

MAINTAINER=	joerg@FreeBSD.org
COMMENT=	FSF GCC 3.x for Atmel AVR 8-bit RISC cross-development

BUILD_DEPENDS=	avr-as:${PORTSDIR}/devel/avr-binutils \
		avr-ld:${PORTSDIR}/devel/avr-binutils
RUN_DEPENDS=	avr-as:${PORTSDIR}/devel/avr-binutils \
		avr-ld:${PORTSDIR}/devel/avr-binutils

WRKSRC=		${WRKDIR}/gcc-${PORTVERSION}

USE_AUTOTOOLS=	autoheader213
USE_PERL5_BUILD=	yes
USE_BZIP2=	yes
USE_GMAKE=	yes
USE_BISON=	build
GNU_CONFIGURE=	yes

CONFLICTS=	avr-gcc-4*

CONFIGURE_ARGS=	--target=avr --disable-nls

MAKE_FLAGS=	LANGUAGES="c c++"
# get rid of that silly -mcpu=pentiumpro FreeBSD 5+ is so fond of. :-(
MAKE_ENV=	MACHINE_ARCH=avr

MAN1=		avr-cpp.1 avr-gcc.1 avr-g++.1 avr-gcov.1
MAN7=		fsf-funding.7 gfdl.7 gpl.7
INFO=		cpp gcc gccint cppinternals gccinstall

# As ${FILESDIR}/patch-binary-constants updates the documentation, we need to
# recreate it.
pre-build:
	cd ${WRKSRC}/gcc &&\
		makeinfo -I doc -I doc/include --no-split -o doc/gcc.info doc/gcc.texi

.include <bsd.port.mk>
