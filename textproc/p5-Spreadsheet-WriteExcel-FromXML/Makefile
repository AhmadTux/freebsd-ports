# New ports collection makefile for:	Spreadsheet::WriteExcel::FromXML
# Date created:				7 May 2004
# Whom:					Bill Swingle <unfurl@freebsd.org>
#
# $FreeBSD$
#

PORTNAME=	Spreadsheet-WriteExcel-FromXML
PORTVERSION=	1.1
PORTEPOCH=	1
CATEGORIES=	textproc perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Create an Excel Spreadsheet from XML

BUILD_DEPENDS=	${SITE_PERL}/OLE/Storage_Lite.pm:${PORTSDIR}/devel/p5-OLE-Storage_Lite \
		${SITE_PERL}/${PERL_ARCH}/XML/Parser.pm:${PORTSDIR}/textproc/p5-XML-Parser \
		${SITE_PERL}/Spreadsheet/WriteExcel.pm:${PORTSDIR}/textproc/p5-Spreadsheet-WriteExcel
RUN_DEPENDS:=	${BUILD_DEPENDS}

PERL_CONFIGURE=	yes

MAN3=		Spreadsheet::WriteExcel::FromXML::Workbook.3 \
		Spreadsheet::WriteExcel::FromXML::Worksheet.3 \
		Spreadsheet::WriteExcel::FromXML.3

.include <bsd.port.mk>
