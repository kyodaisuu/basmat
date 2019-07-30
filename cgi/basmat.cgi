#!/usr/bin/perl --
#
# Web interface of Bashicu Matrix Calculator
#
#   http://gyafun.jp/ln/basmat.cgi
#
#   Project website
#   https://kyodaisuu.github.io/basmat/
#

use strict;

### Initial Setting ###

# Title of the website
my $title    = "Bashicu Matrix Calculator";
# URL of the BM calculator
my $url      = "http://gyafun.jp/ln/basmat.cgi";
# Location of basmat
my $basmat = "basmat/basmat";
# Location of help file to be displayed when no input is given
my $help     = "basmat/help.txt";

# Check if run by cgi
if ( $ENV{'SCRIPT_NAME'} eq "" ) {
    print "This program is to be run as cgi.\n";
    exit 1;
}

############## Get parameters ###############

# Get form data

# Input variales $ini, $ver, $max, $inc will be passed to shell
# and therefore should be sanitized for security.

my %formdata;
my ( $input, $name, $val );
my @inputs = split( '&', $ENV{'QUERY_STRING'} );
foreach $input (@inputs) {
    ( $name, $val ) = split( '=', $input );
    # url decode
    $val =~ tr/+/ /;
    $val =~ s/%([0-9A-Fa-f][0-9A-Fa-f])/pack("H2",$1)/eg;
    $formdata{$name} = $val;
}
my $ini = $formdata{'ini'};
# Remove non-ascii characters and white space
$ini =~ s/[^\x{21}-\x{7E}\s\t\n\r]//g;
my $com = "";
if ( substr( $ini, 0, 1 ) eq "s" ) { $com = "s"; }
if ( substr( $ini, 0, 1 ) eq "b" ) { $com = "b"; }
# Sanitize $ini: remove characters except 0123456789,()[]
$ini =~ s/[^0-9(),\[\]]//g;
# Sanitize $ini: maximum character length = 256
$ini = substr( $ini, 0, 256 );
my $max = $formdata{'max'};
# Sanitize $max
$max =~ s/[^0-9]//g;
my $inc = $formdata{'inc'};
# Sanitize $inc
$inc =~ s/[^1-4]//g;
if ( $inc < 1 or $inc > 4 ) { $inc = 1; }
my $ver    = $formdata{'version'};
my $detail = $formdata{'detail'};
my $escaped;

# Sequence to be calculated $ini is formatted by calling basmat
my $ini1 = `$basmat -t 1 \"$ini\" | grep -v "Not" | head -n 1`;
chomp($ini1);
if ( length( uri_escape($ini1) ) < 256 && substr( $ini1, 0, 1 ) == "(" ) {
    $ini = $ini1;
}
if ( substr( $ini, 0, 1 ) eq "E" ) {
    $ini = "";
}
# $ini0 is sequence to be displayed in a textbox of the html
my $ini0 = $ini;
if ( $ini eq "" ) {
    $ini0 = "(0,0)(1,1)[3]";
}
else {
    $ini0 =~ s/\n//;
}

# If called from outside referrer, maximum length is 20
# otherwise maximum length is 20 to 10000
my $referrer = $ENV{'HTTP_REFERER'};
$referrer =~ s/\?.*$//;
if ( $referrer ne $url ) { $max = 20; }
if ( $max < 20 )         { $max = 20; }
if ( $max > 10000 )      { $max = 10000; }

# Calculation options
my $sel1 = "";
my $sel2 = "";
my $sel3 = "";
my $sel4 = "";
if ( $inc == 1 ) { $sel1 = "selected"; }
if ( $inc == 2 ) { $sel2 = "selected"; }
if ( $inc == 3 ) { $sel3 = "selected"; }
if ( $inc == 4 ) { $sel4 = "selected"; }
my $d;
if ( $detail == 1 ) {
    $d = '-d';
}
else {
    $d      = '';
    $detail = 0;
}

# BM verion
# $ver is sanitized here
my $ver1  = '';
my $ver2  = '';
my $ver21 = '';
my $ver22 = '';
my $ver23 = '';
my $ver3  = '';
my $ver31 = '';
my $ver32 = '';
my $ver33 = '';
my $ver4 = '';
if ( $ver == 1 ) {
    $ver  = 1;
    $ver1 = 'selected';
}
elsif ( $ver == 2 ) {
    $ver  = 2;
    $ver2 = 'selected';
}
elsif ( $ver == 2.1 ) {
    $ver   = 2.1;
    $ver21 = 'selected';
}
elsif ( $ver == 2.2 ) {
    $ver   = 2.2;
    $ver22 = 'selected';
}
elsif ( $ver == 2.3 ) {
    $ver   = 2.3;
    $ver22 = 'selected';
}
elsif ( $ver == 3 ) {
    $ver  = 3;
    $ver3 = 'selected';
}
elsif ( $ver == 3.1 ) {
    $ver   = 3.1;
    $ver31 = 'selected';
}
elsif ( $ver == 3.2 ) {
    $ver   = 3.2;
    $ver32 = 'selected';
}
elsif ( $ver == 3.3 ) {
    $ver   = 3.3;
    $ver33 = 'selected';
}
elsif ( $ver == 4 ) {
    $ver   = 4;
    $ver4 = 'selected';
}
else {
    # Set default version
    $ver  = 4;
    $ver4 = 'selected';
}

################ Main #################

# Print calculation form

