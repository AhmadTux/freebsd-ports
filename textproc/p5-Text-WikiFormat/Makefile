# New ports collection makefile for:	p5-Text-WikiFormat
# Date created:				Sat Jan 17 18:15:01 UTC 2004
# Whom:					Alexander Zhuravlev <zaa@zaa.pp.ru>
#
# $FreeBSD$
#

PORTNAME=	Text-WikiFormat
PORTVERSION=	0.79
CATEGORIES=	textproc perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Module for translating Wiki formatted text into other formats

BUILD_DEPENDS=	${SITE_PERL}/Test/More.pm:${PORTSDIR}/devel/p5-Test-Simple
RUN_DEPENDS=	${SITE_PERL}/URI/Escape.pm:${PORTSDIR}/net/p5-URI \
		${SITE_PERL}/${PERL_ARCH}/Scalar/Util.pm:${PORTSDIR}/lang/p5-Scalar-List-Utils

PERL_MODBUILD=	yes

MAN3=		Text::WikiFormat.3 \
		Text::WikiFormat::Blocks.3

.include <bsd.port.mk>
