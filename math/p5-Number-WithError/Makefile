# New ports collection makefile for:	Number-WithError
# Date created:				15 May 2006
# Whom:					Aaron Dalton <aaron@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	Number-WithError
PORTVERSION=	1.01
CATEGORIES=	math perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	culot@FreeBSD.org
COMMENT=	Numbers with error propagation and scientific rounding

BUILD_DEPENDS=	${SITE_PERL}/${PERL_ARCH}/Params/Util.pm:${PORTSDIR}/devel/p5-Params-Util \
		${SITE_PERL}/prefork.pm:${PORTSDIR}/devel/p5-prefork
RUN_DEPENDS:=	${BUILD_DEPENDS}

MAN3=	Number::WithError.3

PERL_CONFIGURE=	yes

.include <bsd.port.mk>
