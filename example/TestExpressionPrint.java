public class TestExpressionPrint {
  static class MyInteger {
    int value;
    public MyInteger(int value) {
      this.value = value;
    }
  }

  public static void main(String[] args) {
    MyInteger obj1 = new MyInteger(1);
    MyInteger obj2 = new MyInteger(2);
    MyInteger obj3 = new MyInteger(3);
    MyInteger obj4 = new MyInteger(4);
    System.startConcolic();
    System.symbolize(obj1);
    System.symbolize(obj2);
    System.symbolize(obj3);
    System.symbolize(obj4);

    int a = obj1.value + obj2.value;
    int b = obj3.value - 3;
    int c = a * b;
    int d = -c;

    if (d > 0 && b <= 7) {
      int e = b + d;
    } else {
      int e = b - d;
    }

    System.endConcolic();
  }
}