print "Content-type: text/html; charset=UTF-8\n";
print "\n";
print "<!DOCTYPE html>\n";
print "<html lang=\"en\">\n";
print "<head>\n";
print "  <meta charset=\"UTF-8\">\n";
print "  <title>$title</title>\n";
print "  <link rel=\"stylesheet\" type=\"text/css\" href=\"fish.css\">\n";
print "  <script src=\"https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML\"></script>\n";
print "</head>\n";
print "<body>\n";
print "<h1>$title</h1>\n";
print "<div style=\"text-align: center;
   margin: 10px 10px; padding: 8px;
   border: 5px ridge green;
   border-radius: 20px 20px 20px 20px / 20px 20px 20px 20px ;
   font-family: helvetica, sans-serif;
   background-color: #fffdfc\">\n";
print "<form action=\"basmat.cgi\" method=\"get\">\n";
print "<p>Initial variable: <input type=\"text\" name=\"ini\" size=\"55\"
   maxlength=\"256\" value=\"$ini0\"></p>\n";
print "<p>Maximum length: <input type=\"text\" name=\"max\" size=\"4\"
   maxlength=\"10\" value=\"$max\">\n";
print "n increment: <select name=\"inc\">\n";
print "<option value=\"1\" $sel1>(1) Constant</option>\n";
print "<option value=\"2\" $sel2>(2) n = n + 1</option>\n";
print "<option value=\"3\" $sel3>(3) n = n * n</option>\n";
print "<option value=\"4\" $sel4>(4) Simulate Hardy function</option>\n";
print "</select>\n";
print "<p><select name=\"version\">";
print "<option value=\"1\" $ver1>Version 1";
print "<option value=\"2\" $ver2>Version 2";
print "<option value=\"2.1\" $ver21>Version 2.1 (koteitan)";
print "<option value=\"2.2\" $ver22>Version 2.2 (koteitan)";
print "<option value=\"2.3\" $ver22>Version 2.3 (koteitan)";
print "<option value=\"3\" $ver3>Version 3";
print "<option value=\"3.1\" $ver31>Version 3.1 (Nish)\n";
print "<option value=\"3.2\" $ver32>Version 3.2 (Nish)\n";
print "<option value=\"3.3\" $ver33>Version 3.3 (rpakr, Ecl1psed)\n";
print "<option value=\"4\" $ver4>Version 4\n";
print "</select>";

if ( $detail == 0 ) {
    print
"Detailed output: <input type=\"checkbox\" name=\"detail\" value=\"1\">\n";
}
else {
    print
"Detailed output: <input type=\"checkbox\" name=\"detail\" value=\"1\" checked=\"checked\">\n";
}

print "<input type=\"submit\" value=\"Go\"></p>\n";
print "</form>\n";
print "</div>\n";

# Calculation

if ( $ini eq "" ) {

    if ( $com eq "s" ) {  # Show source

        print "<p>Source code of Bashicu matrix calculator is now available at
   <a href=\"https://kyodaisuu.github.io/basmat/\">GitHub repository</a>.
  The main program is
  <a href=\"https://github.com/kyodaisuu/basmat/blob/master/basmat.c\">basmat.c</a>
  and there is a
  <a href=\"https://kyodaisuu.github.io/basmat/definition.html\">Reader's guide of the source code</a>
  because the definition of Bashicu Matrix is written in the C code.</p>
  <p>The web interface internally uses this C program with some limitations on parameters.
  You can follow the
  <a href=\"https://kyodaisuu.github.io/basmat/install.html\">installation instruction</a>
  and run the offline version on your PC.
  In this way you can run the program up to the limit of your system resources
  and time.</p>\n";
    }
    else {  # Show help file
        my $buf;
        open( IN, "< ", $help );
        read( IN, $buf, ( -s $help ) );
        print $buf, "\n";
        close IN;
        print "<ul>\n";
        my @variable = (
            "(0)(1)[3]",       "(0)(1)(1)[3]",
            "(0)(1)(1)(1)[3]", "(0)(1)(2)[3]",
            "(0)(1)(2)(3)[3]", "(0,0)(1,1)[3]"
        );

        foreach my $name (@variable) {
            $escaped = uri_escape($name);
            print
"<li><a href=\"basmat.cgi?ini=$escaped&amp;inc=4\">$name</a> (Hardy)\n";
        }
        @variable = (
            "(0,0)(1,1)[2]",
            "(0,0)(1,1)(1,1)[2]",
            "(0,0,0)(1,1,1)[2]",
            "(0,0,0)(1,1,1)(2,0,0)(1,1,0)(2,1,0)(3,1,0)[2]",
            "(0,0,0)(1,1,1)(2,0,0)(1,1,1)[2]",
            "(0,0,0)(1,0,0)(2,0,0)(2,1,0)(3,1,1)[2]",
            "(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]",
"(0,0,0,0)(1,1,1,1)(2,2,1,1)(3,3,1,1)(4,2,0,0)(5,1,1,1)(6,2,1,1)(7,3,1,1)[1]"
        );
        foreach my $name (@variable) {
            $escaped = uri_escape($name);
            print
              "<li><a href=\"basmat.cgi?ini=$escaped&amp;inc=1\">$name</a>\n";
        }
        print "</ul>\n";
    }
}
else {  # Calculation main
    print "<pre>";
    # Call basmat
    system("$basmat -v $ver $d -s $max -o $inc -t 1000 \"$ini\"");
    print "</pre>\n";
    use URI::Escape;
    $escaped = uri_escape($ini);
    print
"<a href=\"basmat.cgi?ini=$escaped&amp;inc=$inc&amp;version=$ver&amp;detail=$detail\">Permalink of this result</a>";
}

# Print footer

print "<hr>\n";
print "<p style=\"text-align: right;\"><a href=\"$url\">$title</a>
   (<a href=\"https://kyodaisuu.github.io/basmat/\">offline version</a>) by
    <a href=\"http://googology.wikia.com/wiki/User:Kyodaisuu\">Fish</a></p>\n";
print "</body>\n";
print "</html>\n";
