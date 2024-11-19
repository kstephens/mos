#!/usr/bin/env perl

#
# Scans MOS C files for MOS_ANNOT annotation block and
# generates MOS lib annotation files to be loaded on-the-fly
# during annotation queries.
#

$debug = 0;
$verbose = 0;
$parent = undef;
$object = undef;
%annots = ();

%annot_files = ();
$annot_dir = "lib/mos/annot";
$annot_src_suffix = ".mos";

%object_files = ();

$annot_value_path = "_ system annot ";
$annot_value_var = "_curAnnot";
$annot_value = "$annot_value_path $annot_value_var";


#############################################################


sub eof_check {
  if ( $annot ) {
    print STDERR "$0: $in_file:$in_line: ", (scalar @annots), " mos_ANNOT_END missing at end of file.\n";
    print STDERR "'", join("', '", @annots), "'\n";
    $annot = undef;
    @annots = ();
  }
}


$in_file = undef;
$in_line = undef;
sub get_line {
  $_ = <>;
  chop;

  if ( 0 ) {
    if ( $in_file ne $ARGV ) {
      $in_file = $ARGV;
      $in_line = 1;
      print STDERR "$0: $in_file\n" if ( $verbose > 0 );
    }
  }

  if ( s/^\s*\#\s*// ) {
    #print STDERR "$0: # $_\n";
    if ( s/^(\d+)\s*// ) {
       $in_line = $1;
    }
    if ( s/^\"([^\"]*)\"\s*// ) {
      #eof_check();
      $in_file = $1;
      print STDERR "$0: $in_file:$in_line\n" if ( $verbose > 0 );
     }
  }

  #print STDERR "$_\n";

  ++ $in_line;
  print STDERR "." if ( $debug > 2 );

 $_;
}


#############################################################


