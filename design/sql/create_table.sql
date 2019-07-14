-- mysql -h47.110.237.215 -uzhfquser -p'#0xFF00FF' db_zhfq

CREATE DATABASE IF NOT EXISTS db_zhfq DEFAULT CHARSET=utf8;
use db_zhfq;

-- 平台用户表
CREATE TABLE t_user(
	F_id BIGINT UNSIGNED AUTO_INCREMENT COMMENT '序列号',
	F_mobile VARCHAR(11) DEFAULT '0' COMMENT '手机号',
	F_email VARCHAR(32) DEFAULT '@' COMMENT '电子邮箱',
	F_name VARCHAR(16) DEFAULT '0' COMMENT '登陆名',
	F_passwd VARCHAR(16) DEFAULT '0' COMMENT '登陆密码',
	F_person_id VARCHAR(18) DEFAULT '0' COMMENT '人员ID',
	F_head_url VARCHAR(128) DEFAULT '0' COMMENT '头像地址',
	F_follow VARCHAR(1024) DEFAULT '[]' COMMENT '关注对象列表',
	F_create_time DATETIME NOT NULL COMMENT '记录时间',
	F_update_time DATETIME NOT NULL COMMENT '更新时间',
	PRIMARY KEY(F_id),
	KEY idx_mobile(F_mobile),
	KEY idx_create_time(F_create_time)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '用户表';
-- F_follow 用 json 数组表示，关注？意义暂不明显

-- 当前会话管理表
CREATE TABLE t_session(
	F_user_id BIGINT UNSIGNED DEFAULT 0 COMMENT '用户id',
	F_token VARCHAR(32) DEFAULT '0' COMMENT 'token 口令',
	F_login_count INT UNSIGNED DEFAULT 0 COMMENT '登陆次数',
	F_last_login DATETIME DEFAULT NULL COMMENT '上次登陆时间',
	F_last_logout DATETIME DEFAULT NULL COMMENT '上次登出时间',
	PRIMARY KEY(F_user_id),
	KEY idx_token(F_token)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '登陆会话记录表';
-- 可将 token 入在 redis 等内存数据库加速优化

-- 自然人基本信息表，直接用身份证主键
CREATE TABLE t_person(
	F_person_id VARCHAR(18) NOT NULL COMMENT '身份证号',
	F_sex TINYINT NOT NULL COMMENT '性别',
	F_name VARCHAR(8) NOT NULL COMMENT '姓名',
	F_birthday DATE NOT NULL COMMENT '生日',
	F_partner_id VARCHAR(18) NOT NULL COMMENT '配偶',
	PRIMARY KEY(F_person_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '人员表';

-- 公司组织实体表
CREATE TABLE t_company(
	F_id INT UNSIGNED AUTO_INCREMENT COMMENT '序列号',
	F_name VARCHAR(32) NOT NULL COMMENT '公司名',
	F_industry_type VARCHAR(8) NOT NULL COMMENT '行业类型',
	F_province VARCHAR(4) NOT NULL COMMENT '省份',
	F_city VARCHAR(4) NOT NULL COMMENT '城市',
	F_address VARCHAR(32) NOT NULL COMMENT '详细地址',
	F_open_year YEAR NOT NULL COMMENT '创建年份',
	F_organization VARCHAR(1024) DEFAULT NULL COMMENT '组织架构',
	F_create_time DATETIME NOT NULL COMMENT '记录时间',
	F_update_time DATETIME NOT NULL COMMENT '更新时间',
	PRIMARY KEY(F_id),
	KEY idx_name(F_name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '公司表';
-- F_organization 是一个 json 数组字符串，
-- 表示有哪些部门与子部门的层次结构，形如：
-- [{"name":"一级部门A","subpart":[...]}, ...]

-- 公司员工关联表
CREATE TABLE t_staff(
	F_id INT UNSIGNED AUTO_INCREMENT COMMENT '序列号',
	F_company_id INT UNSIGNED NOT NULL COMMENT '公司号',
	F_person_id VARCHAR(18) NOT NULL COMMENT '身份证号',
	F_name VARCHAR(8) NOT NULL COMMENT '姓名',
	F_birthday DATE NOT NULL COMMENT '生日',
	F_position VARCHAR(8) DEFAULT NULL COMMENT '身份位置',
	F_pospath VARCHAR(32) DEFAULT NULL COMMENT '详细部门路径',
	F_status TINYINT DEFAULT 0 COMMENT '状态',
	F_create_time DATETIME NOT NULL COMMENT '记录时间',
	F_update_time DATETIME NOT NULL COMMENT '更新时间',
	PRIMARY KEY(F_id),
	KEY idx_company(F_company_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '员工表';

-- 员工历史就职表
CREATE TABLE t_staff_history(
	F_id INT UNSIGNED AUTO_INCREMENT COMMENT '序列号',
	F_person_id VARCHAR(32) NOT NULL COMMENT '身份证号',
	F_company_id INT UNSIGNED NOT NULL COMMENT '公司号',
	F_position VARCHAR(8) DEFAULT NULL COMMENT '身份位置',
	F_pospath VARCHAR(32) DEFAULT NULL COMMENT '详细部门路径',
	F_start_date DATE NOT NULL COMMENT '就职日期',
	F_end_date DATE DEFAULT NULL COMMENT '离职日期',
	PRIMARY KEY(F_id),
	KEY idx_company(F_company_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '历史员工表';

-- t_staff 表描叙一个人当前状态，从属哪个公司
-- t_staff_history 表描叙一个人历史就职记录
-- 有没人在当前状态也同属两家公司，任不同职位？
-- F_pospath 类似文件路径，用 / 分隔每部分表示所在公司的各级部门

-- 当前安全积分管理表
CREATE TABLE t_person_secure(
	F_person_id VARCHAR(18) NOT NULL COMMENT '身份证ID',
	F_sex TINYINT NOT NULL COMMENT '性别',
	F_name VARCHAR(8) NOT NULL COMMENT '姓名',
	F_birthday DATE NOT NULL COMMENT '生日',
	F_secure_score SMALLINT NOT NULL COMMENT '总评分',
	F_create_time DATETIME NOT NULL COMMENT '记录时间',
	F_update_time DATETIME NOT NULL COMMENT '更新时间',
	F_basic_face VARCHAR(1024) DEFAULT '{}' COMMENT '身份特质',
	F_train_face VARCHAR(1024) DEFAULT '{}' COMMENT '安全教育培训',
	F_action_face VARCHAR(1024) DEFAULT '{}' COMMENT '安全行为考核',
	F_lift_face VARCHAR(1024) DEFAULT '{}' COMMENT '人脉关系',
	F_work_face VARCHAR(1024) DEFAULT '{}' COMMENT '工作环境',
	F_favor_face VARCHAR(1024) DEFAULT '{}' COMMENT '安全行为偏好',
	PRIMARY KEY(F_person_id),
	KEY idx_score(F_secure_score)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '个人安全积分表';
-- 每个维度的安全积分数据用 json 对象表示，灵活扩展

-- 安全积分历史汇总表
CREATE TABLE t_person_secure_history(
	F_id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT '序列号',
	F_person_id VARCHAR(18) NOT NULL COMMENT '身份证ID',
	F_secure_score SMALLINT NOT NULL COMMENT '总评分',
	F_count_date DATE NOT NULL COMMENT '计分日期',
	PRIMARY KEY(F_id),
	KEY idx_person(F_person_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT '个人安全积分历史表';
