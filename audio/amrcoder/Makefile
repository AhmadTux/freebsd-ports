# New ports collection makefile for:	amrcoder
# Date created:				5 March 2005
# Whom:					Andrew Wingorodov (Wingborn) <programmer@coolsoft.ru>
#
# $FreeBSD$
#
#

PORTNAME=	amrcoder
PORTVERSION=	1.0
CATEGORIES=	audio
MASTER_SITES=	http://ftp.us.sinuspl.net/pub/src/ \
		http://ftp.uni-erlangen.de/pub/FreeBSD/ports/distfiles/
EXTRACT_SUFX=	.tgz

MAINTAINER=	ports@FreeBSD.org
COMMENT=	AMR encoder/decoder for Mbuni MMS Gateway (www.mbuni.org)

MAN1=		amrdecoder.1 amrencoder.1

MANCOMPRESSED=	yes

post-extract:
	@${RM} ${WRKSRC}/*.o

.include <bsd.port.mk>
