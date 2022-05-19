public class TestPutstatic {
    public static String value = "v";
    public static void main(String ... args) {
        String str = "s1";
        System.startConcolic();
        System.symbolize(str);
        value = str;

        System.out.println(value);
        System.endConcolic();
    }
}