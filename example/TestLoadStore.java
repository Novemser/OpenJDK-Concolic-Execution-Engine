public class TestLoadStore {
  static class MyInteger {
    long value;
    public MyInteger(long value) {
      this.value = value;
    }
  }
  public static void main(String[] args) {
    long place_holder1=0xdeaddead, place_holder2=0xbabebabe, place_holder3=0xabcdef0;
    MyInteger obj1 = new MyInteger(0x2222333322223333L);
    MyInteger obj2 = new MyInteger(0x4444555544445555L);
    System.startConcolic();
    System.symbolize(obj1);
    System.symbolize(obj2);
    // long a = 0x11111111;
    // long b = 0x22222222;
    // long c = a + b;
    long a = obj1.value;
    long b = a + obj2.value + place_holder2;
    System.endConcolic();
  }
}