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

    public long func(Long input) {
        if (input > 10) {
            return 1;
        } else {
            return 2;
        }
    }

    public static void main(String[] args) {
        Long obj1 = new Long(4);
        Example e = new Example();

        System.startConcolic();
        System.symbolize(obj1);

        long input = obj1;
        input = e.func(input);
        if (input > 0) {

        }
        

        System.endConcolic();
        System.out.println(input);
    }
}