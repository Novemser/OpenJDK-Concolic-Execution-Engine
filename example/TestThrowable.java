public class TestThrowable {

    public static void main(String[] args) {
        Integer i = new Integer(20);

        System.startConcolic();
        System.symbolize(i);

        int ret = 10;
        try {
            ret = func(i);
        } catch (Throwable e) {
        }

        System.endConcolic();
    }

    public static int func(Integer i) throws Throwable{
        if (i != null) {
              throw new Throwable();
      } else {
            return i;
        }
    }



}

