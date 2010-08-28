#!/usr/bin/env perl
while (<>) {
  while ( s/(^|\W)mos_s\s*\(\s*([^\s)]+)\s*\)// ) {
    my $name = $2;
    if ( $name =~ /^[a-z0-9_]+$/i ) {
      $sels{$name} ++;
    }
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_s(", $_, ")\n";
}
print "#undef mos_DEF_s\n"; 

