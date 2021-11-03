# VM Data Structures

## 栈结构

**istate**
- _thread: 指向当前县城
- _bcp: address 类型, 指向当前函数 code base
- _locals: 指向当前帧的 locals 的基址
- _constants: 常量池 **TODO**
- _method: Method* 类型, 指向当前函数
- _mdx: **TODO**
- _stack: 指向当前帧的栈顶
- _msg: 与上层函数通信的消息
- _result: **TODO**
- _prev_link: **TODO**
- _oop_temp: **TODO**
- _stack_base: 当前帧的操作栈基址
- _stack_limit: 当前帧的操作栈最大地址应小于该值, 注意其与 _stack 有所区别.
- _monitor_base: **TODO**
- _self_link: **TODO**
- frame_type: 帧类型
- next_frame: 上一帧的基址

```
current stack pointer 0x7f532ed446b8 0x7f532ed446c8 1
Java frames:
 0x00007f532ed446b0: stack_word[2]         = 0x00007f532ed44770
 0x00007f532ed446b8: stack_word[1]         = 0x00007f5300000005
 0x00007f532ed446c0: stack_word[0]         = 0x00007f5300000008
 0x00007f532ed446c8: (vtable for istate)   = 0x00007f532ed446b0
 0x00007f532ed446d0: istate->_thread       = 0x00007f532800c1d0
 0x00007f532ed446d8: istate->_bcp          = 0x00007f532c5da3fd (bci 5)
 0x00007f532ed446e0: istate->_locals       = 0x00007f532ed44770
 0x00007f532ed446e8: istate->_constants    = 0x00007f532c5da748
 0x00007f532ed446f0: istate->_method       = TestInvoke.add2(LTestInvoke$MyInteger;LTestInvoke$MyInteger;)I
 0x00007f532ed446f8: istate->_mdx          = 0x0000000000000000
 0x00007f532ed44700: istate->_stack        = 0x00007f532ed446b0
 0x00007f532ed44708: istate->_msg          = 0x00007f5300000003
 0x00007f532ed44710: istate->_result       = 0x00007f532c5daad8
 0x00007f532ed44718: (istate->_result)     = 0x00007f532cb31218
 0x00007f532ed44720: (istate->_result)     = 0x0000000000000003
 0x00007f532ed44728: istate->_prev_link    = 0x0000000000000000
 0x00007f532ed44730: istate->_oop_temp     = 0x0000000000000000
 0x00007f532ed44738: istate->_stack_base   = 0x00007f532ed446c8
 0x00007f532ed44740: istate->_stack_limit  = 0x00007f532ed446a8
 0x00007f532ed44748: istate->_monitor_base = 0x00007f532ed446c8
 0x00007f532ed44750: istate->_self_link    = 0x00007f532ed446c8
 0x00007f532ed44758: frame_type            = INTERPRETER_FRAME
 0x00007f532ed44760: next_frame            = 0x00007f532ed44810

 0x00007f532ed44768: local[1]              = 0x00007f5318081bd0
 0x00007f532ed44770: local[0]              = 0x00007f5318081bb0
 0x00007f532ed44778: (vtable for istate)   = 0x00007f532800c9d0
 0x00007f532ed44780: istate->_thread       = 0x00007f532800c1d0
 0x00007f532ed44788: istate->_bcp          = 0x00007f532c5da4c0 (bci 32)
 0x00007f532ed44790: istate->_locals       = 0x00007f532ed44830
 0x00007f532ed44798: istate->_constants    = 0x00007f532c5da748
 0x00007f532ed447a0: istate->_method       = TestInvoke.main([Ljava/lang/String;)V
 0x00007f532ed447a8: istate->_mdx          = 0x0000000000000000
 0x00007f532ed447b0: istate->_stack        = 0x00007f532ed44760
 0x00007f532ed447b8: istate->_msg          = 0x00007f5300000008
 0x00007f532ed447c0: istate->_result       = 0x00007f532c5da408
 0x00007f532ed447c8: (istate->_result)     = 0x00007f532cb31200
 0x00007f532ed447d0: (istate->_result)     = 0x00007f5300000003
 0x00007f532ed447d8: istate->_prev_link    = 0x0000000000000000
 0x00007f532ed447e0: istate->_oop_temp     = 0x0000000000000000
 0x00007f532ed447e8: istate->_stack_base   = 0x00007f532ed44778
 0x00007f532ed447f0: istate->_stack_limit  = 0x00007f532ed44750
 0x00007f532ed447f8: istate->_monitor_base = 0x00007f532ed44778
 0x00007f532ed44800: istate->_self_link    = 0x00007f532ed44778
 0x00007f532ed44808: frame_type            = INTERPRETER_FRAME
 0x00007f532ed44810: next_frame            = 0x00007f532ed44848

 0x00007f532ed44818: local[3]              = 0x0000000000000000
 0x00007f532ed44820: local[2]              = 0x00007f5318081bd0
 0x00007f532ed44828: local[1]              = 0x00007f5318081bb0
 0x00007f532ed44830: local[0]              = 0x00007f5318080060
 0x00007f532ed44838: call_wrapper          = 0x00007f532ed449a0
 0x00007f532ed44840: frame_type            = ENTRY_FRAME
 0x00007f532ed44848: next_frame            = 0x0000000000000000
```


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