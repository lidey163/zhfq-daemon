# API 服务端实现指导

本章描叙服务端接口开发的基本方法与注意事项。

服务端总体框架是由 C++ 开发，包含业务接口功能。不过也开了个后门，允许每个业务
接口使用独立的任意脚本语言开发，当然运行效率与开发效率的交换。
