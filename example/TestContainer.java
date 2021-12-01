import java.util.HashMap;
import java.util.Map;

public class TestContainer {
  static class MyInteger {
    long value;
    public MyInteger(int value) { this.value = value; }
  }
  public static void main(String[] args) {
    testMap();
  }

  public static void testMap() {
    System.out.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");

    Map map = new HashMap();
    Integer i = new Integer(2);
    Long l = new Long(5l);
    long ret;

    System.startConcolic();
    System.symbolize(i);

    map.put(i, l);
    ret = (long)map.get(i);

    if (ret > 100) {
    }

    System.endConcolic();
    System.out.println(ret);
  }
}