# New ports collection makefile for:	net/p5-Net-DNS-ZoneFile
# Date created:				11 June 2002
# Whom:					Anton Berezin <tobez@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	Net-DNS-ZoneFile
PORTVERSION=	1.04
PORTREVISION=	1
CATEGORIES=	dns perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	tobez@FreeBSD.org
COMMENT=	Perl extension to convert a zone file to a collection of RRs

BUILD_DEPENDS=	${SITE_PERL}/${PERL_ARCH}/Net/DNS.pm:${PORTSDIR}/dns/p5-Net-DNS \
		p5-NetAddr-IP>0:${PORTSDIR}/net-mgmt/p5-NetAddr-IP
RUN_DEPENDS:=	${BUILD_DEPENDS}

PERL_CONFIGURE=	yes

MAN3=		Net::DNS::ZoneFile.3

post-patch:
	${PERL} -pi -e 's/^\s*our\s+([\$$\@\%]\w+)/use vars qw($$1);$$1/;' \
		-e '$$_="" if /require 5/;' \
		-e '$$_="" if /^\s*(use|no)\s+warnings;\s*$$/;' \
		${WRKSRC}/ZoneFile.pm

.include <bsd.port.mk>
