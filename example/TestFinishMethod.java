public class TestFinishMethod {

    int returnIntFunc(int i) {
        if (i > 0) {
            return i + 1;
        }
        return i - 1;
    }
    int fieldValInt;

    void putfield(int i) {
        fieldValInt = i;
        if (fieldValInt > 100) {}
    }

    public static void main(String ...args) {
        TestFinishMethod tfm = new TestFinishMethod();

        System.startConcolic();
        Integer val = new Integer(1);
        System.symbolize(val);
        tfm.putfield(val);
//         if (val > 110) {
//         }
//         int res = tfm.returnIntFunc(val);
        System.endConcolic();
    }
}