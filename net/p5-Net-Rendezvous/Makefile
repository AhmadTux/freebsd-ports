# New ports collection makefile for:    Net-Rendezvous
# Date created:                         Fri Feb 20 17:39:48 EST 2004
# Whom:                                 Michael Johnson <ahze@ahze.net>
#
# $FreeBSD$
#

PORTNAME=	Net-Rendezvous
PORTVERSION=	0.90
CATEGORIES=	net perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	ports@FreeBSD.org
COMMENT=	A set of perl modules to utilize mDNS for service discovery

BUILD_DEPENDS=	${SITE_PERL}/${PERL_ARCH}/Net/DNS.pm:${PORTSDIR}/dns/p5-Net-DNS
RUN_DEPENDS:=	${BUILD_DEPENDS}

PERL_CONFIGURE=	yes

MAN3=	Net::Rendezvous.3 \
	Net::Rendezvous::Entry.3

.include <bsd.port.mk>
