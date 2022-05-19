/**
 * CustomInvocation
 */
public class CustomInvocation {

    public int CustomInvocationStub(String str) {
        System.out.println("Inside customStub");
        return 2;
    }

    public int findColumn(String str) {
        System.out.println("Inside findColumn!");
        return 1;
    }

    public static void main(String[] args) {
        System.startConcolic();
        CustomInvocation ci = new CustomInvocationChild();
        ci.CustomInvocationStub("str");
        System.endConcolic();
    }
}