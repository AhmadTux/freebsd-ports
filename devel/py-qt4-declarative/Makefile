# New ports collection makefile for:	py-qt4-declarative
# Date created:				2010-11-05
# Whom:					Max Brazhnikov <makc@FreeBSD.org>
#
# $FreeBSD$

PORTNAME=	declarative
PORTVERSION=	${PYQT4_VERSION}
CATEGORIES=	devel python
MASTER_SITES=	${MASTER_SITES_PYQT4}
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}qt4-
DISTNAME=	${PYQT4_DISTNAME}

MAINTAINER=	kde@FreeBSD.org
COMMENT=	Python bindings for the Qt4 toolkit, QtDeclarative module

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}sip>=${SIP_VERSION}:${PORTSDIR}/devel/py-sip
RUN_DEPENDS=	${PKGNAMEPREFIX}gui>=${PYQT4_VERSION}:${PORTSDIR}/x11-toolkits/py-qt4-gui \
		${PKGNAMEPREFIX}network>=${PYQT4_VERSION}:${PORTSDIR}/net/py-qt4-network

DISTINFO_FILE=	${PYQT4_DISTINFO_FILE}
HAS_CONFIGURE=	yes
USE_PYTHON=	2.5+
USE_QT_VER=	4
QT_COMPONENTS=	declarative qmake_build moc_build

OPTIONS=	API		"Install QtDeclarative API for QScintilla2" on \
		DEBUG		"Enable debugging in generated code" off \
		SIPFILES	"Install the QtDeclarative SIP files" on \
		TRACING		"Enable tracing in generated code" off

PATCHDIR=	${PORTSDIR}/devel/py-qt4-core/files
OPTIONSFILE?=	${PORT_DBDIR}/py-qt4-${PORTNAME}/options
SIPDIR=		${PREFIX}/share/py-sip
QSCIDIR=	${PREFIX}/share/qt4/qsci
CONFIGURE_ENV+=	PYQT4_COMPONENT="${PORTNAME}"
ARGS=		-b ${PREFIX}/bin\
		-d ${PYTHONPREFIX_SITELIBDIR}\
		-p ${PREFIX}/${QT_PLUGINDIR_REL}\
		-q ${QMAKE}\
		--confirm-license\
		--enable QtDeclarative

.include "../py-sip/files/bsd.pyqt.mk"
.include <bsd.port.options.mk>

.if defined(WITH_DEBUG)
ARGS+=		--debug
.endif
.if defined(WITHOUT_API)
ARGS+=		--no-qsci-api
PLIST_SUB+=	API="@comment "
.else
LIB_DEPENDS+=	qscintilla2.5:${PORTSDIR}/devel/qscintilla2
ARGS+=		--qsci-api --qsci-api-destdir=${QSCIDIR}
PLIST_SUB+=	API=""
.endif
.if defined(WITHOUT_SIPFILES)
ARGS+=		--no-sip-files
PLIST_SUB+=	SIP="@comment "
.else
ARGS+=		--sipdir ${SIPDIR}
PLIST_SUB+=	SIP=""
.endif
.if defined(WITH_TRACING)
ARGS+=		--trace
.endif

do-configure:
	cd ${WRKSRC} && ${SETENV} ${CONFIGURE_ENV}\
		${PYTHON_CMD} configure.py ${ARGS}

post-configure:
	${REINPLACE_CMD} -e 's|mkspecs/freebsd-g++|share/qt4/mkspecs/freebsd-g++|'\
		-e 's|CC = cc|CC = ${CC}|'\
		-e 's|CXX = c++|CXX = ${CXX}|'\
		-e 's|LINK = c++|LINK = ${CXX}|'\
		${WRKSRC}/QtDeclarative/Makefile

.include <bsd.port.mk>
