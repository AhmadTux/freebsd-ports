# New ports collection makefile for: hs-wxdirect
# Date created:        January 26, 2012
# Whom:                haskell@FreeBSD.org
#
# $FreeBSD$
#

PORTNAME=	wxdirect
PORTVERSION=	0.13.1.1
CATEGORIES=	x11-toolkits haskell

MAINTAINER=	haskell@FreeBSD.org
COMMENT=	Helper tool for building wxHaskell

LICENSE=	BSD

CABAL_SETUP=	Setup.hs

USE_CABAL=	parsec>=2.1.0 strict>=0.3

EXECUTABLE=	wxdirect

.include "${.CURDIR}/../../lang/ghc/bsd.cabal.mk"
.include <bsd.port.mk>
