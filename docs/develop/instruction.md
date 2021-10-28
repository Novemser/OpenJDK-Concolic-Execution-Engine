# Instructino

## 当前指令相关

- 相关变量:
  - bci: bytecode-index
  - pc: 当前指令地址
- 宏 BCI(): 返回当前 bytecode-index (即在当前 method 下的指令序号)
  - (intptr_t)istate->method()->code_base(): 输出当前函数指令基址
- 