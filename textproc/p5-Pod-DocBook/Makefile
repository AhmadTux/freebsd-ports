# New ports collection makefile for:	Pod-DocBook
# Date created:	   29 July 2004
# Whom:		   Sergey Skvortsov <skv@protey.ru>
#
# $FreeBSD$
#

PORTNAME=	Pod-DocBook
PORTVERSION=	1.2
PORTREVISION=	2
CATEGORIES=	textproc perl5
MASTER_SITES=	CPAN
MASTER_SITE_SUBDIR=	CPAN:NANDU
PKGNAMEPREFIX=	p5-

MAINTAINER=	skv@FreeBSD.org
COMMENT=	Convert Pod data to DocBook SGML

BUILD_DEPENDS=	${SITE_PERL}/Pod/ParseLink.pm:${PORTSDIR}/textproc/p5-podlators \
		${SITE_PERL}/Text/Wrap.pm:${PORTSDIR}/textproc/p5-Text-Tabs+Wrap
RUN_DEPENDS:=	${BUILD_DEPENDS}

PERL_CONFIGURE=	yes

MAN1=		pod2docbook.1
MAN3=		Pod::DocBook.3

.include <bsd.port.mk>
