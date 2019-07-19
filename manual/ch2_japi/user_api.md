# 用户管理模块 API 细则

## 参数总览

在第一节的基础上细化参数如下。

* url 地址： http://47.110.237.215/cgi-bin/zfq_gateway.cgi
* to 参数： `user`

## 具体接口参数

以下针对每个接口，即 `action` 参数名称。输入输出参数表除特殊说明外，
都是指 `data` 对象所支持或要求的字段参数。

### 用户注册 `user_register`

** 输入： **

| 参数 | 类型 | 必填 | 说明 |
| ---- | ---- | ---- | ---- |
| `user_name` | string | N | 登陆名
| `mobile` | string | N | 登陆手机号
| `email` | string | N | 登陆邮箱
| `passwd` | string | Y | 登陆密码

* `user_name/mobile/email` 至少要填一个作为登陆名。
* 登陆密码用小写 md5 值加密传送保存

** 输出： **

| 参数 | 类型 | 必填 | 说明 |
| ---- | ---- | ---- | ---- |
| `user_id` | string | Y | 内部分配的用户 id

** 示例请求： **

```json
POST /cgi-bin/zfq_gateway.cgi

{
  "from":"app","to":"user",
  "session":{},
  "action":"user_register",
  "data":{"user_name":"lidey","mobile":"13566668888","email":"","passwd":"e10adc3949ba59abbe56e057f20f883e"}
}
```

实际 post 的 json 内容一般是紧凑的单行字符串，这里分行写只为可读良好。此例的 `passwd`
是简单密码 `123456` 的 md5 值。

** 示例响应： **

```json
200 OK

{
  "from":"app","to":"user",
  "action":"user_register",
  "error":0,
  "data":{"user_id":"1001"}
}
```

返回 `error` 为零时表示操作成功，则 `data.user_id` 表示新注册分配的唯一 id 。
如果错误码 `error` 非零表示业务操作失败，`data` 无意义，也可能缺失可为空。

http 请求与响应一般还有其他头部，在此从略。如果响应头的状态码不是 200 ，则表示
http 层处理失败，可能不再返回有效的 json 响应内容。

为简单起见，后续接口说明一般不再给出示例，只给出输入输出的 `data` 参数表。

### 用户登陆 `user_login`

** 输入： **

| 参数 | 类型 | 必填 | 说明 |
| ---- | ---- | ---- | ---- |
| `lognin_name` | string | Y | 登陆名
| `passwd` | string | Y | 登陆密码

* `login_name` 登陆名，可用上面的 `user_name/mobile/email` 其一登陆。
* `passwd` 登陆密码，小写 md5 值。

** 输出： **

| 参数 | 类型 | 必填 | 说明 |
| ---- | ---- | ---- | ---- |
| `user_id` | string | Y | 用户 id

### 用户登出 `user_logout`

** 输入： **

| 参数 | 类型 | 必填 | 说明 |
| ---- | ---- | ---- | ---- |
| `user_id` | string | Y | 用户 id

** 输出： ** （无额外参数）

### 新增接口 `action_todo`

** 输入： **

| 参数 | 类型 | 必填 | 说明 |
| ---- | ---- | ---- | ---- |

** 输出： **

| 参数 | 类型 | 必填 | 说明 |
| ---- | ---- | ---- | ---- |

未完待续……段落模块
