#! /usr/bin/env perl
use strict;
use warnings;

# print qq({"greet":"Hello Perl","../error":0});
# print qq({"data":{"greet":"Hello Perl"},"error":0});

use JSON;
my $str_req = shift;
my $json_req = decode_json($str_req);
my $json_data = $json_req->{data} || {};
my $name = $json_data->{name} || "World";

my $json_ret = {greet => "Hello $name!"};
my $str_ret = encode_json($json_ret);

print $str_ret, "\n";
# exit 1;
