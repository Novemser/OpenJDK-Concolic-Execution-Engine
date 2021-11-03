# Jave Object

This document explains how hotspot JVM handles java object.
With following information, you can be familiar with accessing java object information.

## The Header Metadata

All java objects have the same header called [oopDesc](../../../hotspot/src/share/vm/oops/oop.hpp),
which is used to describe the format of java objects in C++.
This [link](https://gist.github.com/arturmkrtchyan/43d6135e8a15798cc46c) and [link](https://www.cnblogs.com/mazhimazhi/p/13289686.html) gives the overview.

`oopDesc` has two following fields:

- **mark**:
- **klass**: 包含了类的信息
  - **InstanceKlass**:
    - 获取 fields 数量: `ik->java_fields_count()`
    - 获取 fields 的 offset: `p ((InstanceKlass*)obj->_metadata._klass)->field_offset(1)`
    - 获取 field 名: `ik->field_name(index)`
    - 获取 field 签名: `ik->field_signature(index)`
    - 获取 field: `ik->field(index)`
      - **FieldInfo**
  - **ArrayKlass**:
    - ArrayKlass::cast(stack_object(topOfStack, -2)->klass())

## 关于对象

- **oop**:
  - 打印类的概览: `obj->print()`: fields 下 `@num` 表示该 field 的 `offset`
    - 访问某个 field: 通过 `int_field(offset)`, `obj_field(offset)` 等. 例如访问 `java/util/Vector` 类型对象的时候, 可以通过 `p obj->obj_field(32)->_metadata._klass->name()->print()` 来访问其 `elementData`
