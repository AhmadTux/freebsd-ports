# New ports collection makefile for:	pmf
# Date created:		24 Sep 1994
# Whom:			adam
#
# $FreeBSD$
#

PORTNAME=	pmf
PORTVERSION=	1.13.1
CATEGORIES=	net
MASTER_SITES=	http://ftp.ntua.gr/pub/net/mud/lpmud/clients/pmf/ \
		LOCAL/glarkin

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Padrone's Mud Frontend

WRKSRC=		${WRKDIR}/pmfdir/src

SYSSRC=		${WRKSRC}/../system_dir
DOCSRC=		${WRKSRC}/../doc
XMPSRC=		${WRKSRC}/../examples
SUB_FILES=	pmfrc.default

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/pmf ${PREFIX}/bin
	@${MKDIR} ${DATADIR}
	${INSTALL_DATA} ${SYSSRC}/NEWS ${DATADIR}
	${INSTALL_DATA} ${WRKDIR}/pmfrc.default ${DATADIR}
	@${MKDIR} ${DATADIR}/helpfiles
	${INSTALL_DATA} ${SYSSRC}/helpfiles/.pmfrc ${DATADIR}/helpfiles
	${INSTALL_DATA} ${SYSSRC}/helpfiles/* ${DATADIR}/helpfiles
.if !defined(NOPORTEXAMPLES)
	@${MKDIR} ${EXAMPLESDIR}
	${INSTALL_DATA} ${XMPSRC}/.pmfrc ${EXAMPLESDIR}/dot.pmfrc
	${INSTALL_DATA} ${XMPSRC}/* ${EXAMPLESDIR}
.endif
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCSDIR}
	${INSTALL_DATA} ${DOCSRC}/* ${DOCSDIR}
.endif

.include <bsd.port.mk>
