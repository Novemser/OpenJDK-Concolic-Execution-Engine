import java.util.*;

public class MapRemoveTest {
    public static void main(String[] args) {
        System.startConcolic();
        Set<Integer> set = new HashSet<>();
        set.add(1);
        Iterator<Integer> iterator = set.iterator();
        while (iterator.hasNext()) {
            iterator.next();
            iterator.remove();
        }
        System.endConcolic();
    }
}