sub parse_string {
  my($str);
  $str = '';

  # Eat leading whitespace.
  s/^\s*//;

  if ( ! s/^\"// ) {
    print STDERR "$0: $in_file:$in_line: expected string: found '$_'\n";
    return undef;
  }

  # Look for non-terminating quote prefix strings.
  QUOTE: while ( s/^([^\"]+)// ) {
    $str .= $1;

    # \\ continuation?
    if ( $_ eq '' && $str =~ s/\\$// ) {
      get_line();
      print STDERR "parse_string: continuation line '$_'\n" if ( $debug > 2 );
    }

    # \\" escape?
    if ( $str =~ /\\$/ && s/^\"// ) {
      $str .= '"';
      print STDERR "parse_string: \\\" escape\n" if ( $debug > 2 );
      next QUOTE;
    }

    # quote end?
    if ( s/^\"// ) {
      s/^\s+//;

      # End of line?
      if ( $_ eq '' ) {
	get_line();
	s/^\s+//;
      }

      # Begining of new string?
      if ( s/^\"// ) {
	print STDERR "parse_string: concat string: '$_'\n" if ( $debug > 2 );
	next QUOTE;
      }

      last QUOTE;
    }

    # Missing end quote?
    if ( $_ eq '' ) {
      print STDERR "$0: $in_file:$in_line: terminating \" missing\n";
    }

    # Next non-end-quote?
  }

  print STDERR "parse_string: got '$str'\n" if ( $debug > 1 );

  $str;
}


#############################################################


sub annot_file {
  my($file) = "$annot_dir/$in_file";
  $file =~ s/\.[^.\/]*$//;
  $file .= $annot_src_suffix;
  $file;
}


sub save_annot {
  my($key) = @_;
  return if ( $pass != 1 );
  my($file) = annot_file();
  $annots{$key} = $annot;
  $files{$key} = $file;
  #print STDERR "save_annot($key) $file\n";
}


sub annot {
  my($obj,$slot,$key) = @_;
  my($a) = $annot;
  my($file) = annot_file();
  my($FILE);

  # Keep track of which annotation files are needed for $obj.
  if ( defined $obj ) {
    my $k;

    $k = $files{$key} if ( defined $key );
    $k = $file if ( ! defined $k );

    #print STDERR "object_files{$obj}->{$k}\n";
    $object_files{$obj}->{$k} ++ if ( defined $k );
  }

  return if ( $pass != 2 );

  # Open output file
  if ( ! ($FILE = $annot_files{$file}) ) {
    $FILE = $annot_files{$file} = $in_file;
    open($FILE, ">$file");
    print $FILE "# Do not modify. Created by annot.pl $in_file\n";
    print $FILE "$annot_value_path _addSlots: { $annot_value_var };\n";
  }

  # Use a store annotation if key is provided.
  if ( defined $key ) {
    my $ak;
    if ( defined($ak = $annots{$key}) ) {
      if ( $a ne $ak ) {
	$a = $ak;
      }
    }
  }

  # Cache the annotation string.
  if ( $annot_cur{$FILE} ne $a ) {
    $annot_cur{$FILE} = $a;
    print $FILE "$annot_value: \"$a\";\n";
  }

  # self is the object's desc, so use the __annot:(For:) selector
  print $FILE "self $obj _annot: ($annot_value)";
  if ( defined $slot ) {
    # Use the C symbol escape notation.
    # See selsupp.c
    print $FILE " For: \"\\01$slot\";\n";
  }
  print $FILE ";\n";
}



#############################################################
# Scan input file lines


@ARGV_save = @ARGV;


foreach $pass ( 1..2 ) {
  print STDERR "$0: pass $pass\n";

  @ARGV = @ARGV_save;

  $annot = undef;
  @annots = ();

  while ( defined(get_line()) ) {
    # mos_ANNOT BEGIN
    if ( s/^\s*mos_ANNOT\s*[(]// ) {
      $str = parse_string();

     if ( ! s/\s*[)]\s*;?\s*$// ) {
      print STDERR "$0: $in_file:$in_line: Missing ) at mos_ANNOT\n";
    }
    push(@annots, $annot);
    if ( defined($annot) ) {
      $annot .= '\x7f ';
    }
    $annot .= $str;
    print STDERR "$in_file:$in_line: mos_ANNOT($annot)\n" if ( $debug > 0 );
  # mos_ANNOT END
  } elsif ( s/^\s*mos_ANNOT_END// ) {
    if ( ! @annots ) {
      print STDERR "$0: $in_file:$in_line: Too many mos_ANNOT_END\n";
    }
    $annot = pop(@annots);
    print STDERR "$in_file:$in_line: mos_ANNOT_END($annot)\n" if ( $debug > 0 );
  } elsif ( defined($annot) ) {
    # mos_* imbedded in mos_ANNOT blocks
    my $matched;

    do {
      $matched = 0;
      # mos_METHOD annotation
      if ( s/^\s*mos_METHOD\s*[(]([^)]*)[)]// ) {
        $matched = 1;
        $_ = $1;
	s/\s+//;
	m/^([^,\s]+)\s*,\s*(.+)$/;
	$mobj = $1;
	$msel = $2;
	print STDERR "$in_file:$in_line: mos_METHOD($mobj,$msel)\n" if ( $debug > 0 );
	save_annot("M$mobj,$msel") if ( $pass == 1 );
	# mos_OBJECT annotation
      } elsif ( s/mos_OBJECT\s*[(]([^)]*)[)]// ) {
        $matched = 1;
	$_ = $1;
	s/\s+//;
	$object = $_;
	print STDERR "$in_file:$in_line: mos_OBJECT($_)\n" if ( $debug > 0 );
	save_annot("O$object");
	annot($object, undef, "O$object");
	# mos_OBJECT annotation END
      } elsif ( s/mos_OBJECT_END\s*[(]([^)]*)[)]// ) {
        $matched = 1;
        $object = undef;

	# Within static object block?
      } elsif ( defined $object ) {
	# Static object block
	if ( 0 ) {

	  # Slot annotation?
	} elsif ( s/mos_OBJECT_[AG]\s*[(]([^)]*)[)]// ) {
          $matched = 1;
          $_ = $1;
    	  s/\s+//;
	  m/^([^,\s]+)\s*,\s*(.+)$/;
	  $msel = $1;
	  $mind = $2;
	  print STDERR "$in_file:$in_line: $object: mos_OBJECT_[AG]($msel,$mind)\n" if ( $debug > 0 );
	  annot($object, $msel);

	  # Method annotation?
	} elsif ( s/mos_OBJECT_M\s*[(]([^)]*)[)]// ) {
          $matched = 1;
	  $_ = $1;
	  s/\s+//;
	  m/^([^,\s]+)\s*,\s*(.+)$/;
	  $mobj = $1;
	  $msel = $2;
	  print STDERR "$in_file:$in_line: $object: mos_OBJECT_M($mobj,$msel)\n" if ( $debug > 0 );
	  annot($object, $msel, "M$mobj,$msel");
	}
      }
    } while ( $matched );
  }
}

eof_check();
}


# Terminate all open annotation files.
foreach ( keys(%annot_files) ) {
  print $_ "$annot_value: ", '@u', ";\n";
  print $_ "1;\n";
  close($_);
}


# Write annot.def to STDOUT
print "/* Do not modify. Created by annot.pl @ARGV */\n";

foreach $o ( sort keys %object_files ) {
  $file = join("\\t", sort keys %{$object_files{$o}});
  print "mos_DEF_ANNOT($o,\"$file\")\n";
}

print "#undef mos_DEF_ANNOT\n";

1;
