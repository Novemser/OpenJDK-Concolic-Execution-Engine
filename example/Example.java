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
        public Byte b;
        public Character c;
        public Double d;
        public Float f;
        public Integer i;
        public Short s;
        public TestInnerClass tic;
        // Boolean bo;
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
        TestClass test = new TestClass();
        System.startConcolic();
        System.symbolize(test);
        int a = 1000;
        int b = test.i;
        int c = a + b;
        System.endConcolic();
        // System.out.println(Integer.MIN_VALUE);
        // System.out.println((new Integer(0)).MIN_VALUE);
    }
}