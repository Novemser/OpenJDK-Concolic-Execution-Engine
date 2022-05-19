public class TestValueOfCache {
public static void main(String[] args) {
    Long id = new Long(10L);
    System.startConcolic();
    System.symbolize(id);

//     Long val = Long.valueOf(id);
    if (Long.valueOf(id) > 100) {
    }
    System.endConcolic();
//     System.out.println(val);
}
}