package concolic.test.string;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertTrue;

public class InternTest {
    @Test
    public void testInternPc() {
        System.startConcolic();
        String val = new String("ME");
        System.symbolize(val);
        String internStr = val.intern();
        if (internStr.contains("ME")) {}
        String pc = System.getPathCondition();
        assertTrue(pc.contains("intern"));
        assertTrue(pc.contains("ME"));
        System.endConcolic();
    }

    @Test
    public void testInternEquals() {
        System.startConcolic();
        String val1 = new String("ME");
        String val2 = new String("ME");
        System.symbolize(val1);
        System.symbolize(val2);

        if (val1.intern() == val2.intern()) {
            System.endConcolic();
            return;
        }

        System.endConcolic();
        assertTrue(false);
    }

    @Test
    public void testInternNEquals() {
        System.startConcolic();
        String val1 = new String("ME");
        String val2 = new String("EM");
        System.symbolize(val1);
        System.symbolize(val2);

        if (val1.intern() == val2.intern()) {
            System.endConcolic();
            assertTrue(false);
            return;
        }

        System.endConcolic();
    }
}
