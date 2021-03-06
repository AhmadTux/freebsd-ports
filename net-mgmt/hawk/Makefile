# New ports collection makefile for:	hawk
# Date created:				06 February 2012
# Whom:					Doug Barton <dougb@FreeBSD.org>
#
# $FreeBSD$

PORTNAME=	hawk
PORTVERSION=	0.6
PORTREVISION=	1
CATEGORIES=	net-mgmt
MASTER_SITES=	SF/iphawk/iphawk/Hawk%20${PORTVERSION}/

MAINTAINER=	dougb@FreeBSD.org
COMMENT=	Track uptime and DNS status for machines on your networks

RUN_DEPENDS=	${SITE_PERL}/Net/Netmask.pm:${PORTSDIR}/net-mgmt/p5-Net-Netmask \
		${SITE_PERL}/Net/Ping/External.pm:${PORTSDIR}/net/p5-Net-Ping-External \
		${SITE_PERL}/${PERL_ARCH}/DBI.pm:${PORTSDIR}/databases/p5-DBI \
		${SITE_PERL}/${PERL_ARCH}/DBD/mysql.pm:${PORTSDIR}/databases/p5-DBD-mysql

USE_PERL5=	yes
USE_MYSQL=	server
USE_PHP=	mysql
WANT_PHP_MOD=	yes

USE_RC_SUBR=	hawk

SUB_LIST=	PERL=${PERL}

post-patch:
	@${SED} -e 's#%%PREFIX%%#${PREFIX}#' \
	    ${WRKSRC}/daemon/hawk > ${WRKSRC}/daemon/hawk.sed
	@${MV} ${WRKSRC}/daemon/hawk.sed ${WRKSRC}/daemon/hawk
	@${SED} -e 's#hawk.conf.inc#${PREFIX}/etc/hawk/php.conf#' \
	    ${WRKSRC}/php/hawk.php > ${WRKSRC}/php/hawk.php.sed
	@${MV} ${WRKSRC}/php/hawk.php.sed ${WRKSRC}/php/hawk.php

do-build:
# Empty target, nothing to build

do-install:
	${MKDIR} ${PREFIX}/etc/hawk
	${INSTALL_DATA} ${WRKSRC}/daemon/hawk.conf ${PREFIX}/etc/hawk/daemon.conf.sample
	${INSTALL_SCRIPT} ${WRKSRC}/daemon/hawk ${PREFIX}/sbin
	${MKDIR} ${WWWDIR}/images
	(for f in ${WRKSRC}/php/images/* ${FILESDIR}/favicon.ico; do \
		${INSTALL_DATA} $$f ${WWWDIR}/images; \
	done)
	${INSTALL_DATA} ${WRKSRC}/php/hawk.php ${WWWDIR}
	${INSTALL_DATA} ${WRKSRC}/php/hawk.css ${WWWDIR}
	${INSTALL_DATA} ${WRKSRC}/php/hawk.conf.inc \
	    ${PREFIX}/etc/hawk/php.conf.sample
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/README ${DOCSDIR}
	${INSTALL_DATA} ${FILESDIR}/create_hawk_db.sql ${DOCSDIR}
	${INSTALL_DATA} ${FILESDIR}/create_hawk_user.sql ${DOCSDIR}
.endif

PKGMESSAGE=	${WRKDIR}/pkg-message

post-install:
	@${ECHO} '******************************************************************************' > ${PKGMESSAGE}
	@${ECHO} '***    See ${PREFIX}/share/doc/hawk/create_hawk* for instructions on	   ***' >> ${PKGMESSAGE}
	@${ECHO} '***    creating the database and user that the PHP code expects.	   ***' >> ${PKGMESSAGE}
	@${ECHO} '***									   ***' >> ${PKGMESSAGE}
	@${ECHO} '***    Copy daemon.conf.sample and php.conf.sample to their non-sample     ***' >> ${PKGMESSAGE}
	@${ECHO} '***    names in ${PREFIX}/etc/hawk, and configure local networks, etc.    ***' >> ${PKGMESSAGE}
	@${ECHO} '******************************************************************************' >> ${PKGMESSAGE}

	@${CAT} ${PKGMESSAGE}

.include <bsd.port.mk>
