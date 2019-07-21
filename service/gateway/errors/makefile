.PHONY: all sql cpp md mysql clean

SRC = ./errorcode.hpp
SCRIPT = ./head2error.pl
MYSQL_CMD = mysql -h47.110.237.215 -uzhfquser -p'\#0xFF00FF' --default-character-set=utf8 db_zhfq

all:
	@echo make sql -- 生成插入表记录的语句 _errormsg.sql
	@echo make cpp -- 生成cpp初始化代码段  _errormsg.inc
	@echo make md --  生成mardown文档表格  _errormsg.md
	@echo make mysql -- 将错误码更新至 mysql 表内，须预建

sql: _errormsg.sql
cpp: _errormsg.inc
md: _errormsg.md
mysql: _to_mysql.tch

clean:
	rm -rf _errormsg.sql _errormsg.inc

_errormsg.sql : $(SRC) $(SCRIPT)
	$(SCRIPT) --format=sql $< > $@

_errormsg.inc : $(SRC) $(SCRIPT)
	$(SCRIPT) --format=cpp $< > $@

_errormsg.md : $(SRC) $(SCRIPT)
	$(SCRIPT) --format=md $< > $@

_to_mysql.tch : _errormsg.sql
	$(MYSQL_CMD) < $^
	touch $@
