#!/bin/perl
# $Id: sel.pl,v 1.2 1999-02-19 09:26:24 stephensk Exp $
while (<>) {
  while ( s/(^|\W)mos_s\s*[(]\s*([^\s)]+)\s*[)]// ) {
    $sels{$2} ++;
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_s(", $_, ")\n";
}
print "#undef mos_DEF_s\n"; 

