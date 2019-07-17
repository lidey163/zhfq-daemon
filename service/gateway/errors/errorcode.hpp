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
	ERR_SCRIPT_NOT_FOUND = 1003,  // 脚本未找到
	ERR_SCRIPT_NOT_EXECT = 1004,  // 脚本不可执行
	ERR_SCRIPT_EXE_FAILED = 1005, // 脚本执行失败

	ERR_DATA_NOT_JSON = 1011, // 请求数据不是合法 json
	ERR_DATA_NOT_OBJECT = 1012, // 数据不是合法对象
	ERR_DATA_NOT_ARRAY = 1013, // 数据不是合法数组
	ERR_OBJECT_NO_KEY = 1014, // 对象缺少键值

	ERR_ARGUMENT_LOSS = 1021, // 参数缺失不全
	ERR_ARGUMENT_EMPTY = 1022, // 参数为空
	ERR_ARGUMENT_INVALID = 1023, // 非法参数值
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
