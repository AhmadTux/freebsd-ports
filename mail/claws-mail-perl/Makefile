# New ports collection makefile for:	sylpheed-claws-perl
# Date created:	3 April 2006
# Whom:	Pawel Pekala <c0rn@o2.pl>
#
# $FreeBSD$
#

PORTNAME=	perl
PORTVERSION=	0.9.19
CATEGORIES=	mail
MASTER_SITES=	http://www.claws-mail.org/downloads/plugins/
PKGNAMEPREFIX=	claws-mail-
DISTNAME=	perl_plugin-${PORTVERSION}

MAINTAINER=	pawel@FreeBSD.org
COMMENT=	Perl filtering plugin for claws-mail

LICENSE=	GPLv3

BUILD_DEPENDS=	claws-mail>=3.8.0:${PORTSDIR}/mail/claws-mail
RUN_DEPENDS=	claws-mail>=3.8.0:${PORTSDIR}/mail/claws-mail

USE_GMAKE=	yes
USE_AUTOTOOLS=	autoconf
USE_PERL5=	yes
MAKE_JOBS_SAFE=	yes

PLIST_FILES=	lib/claws-mail/plugins/perl_plugin.a \
		lib/claws-mail/plugins/perl_plugin.la \
		lib/claws-mail/plugins/perl_plugin.so \
		share/claws-mail/matcherrc2perlfilter.pl

post-install:
	@${MKDIR} ${PREFIX}/share/claws-mail
	@${INSTALL_SCRIPT} ${WRKSRC}/tools/matcherrc2perlfilter.pl \
		${PREFIX}/share/claws-mail/

.include <bsd.port.mk>
