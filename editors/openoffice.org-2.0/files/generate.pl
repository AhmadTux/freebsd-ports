#!/usr/bin/perl

# generate full build shell script for OpenOffice.org
# Whom:         Maho Nakata <maho@FreeBSD.org>
# $FreeBSD: ports/editors/openoffice-2.0-devel/files/generate.pl,v 1.2 2005/01/24 12:45:55 maho Exp $

print "#!/bin/csh\n";
print "/usr/bin/time -h make WITH_CCACHE=yes package package-rename solver sdk deinstall languagepack >& log.en\n";
print "rm work/.configure* work/.build* \n";
print "/usr/bin/time -h make ALL_LOCALIZED_LANGS=yes >& log.all\n";

open ( FILE,  "< Makefile.localized") ;
while(<FILE>){
@tmp=split (' ',$_);
@tmp2=split ('"',$tmp[3]);
if ( $tmp[0] eq ".if" && $tmp[1] eq "\${LOCALIZED_LANG}" ) { $LANG=$tmp2[1]; 
	print "make TWEAK_L10N=yes LOCALIZED_LANG=$LANG pre-everything\n";
	print "/usr/bin/time -h make LOCALIZED_LANG=$LANG WITH_CCACHE=yes languagepack package package-rename deinstall >& log.$LANG\n";
	}
}
close FILE;

