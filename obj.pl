#!/bin/perl
# $Id: obj.pl,v 1.2 1999-02-19 09:26:23 stephensk Exp $
while (<>) {
  while ( s/^mos_OBJECT_END\s*[(]\s*([^\s)]+)\s*[)]// ) {
    $sels{$1} ++;
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_OBJECT(", $_, ")\n";
}
print "#undef mos_DEF_OBJECT\n"; 

1;
