#ifndef ERRORCODE_HPP__
#define ERRORCODE_HPP__

#include "errorutil.hpp"

enum ErrSystemType
{
	ERR_SYSTEM_ERROR = -1000, // 系统错误
	ERR_SYSTEM_BUSY = -1001, // 系统繁忙
};

enum ErrActionType
{
	ERR_SERVICE_NOT_FOUND = 1001, // 服务未找到
	ERR_ACTION_NOT_FOUND = 1002,  // 接口未找到
	ERR_SCRIPT_NOT_FOUND = 1003,  // 未找到可执行脚本
	ERR_SCRIPT_EXE_FAILED = 1004, // 脚本执行失败

	ERR_DATA_NOT_JSON = 1011, // 请求数据不是合法 json
	ERR_DATA_NOT_OBJECT = 1012, // 数据不是合法对象
	ERR_DATA_NOT_ARRAY = 1013, // 数据不是合法数组
	ERR_OBJECT_EMPTY = 1014, // 对象为空
	ERR_OBJECT_NO_KEY = 1015, // 对象缺少键值

	ERR_REQUEST_EMPTY = 10201, // 请求数据为空
	ERR_REQUEST_NOJSON = 10202, // 请求数据不是有效 json
	ERR_REQUEST_NOOBJ = 10203, // 请求数据不是json对象
	ERR_REQUEST_NOTO = 10204, // 请求缺少 to 标识
	ERR_REQUEST_NOACTION = 10205, // 请求缺少 action 标识
	ERR_REQUEST_NODATA = 10206, // 请求缺少 data 对象

	ERR_RESPONSE_NOJSON = 10301, // 返回值不是有效 json
	ERR_RESPONSE_NODATA = 10302, // 返回值无 data 对象

	ERR_ARGUMENT_LOSS = 1041, // 参数缺失不全
	ERR_ARGUMENT_EMPTY = 1042, // 参数为空
	ERR_ARGUMENT_INVALID = 1043, // 非法参数值

};

enum ErrUserType
{
	ERR_USER_NAME_EXISTS = 20001, // 用户名已存在
	ERR_USER_MOBILE_EXISTS = 20002, // 用户手机号已存在
	ERR_USER_EMAIL_EXISTS = 20003, // 用户邮箱已存在
	ERR_USER_NOT_EXISTS = 20004, // 用户不存在
	ERR_USER_NOT_LOGIN = 20005, // 用户未登陆
	ERR_USER_SESSION_EXPIRE = 20006, // 用户会话过期
};

#endif /* end of include guard: ERRORCODE_HPP__ */
