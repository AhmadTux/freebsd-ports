# New ports collection makefile for:	codespell
# Date created:				6 August 2011
# Whom:					Glen Barber <gjb@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	codespell
PORTVERSION=	1.3
CATEGORIES=	textproc
MASTER_SITES=	http://packages.profusion.mobi/${PORTNAME}/ \
		http://www.glenbarber.us/ports/${CATEGORIES}/${PORTNAME}/

MAINTAINER=	gjb@FreeBSD.org
COMMENT=	Source code spelling checker

LICENSE=	GPLv2

USE_PYTHON=	3.1+

PLIST_FILES=	bin/codespell
PORTDOCS=	COPYING README TODO
PORTEXAMPLES=	*

SUB_FILES+=	pkg-message

.include <bsd.port.pre.mk>

do-build:
	${REINPLACE_CMD} 's|#!/usr/bin/env python3|#!${LOCALBASE}/bin/${PYTHON_VERSION}|' \
		${WRKSRC}/codespell.py

do-install:
	${INSTALL_SCRIPT} ${WRKSRC}/codespell.py ${PREFIX}/bin/codespell

post-install:
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCSDIR}
. for i in ${PORTDOCS}
	@${INSTALL_DATA} ${WRKSRC}/${i} ${DOCSDIR}
. endfor
.endif

.for i in data example
	@${MKDIR} ${EXAMPLESDIR}/${i}
	@${INSTALL_DATA} ${WRKSRC}/${i}/* ${EXAMPLESDIR}/${i}
.endfor
	@${CAT} ${PKGMESSAGE}

.include <bsd.port.post.mk>
