# New ports collection makefile for:	RRD-Simple
# Date created:				2006/07/21
# Whom:					chinsan <chinsan.tw@gmail.com>
#
# $FreeBSD$
#

PORTNAME=	RRD-Simple
PORTVERSION=	1.44
PORTREVISION=	3
CATEGORIES=	databases perl5
MASTER_SITES=	CPAN
MASTER_SITE_SUBDIR=	Proc/NICOLAW
PKGNAMEPREFIX=	p5-

MAINTAINER=	perl@FreeBSD.org
COMMENT=	Simple interface to create and store data in RRD files

BUILD_DEPENDS=	${SITE_PERL}/${PERL_ARCH}/RRDs.pm:${PORTSDIR}/databases/rrdtool
RUN_DEPENDS:=	${BUILD_DEPENDS}

PERL_MODBUILD=	yes
MAN3=		RRD::Simple.3 RRD::Simple::Examples.3

.include <bsd.port.mk>
