public class TestLoadStore {
  static class MyInteger {
    long value;
    public MyInteger(long value) {
      this.value = value;
    }
  }
  public static void main(String[] args) {
    long place_holder1, place_holder2, place_holder3;
    MyInteger obj1 = new MyInteger(3);
    MyInteger obj2 = new MyInteger(5);
    System.startConcolic();
    System.symbolize(obj1);
    System.symbolize(obj2);
    long a = obj1.value;
    long b = obj1.value + a;
    System.endConcolic();   
  }
}