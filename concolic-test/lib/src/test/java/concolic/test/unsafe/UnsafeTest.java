package concolic.test.unsafe;

import org.junit.jupiter.api.Test;
import sun.misc.Unsafe;

import java.lang.reflect.Field;

public class UnsafeTest {
    @Test
    public void testUnsafeShouldOk() throws Exception {
        System.startConcolic();
        Field f = Unsafe.class.getDeclaredField("theUnsafe");
        f.setAccessible(true);
        Unsafe unsafe = (Unsafe) f.get(null);
        unsafe.addressSize();
        System.endConcolic();
    }
}
