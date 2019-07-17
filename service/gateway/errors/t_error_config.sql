CREATE TABLE t_error_config (
	F_code INT NOT NULL COMMENT '错误号',
	F_macro VARCHAR(32) NOT NULL COMMENT '宏定义',
	F_message VARCHAR(64) DEFAULT NULL COMMENT '错误消息',
	F_group VARCHAR(16) DEFAULT '0' COMMENT '错误分组',
	PRIMARY KEY (`F_code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '错误配置表';

