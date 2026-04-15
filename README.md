# OS课间实验
> 这ubuntu的输入法是真sb.

## Hint
VSCode无法直接给用户态代码打断点.需要在`DEBUG CONSOLE`里写上

```rust
-exec add-symbol-file user/_xargs 0 //_xargs是你想加断点的c代码程序
```

## Lab1
目标:实现sleep,find,findx函数.记得改`.Makefile`!
## 加强版系统调用
目标:要学生增加一个系统调用,用于检查某种状态下的进程数量.接受一个整型参数：
1) 如果传入参数为 1,则系统调用返回运行态进程的数量;
2) 如果传入参数为 2,则系统调用返回就绪态进程的数量;
3) 如果传入参数为 3,则系统调用返回阻塞态进程的数量.

本质上,我们需要在用户态调用一个接口,然后在内核态实现这个接口,并且在内核态实现这个接口的过程中,我们需要访问内核中的进程管理数据结构,来统计不同状态下的进程数量.

参考

`syscall.c,syscall.h,sysproc.c`(`./kernel`),

`proctest.c,user.h,usys.pl`(`./user`)

# Acknowledge
thx [Zoomy]("https://github.com/zoomy14112")

thx [Larry]("https://github.com/Larryyyyyyy")


