# New ports collection makefile for:	p5-MediaWiki
# Date created:		2006-08-15
# Whom:			Gea-Suan Lin <gslin@gslin.org>
#
# $FreeBSD$
#

PORTNAME=	MediaWiki
PORTVERSION=	1.13
CATEGORIES=	www perl5
MASTER_SITES=	CPAN
MASTER_SITE_SUBDIR=	CPAN:SPECTRUM
PKGNAMEPREFIX=	p5-

MAINTAINER=	perl@FreeBSD.org
COMMENT=	OOP MediaWiki engine client

RUN_DEPENDS=	${SITE_PERL}/Config/IniHash.pm:${PORTSDIR}/devel/p5-Config-IniHash \
		${SITE_PERL}/HTTP/Request/Common.pm:${PORTSDIR}/www/p5-libwww \
		${SITE_PERL}/LWP.pm:${PORTSDIR}/www/p5-libwww
BUILD_DEPENDS=	${RUN_DEPENDS}

PERL_CONFIGURE=	yes

MAN3=		MediaWiki.3

.include <bsd.port.mk>
