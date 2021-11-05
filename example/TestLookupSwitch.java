public class TestLookupSwitch {
  static class MyInteger {
    int value;
    MyInteger(int value) {
      this.value = value;
    }
  }

  static int testTableSwitch(MyInteger i) {
    int result;
    switch (i.value) {
      case 10:
        result = 10 + i.value;
        break;
      case 100:
        result = 100 + i.value;
        break;
      case 1000:
        result = 1000 + i.value;
        break;
      default:
        result = -1 + i.value;
        break;
    }
    return result;
  }

  public static void main(String[] args) {
    MyInteger obj0 = new MyInteger(0);
    MyInteger obj100 = new MyInteger(100);
    MyInteger obj500 = new MyInteger(500);
    String s = new String("b");
    System.startConcolic();
    System.symbolize(obj0);
    System.symbolize(obj100);
    System.symbolize(obj500);

    testTableSwitch(obj0);
    testTableSwitch(obj100);
    testTableSwitch(obj500);

    System.endConcolic();
  }
}