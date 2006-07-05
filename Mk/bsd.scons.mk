#-*- mode: Makefile; tab-width: 4; -*-
# ex:ts=4
#
# $FreeBSD: ports/Mk/bsd.scons.mk,v 1.2 2006/07/05 19:50:28 linimon Exp $
#
# bsd.scons.mk - Python-based SCons build system interface.
# Author: Alexander Botero-Lowry <alex@foxybanana.com>
#
# Please view me with 4 column tabs!

# Please make sure all changes to this file are passed through the maintainer.
SCONS_MAINTAINER=	alex@foxybanana.com

#
# SCONS_BIN is the location where the scons port installs the scons
# executable.
#
# SCONS_PORT is where the scons port is located in the ports tree.
#
SCONS_BIN=	${LOCALBASE}/bin/scons
SCONS_PORT=	${PORTSDIR}/devel/scons

#
# CCFLAGS is the scons equivalent of CFLAGS. So we should bring in our
# FreeBSD CFLAGS.
#
# LINKFLAGS is equivalent to LDFLAGS in make speak, so we bring in the
# FreeBSD default LDFLAGS.
#
# Some scons projects may honor PKGCONFIGDIR, which tells them where to
# look for, and install, pkgconfig files.
#
CCFLAGS?=	${CFLAGS}
LINKFLAGS?=	${LDFLAGS}
PKGCONFIGDIR?=	${LOCALBASE}/libdata/pkgconfig

CCFLAGS+=	-I${LOCALBASE}/include
CXXFLAGS+=	-I${LOCALBASE}/include
LINKFLAGS+=	-L${LOCALBASE}/lib

#
# SCONS_ENV is where we pass all the stuff that should be the
# same for any scons port to scons. Things like CCFLAGS, and LINKFLAGS
# go here.
#
# SCONS_ARG is where you pass port specific scons flags to the scons
# enviornment.
#
# SCONS_BUILDENV is where you pass variables you want to be in the
# System Enviornment instead of the SCons Enviornment.
#
# SCONS_TARGET is the same as MAKE_TARGET it is passed as the last
# argument to scons.
#
SCONS_ENV?=	CCFLAGS="${CCFLAGS}" CXXFLAGS="${CXXFLAGS}" \
		LINKFLAGS="${LINKFLAGS}" PKGCONFIGDIR="${PKGCONFIGDIR}" \
		PREFIX="${PREFIX}" CC="${CC}" CXX="${CXX}"
SCONS_ARGS?=
SCONS_BUILDENV?=
SCONS_TARGET?=

#
# SCONS_INSTALL_TARGET is the default target to be used when
# installing a port using scons.
#
SCONS_INSTALL_TARGET?=	${INSTALL_TARGET}

#
# Make sure we depend on scons
#
BUILD_DEPENDS+=	${SCONS_BIN}:${SCONS_PORT}

.if !target(do-build)
do-build:
	@cd ${WRKSRC} && \
	${SETENV} ${SCONS_BUILDENV} ${SCONS_BIN} ${SCONS_ENV} ${SCONS_ARGS} \
	${SCONS_TARGET}
.endif

.if !target(do-install)
do-install:
	@cd ${WRKSRC} && ${SETENV} ${SCONS_BUILDENV} ${SCONS_BIN} ${SCONS_ENV} \
	${SCONS_ARGS} ${SCONS_INSTALL_TARGET}
.endif

