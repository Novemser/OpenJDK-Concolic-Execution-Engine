package concolic.test.webridge;

import org.junit.jupiter.api.Test;

public class JavaEntryPointTest {
    @Test
    public void testWeBridgeEntryPoint() {
        System.weBridgeAnalysis(this.getClass().getClassLoader());
    }
}
