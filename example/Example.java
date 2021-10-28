public class Example {
    public static void main(String[] args) {
        System.startConcolic();
        int a = 1;
        int b = 2;
        int c = a + b;
        System.out.println(c);
        System.symbolize(new Integer(6));
        System.endConcolic();
    }
}