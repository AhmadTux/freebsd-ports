# New ports collection makefile for:	p5-HTTP-Recorder
# Date created:				1 April 2004
# Whom:	      				mat
#
# $FreeBSD$
#

PORTNAME=	HTTP-Recorder
PORTVERSION=	0.05
PORTREVISION=	1
CATEGORIES=	www perl5
MASTER_SITES=	CPAN
MASTER_SITE_SUBDIR=	CPAN:LEIRA
PKGNAMEPREFIX=	p5-

MAINTAINER=	perl@FreeBSD.org
COMMENT=	Record interaction with websites

BUILD_DEPENDS=	${SITE_PERL}/LWP/UserAgent.pm:${PORTSDIR}/www/p5-libwww \
		${SITE_PERL}/${PERL_ARCH}/HTML/TokeParser.pm:${PORTSDIR}/www/p5-HTML-Parser \
		${SITE_PERL}/HTTP/Request/Params.pm:${PORTSDIR}/www/p5-HTTP-Request-Params
RUN_DEPENDS:=	${BUILD_DEPENDS}

PERL_CONFIGURE=	yes

MAN3=		HTTP::Recorder.3

post-patch:
	${RM} -f ${WRKSRC}/lib/HTTP/Recorder.pm.orig

.include <bsd.port.mk>
