# New ports collection makefile for:	p5-PPIx-EditorTools
# Date created:				July 12th 2009
# Whom:	      				Cezary Morga <cm@therek.net>
#
# $FreeBSD$
#

PORTNAME=	PPIx-EditorTools
PORTVERSION=	0.11
CATEGORIES=	textproc perl5
MASTER_SITES=	CPAN
MASTER_SITE_SUBDIR=	CPAN:AZAWAWI
PKGNAMEPREFIX=	p5-

MAINTAINER=	cm@therek.net
COMMENT=	Utility methods and base class for manipulating Perl via PPI

BUILD_DEPENDS=	p5-Class-XSAccessor>=1.02:${PORTSDIR}/devel/p5-Class-XSAccessor \
		p5-PPI>=1.203:${PORTSDIR}/textproc/p5-PPI
RUN_DEPENDS:=	${BUILD_DEPENDS}
TEST_DEPENDS=	p5-Test-Differences>=0:${PORTSDIR}/devel/p5-Test-Differences \
		p5-Test-Most>=0:${PORTSDIR}/devel/p5-Test-Most

PERL_CONFIGURE=	yes

MAN3=		PPIx::EditorTools::RenamePackage.3 \
		PPIx::EditorTools::RenamePackageFromPath.3 \
		PPIx::EditorTools::ReturnObject.3 \
		PPIx::EditorTools::FindVariableDeclaration.3 \
		PPIx::EditorTools::IntroduceTemporaryVariable.3 \
		PPIx::EditorTools::RenameVariable.3 \
		PPIx::EditorTools.3 \
		PPIx::EditorTools::FindUnmatchedBrace.3

.include <bsd.port.mk>
