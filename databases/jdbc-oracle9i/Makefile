# New ports collection makefile for:	The Oracle 8.1.6 JDBC Drivers
# Date created:		2 January 2000
# Whom:			Patrick Gardella <patrick@freebsd.org>
#
# $FreeBSD$
#

PORTNAME=	oracle9i
PORTVERSION=	9.2.0.8
CATEGORIES=	databases java
MASTER_SITES=	# none
PKGNAMEPREFIX=	jdbc-
DISTFILES=	${CLASSES} ${OCRS_FILE}
DIST_SUBDIR=	oracle9i
EXTRACT_ONLY=	# none

MAINTAINER=	thierry@FreeBSD.org
COMMENT=	The JDBC drivers for Oracle 9i

RESTRICTED=	Must be downloaded direct from Oracle via www interface

USE_JAVA=	yes
JAVA_VERSION=	1.5+

NO_BUILD=	yes
NO_WRKSUBDIR=	yes

CLASSES=	ojdbc14.jar
OCRS_FILE=	ocrs12.jar

PLIST_FILES=	%%JAVAJARDIR%%/${CLASSES}	\
		%%JAVAJARDIR%%/${OCRS_FILE}

.include <bsd.port.pre.mk>

.if !exists(${CLASSES:S|^|${_DISTDIR}/|}})	\
 && !exists(${OCRS_FILE:S|^|${_DISTDIR}/|})
IGNORE=Please manually download ${DISTFILES} from http://www.oracle.com/technetwork/database/enterprise-edition/jdbc9201-092698.html Oracle9i ${PORTVERSION} JDBC Drivers for use with JDK 1.4 \(JDBC classes + Additional RowSet support\). Put ${DISTFILES} into the directory ${_DISTDIR} and run make again.
.endif

do-install:
	${INSTALL_DATA} ${DISTFILES:S|^|${_DISTDIR}/|} ${JAVAJARDIR}

.include <bsd.port.post.mk>
