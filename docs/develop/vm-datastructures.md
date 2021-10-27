# VM Data Structures

## 线程

- **Thread**:
  - 有用的函数
    - 获取当前线程: `Thread::current()`
    - 

## 关于类

- **klass**: 包含了类的信息
  - **InstanceKclass**:
    - 获取 fields 数量: `ik->java_fields_count()`
    - 获取 fields 的 offset: `p ((InstanceKlass*)obj->_metadata._klass)->field_offset(1)`
    - 获取 field 名: `ik->field_name(index)`
    - 获取 field 签名: `ik->field_signature(index)`
    - 获取 field: `ik->field(index)`
      - **FieldInfo**

## 关于对象

- **oop**:
  - 打印类的概览: `obj->print()`: fields 下 `@num` 表示该 field 的 `offset`
    - 访问某个 field: 通过 `int_field(offset)`, `obj_field(offset)` 等. 例如访问 `java/util/Vector` 类型对象的时候, 可以通过 `p obj->obj_field(32)->_metadata._klass->name()->print()` 来访问其 `elementData`


## 工具类

- Symbol: 
  - 已知用途: 描述 klass 的 name
  - 有用的函数: 
    - 判断类名: `k_entry->name()->equals("java/lang/Object")`
    - 转化为 `char*` 表示: `as_C_string()`. 注: 由于这个 array 是用它的宏分配的, 需要用相应的释放方法(如下, 在 `rm` 的生命周期结束时, 自动释放其后分配的所有空间)
        ```cpp
        {
            ResourceMark rm;
            char* = callee->method_holder()->name()->as_C_string();
        }
        ```