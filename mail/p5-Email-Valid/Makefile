# New ports collection makefile for:	mail/p5-Email-Valid
# Date created:				28 December 2000
# Whom:					Anton Berezin <tobez@tobez.org>
#
# $FreeBSD$
#

PORTNAME=	Email-Valid
PORTVERSION=	0.185
CATEGORIES=	mail perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	tobez@FreeBSD.org
COMMENT=	Check validity of Internet email addresses

COMMON_DEPENDS=	p5-Net-DNS>0:${PORTSDIR}/dns/p5-Net-DNS \
		p5-Mail-Tools>0:${PORTSDIR}/mail/p5-Mail-Tools \
		p5-Net-Domain-TLD>0:${PORTSDIR}/dns/p5-Net-Domain-TLD
BUILD_DEPENDS=	${COMMON_DEPENDS}
RUN_DEPENDS=	${COMMON_DEPENDS}

PERL_CONFIGURE=	yes

MAN3=		Email::Valid.3

.include <bsd.port.mk>
