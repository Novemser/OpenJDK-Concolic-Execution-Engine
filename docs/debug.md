# Debug

This is a document for debugging OpenJDK with `gdb`.

-----

## Use CLion

还没成功. 参考链接:

https://rqsir.github.io/2019/04/19/openjdk-8-%E4%BD%BF%E7%94%A8Clion%E8%B0%83%E8%AF%95%E6%BA%90%E7%A0%81/
https://www.cnblogs.com/yonghengzh/p/14266121.html

## Commands

- 仅提供 char* 指针, 打印一个字符串: `p str@len`. 如: `p k_entry->name()->_body@k_entry->name()->_length`

## Run

### Breakpoints

- While 循环中, 判断指令类型的 switch 语句:
    ```gdb
    break /home/hank/openjdk8/hotspot/src/share/vm/interpreter/bytecodeInterpreter.cpp:1011
    ```
    - 指令类型编码: `print opcode`, 具体对应关系见 `bytecodes.hpp`

## Common Datastructure

### 关于类

- **klass**: 包含了类的信息
  - **InstanceKclass**:
    - 获取 fields 数量: `ik->java_fields_count()`
    - 获取 fields 的 offset: `p ((InstanceKlass*)obj->_metadata._klass)->field_offset(1)`
    - 获取 field 名: `ik->field_name(index)`
    - 获取 field 签名: `ik->field_signature(index)`
    - 获取 field: `ik->field(index)`
      - **FieldInfo**: 
      - 

### 关于对象

- **oop**:
  - 打印类的概览: `obj->print()`: fields 下 `@num` 表示该 field 的 `offset`
    - 访问某个 field: 通过 `int_field(offset)`, `obj_field(offset)` 等. 例如访问 `java/util/Vector` 类型对象的时候, 可以通过 `p obj->obj_field(32)->_metadata._klass->name()->print()` 来访问其 `elementData`


### 工具类

- Symbol: 
  - 已知用途: 描述 klass 的 name
  - 有用的函数: 
    - 判断类名: `k_entry->name()->equals("java/lang/Object")`