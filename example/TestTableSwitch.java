public class TestTableSwitch {
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
        result = 1 + i.value;
        break;
      case 11:
        result = 2 + i.value;
        break;
      case 12:
      case 13:
        result = 3 + i.value;
        break;
      case 15:
        result = 5 + i.value;
        break;
      default:
        result = -1 + i.value;
        break;
    }
    return result;
  }

  static void testStringTableSwitch(String s) {
    int result;
    switch (s) {
      case "a":
        result = 1;
        break;
      case "b":
        result = 2;
        break;
      case "c":
      case "d":
        result = 3;
        break;
      default:
        result = -1;
        break;
    }
  }

  public static void main(String[] args) {
    MyInteger obj5 = new MyInteger(5);
    MyInteger obj11 = new MyInteger(11);
    MyInteger obj14 = new MyInteger(14);
    String s = new String("b");
    System.startConcolic();
    System.symbolize(obj5);
    System.symbolize(obj11);
    System.symbolize(obj14);

    testTableSwitch(obj5);
    testTableSwitch(obj11);
    testTableSwitch(obj14);
    // testStringTableSwitch(s);
    int c = 5;
    System.endConcolic();
  }
}