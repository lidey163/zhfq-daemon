.PHONY: all clean echo md mysql resql

SQL = ./create_table.sql
BIN = ./formatsql.pl
MYSQL_CMD = mysql -h47.110.237.215 -uzhfquser -p'\#0xFF00FF' --default-character-set=utf8 db_zhfq

all:
	@echo make md -- 将 sql 转为 markdown 文档 _table_defined.md
	@echo make mysql -- 将 sql 语句导入 mysql 运行
	@echo make resql -- 重建表 tables="t1 t2" [drop=1 noexec=1] 
	@echo make echo -- 显示用到的变量

md : _table_defined.md $(SQL) $(BIN)
	$(BIN) $(SQL) > $<

mysql : _to_mysql.tch $(SQL)
	$(MYSQL_CMD) < $(SQL)
	touch $<

ifeq ($(drop), 1)
DROP_FLAG = "--drop"
endif

ifeq ($(noexec), 1)
resql : $(SQL) ./recreate_table.pl
	./recreate_table.pl $(DROP_FLAG) --file=$(SQL) $(tables)
else
resql : $(SQL) ./recreate_table.pl
	./recreate_table.pl $(DROP_FLAG) --file=$(SQL) $(tables) | $(MYSQL_CMD)
endif

echo :
	@echo MYSQL_CMD: $(MYSQL_CMD)
	@echo tables: $(tables)
	@echo drop?:  $(DROP_FLAG)
