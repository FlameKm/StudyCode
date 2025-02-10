# ERPC

## 描述
嵌入式 RPC 框架，支持多种语言，该 DEMO 用 C 实现

erpc sdk（下面官网）得到 erocgen 工具，可以生成客户端和服务端代码，然后通过 erpc sdk 提供的 API 进行调用。

erpc 文件夹是 erpc sdk 生成的源码，其中的 liba 是 x86_64-linux 编译的。

erpcgen 是 erpcgen 工具生成的代码，其中 hello.erpc 是用于生成的 IDL 文件。

## CODE

```shell
cd erpcgen
erpcgen ./hello.erpc
# 后面就是 cmake 正常的编译流程
```

实现的功能是 client 调用存在于 server 的函数


## 参考链接
官网：
- https://github.com/EmbeddedRPC/erpc/wiki
  API：
- https://embeddedrpc.github.io/eRPC/index.html
- https://github.com/EmbeddedRPC/erpc/wiki/IDL-Reference
-  https://cloud.tencent.com/developer/article/1620285