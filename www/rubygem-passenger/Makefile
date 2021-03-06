# Ports collection makefile for:	rubygem-passenger
# Date created:				Dec 18, 2008
# Whom:					Jacob Atzen <jatzen@gmail.com>
#
# $FreeBSD$

PORTNAME=	passenger
PORTVERSION=	3.0.11
PORTREVISION=	1
CATEGORIES=	www rubygems
MASTER_SITES=	RG
PKGNAMEPREFIX=	rubygem-

MAINTAINER=	osa@FreeBSD.ORG
COMMENT=	Modules for running Ruby on Rails and Rack applications

OPTIONS=	APACHEPORT	"Use apache22"			on  \
		NGINXPORT	"Use nginx"			off \
		DEBUG		"Build with debugging info"	off \
		SYMLINK 	"Create 'passenger' symlink"	off

USE_RUBY=	yes
USE_RAKE=	yes
USE_RUBYGEMS=	yes
RUBYGEM_AUTOPLIST=	yes

.include <bsd.port.options.mk>

.if defined(WITH_APACHEPORT) && defined(WITH_NGINXPORT)
IGNORE=		supports only one web-server: apache22 or nginx. \
		Please 'make config' again
.endif

.if !defined(WITHOUT_APACHEPORT)
USE_APACHE=	22+
.endif

.if defined(WITH_NGINXPORT)
NGINXVERSION!=	${PKG_INFO} -qO www/nginx-devel 2>/dev/null || true
.if empty(NGINXVERSION)
BUILD_DEPENDS+=	nginx:${PORTSDIR}/www/nginx
.else
BUILD_DEPENDS+=	nginx:${PORTSDIR}/www/nginx-devel
.endif
.endif

LIB_DEPENDS+=	ev.4:${PORTSDIR}/devel/libev \
		curl.6:${PORTSDIR}/ftp/curl
BUILD_DEPENDS+=	rubygem-fastthread>=1.0.1:${PORTSDIR}/devel/rubygem-fastthread \
		rubygem-rack>=0:${PORTSDIR}/www/rubygem-rack \
		rubygem-daemon_controller>=0.2.5:${PORTSDIR}/devel/rubygem-daemon_controller
RUN_DEPENDS:=	${BUILD_DEPENDS}

SUB_LIST+=	GEM_LIB_DIR=${GEM_LIB_DIR} RUBY=${RUBY}
SUB_FILES=	pkg-message

PLIST_FILES=	bin/passenger \
		bin/passenger-config \
		bin/passenger-install-apache2-module \
		bin/passenger-make-enterprisey \
		bin/passenger-memory-stats \
		bin/passenger-status \
		bin/passenger-install-nginx-module
.if defined(WITH_SYMLINK)
PLIST_FILES+=	${GEMS_DIR}/${PORTNAME}
SUB_LIST+=	PASSENGER_INSTALL_DIR="${PREFIX}/${GEMS_DIR}/${PORTNAME}"
.endif

.if !defined(WITH_SYMLINK)
SUB_LIST+=	PASSENGER_INSTALL_DIR="${PREFIX}/${GEM_LIB_DIR}"
.endif

pre-patch:
.if defined(WITH_NGINXPORT)
	@${ECHO_CMD}
	@${ECHO_CMD} "Do not forget compile www/nginx"
	@${ECHO_CMD} "with PASSENGER_MODULE support"
	@${ECHO_CMD}
.endif

post-install:
	@${REINPLACE_CMD} \
		's!-lpthread!${PTHREAD_LIBS}!g' \
		${PREFIX}/${GEM_LIB_DIR}/ext/nginx/config
	@${REINPLACE_CMD} \
		's!-Wall!!g; \
		s!gcc!${CC}!g; \
		s!g++!${CXX}!g; \
		s!#{PlatformInfo.debugging_cflags}!${CFLAGS}!g; \
		s!-O2!!g; \
		46s!true!false!' \
		${PREFIX}/${GEM_LIB_DIR}/build/config.rb
.if !defined(WITH_DEBUG)
	@${REINPLACE_CMD} \
		's!-DPASSENGER_DEBUG!-DNDEBUG!g' \
		${PREFIX}/${GEM_LIB_DIR}/build/config.rb
.endif
	@${REINPLACE_CMD} \
		's!-lpthread!${PTHREAD_LIBS}!g' \
		${PREFIX}/${GEM_LIB_DIR}/lib/phusion_passenger/platform_info/compiler.rb
	@${REINPLACE_CMD} \
		's!#{LIBEV_LIBS}!-L${LOCALBASE}/lib -lev!g' \
		${PREFIX}/${GEM_LIB_DIR}/build/agents.rb
	@${REINPLACE_CMD} \
		's!-Iext/libev!!; \
		268s!/usr/include/libev!${LOCALBASE}/include!; \
		269s!-lev!-L${LOCALBASE}/lib -lev!' \
		${PREFIX}/${GEM_LIB_DIR}/build/common_library.rb

.if !defined(WITHOUT_APACHEPORT)
	(${PREFIX}/bin/passenger-install-apache2-module --auto)
	@${CAT} ${PKGMESSAGE}
.endif

.if defined(WITH_NGINXPORT)
	(cd ${PREFIX}/${GEM_LIB_DIR} && ${RAKE_BIN} nginx)
.endif

	${FIND} ${PREFIX}/${GEM_LIB_DIR} -name '*.o' -delete
	${FIND} ${PREFIX}/${GEM_LIB_DIR} -name '*.bak' -delete
.if defined(WITH_SYMLINK)
	${LN} -s ${PREFIX}/${GEM_LIB_DIR} ${PREFIX}/${GEMS_DIR}/${PORTNAME}
.endif

.include <bsd.port.mk>
