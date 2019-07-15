# 用户管理模块 API 细则

## 参数总览

在第一节的基础上细化参数如下。

* url 地址： http://47.110.237.215/cgi-bin/zfq_gateway.cgi
* to 参数： `user`

## 具体接口参数

以下针对每个接口，除特殊说明外，都是指 `data` 对象所支持或要求的字段参数。

### 用户注册 `user_register`

输入：

* `user_name` 登陆名代号，一般是英文字母名称；
* `mobile` 登陆手机号，11位数字；
* `email` 登陆邮箱，符合 email 地址的字符串；
* `passwd` 登陆密码，用 32 位小写 md5 值加密传送并保存。

`user_name/mobile/email` 至少要填一个作为登陆名。

输出：

* `user_id` 若注册成功，返回内部分配的用户 id 。

### 用户登陆 `user_login`

输入：

* `login_name` 登陆名，可用上面的 `user_name/mobile/email` 其一登陆。
* `passwd` 登陆密码，小写 md5 值。

输出：

* `user_id` 若注册成功，返回内部唯一标志的用户 id 。

### 用户登出 `user_login`

输入：

* `login_name` 登陆名，可用上面的 `user_name/mobile/email` 其一登陆。
* `passwd` 登陆密码，小写 md5 值。

输出：

* `user_id` 若注册成功，返回内部唯一标志的用户 id 。
