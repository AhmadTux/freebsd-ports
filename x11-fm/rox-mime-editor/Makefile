# Ports collection Makefile for:	rox-mime-editor
# Date created:				Mar 24 2003
# Whom:					olgeni@FreeBSD.org
#
# $FreeBSD$
#

PORTNAME=	rox-mime-editor
PORTVERSION=	0.6
CATEGORIES=	x11-fm
MASTER_SITES=	SF/rox/MIME-Editor/${PORTVERSION}
DISTNAME=	mime-editor-${PORTVERSION}

MAINTAINER=	olgeni@FreeBSD.org
COMMENT=	MIME type editor for the ROX desktop

RUN_DEPENDS=	${LOCALBASE}/bin/rox:${PORTSDIR}/x11-fm/rox-filer \
		${LOCALBASE}/lib/ROX-Lib2/python/rox/__init__.py:${PORTSDIR}/devel/py-roxlib

USE_BZIP2=	yes
USE_PYTHON=	yes

do-build:
	@${PYTHON_CMD} -m compileall ${WRKSRC}
	@${PYTHON_CMD} -O -m compileall ${WRKSRC}

pre-install:
	@${REINPLACE_CMD} -e 's,/usr/local,${LOCALBASE},g' \
				${WRKSRC}/MIME-Editor/findrox.py
	@${FIND} ${WRKSRC} -name *.bak -delete

do-install:
	@${MKDIR} ${PREFIX}/apps
	@${CP} -r ${WRKSRC}/MIME-Editor ${PREFIX}/apps/
	@${CHOWN} -R ${SHAREOWN}:${SHAREGRP} ${PREFIX}/apps/MIME-Editor

.include <bsd.port.mk>
