![logo](./WeBridge.png)

[![pipeline status](https://ipads.se.sjtu.edu.cn:1312/ipads-storage/codebase/wbridge/openjdk8-webridge/badges/master/pipeline.svg)](https://ipads.se.sjtu.edu.cn:1312/ipads-storage/codebase/wbridge/openjdk8-webridge/-/commits/master)

# WeBridge OpenJDK Concolic Execution Engine

Implementation of a concolic execution engine on top of OpenJDK 8.

The engine currently supports interpreter mode execution.

# Prerequisites
We have tested the code in the following environment:

Operating system: Ubuntu 16.04

GCC/G++: 6.5.0

# How to build
First install necessary dependencies.
```shell
./scripts/install_dependencies.sh
```
Next, configure the project.
```shell
./scripts/configure.sh
```
Then, build the project.
```shell
make all -j CONF=linux-x86_64-normal-zero-release
```

The built concolic execution JDK path will be in `./build/linux-x86_64-normal-zero-release/images/j2sdk-image`. Next, set this path as your default `JAVA_HOME` to use the concolic execution JDK.

# Quick start

Let's first star with a demo example that uses the concolic JDK to collect the path conditions of a simple [program](./example/Demo.java):

Create a `Demo.java` with the following content:
```java
import java.util.*;

public class Demo {
	public static void main(String[] args) throws Exception {
        Integer val = new Integer(1);
        String str = new String("ipads");

		// initialize the concolic execution engine, and symbolize the parameters that you'd like to track with symbolic information
		System.startConcolic();
		System.symbolize(val);
		System.symbolize(str);

        // compare the symbolic variable `val` with a concrete constant value. We would expect the path condition to be `val != 3`
        if (val == 3) {
			System.out.println("Value = 3");
		}

        // next, we add another path condition
		if (str.startsWith("ip")) {
			System.out.println("Str starts with ip");
		}
        System.out.println("Path condition=" + System.getPathCondition());
        System.endConcolic();
	}
}
```

Compile `Demo.java` with the `javac` compiler in the concolic JDK:
```shell
$JAVA_HOME/bin/javac Demo.java
```

Run the compiled program with
```shell
$JAVA_HOME/bin/java Demo
```

We expect to see the following output:
```shell
Starting concolic execution
Symbolize!
 symbolize field value rid:1 index:7 type:I offset:24
Symbolize!
Str starts with ip
Path condition=[{"_type":"BinaryExpression","_left":{"_type":"SymbolExpression","_java_type":"I","_exp":"M_I_field_java_lang_Integer_value"},"_op":"!=","_right":{"_type":"ConstExpr","_java_type":"I","_exp":"Y_I_3"}},{"_type":"OpStrExpression","_name":"startsWith","_is_not":false,"_param_list":[{"_type":"SymbolExpression","_java_type":"java/lang/String","_exp":"M_L'String'_2"},{"_type":"ConStringSymbolExp","_java_type":"java/lang/String","_exp":"Y_L'String'_ip"}]}]Cleaning 8 dangling expressions
Checking memory leaks for Expression, 0 remains...
```
The path conditions are printted as JSON, you can format it to make it more easy to follow:
```json
[
    {
        "_type":"BinaryExpression",
        "_left":{
            "_type":"SymbolExpression",
            "_java_type":"I",
            "_exp":"M_I_field_java_lang_Integer_value"
        },
        "_op":"!=",
        "_right":{
            "_type":"ConstExpr",
            "_java_type":"I",
            "_exp":"Y_I_3"
        }
    },
    {
        "_type":"OpStrExpression",
        "_name":"startsWith",
        "_is_not":false,
        "_param_list":[
            {
                "_type":"SymbolExpression",
                "_java_type":"java/lang/String",
                "_exp":"M_L'String'_2"
            },
            {
                "_type":"ConStringSymbolExp",
                "_java_type":"java/lang/String",
                "_exp":"Y_L'String'_ip"
            }
        ]
    }
]
```
Congratulations! You have now acquired the path conditions of a very simple Java application.

## Other example
Refer to the `./concolic-test` directory to see examples on how to run Java programs with the concolic execution engine.


## Running real-world web applications [WIP]