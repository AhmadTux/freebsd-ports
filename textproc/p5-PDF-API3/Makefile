# Ports collection makefile for:	p5-PDF-API3
# Date created:			26 April, 2009
# Whom:				Wen Heping <wenheping@gmail.com>
#
# $FreeBSD$
#

PORTNAME=	PDF-API3
PORTVERSION=	3.001
CATEGORIES=	textproc perl5
MASTER_SITES=	CPAN
PKGNAMEPREFIX=	p5-

MAINTAINER=	wenheping@gmail.com
COMMENT=	The Next Generation of Text::PDF::API2

PERL_CONFIGURE=	yes

MAN3=	PDF::API3::Compat::API2::Basic::TTF::GDEF.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form::BarCode::codabar.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CMap::simplified.3 \
	PDF::API3::Compat::API2::Resource::Font::Postscript.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort::Contextual.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont.3 \
	PDF::API3::Compat::API2::Resource::Pattern.3 \
	PDF::API3.3 \
	PDF::API3::Compat::API2::Outline.3 \
	PDF::API3::Compat::API2::Resource::Shading.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::courierbold.3 \
	PDF::API3::Compat::API2::Basic::TTF::Kern::Subtable.3 \
	PDF::API3::Compat::API2::Basic::TTF::OS_2.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::helveticabold.3 \
	PDF::API3::Compat::API2::Resource::Font.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form::BarCode.3 \
	PDF::API3::Compat::API2::Basic::PDF::Name.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::trebuchetitalic.3 \
	PDF::API3::Compat::API2::Basic::TTF::Fpgm.3 \
	PDF::API3::Compat::API2::Resource::ColorSpace::Indexed.3 \
	PDF::API3::Compat::API2::Resource::XObject::Image::GD.3 \
	PDF::API3::Compat::API2::Outlines.3 \
	PDF::API3::Compat::API2::Resource::UniFont.3 \
	PDF::API3::Compat::API2::Lite.3 \
	PDF::API3::Compat::API2::Basic::TTF::PCLT.3 \
	PDF::API3::Compat::API2::Basic::TTF::Bsln.3 \
	PDF::API3::Compat::API2::Resource::ColorSpace::Separation.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::courieroblique.3 \
	PDF::API3::Compat::API2::Basic::TTF::Ttc.3 \
	PDF::API3::Compat::API2::Basic::TTF::Maxp.3 \
	PDF::API3::Compat::API2::Basic::TTF::Vhea.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort::Subtable.3 \
	PDF::API3::Compat::API2::Basic::TTF::Glyph.3 \
	PDF::API3::Compat::API2::Util.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::bankgothic.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::TrueType.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort::Ligature.3 \
	PDF::API3::Compat::API2::Basic::TTF::Kern::StateTable.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CMap::korean.3 \
	PDF::API3::Compat::API2::Resource::CIDFont.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::verdanabolditalic.3 \
	PDF::API3::Compat::API2::Resource.3 \
	PDF::API3::Compat::API2::Basic::TTF::GPOS.3 \
	PDF::API3::Compat::API2::Resource::Font::SynFont.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CMap::traditional.3 \
	PDF::API3::Compat::API2::Basic::TTF::OldCmap.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form::Hybrid.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::trebuchet.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::verdana.3 \
	PDF::API3::Compat::API2::Basic::TTF::Head.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form::BarCode::code3of9.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::georgiaitalic.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::helvetica.3 \
	PDF::API3::Compat::API2::Basic::TTF::XMLparse.3 \
	PDF::API3::Compat::API2::Basic::TTF::Hdmx.3 \
	PDF::API3::Compat::API2::Basic::PDF::Pages.3 \
	PDF::API3::Compat::API2::NamedDestination.3 \
	PDF::API3::Compat::API2::Basic::PDF::Bool.3 \
	PDF::API3::Compat::API2::Resource::ColorSpace::Indexed::ACTFile.3 \
	PDF::API3::Compat::API2::Basic::TTF::Table.3 \
	PDF::API3::Compat::API2::Basic::TTF::LTSH.3 \
	PDF::API3::Compat::API2::Basic::TTF::Glyf.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::georgiabold.3 \
	PDF::API3::Compat::API2::Resource::BaseFont.3 \
	PDF::API3::Compat::API2::Basic::TTF::Cvt_.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CJKFont::adobemyungjostdmediumacro.3 \
	PDF::API3::Compat::API2::Basic::TTF::Segarr.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CJKFont::kozgopromediumacro.3 \
	PDF::API3::Compat::API2::Basic::TTF::Fdsc.3 \
	PDF::API3::Compat::API2::Basic::TTF::Hhea.3 \
	PDF::API3::Compat::API2::Basic::TTF::Cmap.3 \
	PDF::API3::Compat::API2::Resource::ColorSpace.3 \
	PDF::API3::Compat::API2::Basic::TTF::Kern::ClassArray.3 \
	PDF::API3::Compat::API2::Resource::XObject::Image::PNM.3 \
	PDF::API3::Compat::API2::Basic::PDF::String.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::helveticaoblique.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort.3 \
	PDF::API3::Compat::API2::Win32.3 \
	PDF::API3::Compat::API2::Resource::ColorSpace::DeviceN.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CJKFont::adobemingstdlightacro.3 \
	PDF::API3::Compat::API2::Basic::TTF::Feat.3 \
	PDF::API3::Compat::API2::Basic::PDF::Page.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::timesbolditalic.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::timesbold.3 \
	PDF::API3::Compat::API2::Basic::TTF::OldMort.3 \
	PDF::API3::Compat::API2::Basic::TTF::Ttopen.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::verdanaitalic.3 \
	PDF::API3::Compat::API2::Content::Text.3 \
	PDF::API3::Compat::API2::Basic::PDF::Dict.3 \
	PDF::API3::Compat::API2::Resource::XObject::Image.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form::BarCode::code128.3 \
	PDF::API3::Compat::API2::Resource::Font::neTrueType.3 \
	PDF::API3::Compat::API2::Resource::XObject::Image::PNG.3 \
	PDF::API3::Compat::API2::Resource::XObject::Image::GIF.3 \
	PDF::API3::Compat::API2::Basic::TTF::Kern::OrderedList.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::TrueType::FontFile.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::timesitalic.3 \
	PDF::API3::Compat::API2::Basic::TTF::Name.3 \
	PDF::API3::Compat::API2::Annotation.3 \
	PDF::API3::Compat::API2::Basic::TTF::Kern.3 \
	PDF::API3::Compat::API2::Basic::TTF::Vmtx.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CMap::japanese.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort::Insertion.3 \
	PDF::API3::Compat::API2::Basic::PDF::Objind.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form::BarCode::ean13.3 \
	PDF::API3::Compat::API2::Page.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::webdings.3 \
	PDF::API3::Compat::API2::Basic::TTF::Delta.3 \
	PDF::API3::Compat::API2::Basic::TTF::Post.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::helveticaboldoblique.3 \
	PDF::API3::Compat::API2::HOWTO.3 \
	PDF::API3::Compat::API2::Basic::PDF::Filter.3 \
	PDF::API3::Compat::API2::Resource::XObject::Image::TIFF.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort::Rearrangement.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::trebuchetbold.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort::Chain.3 \
	PDF::API3::Compat::API2::Basic::TTF::Anchor.3 \
	PDF::API3::Compat::API2::Basic::TTF::Font.3 \
	PDF::API3::Compat::API2::Basic::TTF::Utils.3 \
	PDF::API3::Compat::API2::Resource::ExtGState.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CJKFont::kozminproregularacro.3 \
	PDF::API3::Compat::API2.3 \
	PDF::API3::Compat::API2::Resource::ColorSpace::Indexed::WebColor.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::trebuchetbolditalic.3 \
	PDF::API3::Compat::API2::Resource::ColorSpace::Indexed::Hue.3 \
	PDF::API3::Compat::API2::Basic::TTF::Prop.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CJKFont.3 \
	PDF::API3::Compat::API2::Basic::TTF::Loca.3 \
	PDF::API3::Compat::API2::Basic::TTF::Mort::Noncontextual.3 \
	PDF::API3::Compat::API2::Content.3 \
	PDF::API3::Compat::API2::Basic::PDF::Array.3 \
	PDF::API3::Compat::API2::Basic::TTF::GSUB.3 \
	PDF::API3::Compat::API2::Basic::PDF::Utils.3 \
	PDF::API3::Compat::API2::Basic::PDF::File.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::timesroman.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::zapfdingbats.3 \
	PDF::API3::Compat::API2::Resource::Font::BdFont.3 \
	PDF::API3::Compat::API2::Basic::TTF::AATutils.3 \
	PDF::API3::Compat::API2::Resource::XObject.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::courierboldoblique.3 \
	PDF::API3::Compat::API2::Basic::TTF::Prep.3 \
	PDF::API3::Compat::API2::Resource::XObject::Image::JPEG.3 \
	PDF::API3::Compat::API2::Basic::TTF::AATKern.3 \
	PDF::API3::Compat::API2::Basic::PDF::Number.3 \
	PDF::API3::Compat::API2::Basic::TTF::Hmtx.3 \
	PDF::API3::Compat::API2::Basic::TTF::Coverage.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::courier.3 \
	PDF::API3::Compat::API2::Basic::PDF::Null.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::symbol.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::georgiabolditalic.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::wingdings.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::verdanabold.3 \
	PDF::API3::Compat::API2::Resource::XObject::Form::BarCode::int2of5.3 \
	PDF::API3::Compat::API2::Resource::CIDFont::CJKFont::adobesongstdlightacro.3 \
	PDF::API3::Compat::API2::Resource::Font::CoreFont::georgia.3 \
	PDF::API3::Compat::API2::Basic::TTF::Kern::CompactClassArray.3 \
	PDF::API3::Compat::API2::Basic::TTF::Fmtx.3

.include <bsd.port.mk>
