.PHONY: all sql cpp clean

SRC = ./errorcode.hpp
SCRIPT = ./head2error.pl

all:
	@echo make sql -- 生成插入表记录的语句 errormsg.sql
	@echo make cpp -- 生成cpp初始化代码段  errormsg.inc

sql: _errormsg.sql
cpp: _errormsg.inc

clean:
	rm -rf _errormsg.sql _errormsg.inc

_errormsg.sql : $(SRC) $(SCRIPT)
	$(SCRIPT) --format=sql $< > $@

_errormsg.inc : $(SRC) $(SCRIPT)
	$(SCRIPT) --format=cpp $< > $@
