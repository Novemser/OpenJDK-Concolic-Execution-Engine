public class Example {
    static class TestInnerClass {
        Integer inner_i;
        Double inner_d;
        TestInnerClass(Integer i) {
            inner_i = i;
            inner_d = Double.valueOf(i);
        }
    }
    static class TestClass {
        Byte b;
        Character c;
        Double d;
        Float f;
        Integer i;
        Short s;
        // Boolean bo;
        TestInnerClass tic;
        // String s;
        TestClass() {
            b = 1;
            c = new Character('2');
            d = new Double(3.0);
            f = new Float(4.0);
            i = new Integer(5);
            s = 6;
            // bo = false;
            tic = new TestInnerClass(i);
        }
    }

    public static void main(String[] args) {
        new Integer(0);
        new TestClass();
        System.startConcolic();
        int a = 1000;
        int b = 2;
        int c = a + b;
        System.symbolize(new TestClass());
        System.endConcolic();
        System.out.println(Integer.MIN_VALUE);
        System.out.println((new Integer(0)).MIN_VALUE);
    }
}