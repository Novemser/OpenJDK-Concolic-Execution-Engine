public class TestObjArray {
  static class MyInteger {
    long value;
    public MyInteger(int value) {
      this.value = value;
    }
  }
  public static void main(String[] args) {
    MyInteger a[] = new MyInteger[]{
      new MyInteger(10),
      new MyInteger(20),
      new MyInteger(30),
      new MyInteger(40),
      new MyInteger(50),
      new MyInteger(60),
      new MyInteger(70),
      new MyInteger(80),
      new MyInteger(90),
      new MyInteger(100)
    };
    MyInteger i = new MyInteger(63);
    System.startConcolic();
//     System.symbolize(a);
    System.symbolize(i);

    a[5].value = i.value;
    long b = a[3].value + a[5].value;
    int c = a.length + 9;
    if (a[5].value > 0) {}

    System.endConcolic();
    System.out.println(b);
    System.out.println(c);
  }
}