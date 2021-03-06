# New ports collection makefile for:	databases/p5-DBIx-Class-EncodedColumn
# Date created:				29 Mar 2009
# Whom:					Lars Balker Rasmussen <lbr@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	DBIx-Class-EncodedColumn
PORTVERSION=	0.00011
CATEGORIES=	databases perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	perl@FreeBSD.org
COMMENT=	Automatically encode columns

LICENSE=	ART10 GPLv1
LICENSE_COMB=	dual

BUILD_DEPENDS=	p5-DBIx-Class>=0.06002:${PORTSDIR}/databases/p5-DBIx-Class \
		p5-Sub-Name>=0.04:${PORTSDIR}/devel/p5-Sub-Name \
		p5-SQL-Translator>=0.09.003:${PORTSDIR}/databases/p5-SQL-Translator
RUN_DEPENDS:=	${BUILD_DEPENDS}
TEST_DEPENDS=	p5-Test-Exception>=0:${PORTSDIR}/devel/p5-Test-Exception \
		p5-DBD-SQLite>=0:${PORTSDIR}/databases/p5-DBD-SQLite \
		p5-Dir-Self>=0:${PORTSDIR}/devel/p5-Dir-Self

PERL_CONFIGURE=	yes

MAN3=	DBIx::Class::EncodedColumn.3 \
	DBIx::Class::EncodedColumn::Crypt.3 \
	DBIx::Class::EncodedColumn::Crypt::Eksblowfish::Bcrypt.3 \
	DBIx::Class::EncodedColumn::Crypt::OpenPGP.3 \
	DBIx::Class::EncodedColumn::Digest.3

post-extract:
	@${PERL} -i -ne 'print unless m,^build_requires, || m,^feature, .. m,;,' ${WRKSRC}/Makefile.PL

OPTIONS=	SHA "Digest::SHA Support" On \
		BLOWFISH "Blowfish Support?" On \
		OPENPGP "Crypt::OpenPGP (gpg) Support" On

.include <bsd.port.pre.mk>

.if defined(WITH_SHA)
. if ${PERL_LEVEL} < 501000
RUN_DEPENDS+=	p5-Digest-SHA>=0:${PORTSDIR}/security/p5-Digest-SHA
. endif
.endif
.if defined(WITH_BLOWFISH)
RUN_DEPENDS+=	p5-Crypt-Eksblowfish>=0:${PORTSDIR}/security/p5-Crypt-Eksblowfish
.endif
.if defined(WITH_OPENPGP)
RUN_DEPENDS+=	p5-Crypt-OpenPGP>=0:${PORTSDIR}/security/p5-Crypt-OpenPGP \
		p5-Crypt-CAST5_PP>=0:${PORTSDIR}/security/p5-Crypt-CAST5_PP
.endif

.include <bsd.port.post.mk>
