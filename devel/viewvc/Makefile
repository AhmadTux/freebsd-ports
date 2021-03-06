# New ports collection makefile for:	viewcvs
# Date created:				Sun 02 Jul 2000
# Whom:					will
#
# $FreeBSD$
#

PORTNAME=	viewvc
PORTVERSION=	1.1.13
#PORTREVISION=	1
CATEGORIES=	devel python
MASTER_SITES=	http://www.viewvc.org/ \
		LOCAL/ohauer

MAINTAINER=	ohauer@FreeBSD.org
COMMENT=	Web-based Version Control Repository Browsing

LICENSE=	BSD

OPTIONS=	SUBVERSION "use svn binding" on \
		APACHE "Use apache as webserver" on \
		LIGHTTPD "Use lighttpd as webserver" off \
		MODPYTHON3 "Enable mod_python3 support (deprecated)" off \
		MODWSGI3 "Enable mod_wsgi3 support" off \
		MYSQL "Enable experimental MYSQL support" off

NO_BUILD=	yes
USE_PYTHON=	yes
SUB_FILES=	pkg-message
SUB_LIST=	INSTDIR="${PREFIX}/${INSTDIR}" \
		PYTHON_CMD=${PYTHON_CMD} \
		ECHO=${ECHO} EGREP=${EGREP} TOUCH=${TOUCH} \
		CHOWN=${CHOWN} RM=${RM}

INSTDIR?=	${PORTNAME}
PLIST_SUB=	INSTDIR=${INSTDIR}

.include <bsd.port.options.mk>

# standalone script is usefull even if a webserver was choosen.
USE_RC_SUBR=	viewvc

RUN_DEPENDS+=	${LOCALBASE}/bin/pygmentize:${PORTSDIR}/textproc/py-pygments

# viewvc can support CVS or SVN, controlled with the parameter
# cvs_roots (for CVS), svn_roots (for Subversion) in viewvc.conf
.if defined (WITH_SUBVERSION)
RUN_DEPENDS+=	${PYTHON_LIBDIR}/site-packages/svn/__init__.py:${PORTSDIR}/devel/py-subversion
.endif

.if defined (WITH_APACHE)
USE_APACHE_RUN=	20+
.endif

.if defined(WITH_LIGHTTPD)
RUN_DEPENDS+=	lighttpd:${PORTSDIR}/www/lighttpd
.endif

.if defined(WITH_MODPYTHON3)
RUN_DEPENDS+=	${LOCALBASE}/${APACHEMODDIR}/mod_python.so:${PORTSDIR}/www/mod_python3
.endif

.if defined(WITH_MODWSGI3)
RUN_DEPENDS+=	${LOCALBASE}/${APACHEMODDIR}/mod_wsgi.so:${PORTSDIR}/www/mod_wsgi3
.endif

# This feature is a clone of the Mozilla Project's Bonsai database.
# It catalogs every commit in the CVS or Subversion repository into a SQL database.
# In fact, the databases are 100% compatible.
.if defined(WITH_MYSQL)
RUN_DEPENDS+=	${PYTHON_PKGNAMEPREFIX}MySQLdb>=0:${PORTSDIR}/databases/py-MySQLdb
.endif

pre-everything::
.if defined(WITH_APACHE) && defined(WITH_LIGHTTPD)
	@${ECHO_CMD} "It doesn't make sense to depend on Apache *and* lighttpd - choose only one."
	@${FALSE}
.endif

.if defined(WITH_MODPYTHON3) && !defined(WITH_APACHE) && defined(WITH_LIGHTTPD)
	@${ECHO_CMD} "mod_python3 needs Apache, please select Apache and deselect lighttpd"
	@${FALSE}
.elif defined(WITH_MODPYTHON3) && !defined(WITH_APACHE)
	@${ECHO_CMD} "mod_python3 needs Apache, please select Apache"
	@${FALSE}
.elif defined(WITH_MODWSGI3) && !defined(WITH_APACHE) && defined(WITH_LIGHTTPD)
	@${ECHO_CMD} "mod_wsgi3 needs Apache, please select Apache and deselect lighttpd"
	@${FALSE}
.elif defined(WITH_MODWSGI3) && !defined(WITH_APACHE)
	@${ECHO_CMD} "mod_wsgi3 needs Apache, please select Apache"
	@${FALSE}
.endif

do-install:
	@(cd ${WRKSRC} && ${PYTHON_CMD} viewvc-install --prefix=${PREFIX}/${INSTDIR} --destdir="" --clean-mode=false)

post-install:
	@${CAT} ${PKGMESSAGE}

.include <bsd.port.mk>
