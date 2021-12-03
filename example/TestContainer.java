import java.util.HashMap;
import java.util.Map;
import java.util.HashSet;
import java.util.Set;

public class TestContainer {
  static class MyInteger {
    int value;
    public MyInteger(int value) { this.value = value; }
  }
  public static void main(String[] args) {
    testMap();
  }

  public static void testMap() {
    System.out.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");

    Map map = new HashMap();
    Set set = new HashSet();
    Integer i = new Integer(2);
    MyInteger l = new MyInteger(5);

    System.startConcolic();
    System.symbolize(i);
    System.symbolize(l);

    set.add(i);
//     MyInteger ret = (MyInteger)map.get(i);

    System.endConcolic();
    System.out.println(i);
  }
}