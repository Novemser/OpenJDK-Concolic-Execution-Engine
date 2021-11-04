public class TestMultiArray {
  static class MyInteger {
    int value;
    public MyInteger(int value) {
      this.value = value;
    }
  }
  
  public static void main(String[] args) {
    int[][] a = new int[3][];
    a[0] = new int[]{10, 20};
    a[1] = new int[]{30};
    a[2] = new int[]{40, 50, 60};
    MyInteger i = new MyInteger(63);

    System.startConcolic();
    System.symbolize(a);
    System.symbolize(i);

    a[1][0] = i.value + a[2][2];
    int l = a.length + 11;
    int l1 = a[1].length + 9;
    System.endConcolic();
  }
}