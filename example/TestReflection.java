
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

class ReflectionDemo {
  private String str;

  public ReflectionDemo() { str = "Techvidvan Java Tutorial"; }

  public Integer method(Integer num1) {
    if (num1 > 0) {
      return num1 + 1000;
    } else {
      return num1 + 2000;
    }
  }
}

public class TestReflection {

  public static void main(String[] args) throws NoSuchMethodException,
                                                InvocationTargetException,
                                                IllegalAccessException {
    reflectionTest();
  }

  public static void reflectionTest() throws NoSuchMethodException,
                                             InvocationTargetException,
                                             IllegalAccessException {
    System.out.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");

    ReflectionDemo obj = new ReflectionDemo();
    Class cls = obj.getClass();
    System.out.println("\nThe name of the class is: " + cls.getName());

    Constructor constructor = cls.getConstructor();
    System.out.println("\nThe name of the constructor is: " +
                       constructor.getName());
    System.out.println("\nThe public methods of the class are: ");
    Method methodCall = cls.getDeclaredMethod("method", Integer.class);

    Integer i = new Integer(2);
    Integer ret;

    System.startConcolic();
    System.symbolize(i);

    ret = (Integer)methodCall.invoke(obj, i);
    //        ret = obj.method(i);

    if (ret > 100) {
    }

    System.endConcolic();
    System.out.println(ret);
  }
}