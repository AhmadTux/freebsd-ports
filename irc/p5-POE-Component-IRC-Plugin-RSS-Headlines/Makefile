# New ports collection makefile for:	p5-POE-Component-IRC-Plugin-RSS-Headlines
# Date created:	   20.06.2008
# Whom:		   Martin Wilke <miwi@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	POE-Component-IRC-Plugin-RSS-Headlines
PORTVERSION=	1.08
CATEGORIES=	irc perl5 devel
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	miwi@FreeBSD.org
COMMENT=	A POE::Component::IRC plugin that provides RSS headline retrieval

BUILD_DEPENDS=	${SITE_PERL}/POE.pm:${PORTSDIR}/devel/p5-POE \
		${SITE_PERL}/POE/Component/IRC.pm:${PORTSDIR}/irc/p5-POE-Component-IRC \
		p5-POE-Component-Client-DNS>=1.00:${PORTSDIR}/dns/p5-POE-Component-Client-DNS
RUN_DEPENDS=	${SITE_PERL}/POE.pm:${PORTSDIR}/devel/p5-POE \
		${SITE_PERL}/POE/Component/IRC.pm:${PORTSDIR}/irc/p5-POE-Component-IRC \
		p5-POE-Component-Client-DNS>=1.00:${PORTSDIR}/dns/p5-POE-Component-Client-DNS

PLIST_SUB=	EXAMPLESDIR="share/examples/${PORTNAME}"
PERL_CONFIGURE=	yes

MAN3=		POE::Component::IRC::Plugin::RSS::Headlines.3

post-install:
.ifndef(NOPORTEXAMPLES)
	@${MKDIR} ${EXAMPLESDIR}
	${INSTALL_SCRIPT} ${WRKSRC}/examples/* ${EXAMPLESDIR}
.endif

.include <bsd.port.mk>
