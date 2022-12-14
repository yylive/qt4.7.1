#!/usr/bin/perl -w
#############################################################################
##
## Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
## All rights reserved.
## Contact: http://www.qt-project.org/legal
##
## This file is part of the S60 port of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:LGPL$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and Digia.  For licensing terms and
## conditions see http://qt.digia.com/licensing.  For further information
## use the contact form at http://qt.digia.com/contact-us.
##
## GNU Lesser General Public License Usage
## Alternatively, this file may be used under the terms of the GNU Lesser
## General Public License version 2.1 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU Lesser General Public License version 2.1 requirements
## will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
##
## In addition, as a special exception, Digia gives you certain additional
## rights.  These rights are described in the Digia Qt LGPL Exception
## version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3.0 as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU General Public License version 3.0 requirements will be
## met: http://www.gnu.org/copyleft/gpl.html.
##
##
## $QT_END_LICENSE$
##
#############################################################################

# A script to get around some shortcomings in elf2e32, namely:
# - Returning 0 even when there are errors.
# - Excluding symbols from the dso file even when they are present in the ELF file.
# - Including symbols in the the dso file even when they are not present in the ELF file.
# - Overwriting the old dso file even when there are no changes (increases build time).

use File::Copy;

my @args = ();
my @definput;
my @defoutput;
my @dso;
my @tmpdso;
foreach (@ARGV) {
    if (/^--definput/o) {
        @definput = split('=', $_);
    } elsif (/^--defoutput/o) {
        @defoutput = split('=', $_);
    } elsif (/^--dso/o) {
        @dso = split('=', $_);
    } elsif (/^--tmpdso/o) {
        @tmpdso = split('=', $_);
        $tmpdso[0] = "--dso";
    } else {
        push(@args, $_);
    }
}

@definput = () if (!@definput || ! -e $definput[1]);

if (@dso && !@tmpdso || !@dso && @tmpdso) {
    print("--dso and --tmpdso must be used together.\n");
    exit 1;
}

my $buildingLibrary = (@defoutput && @dso) ? 1 : 0;

my $fixupFile = "";
my $runCount = 0;
my $returnCode = 0;

