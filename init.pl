#!/bin/perl
while (<>) {
  while ( s/^mos_INIT[(]\s*([^\s)]+)\s*[)]// ) {
    $sels{$1} ++;
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_INIT(", $_, ")\n";
}
print "#undef mos_DEF_INIT\n"; 
