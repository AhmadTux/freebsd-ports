# New ports collection makefile for:    p5-Mysql-modules
# Date created:				September 29th 1996
# Whom:	      				James FitzGibbon <jfitz@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	Mysql-modules
PORTVERSION=	1.2219
CATEGORIES=	databases perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-
DISTNAME=	Msql-Mysql-modules-${PORTVERSION}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Perl5 modules for accessing MySQL databases

BUILD_DEPENDS=	${SITE_PERL}/Data/ShowTable.pm:${PORTSDIR}/devel/p5-Data-ShowTable \
		${SITE_PERL}/${PERL_ARCH}/DBI.pm:${PORTSDIR}/databases/p5-DBI
RUN_DEPENDS:=	${BUILD_DEPENDS}

CONFLICTS=	p5-DBD-mysql-* p5-DBD-mysql4[0-9]-* p5-DBD-mysql5[0-9]-*

USE_MYSQL=	yes

PERL_CONFIGURE=	yes
CONFIGURE_ARGS=	PREFIX=${PREFIX} CC="${CC}" CCFLAGS="${CFLAGS}" \
		--noprompt --mysql-install --nomsql-install
CONFIGURE_ENV=	MYSQL_HOME=${PREFIX} IN_MYSQL_DISTRIBUTION=1

MAN1=		dbimon.1
MAN3=		Bundle::DBD::mysql.3 \
		DBD::mysql.3 \
		Mysql.3

pre-configure:
		@${PERL} -p -i -e 's| -lgz||' ${WRKSRC}/lib/DBD/mysql/Install.pm

.include <bsd.port.mk>
