# $FreeBSD: ports/devel/p5-Module-Build/inc.build.mk,v 1.1 2003/08/21 13:09:52 mat Exp $
# To be used by modules using Module::Build

.if ${PORTNAME} != Module-Build
BUILD_DEPENDS+=		${SITE_PERL}/Module/Build.pm:${PORTSDIR}/devel/p5-Module-Build
.endif

CONFIGURE_ARGS+=	install_path=lib="${SITE_PERL:S|^${LOCALBASE}/|${PREFIX}/|}" \
			install_path=arch="${SITE_PERL:S|^${LOCALBASE}/|${PREFIX}/|}/${PERL_ARCH}" \
			install_path=script="${PREFIX}/bin" \
			install_path=bin="${PREFIX}/bin" \
			install_path=libdoc="${MAN3PREFIX}/man/man3" \
			install_path=bindoc="${MAN1PREFIX}/man/man1"

