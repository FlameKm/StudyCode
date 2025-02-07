# SHM 内存共享

```shell
cmake -S . -B ./build && cmake --build ./build
```


先执行 app_read, 再执行 app_write

实现的功能是：
- app_read 读取共享内存中的数据
- app_write 写入数据到共享内存中


来源：https://www.cnblogs.com/52php/p/5861372.html