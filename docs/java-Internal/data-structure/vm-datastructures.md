# VM Data Structures

## 线程

- **Thread**:
  - 有用的函数
    - 获取当前线程: `Thread::current()`
    - 

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