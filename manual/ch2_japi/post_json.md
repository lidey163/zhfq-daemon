# POST JSON 模式的通用接口模式

可通过常规 http 请求实现的功能原则上都用这种接口规范。

## 接口规范

http 请求用 post 方法。请求与响应内容都使用 json 格式，`content-type` 
可设为 `text/json` 或 `application/json` 。

请求格式形如：

```json
{
  "from":"app","to":"serviceA",
  "session":{"device_id":"xxxx","platform":"ios","token":"xxx",...},
  "action":"xxx",
  "data":{...}
}
```

响应格式形如：

```json
{
  "action":"xxx",
  "error":0,"errstr":"",
  "data":{...}
}
```

一些要点说明：

* 首先有个入口网关服务器接收请求，根据 "to" 参数转发到负责相关具体业务的微服务
  器。"from" 不仅是为对称原因设计的参数，更可用以区分是来自 "web" 或 "app" 等
  不同前端的请求，如果有开发工具箱，也可以将 "from" 设为诸如 "toolbox" 或
  "debug-test" 的来源，以便区分作特殊处理。
* "session" 是有关连接会话的其他信息，包含但不限于会话id，设备id（或ip）等，
  用于控制验证。
* "action" 与 "data" 是具体的 api 参数。如果将整个网关服务视为一个统一的 api
  ，则 "action" 代表每个子 api ，各有不同的具体参数放在 "data" 中。
* 返回内容也放在 "data" 中。另外有个 "error" 错误码代表业务逻辑处理是否成功，
  如果有错误，可将详情错误消息放在 "errstr" 。至于 "action" 是近请求原路返回，
  如有需求，"from"/"to"/"session" 等参数也可原路返回。

## 执行流程

一个请求-响应的总体流程大约如下：

```
http <--> nginx <--> gateway(a fastcgi server) <--> serviceA|serviceB <--> mysql
serviceA <---> serviceB <---> serviceC ...
```

一条 http 请求，首先由 nginx 代理，通过 fastcgi 协议转向业务入口的网关服，然后
通过普通 socket 调用内部微服务，根据业务复杂度，有可能每个微服务之间还有互相调
用，以完成业务需求。内部微服务可用 rpc 调用，以 protobuf 进行数据交换。

在 nginx （或采用其他 web 服务器）中，可通过配置规则，将 cgi 路径映射到具体的
网关服。也可以进行负载均衡配置，不过首先限制瓶颈的应该会有数据库读写。

实践中的关键是微服务的划分。根据业务功能的重要程度与（或）使用频度等指标，化繁
为简，隔离为不同的服务。也利于在团队中分工维护。具体应用中，也不一定限制只用一
个网关服（fastcgi），或者在某些简单的业务中也可直接在 fastcgi 中完成，不必再调
用后续微服务。

## 适用场合与限制

首先，显然这只能对应于无状态的 http 请求，无法用于需要维护状态的连接如游戏、在
线聊天、流媒体等。

其次，也不适用于上传图片或文件的功能。因为上传文件的 post 内容不会是简单的
json 格式，若按 web 表单上传文件，一般是 `multipart/form-data` 加密数据。在这
情况下，适合使用单独的 cgi 来处理上传图片或文件。

不过如果是上传小图片，可将二进制数据以 base64 加密后用 json 提交，形如：

```json
{
  "action":"upload_png",
    "data":{"file":"xxx","size":1234,"base64":"xxx"}
}
```

但这不具备流量优势，算是一种 hack 吧。

除此之外，能用于大部分互联网 app 前后端的数据交换接口设计。统一风格，便于维护
与扩展。

