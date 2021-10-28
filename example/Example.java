public class Example {
    public static void main(String[] args) {
        Integer a = new Integer(1);
        Integer b = new Integer(2);

        System.startConcolic();
        System.symbolize(a);

        Integer c = a + b;
        System.endConcolic();

        System.out.println(c);
    }
}