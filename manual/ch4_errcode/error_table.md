# 错误码一览表

<!--@include ../../service/gateway/errors/_errormsg.md -->

## ErrSystemType

| 错误码 | 宏定义 | 错误消息 |
| ------ | ------ | -------- |
| -1000 | `ERR_SYSTEM_ERROR` | 系统错误 |
| -1001 | `ERR_SYSTEM_BUSY` | 系统繁忙 |

## ErrActionType

| 错误码 | 宏定义 | 错误消息 |
| ------ | ------ | -------- |
| 1001 | `ERR_SERVICE_NOT_FOUND` | 服务未找到 |
| 1002 | `ERR_ACTION_NOT_FOUND` | 接口未找到 |
| 1003 | `ERR_SCRIPT_NOT_FOUND` | 未找到可执行脚本 |
| 1004 | `ERR_SCRIPT_EXE_FAILED` | 脚本执行失败 |
| 1011 | `ERR_DATA_NOT_JSON` | 请求数据不是合法 json |
| 1012 | `ERR_DATA_NOT_OBJECT` | 数据不是合法对象 |
| 1013 | `ERR_DATA_NOT_ARRAY` | 数据不是合法数组 |
| 1014 | `ERR_OBJECT_EMPTY` | 对象为空 |
| 1015 | `ERR_OBJECT_NO_KEY` | 对象缺少键值 |
| 10201 | `ERR_REQUEST_EMPTY` | 请求数据为空 |
| 10202 | `ERR_REQUEST_NOJSON` | 请求数据不是有效 json |
| 10203 | `ERR_REQUEST_NOOBJ` | 请求数据不是json对象 |
| 10204 | `ERR_REQUEST_NOTO` | 请求缺少 to 标识 |
| 10205 | `ERR_REQUEST_NOACTION` | 请求缺少 action 标识 |
| 10206 | `ERR_REQUEST_NODATA` | 请求缺少 data 对象 |
| 10301 | `ERR_RESPONSE_NOJSON` | 返回值不是有效 json |
| 10302 | `ERR_RESPONSE_NODATA` | 返回值无 data 对象 |
| 1041 | `ERR_ARGUMENT_LOSS` | 参数缺失不全 |
| 1042 | `ERR_ARGUMENT_EMPTY` | 参数为空 |
| 1043 | `ERR_ARGUMENT_INVALID` | 非法参数值 |

## ErrUserType

| 错误码 | 宏定义 | 错误消息 |
| ------ | ------ | -------- |
| 20001 | `ERR_USER_NAME_EXISTS` | 用户名已存在 |
| 20002 | `ERR_USER_MOBILE_EXISTS` | 用户手机号已存在 |
| 20003 | `ERR_USER_EMAIL_EXISTS` | 用户邮箱已存在 |
| 20004 | `ERR_USER_NOT_EXISTS` | 用户不存在 |
| 20005 | `ERR_USER_NOT_LOGIN` | 用户未登陆 |
| 20006 | `ERR_USER_SESSION_EXPIRE` | 用户会话过期 |

<!--@include/ -->
