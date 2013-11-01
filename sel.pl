#!/usr/bin/env perl
while (<>) {
  while ( s/(^|\W)mos_s\s*\(\s*([^\s)]+)\s*\)/<<$2>>/ ) {
    my $name = $2;
    # print STDERR "sel='", $name, "' in line ", $., ": '", $_, "'\n";
    if ( $name =~ /^[a-z0-9_]+$/i ) {
      $sels{$name} ++;
    }
  }
}
for ( sort(keys(%sels)) ) {
  print "mos_DEF_s(", $_, ")\n";
}
print "#undef mos_DEF_s\n"; 

