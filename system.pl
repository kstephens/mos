#!/bin/perl
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
