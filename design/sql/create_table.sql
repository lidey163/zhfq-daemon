create table t_person(
	F_person_id varchar(18) not null comment '身份证号',
	F_sex tinyint not null comment '性别',
	F_name varchar(8) not null comment '姓名',
	F_birthday date not null comment '生日',
	F_partner_id varchar(18) not null commnet '配偶',
	primary key (F_person_id)
) comment '用户表';

create table t_company(
	F_id int unsigned auto creasement comment '序列号',
	F_name varchar(32) not null comment '公司名',
	F_industry_type varchar(8) not null comment '行业类型',
	F_province varchar(4) not null comment '省份',
	F_city varchar(4) not null comment '城市',
	F_address varchar(32) not null comment '详细地址',
	F_open_year year not null comment '创建年份',
	F_organization varchar(1024) default null comment '组织架构',
	F_create_time datetime not null comment '记录时间',
	F_update_time datetime not null comment '更新时间',
	primary key (F_id),
	key idx_name(F_name)
) comment '公司表';

-- F_organization 是一个 json 数组字符串，
-- 表示有哪些部门与子部门的层次结构，形如：
-- [{"name":"一级部门A","subpart":[...]}, ...]

create table t_staff(
	F_id int unsigned auto creasement comment '序列号',
	F_company_id int unsigned not null comment '公司号',
	F_person_id varchar(18) not null comment '身份证号',
	F_name varchar(8) not null comment '姓名',
	F_birthday date not null comment '生日',
	F_position varchar(8) default null comment '身份位置',
	F_pospath varchar(32) default null comment '详细部门路径',
	F_status tinyint default 0 comment '状态',
	F_create_time datetime not null comment '记录时间',
	F_update_time datetime not null comment '更新时间',
	primary key(F_id),
	key idx_company(F_company_id)
) comment '员工表';

create table t_staff_history(
	F_id int unsigned auto creasement comment '序列号',
	F_person_id varchar(32) not null comment '身份证号',
	F_company_id int unsigned not null comment '公司号',
	F_position varchar(8) default null comment '身份位置',
	F_pospath varchar(32) default null comment '详细部门路径',
	F_start_date date not null comment '就职日期',
	F_end_date date default null comment '离职日期',
	primary key(F_id),
	key idx_company(F_company_id)
) comment '历史员工表';

-- t_staff 表描叙一个人当前状态，从属哪个公司
-- t_staff_history 表描叙一个人历史就职记录
-- 有没人在当前状态也同属两家公司，任不同职位？
-- F_pospath 类似文件路径，用 / 分隔每部分表示所在公司的各级部门

create table t_person_secure(
	F_person_id varchar(18) not null comment '身份证id',
	F_sex tinyint not null comment '性别',
	F_name varchar(8) not null comment '姓名',
	F_birthday date not null comment '生日',
	F_secure_score smallint not null comment '总评分',
	F_create_time datetime not null comment '记录时间',
	F_update_time datetime not null comment '更新时间',
	F_basic_face varchar(1024) default '{}' comment '身份特质',
	F_train_face varchar(1024) default '{}' comment '安全教育培训',
	F_action_face varchar(1024) default '{}' comment '安全行为考核',
	F_lift_face varchar(1024) default '{}' comment '人脉关系',
	F_work_face varchar(1024) default '{}' comment '工作环境',
	F_favor_face varchar(1024) default '{}' comment '安全行为偏好',
	primary key(F_person_id),
	key idx_score(F_secure_score)
) comment '个人安全积分表';

create table t_person_secure_history(
	F_id bigint unsigned not null auto creasement comment '序列号',
	F_person_id varchar(18) not null comment '身份证id',
	F_secure_score smallint not null comment '总评分',
	F_count_date date not null comment '计分日期',
	primary key (F_id),
	idx_person (F_person_id)
) comment '个人安全积分历史表';
