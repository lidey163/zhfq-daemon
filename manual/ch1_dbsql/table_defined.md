# 表结构定义一览

本节文档从规范的建表 sql 语句提取生成，以便参考。
以下表中数据类型列的 `+` 前缀表示各种无符号整型。
索引 `PK` 表示主键，`UK` 表示唯一键，其他可重复键用 `MK` 表示，
涉及联合几个字段当作索引的加数字后缀表示顺序，如 `PK.1 PK.2` 
或 `UK1.1 UK1.2` `MK2.1 MK2.2` 等。

<!--@include ../../sql/_table_defined.md -->

## t_user: 用户表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_id` | +BIGINT | NOT NULL | PK | 序列号 |
| `F_mobile` | VARCHAR(11) | '0' | MK | 手机号 |
| `F_email` | VARCHAR(32) | '@' |  | 电子邮箱 |
| `F_name` | VARCHAR(16) | '0' |  | 登陆名 |
| `F_passwd` | VARCHAR(16) | '0' |  | 登陆密码 |
| `F_person_id` | VARCHAR(18) | '0' |  | 人员ID |
| `F_head_url` | VARCHAR(128) | '0' |  | 头像地址 |
| `F_follow` | VARCHAR(1024) | '[]' |  | 关注对象列表 |
| `F_create_time` | DATETIME | NOT NULL | MK | 记录时间 |
| `F_update_time` | DATETIME | NOT NULL |  | 更新时间 |

## t_session: 登陆会话记录表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_user_id` | +BIGINT | 0 | PK | 用户id |
| `F_token` | VARCHAR(32) | '0' | MK | token 口令 |
| `F_login_count` | +INT | 0 |  | 登陆次数 |
| `F_last_login` | DATETIME | NULL |  | 上次登陆时间 |
| `F_last_logout` | DATETIME | NULL |  | 上次登出时间 |

## t_person: 人员表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_person_id` | VARCHAR(18) | NOT NULL | PK | 身份证号 |
| `F_sex` | TINYINT | NOT NULL |  | 性别 |
| `F_name` | VARCHAR(8) | NOT NULL |  | 姓名 |
| `F_birthday` | DATE | NOT NULL |  | 生日 |
| `F_partner_id` | VARCHAR(18) | NOT NULL |  | 配偶 |

## t_company: 公司表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_id` | +INT | NOT NULL | PK | 序列号 |
| `F_name` | VARCHAR(32) | NOT NULL | MK | 公司名 |
| `F_industry_type` | VARCHAR(8) | NOT NULL |  | 行业类型 |
| `F_province` | VARCHAR(4) | NOT NULL |  | 省份 |
| `F_city` | VARCHAR(4) | NOT NULL |  | 城市 |
| `F_address` | VARCHAR(32) | NOT NULL |  | 详细地址 |
| `F_open_year` | YEAR | NOT NULL |  | 创建年份 |
| `F_organization` | VARCHAR(1024) | NULL |  | 组织架构 |
| `F_create_time` | DATETIME | NOT NULL |  | 记录时间 |
| `F_update_time` | DATETIME | NOT NULL |  | 更新时间 |

## t_staff: 员工表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_id` | +INT | NOT NULL | PK | 序列号 |
| `F_company_id` | +INT | NOT NULL | MK | 公司号 |
| `F_person_id` | VARCHAR(18) | NOT NULL |  | 身份证号 |
| `F_name` | VARCHAR(8) | NOT NULL |  | 姓名 |
| `F_birthday` | DATE | NOT NULL |  | 生日 |
| `F_position` | VARCHAR(8) | NULL |  | 身份位置 |
| `F_pospath` | VARCHAR(32) | NULL |  | 详细部门路径 |
| `F_status` | TINYINT | 0 |  | 状态 |
| `F_create_time` | DATETIME | NOT NULL |  | 记录时间 |
| `F_update_time` | DATETIME | NOT NULL |  | 更新时间 |

## t_staff_history: 历史员工表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_id` | +INT | NOT NULL | PK | 序列号 |
| `F_person_id` | VARCHAR(32) | NOT NULL |  | 身份证号 |
| `F_company_id` | +INT | NOT NULL | MK | 公司号 |
| `F_position` | VARCHAR(8) | NULL |  | 身份位置 |
| `F_pospath` | VARCHAR(32) | NULL |  | 详细部门路径 |
| `F_start_date` | DATE | NOT NULL |  | 就职日期 |
| `F_end_date` | DATE | NULL |  | 离职日期 |

## t_person_secure: 个人安全积分表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_person_id` | VARCHAR(18) | NOT NULL | PK | 身份证ID |
| `F_sex` | TINYINT | NOT NULL |  | 性别 |
| `F_name` | VARCHAR(8) | NOT NULL |  | 姓名 |
| `F_birthday` | DATE | NOT NULL |  | 生日 |
| `F_secure_score` | SMALLINT | NOT NULL | MK | 总评分 |
| `F_create_time` | DATETIME | NOT NULL |  | 记录时间 |
| `F_update_time` | DATETIME | NOT NULL |  | 更新时间 |
| `F_basic_face` | VARCHAR(1024) | '{}' |  | 身份特质 |
| `F_train_face` | VARCHAR(1024) | '{}' |  | 安全教育培训 |
| `F_action_face` | VARCHAR(1024) | '{}' |  | 安全行为考核 |
| `F_lift_face` | VARCHAR(1024) | '{}' |  | 人脉关系 |
| `F_work_face` | VARCHAR(1024) | '{}' |  | 工作环境 |
| `F_favor_face` | VARCHAR(1024) | '{}' |  | 安全行为偏好 |

## t_person_secure_history: 个人安全积分历史表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_id` | +BIGINT | NOT NULL | PK | 序列号 |
| `F_person_id` | VARCHAR(18) | NOT NULL | MK | 身份证ID |
| `F_secure_score` | SMALLINT | NOT NULL |  | 总评分 |
| `F_count_date` | DATE | NOT NULL |  | 计分日期 |

## t_error_config: 错误码消息配置表

| 字段列名 | 数据类型 | 默认值 | 索引 | 注释说明 |
| -------- | -------- | ------ | ---- | ------- |
| `F_code` | INT | NOT NULL |  | 错误号 |
| `F_macro` | VARCHAR(32) | NOT NULL |  | 宏定义 |
| `F_message` | VARCHAR(64) | NULL |  | 错误消息 |
| `F_group` | VARCHAR(16) | '0' |  | 错误分组 |

<!--@include/ -->
