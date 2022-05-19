public class CustomInvocationChild extends CustomInvocation {
    @Override
    public int findColumn(String str) {
        System.out.println("Inside findColumn child!");
        return 42;
    }
}
