# New ports collection makefile for:	flowtag
# Date created:		14 July 2008
# Whom:			Lee Hinman <lee@writequit.org>
#
# $FreeBSD$
#

PORTNAME=	flowtag
PORTVERSION=	2.0.5
CATEGORIES=	security
MASTER_SITES=	http://hexbit.csc.gatech.edu/portfiles/ \
		http://writequit.org/misc/files/ \
		http://chrislee.dhs.org/pages/research/flowtag/
EXTRACT_SUFX=	.tgz

MAINTAINER=	lee@writequit.org
COMMENT=	A collaborative attack-analysis tool for security researchers

USE_RUBY=	yes
USE_RUBY_SETUP=	yes

MAN1=	flowtag.1 listflows.1 pcap2flowdb.1 printflow.1
MANCOMPRESSED=	yes

.include <bsd.port.pre.mk>

RUN_DEPENDS+=	${RUBY_LIBDIR}/tk/canvas.rb:${PORTSDIR}/x11-toolkits/ruby-tk

.include <bsd.port.post.mk>
