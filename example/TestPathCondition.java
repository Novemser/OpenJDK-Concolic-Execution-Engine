public class TestPathCondition {
    static class MyInteger {
        int value;
        public MyInteger(int value) {
            this.value = value;
        }
    }

    static class MyFloat {
        float value;
        public MyFloat(float value) {
            this.value = value;
        }
    }
    
    public static void main(String[] args) {
        int[] arr = new int[]{1,2,3,4,5};
        MyInteger i1 = new MyInteger(4);
        MyInteger i2 = new MyInteger(2);
        MyInteger i3 = new MyInteger(5);
        MyFloat f1 = new MyFloat(1.7f);
        MyFloat f2 = new MyFloat(1.7f);
        
        System.startConcolic();
        System.symbolize(arr);
        System.symbolize(i1);
        System.symbolize(i2);
        System.symbolize(i3);
        System.symbolize(f1);
        System.symbolize(f2);

        arr[i1.value] = i2.value;
        arr[i2.value] = i3.value;

        int e1 = arr[i2.value];
        int e2 = arr[i2.value];
        int e3 = arr[4];

        if ((e1 + 3) / 4 > 5) {}
        if (-e2 * e3 <= 0) {}
        // if ((i1.value & i2.value) > 0) {}
        if (-arr.length > 0 && arr[2] < 9);
        if (f1.value < f2.value) {}
        if (e3 > 6) {};

        System.endConcolic();
    }
}