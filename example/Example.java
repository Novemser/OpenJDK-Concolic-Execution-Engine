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

    static class MyInteger {
        public int value;

        public MyInteger(int v) { value = v; }
    }

    static int test_add(MyInteger a, MyInteger b) {
        int c = a.value + b.value;
        return c;
    }

    static class MyLong {
        public long value;

        public MyLong(long v) { value = v; }
    }

    static class Myfloat {
        public float value;

        public Myfloat(float v) { value = v; }
    }

    static class MyDouble {
        public double value;

        public MyDouble(double v) { value = v; }
    }

    public static void main(String[] args) {
        MyInteger obj1 = new MyInteger(4);
     
        System.startConcolic();
        System.symbolize(obj1);

        int a = obj1.value;
        int[] array = new int[4];
        array[obj1.value - 2] += 2;
        a = array[obj1.value - 2];
        // array[2] = obj1.value;
        // array[2] += obj1.value;
        

        System.endConcolic();
        // System.out.println(array[1]);
    }

    // public static void main(String[] args) {
    //     MyInteger obj1 = new MyInteger(1);
    //     MyInteger obj2 = new MyInteger(2);
    //     MyInteger obj3 = new MyInteger(3);
    //     // MyLong obj1 = new MyLong(1000);
    //     // MyLong obj2 = new MyLong(20);
    //     // MyLong obj3 = new MyLong(30);
    //     // Myfloat obj1 = new MyLong(1);
    //     // Myfloat obj2 = new MyLong(2);
    //     // Myfloat obj3 = new MyLong(3);

    //     System.startConcolic();
    //     System.symbolize(obj1);
    //     System.symbolize(obj2);
        
    //     obj3.value = obj1.value + 2;
    //     obj3.value = -obj3.value;

    //     if (obj3.value < obj2.value) {
    //         obj3.value = obj1.value;
    //     }
    //     // obj3.value = obj1.value << obj2.value;

    //     System.endConcolic();
    //     System.out.println(obj3.value);
    // }
}