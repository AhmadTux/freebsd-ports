# New ports collection makefile for:   p5-Hash-Merge
# Date created:        22 June 2004
# Whom:                snowchyld
#
# $FreeBSD$

PORTNAME=	Hash-Merge
PORTVERSION=	0.12
CATEGORIES=	textproc perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	perl@FreeBSD.org
COMMENT=	Merges hashes

RUN_DEPENDS=	${SITE_PERL}/${PERL_ARCH}/Clone.pm:${PORTSDIR}/devel/p5-Clone
BUILD_DEPENDS=	${RUN_DEPENDS}

MAN3=		Hash::Merge.3

PERL_CONFIGURE=	yes

.include <bsd.port.mk>
