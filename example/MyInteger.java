public class MyInteger {

    public int value;

    public MyInteger(int v) { value = v; }

    public static int ver4() {
        MyInteger obj1 = new MyInteger(1000000000);
        MyInteger obj2 = new MyInteger(20);
        MyInteger obj3 = new MyInteger(30);

        System.startConcolic();
        System.symbolize(obj1);
        System.symbolize(obj2);
        
        obj3.value = obj1.value + obj2.value;

        System.endConcolic();
        return obj3.value;
    }

    public static void main(String[] args) {
        ver1();
    }

    public static int ver1() {
        int a = 10;
        int b = 20;
        int c = 30;

        c = (a + b) * c;

        if (c < 1000) {
            c += 100;
        }

        return c;
    }

    public static int ver2() {
        int a = 10;
        int b = 20;
        int c = 30;

        Integer c_wrapper = new Integer(c);
        System.startConcolic();
        System.symbolize(c_wrapper);
        c = c_wrapper;
        
        c = (a + b) * c;

        if (c < 1000) {
            c += 100;
        }

        System.endConcolic();
        return c;
    }

    public static int ver3() {
        Integer a = new Integer(10);
        Integer b = new Integer(20);
        Integer c = new Integer(30);

        System.startConcolic();
        System.symbolize(c);
        
        c = (a + b) * c;

        if (c < 1000) {
            c += 100;
        }

        System.endConcolic();
        return c;
    }

}

