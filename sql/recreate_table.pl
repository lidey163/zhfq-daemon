#! /usr/bin/env perl
# 提取部分表的建表语句，并修饰为重建语句，添加 IF NOT EXISTS
# ./recreate_table.pl [--file create_table.sql --drop] table1 table2
# 无表名参数时输出所有表
# 打印到标准输出
use strict;
use warnings;
use Getopt::Long;

my $sql_file = "create_table.sql";
my $drop;

GetOptions ("file=s" => \$sql_file, "drop" => \$drop)
	or die("Error in command line arguments\n");

my @tables = @ARGV;
my $all_table = (scalar(@tables) == 0);
my %table_search = map { $_ => 1} @tables;

my $reg_table_begin = qr/^\s*CREATE\s+TABLE\s+(\w+)\s*\(/i;
my $reg_table_end = qr/^\s*\).*COMMENT\s+'(.+)';\s*$/;

my $matched = 0;
my $in_table = 0;

open(my $fh, '<', $sql_file) or die "cannot open $sql_file $!";
while (<$fh>) {
	next if /^\s*--/;
	next if /^\s*$/;
	if ($_ =~ $reg_table_begin) {
		my $table_name = $1;
		if (!$all_table && !%table_search) {
			warn "finish search tables, exit early\n";
			last;
		}

		if ($all_table || match_table($table_name)) {
			if ($drop) {
				print "DROP TABLE IF EXISTS $table_name;\n";
			}
			print "CREATE TABLE IF NOT EXISTS $table_name(\n";
			$in_table = 1;
			next;
		}
	}

	if ($in_table) {
		print $_;
	}

	if ($_ =~ $reg_table_end) {
		$in_table = 0;
		next;
	}
}
close($fh);

sub match_table
{
	my ($table_name) = @_;
	my $flag = $table_search{$table_name};
	if ($flag) {
		delete $table_search{$table_name};
		$matched = $matched + 1;
	}
	return $flag;
}
1;
