# New ports collection makefile for:    par2cmdline-tbb
# Whom:                                 Ganael Laplanche <ganael.laplanche@martymac.org>
# Date created:                         27 December 2007
#
# $FreeBSD$
#

PORTNAME=	par2cmdline-tbb
PORTVERSION=	20100203
PORTREVISION=	5
CATEGORIES=	archivers
MASTER_SITES=	http://chuchusoft.com/par2_tbb/
DISTNAME=	par2cmdline-0.4-tbb-${PORTVERSION}

MAINTAINER=	martymac@FreeBSD.org
COMMENT=	Tools for using Parity v2 Archive files (tbb version)

LIB_DEPENDS=	tbb.4:${PORTSDIR}/devel/tbb

LICENSE=	GPLv2

ONLY_FOR_ARCHS=	i386 amd64 ia64
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib ${PTHREAD_LIBS}
USE_AUTOTOOLS=	aclocal automake autoconf
USE_GCC=	4.2+

PORTDOCS=	README
CONFLICTS=	par2cmdline-[0-9]*
PLIST_FILES=	bin/par2 bin/par2create bin/par2repair bin/par2verify

post-extract:
	@${CHMOD} u+x ${WRKSRC}/install-sh

post-install:
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/README ${DOCSDIR}
.endif

.include <bsd.port.mk>