while (1) {
    if (++$runCount > 2) {
        print("Internal error in $0, link succeeded, but exports may be wrong.\n");
        last;
    }

    my $elf2e32Pipe;
    my $elf2e32Cmd = "elf2e32 @args"
         . " " . join("=", @definput)
         . " " . join("=", @defoutput)
         . " " . join("=", @tmpdso);
    open($elf2e32Pipe, "$elf2e32Cmd 2>&1 |") or die ("Could not run elf2e32");

    my %fixupSymbols;
    my $foundBrokenSymbols = 0;
    my $errors = 0;
    while (<$elf2e32Pipe>) {
        print;
        if (/Error:/io) {
            $errors = 1;
        } elsif (/symbol ([a-z0-9_]+) absent in the DEF file, but present in the ELF file/io) {
            $fixupSymbols{$1} = 1;
            $foundBrokenSymbols = 1;
        } elsif (/[0-9]+ Frozen Export\(s\) missing from the ELF file/io) {
            $foundBrokenSymbols = 1;
        }
    }
    close($elf2e32Pipe);

    if ($errors) {
        $returnCode = 1;
        last;
    }

    if ($buildingLibrary && $runCount == 1) {
        my $tmpDefFile;
        my $newDefFile;
        my $origDefFile;
        my $savedNewDefFileLine = "";
        if ($definput[1]) {
            open($origDefFile, "< $definput[1]") or die("Could not open $definput[1]");
        }
        open($newDefFile, "< $defoutput[1]") or die("Could not open $defoutput[1]");
        open($tmpDefFile, "> $defoutput[1].tmp") or die("Could not open $defoutput[1].tmp");
        print($tmpDefFile "EXPORTS\n");
        $fixupFile = "$defoutput[1].tmp";
        while (1) {
            my $origDefLine;
            my $origSym;
            my $origOrdinal;
            my $origExtraData;
            my $newDefLine;
            my $newSym;
            my $newOrdinal;
            my $newExtraData;
            my $defLine;
            my $sym;
            my $ordinal;
            my $extraData;
            if ($definput[1]) {
                # Read from original def file, and skip non-symbol lines
                while (1) {
                    $origDefLine = <$origDefFile>;
                    if (defined($origDefLine)) {
                        $origDefLine =~ s/[\n\r]//;
                        if ($origDefLine =~ /([a-z0-9_]+) +\@ *([0-9]+) (.*)/i) {
                            $origSym = $1;
                            $origOrdinal = $2;
                            $origExtraData = $3;
                            last;
                        }
                    } else {
                        last;
                    }
                }
            }

            if ($savedNewDefFileLine) {
                # This happens if the new def file was missing an entry.
                $newDefLine = $savedNewDefFileLine;
                $newDefLine =~ /([a-z0-9_]+) +\@ *([0-9]+) (.*)/i or die("$0: Shouldn't happen");
                $newSym = $1;
                $newOrdinal = $2;
                $newExtraData = $3;
            } else {
                # Read from new def file, and skip non-symbol lines
                while (1) {
                    $newDefLine = <$newDefFile>;
                    if (defined($newDefLine)) {
                        $newDefLine =~ s/[\n\r]//;
                        if ($newDefLine =~ /([a-z0-9_]+) +\@ *([0-9]+) (.*)/i) {
                            $newSym = $1;
                            $newOrdinal = $2;
                            $newExtraData = $3;
                            last;
                        }
                    } else {
                        last;
                    }
                }
            }
            $savedNewDefFileLine = "";
            last if (!defined($origDefLine) && !defined($newDefLine));

            if (defined($origOrdinal) && (!defined($newOrdinal) || $origOrdinal != $newOrdinal)) {
                # If the symbol is missing from the new def file, use the original symbol.
                $savedNewDefFileLine = $newDefLine;
                $defLine = $origDefLine;
                $sym = $origSym;
                $ordinal = $origOrdinal;
                $extraData = $origExtraData;
            } else {
                $defLine = $newDefLine;
                $sym = $newSym;
                $ordinal = $newOrdinal;
                if ($newExtraData =~ /ABSENT/) {
                    # Special case to keep "DATA [0-9]+" data in absent entries.
                    $extraData = $origExtraData;
                } else {
                    $extraData = $newExtraData;
                }
            }
            if (exists($fixupSymbols{$sym})) {
                # Fix symbols that have returned after first being marked ABSENT.
                $extraData =~ s/ ABSENT//;
            } elsif ($defLine =~ s/; MISSING://) {
                # Auto-absent symbols.
                $extraData .= " ABSENT";
            }
            print($tmpDefFile "\t$sym \@ $ordinal $extraData\n");
        }
        print($tmpDefFile "\n");
        close($origDefFile) if ($definput[1]);
        close($newDefFile);
        close($tmpDefFile);

        $definput[1] = "$defoutput[1].tmp";

        if (!$foundBrokenSymbols || $errors) {
            last;
        }

        print("Rerunning elf2e32 due to DEF file / ELF file mismatch\n");
    } else {
        last;
    }
};

if ($fixupFile) {
    unlink($defoutput[1]);
    move($fixupFile, $defoutput[1]);
}

exit $returnCode if ($returnCode != 0);

if ($buildingLibrary) {
    my $differenceFound = 0;

    if (-e $dso[1]) {
        my $dsoFile;
        my $tmpdsoFile;
        my $dsoBuf;
        my $tmpdsoBuf;
        open($dsoFile, "< $dso[1]") or die("Could not open $dso[1]");
        open($tmpdsoFile, "< $tmpdso[1]") or die("Could not open $tmpdso[1]");
        binmode($dsoFile);
        binmode($tmpdsoFile);
        while(read($dsoFile, $dsoBuf, 4096) && read($tmpdsoFile, $tmpdsoBuf, 4096)) {
            if ($dsoBuf ne $tmpdsoBuf) {
                $differenceFound = 1;
            }
        }
        close($tmpdsoFile);
        close($dsoFile);
    } else {
        $differenceFound = 1;
    }

    if ($differenceFound) {
        copy($tmpdso[1], $dso[1]);
    }
}
