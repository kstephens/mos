#!/bin/perl
# $Id: init.pl,v 1.2 1999-02-19 09:26:22 stephensk Exp $
while (<>) {
  while ( s/^mos_INIT[(]\s*([^\s)]+)\s*[)]// ) {
    $sels{$1} ++;
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_INIT(", $_, ")\n";
}
print "#undef mos_DEF_INIT\n"; 
