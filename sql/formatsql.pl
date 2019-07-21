#! /usr/bin/env perl
# 将建表语句转化为 markdonw 文档
# 从标准输入或文件名参数，向标准输出
package formatsql;
use strict;
use warnings;

# 正则表达式
my $reg_table_begin = qr/^\s*CREATE\s+TABLE\s+(\w+)\s*\(/i;
my $reg_table_end = qr/^\s*\).*COMMENT\s+'(.+)';\s*$/;
my $reg_field_def = qr/^\s*(F_\w+)\s+(.+)\s+COMMENT\s+'(.+)',\s*$/i;
my $reg_key_def = qr/^\s*(\w+\s+)?KEY\s*(\w+)?\s*\((\w+)\)/i;

# 表格构
my $tables = {tablist => []};
# $table_ref = {collist => [], name, comment}
# $field_ref = {name, type, default, iskey, comment}

# 输出 markdonw 格式的表格
sub output_markdonw
{
	#my ($var) = @_;
	foreach my $table_name (@{$tables->{tablist}}) {
		warn "output table: $table_name\n";
		my $table_ref = $tables->{$table_name};
		my $table_comment = $table_ref->{comment};
		print "\n";
		print "## `$table_name`: $table_comment\n";
		print "\n";
		print "| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |\n";
		print "| -------- | -------- | ------ | ---- | ------- |\n";

		foreach my $field_name (@{$table_ref->{collist}}) {
			warn "output field $field_name\n";
			my $field_ref = $table_ref->{$field_name};
			my $data_type = $field_ref->{type};
			my $default = $field_ref->{default};
			my $iskey = $field_ref->{iskey} // '';
			my $comment = $field_ref->{comment};
			print "| `$field_name` | $data_type | $default | $iskey | $comment |\n";
		}
	}
}

# 从 <> 读入整个 sql 脚本，保存每个表定义至 $tables
sub read_sql
{
	my ($var) = @_;
	while (<>) {
		next if /^\s*--/;
		next if /^\s*$/;
		if ($_ =~ $reg_table_begin) {
			my $table_name = $1;
			warn "found table: $table_name\n";
			my $table_ref = {collist => []};
			$table_ref->{name} = $table_name;
			while (<>) {
				if ($_ =~ $reg_table_end) {
					my $table_comment = $1;
					$table_ref->{comment} = $table_comment;
					warn "finish table: $table_name\n";
					last;
				}
				elsif ($_ =~ $reg_field_def) {
					my $field_name = $1;
					my $field_extra = $2;
					my $field_ref = {name => $field_name, comment => $3};
					parse_field_extra($field_ref, $field_extra);
					push(@{$table_ref->{collist}}, $field_name);
					$table_ref->{$field_name} = $field_ref;
					warn "finish field $field_name\n";
				}
				elsif ($_ =~ $reg_key_def) {
					my $key_type = $1;
					my $key_name = $2;
					my $fields = $3;
					parse_add_key($table_ref, $key_type, $fields);
				}
			}
			$tables->{$table_name} = $table_ref;
			push(@{$tables->{tablist}}, $table_name);
		}
	}
}

# 解析列定义的额外信息，首尾的字段名与注释除外
sub parse_field_extra
{
	my ($field_ref, $field_extra) = @_;
	if ($field_extra =~ /^\s*(\w+(?:\(\d+\))?)(\s+UNSIGNED)?/i) {
		my $data_type = $1;
		my $unsigned = $2;
		if ($unsigned && $data_type =~ /INT/i) {
			$data_type = "+$data_type";
		}
		$field_ref->{type} = $data_type;
	}

	if ($field_extra =~ /DEFAULT\s+(\S+)/i) {
		$field_ref->{default} = $1;
	}
	elsif ($field_extra =~ /NOT\s+NULL/i) {
		$field_ref->{default} = "NOT NULL";
	}
	else {
		$field_ref->{default} = "NOT NULL";
	}
}

# 标记列的索引类型
# 主键 PK ，唯一键 UK，非唯一键 MK
# 联合索引加数字后缀，同后缀的表示同组
sub parse_add_key
{
	my ($table_ref, $key_type, $fields) = @_;

	my $key_mark = "MK";
	if ($key_type) {
		if ($key_type =~ /PRIMARY/i) {
			$key_mark = "PK";
		}
		elsif ($key_type =~ /UNIQUE/i) {
			$key_mark = "UK";
		}
	}

	$fields =~ s/^\s*//;
	$fields =~ s/\s*$//;
	my @key_fields = split(/,\s*/, $fields);
	my $is_combine = 0;
	if (scalar(@key_fields) > 1) {
		$is_combine = 1;
		my $combine_seq = $table_ref->{combine_key} // 0;
		if ($combine_seq > 0 || $key_mark !~ /PK/i) {
			$key_mark = "$key_mark" . $combine_seq;
		}
		$combine_seq = $combine_seq + 1;
		$table_ref->{combine_key} = $combine_seq;
	}

	my $combine_no = 1;
	foreach my $field (@key_fields) {
		if ($table_ref->{$field}) {
			if ($is_combine) {
				$table_ref->{$field}->{iskey} = "$key_mark.$combine_no";
				$combine_no = $combine_no + 1;
			}
			else {
				$table_ref->{$field}->{iskey} = $key_mark;
			}
		}
	}
}

##-- MAIN --##
sub main
{
	my @argv = @_;
	&read_sql;
	&output_markdonw;
}

##-- SUBS --##

##-- END --##
&main(@ARGV) unless defined caller;
1;
__END__
