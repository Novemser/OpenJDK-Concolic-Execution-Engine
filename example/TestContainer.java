import java.util.HashMap;
import java.util.Map;

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
    Integer i = new Integer(2);
    MyInteger l = new MyInteger(5);

    System.startConcolic();
    System.symbolize(i);
    System.symbolize(l);

    map.put(i, l);
    MyInteger ret = (MyInteger)map.get(i);

    if (ret.value > 100) {
    }

    System.endConcolic();
    System.out.println(ret);
  }
}