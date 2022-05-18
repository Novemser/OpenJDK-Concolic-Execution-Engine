package concolic.test.array;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertTrue;

public class ArrayTest {
    static class InternalLong {
        long value;
        public InternalLong(int value) {
            this.value = value;
        }
    }

    @Test
    void testConcreteArrayStoreSymbolicValue() {
        long a[] = new long[]{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        InternalLong i = new InternalLong(42);
        System.out.println("S-1");
        System.startConcolic();
//        System.symbolize(i);
        System.out.println("S0");
//        a[5] = i.value;
//        if (a[5] > 0) {}
        System.out.println("S1");
//        String pathCondition = System.getPathCondition();
        System.out.println("S2");
//        System.out.println(pathCondition);
//        assertTrue(pathCondition.contains("PathCondition"));
        System.endConcolic();
    }
}
