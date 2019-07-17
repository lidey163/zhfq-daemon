#! /usr/bin/env perl
=pod
提取头文件的错误常量定义（及其尾注释），生成 sql 脚本，用于插入错误表
或生成 cpp 代码段，适于 errormsg.cpp 中初始化 m_mapErrorCode
结果打印至标准输出，可重定向或直接管道至 mysql 
用法示例：
./head2error.pl [--format=sql|cpp] errorcode.hpp > errorcode.sql
./head2error.pl errorcode.hpp | mysql ....

=cut

use strict;
use warnings;
use Getopt::Long;

my $format = 'sql';
GetOptions ("format=s" => \$format,)
	or die("Error in command line arguments\n");

if (!@ARGV) {
	print <<EOF;
usage example:
./head2error.pl [--format=sql|cpp] errorcode.hpp > errorcode.sql
./head2error.pl errorcode.hpp | mysql ....
EOF
	exit 1;
}

my $TABLE = 't_error_config';
my $FIELD = 'F_code, F_macro, F_message, F_group';

my $reg_skip = qr{^\s*(#|//|$)};
my $reg_enum = qr{^\s*enum\s+(\w+)};
my $reg_error = qr{^\s*(ERR_\w+)\s*=\s*(-?\d+)\s*,\s*//\s*(.+)$};
my $cur_group = 'default';

while (<>) {
	next if $_ =~ $reg_skip;
	if ($_ =~ $reg_enum) {
		on_group($1);
	}
	elsif ($_ =~ $reg_error) {
		on_erorr($1, $2, $3);
	}
}

sub on_group
{
	my ($group) = @_;
	$cur_group = $group;
	if ($format eq 'cpp') {
		print "// $group\n";
	}
}

sub on_erorr
{
	my ($mocro, $code, $msg) = @_;
	if ($format eq 'cpp') {
		print qq{m_mapErrorCode[$code] = "$msg";\n};
	}
	elsif ($format eq 'sql') {
		print qq{REPLACE INTO $TABLE($FIELD) VALUES($code, '$mocro', '$msg', '$cur_group');\n};
	}
}
