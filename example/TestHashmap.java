import java.util.Map;
import java.util.HashMap;

public class TestHashmap {
    public static void main(String...args) {
        Map<Integer, String> map = new HashMap<>();
        Integer k = 1;
        System.startConcolic();
        System.symbolize(k);
        String v = "value";
        Integer k2 = 2;
        map.put(k, v);
        map.put(k2, v);
//         if (!k.equals(k2)) {
//
//         }
        System.endConcolic();
    }
}