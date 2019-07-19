# 用任意脚本实现业务接口

## 概述

当某个业务接口在服务器中没有 C++ 实现的处理函数，则会尝试查找相应的处理脚本。
将请求数据的 json 字符串当作唯一的命令行参数调用脚本，并捕获脚本的标准输出当作
接口的响应内容，故而按规范要求输出合法的 json 字符串。

类似在命令行执行脚本：

```bash
$ /path/to/script '{"from":"app","to":"xxx","action":"xxx","data":{}}'
```

类似标准 CGI 的调用模型，但更为简便，只要处理命令行参数，并向标准输出。所以理
论上，只要能方便处理 json 的脚本语言都可以使用，当然在实际业务中还要能读写
mysql 数据库。

## 脚本位置约定

脚本位置应位于 `$(script_base_dir)/to/action[.ext]` 。

其中脚本基础目录是服务可配置的，然后以请求数据的 `to` 参数当作一级子目录名，
`action` 参数当作脚本文件名。

脚本文件必须是可执行的，后缀名则是可选的。

## 后缀名考虑

在 linux 系统中，文件后缀名从来就不是必须的。但习惯上会根据脚本语言添加后缀，
如 perl 脚本后缀 `.pl` ，python 脚本后缀 `.py` 等。

在查找某个业务接口的处理脚本时，是完全按照 `action` 参数名查找的。所以不应该有
添加额外的后缀名。但是可以使用链接或软链接关联习惯上有后缀的脚本文件，如：

```bash
$ ln -s action_name.pl action_name
$ ln -s action_name.py action_name
```

如此还可以尝试不同的脚本来实现接口，只要改一下链接指向。

另一个方案是约定接口名本身就有后缀，例如请求内容类似：

```json
{"from":"app","to":"xxx","action":"action_name.py","data":{}}
```

如此就能精确查找到 `xxx/action_name.py` 脚本。

不过在接口名约定上添加特定后缀的做法，不利于迁移。假如将来想将该接口重写为 C++
处理函数，则可能要求客户端调用处也改接口名。

## 输出格式

脚本的标准输出的 json 格式，可参考上一节的 C++ 接口处理函数的返回参数，此不重
复。

完整输出须包含 `data` ，大约如下：

```json
{"error":0,"data":{"key":"val"}}
{"error":-1,"data":{}}
```

精简输出不须包含 `data` ，假设输出的 json 即为内嵌的 `data` ，形如：

```json
{"key":"val"}
{"../error":-1}
```

输出的 json 字符串不一定单行，但一般 json 库序列化的默认行为是单行格式。

一些脚本语言有指定退出码（exit status）的方法，或许可直接用退出码表示要返回的
错误码，如 `exit 1011;` 。但此方法对另一些脚本语言或许不可靠，故而建议显式向标
准输出 `error` 或 `../error` 字段表示错误码。如要定制错误消息，同样可输出
`errstr` 或 `../errstr` 字段。当然没有错误时可省略。

## 示例脚本

假设用 perl 语言写的一个 `debug/hello.pl` 接口。

```perl
#! /usr/bin/env perl
print qq({"greet":"Hello Perl","../error":0});
# print qq({"data":{"greet":"Hello Perl"},"error":0});
```

当然这只是打印输出固定字符串，无多大意义。要处理输入参数，可使用 JSON 包。

```perl
#! /usr/bin/env perl
use JSON;
my $str_req = shift; # 读取命令行参数
my $json_req = decode_json($str_req); # 将json串解码为内部数据结构
my $json_data = $json_req->{data} || {}; # 取 data 对象
my $name = $json_data->{name} || "World"; # 取 data.name 值

my $json_ret = {greet => "Hello $name!"}; # 组建json对应的内部数据结构
my $str_ret = encode_json($json_ret); # 序列化json串

print $str_ret, "\n"; # 标准输出
# exit 0;
```

参考注释，用其他语言写脚本的基本流程也类似。可先在命令行测试该脚本。

```bash
$ ./hello.pl '{}'
# 输出 {"greet":"Hello World!"}
$ ./hello.pl '{"data":{"name":"Perl"}}'
# 输出 {"greet":"Hello Perl!"}
```

然后通过网关服务用 http 请求，参考接口请求规范大约如：

```json
POST /cgi-bin/zhfq-gateway.cgi

{"from":"app","to":"debug","action":"hello.pl","data":{"name":"Perl"}}
```

响应大约如：

```json
{
  "from":"app","to":"debug","action":"hello.pl",
  "error":0,
  "data":{"greet":"Hello Perl!"}
}
```
