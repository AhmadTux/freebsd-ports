# New ports collection makefile for:	www/p5-HTTP-Server-Simple-Static
# Date created:				30 March 2005
# Whom:					Lars Thegler <lth@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	HTTP-Server-Simple-Static
PORTVERSION=	0.07
CATEGORIES=	www perl5
MASTER_SITES=	CPAN
MASTER_SITE_SUBDIR=	HTTP
PKGNAMEPREFIX=	p5-

MAINTAINER=	lth@FreeBSD.org
COMMENT=	Serve static files with HTTP::Server::Simple

RUN_DEPENDS=	${SITE_PERL}/HTTP/Server/Simple.pm:${PORTSDIR}/www/p5-HTTP-Server-Simple \
		${SITE_PERL}/URI/Escape.pm:${PORTSDIR}/net/p5-URI \
		${SITE_PERL}/MIME/Types.pm:${PORTSDIR}/mail/p5-MIME-Types \
		${SITE_PERL}/File/MMagic.pm:${PORTSDIR}/devel/p5-File-MMagic
BUILD_DEPENDS=	${RUN_DEPENDS}

PERL_MODBUILD=	yes

MAN3=	HTTP::Server::Simple::Static.3

.include <bsd.port.mk>
