# webserver

尝试复现一个 webserver。
写不出来我是狗。

## log

- 一个固定大小的线程池
- 实现了只处理一个连接的服务端，能处理并响应指定 http 请求
- 实现了简易的日志模块（同步的方式）

## error

记录一些存在的问题。

- 浏览器访问时有时会出现两次相同的访问，会发送两次 http。

参考：

- <https://github.com/qinguoyi/TinyWebServer>
- <https://github.com/markparticle/WebServer>
- <https://github.com/cmu-db/bustub>(cmake)
