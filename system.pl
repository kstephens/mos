#!/bin/perl
# $Id: system.pl,v 1.2 1999-02-19 09:26:26 stephensk Exp $
while (<>) {
  while ( s/^mos_OBJECT_END\s*[(]\s*([^\s,)]+)\s*[)]// ) { 
    $sels{$1} ++;
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_SYSTEM(", $_, ")\n";
}
print "#undef mos_DEF_SYSTEM\n";

1;
