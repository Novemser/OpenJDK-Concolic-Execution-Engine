public class Example {

  public int func(Integer input) {
    if (input > 10) {
      return input - 10;
    } else {
      return input + 10;
    }
  }

  public static void main(String[] args) { testInt(); }

  public static void testInt() {
    Example e = new Example();
    Integer a = new Integer(5);
    Integer b = new Integer(3);
    Integer[] arr = new Integer[a];
    for (int i = 0; i < a; ++i) {
      arr[i] = new Integer(i);
    }

    System.startConcolic();
    System.symbolizeMethod("Example", "func");
    System.symbolize(a);
    System.symbolize(b);

    // arr[b] = a;

    // int input = e.func(arr[1]);
    int input = 2 + e.func(a);

    if (input > 16) {
    }

    System.endConcolic();
  }

  public static void testStartsWith() {
    String d = "123";
    if (d.startsWith("1")) {
    }
  }

  public static void testString() {
    String a = new String("abcd");
    String b = new String("ab");
    // Example e = new Example();
    // Integer a = new Integer(5);

    System.startConcolic();

    String c = new String("123");
    // MyString s = new MyString();

    System.symbolize(c);

    // int input = e.func(a);

    testStartsWith();

    // if (s.value.startsWith("1")) {
    // }

    // if (a.startsWith(b)) {
    // }

    System.endConcolic();
  }

  static class MyString { public String value = "123"; }

  static class TestInnerClass {
    Integer inner_i;
    Double inner_d;
    TestInnerClass(Integer i) {
      inner_i = i;
      inner_d = Double.valueOf(i);
    }
  }
  static class TestClass {
    public Byte b;
    public Character c;
    public Double d;
    public Float f;
    public Integer i;
    public Short s;
    public TestInnerClass tic;
    // Boolean bo;
    // String s;
    TestClass() {
      b = 1;
      c = new Character('2');
      d = new Double(3.0);
      f = new Float(4.0);
      i = new Integer(5);
      s = 6;
      // bo = false;
      tic = new TestInnerClass(i);
    }
  }
}