# Debug

This is a document for debugging OpenJDK with `gdb`.

-----

## Use CLion

还没成功. 参考链接:

https://rqsir.github.io/2019/04/19/openjdk-8-%E4%BD%BF%E7%94%A8Clion%E8%B0%83%E8%AF%95%E6%BA%90%E7%A0%81/
https://www.cnblogs.com/yonghengzh/p/14266121.html

## Run

### Breakpoints

- While 循环中, 判断指令类型的 switch 语句:
    ```gdb
    break /home/hank/openjdk8/hotspot/src/share/vm/interpreter/bytecodeInterpreter.cpp:1011
    ```
    - 指令类型编码: `print opcode`, 具体对应关系见 `bytecodes.hpp`

