#!/bin/perl

while (<>) {
  while ( s/^mos_METHOD\s*[(]\s*([^\s)]+)\s*[)]// ) {
#    print STDERR "$1\n";
    $sels{$1} ++;
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_METHOD(", $_, ")\n";
}
print "#undef mos_DEF_METHOD\n";

1;
