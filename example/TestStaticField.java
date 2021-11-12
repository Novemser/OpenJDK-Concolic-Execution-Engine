public class TestStaticField {

    static class MyObject {
        public static int STATIC_VALUE = 0x80000000;
        // public static int STATIC_VALUE2 = 0x80000002;

        int value;

        MyObject(int value) {
            this.value = value;
        }
    }

    static class MySubClass extends MyObject {
        public static int SUB_STATIC_VALUE = 0x0001;
        // public static int SUB_STATIC_VALUE2 = 0x0002;
        // public static int SUB_STATIC_VALUE3 = 0x0003;

        int subValue;

        MySubClass(int subValue) {
            super(subValue + 1);
            this.subValue = subValue;
        }
    }

    static void testMyObjectStatic() {
        MyObject i1 = new MyObject(5);
        MyObject i2 = new MyObject(7);

        System.startConcolic();
        System.symbolize(i1);
        System.symbolize(i2);

        int a = i1.STATIC_VALUE + 6;
        // test putstatic through instance
        i2.STATIC_VALUE = i1.value + 3;
        // test getstatic through instance
        int b = i2.STATIC_VALUE + 9;
        // test putstatic through class
        MyObject.STATIC_VALUE = i2.value + 1;
        // test getstatic through class
        int c = MyObject.STATIC_VALUE + 11;

        System.endConcolic();
    }

    static void testMySubClassStatic() {
        MySubClass s1 = new MySubClass(5);
        MySubClass s2 = new MySubClass(7);

        System.startConcolic();
        System.symbolize(s1);
        System.symbolize(s2);
        
        // int a = s1.value;
        // int b = s1.subValue;
        // // s1.SUB_STATIC_VALUE = s1.value;
        
        int a = s1.STATIC_VALUE + 1;
        int b = s1.SUB_STATIC_VALUE + 2;
        int c = s1.subValue + 3;
        int d = s1.value + 4;
        s1.STATIC_VALUE = c;
        s1.SUB_STATIC_VALUE = d;
        a = s2.STATIC_VALUE + 5;
        b = s2.SUB_STATIC_VALUE + 6;
        int e = MySubClass.STATIC_VALUE + 7;

        System.endConcolic();
    }

    public static void main(String[] args) {
        // testMyObjectStatic();
        testMySubClassStatic();
    }
}