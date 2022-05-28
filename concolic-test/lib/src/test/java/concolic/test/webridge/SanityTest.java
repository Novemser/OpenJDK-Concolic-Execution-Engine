package concolic.test.webridge;

import org.junit.jupiter.api.Test;

public class SanityTest {
    @Test
    public void testEndConcolicNotProper() {
        System.endConcolic();
    }
}
