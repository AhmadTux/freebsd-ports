# Ports collection makefile for:	rubygem-httparty
# Date created:				Nov 07, 2008
# Whom:					Wen Heping <wenheping@gmail.com>
#
# $FreeBSD$

PORTNAME=	httparty
PORTVERSION=	0.8.1
CATEGORIES=	www rubygems
MASTER_SITES=	RG

MAINTAINER=	ruby@FreeBSD.org
COMMENT=	Ruby Library that Makes HTTP Fun Again

RUN_DEPENDS=	rubygem-multi_json>=0:${PORTSDIR}/devel/rubygem-multi_json \
		rubygem-multi_xml>=0:${PORTSDIR}/textproc/rubygem-multi_xml

USE_RUBY=	yes
USE_RUBYGEMS=	yes
RUBYGEM_AUTOPLIST=	yes

PLIST_FILES=	bin/httparty

.include <bsd.port.mk>
