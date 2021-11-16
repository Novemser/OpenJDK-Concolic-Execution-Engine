public class Example {

  public int func(Integer input) {
    if (input > 10) {
      return input - 10;
    } else {
      return input + 10;
    }
  }

  public static void main(String[] args) {
    String a = new String("abcd");
    String b = new String("ab");
    // Example e = new Example();
    // Integer a = new Integer(5);

    System.startConcolic();
    // System.symbolizeMethod("Example", "func");
    System.symbolize(a);
    
    // int input = e.func(a);

    // if (input > 16) {
    // }

    if (a.startsWith(b)) {
    }

    System.endConcolic();
  }

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