package concolic.test.string;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.nio.charset.StandardCharsets;

public class CharsetTest {
    @AfterEach
    public void end() {
        System.endConcolic();
    }

    @Test
    public void testUTF8() {
        System.startConcolic();
        String val = new String("SJTU");
        System.symbolize(val);
        int length = val.getBytes(StandardCharsets.UTF_8).length;
        System.out.println();
    }
}
