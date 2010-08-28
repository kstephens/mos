#!/usr/bin/env perl
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
