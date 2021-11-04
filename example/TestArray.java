public class TestArray {
  static class MyInteger {
    long value;
    public MyInteger(int value) {
      this.value = value;
    }
  }
  public static void main(String[] args) {
    long a[] = new long[]{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    MyInteger i = new MyInteger(63);
    System.startConcolic();
    System.symbolize(a);
    System.symbolize(i);

    a[5] = i.value;
    long b = a[3] + a[5];
    long d = b + 7;
    int c = a.length + 9;

    System.endConcolic();
    System.out.println(b);
    System.out.println(c);
  }
}