# New ports collection makefile for:	devel/p5-autobox
# Date created:				25 July 2009
# Whom:					Bill Brinzer <bill.brinzer@gmail.com>
#
# $FreeBSD$
#

PORTNAME=	autobox
PORTVERSION=	2.71
CATEGORIES=	devel perl5
MASTER_SITES=	CPAN
MASTER_SITE_SUBDIR=	CPAN:CHOCOLATE
PKGNAMEPREFIX=	p5-

MAINTAINER=	bill.brinzer@gmail.com
COMMENT=	Call methods on native Perl types

BUILD_DEPENDS=	p5-Scope-Guard>=0.20:${PORTSDIR}/devel/p5-Scope-Guard
RUN_DEPENDS:=	${BUILD_DEPENDS}

PERL_CONFIGURE=	yes

MAN3=	autobox.3

.include <bsd.port.mk>
