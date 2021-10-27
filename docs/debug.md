# Debug

This is a document for debugging OpenJDK with `gdb`.

Before debugging, you should build jdk and run

```shell
./scripts/unzipDebuginfo.sh
```
-----

## Breakpoints
Our debugging target is [bytecode interpretor](../hotspot/src/share/vm/interpreter/bytecodeInterpreter.cpp)


## Commands

- 仅提供 char* 指针, 打印一个字符串: `p str@len`. 如: `p k_entry->name()->_body@k_entry->name()->_length`

## Run

```gdb
break hotspot/src/share/vm/interpreter/bytecodeInterpreter.cpp:1011
```

## Use CLion

还没成功. 参考链接:

https://rqsir.github.io/2019/04/19/openjdk-8-%E4%BD%BF%E7%94%A8Clion%E8%B0%83%E8%AF%95%E6%BA%90%E7%A0%81/
https://www.cnblogs.com/yonghengzh/p/14266121.html
