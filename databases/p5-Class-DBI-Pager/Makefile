# New ports collection makefile for:	p5-Class-DBI-Pager
# Date created:	   21 july 2003
# Whom:		   Mathieu Arnold <m@absolight.net>
#
# $FreeBSD$
#

PORTNAME=	Class-DBI-Pager
PORTVERSION=	0.08
CATEGORIES=	databases perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	perl@FreeBSD.org
COMMENT=	Pager utility for Class::DBI

BUILD_DEPENDS=	${RUN_DEPENDS}
RUN_DEPENDS=	${SITE_PERL}/Class/DBI.pm:${PORTSDIR}/databases/p5-Class-DBI \
		${SITE_PERL}/Exporter/Lite.pm:${PORTSDIR}/devel/p5-Exporter-Lite \
		${SITE_PERL}/Data/Page.pm:${PORTSDIR}/databases/p5-Data-Page \
		${SITE_PERL}/${PERL_ARCH}/Clone.pm:${PORTSDIR}/devel/p5-Clone

PERL_CONFIGURE=	yes

MAN3=		Class::DBI::Pager.3

.include <bsd.port.mk>
