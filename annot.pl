#!/bin/perl

$annot = undef;
@annots = ();
$parent = undef;
$object = undef;
%annots = ();

%annot_files = ();
$annot_dir = "lib/mos/annot";
$annot_src_suffix = ".mos";

$annot_value_path = "_ system annot ";
$annot_value_var = "_curAnnot";
$annot_value = "$annot_value_path $annot_value_var";

print "/* Do not modify. Created by annot.pl */\n";

sub annot {
    my($o,$s,$annot) = @_;
    my($file) = "$annot_dir/$o$annot_src_suffix";
    my($FILE);

    if ( ! ($FILE = $annot_files{$file}) ) {
	$FILE = $annot_files{$file} = $o;
	open($FILE, ">$file");
	print "mos_DEF_ANNOT($o,\"$file\")\n";
	print $FILE "# Do not modify. Created by annot.pl\n";
	print $FILE "$annot_value_path _addSlots: { $annot_value_var };\n";
    }

    if ( $annot_cur{$o} ne $annot ) {
	$annot_cur{$o} = $annot;
	print $FILE "$annot_value: \"$annot\";\n";
    }

    # self is the object's desc, so use the __annot:(For:) selector
    print $FILE "self __annot: $annot_value";
    if ( $s ) {
	print $FILE " For: @\"$s\";\n";
    }
    print $FILE ";\n";
}

while (<>) {
    chop;
    if ( s/^\s*mos_ANNOT[(]\s*"// ) {
        $str = '';
        while ( /\\$/ ) {
           s/\\$//;
           $str .= $_ . '\n';
           $_ = <>;
           chop;
        }
        $str .= $_;
        $str =~ s/"\s*[)]\s*;?\s*$//;
	 push(@annots, $annot);
	 if ( defined($annot) ) {
	     $annot .= '\x7f ';
	 }
	 $annot .= $str;
     } elsif ( s/^\s*mos_ANNOT_END// ) {
	 $annot = pop(@annots);
     } elsif ( defined($annot) ) {
	 if ( s/^\s*mos_METHOD[(]\s*// ) {
	     s/\s*[)]\s*$//;
	      s/\s+//;
	      print "\n/* mos_METHOD($_) */\n" if ( 0 );
	      $annots{"M" . $_} = $annot;
	 } elsif ( s/^\s*mos_OBJECT[(]\s*// ) {
	     s/\s*[)]\s*$//;
	     s/\s+//;
	     $object = $_;
	     print "\n/* mos_OBJECT($_) */\n" if ( 0 );
	     $annots{"O" . $_} = $annot;
	     annot($object, undef, $annot);
	 } elsif ( s/^\s*mos_OBJECT_END[(]\s*// ) {
	     $object = undef;
	 } elsif ( $object ) {
	     if ( 0 ) {
	     }
	     elsif ( s/^\s*mos_OBJECT_[AG][(]\s*// ) {
		 s/\s*,\s*[0-9]+\s*[)]\s*$//;
		     s/\s+//;
		 m/^([^,\s]+)\s*,\s*(.+)$/;
		     $msel = $1;
		     $mind = $2;
		     print "\n/* mos_OBJECT_[AG]($_) */\n" if ( 0 );
		     if ( $annot ne $annots{"O" . $object} ) {
			 annot($object, $msel, $annot);
		     }
		 }
	     elsif ( s/^\s*mos_OBJECT_A[(]\s*// ) {
		 s/\s*,\s*[0-9]+\s*[)]\s*$//;
		     s/\s+//;
		 m/^([^,\s]+)\s*,\s*(.+)$/;
		     $msel = $1;
		     $mind = $2;
		     print "\n/* mos_OBJECT_A($_) */\n" if ( 0 );
		     if ( $annots{"O" . $object} ne $annot ) {
			 annot($object, $msel, $annot);
		     }
	      }
	     elsif ( s/^\s*mos_OBJECT_M[(]\s*// ) {
		 s/\s*[)]\s*$//;
		     s/\s+//;
		 m/^([^,\s]+)\s*,\s*(.+)$/;
		     $mobj = $1;
		     $msel = $2;
		     print "\n/* mos_OBJECT_M($mobj,$msel) */\n" if ( 0 );
		     $a = $annots{"M" . $_};
		     if ( $a ne $annot ) {
			 annot($object, $msel, $a);
		     }
	     }
	 }
	       }
	       }
	       
    foreach ( keys(%annot_files) ) {
	print $_ "$annot_value: ", '@u', ";\n";
	print $_ "1;\n";
	close($_);
    }

print "#undef mos_DEF_ANNOT\n";

	       1;




