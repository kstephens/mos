#!/bin/perl
# $Id: meth.pl,v 1.2 1999-02-19 09:26:23 stephensk Exp $

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
