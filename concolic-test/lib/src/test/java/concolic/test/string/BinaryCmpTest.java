package concolic.test.string;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class BinaryCmpTest {
    @BeforeEach
    public void begin() {
        System.startConcolic();
    }

    @AfterEach
    public void finish() {
        System.endConcolic();
    }

    @Test
    public void testEquals() {
        String val = new String("AllTooWell");
        System.symbolize(val);
        if (val.equals("AllTooWell")) {

        }
        String pc = System.getPathCondition();
        assertEquals("[{\"_type\":\"OpStrExpression\",\"_name\":\"equals\",\"_param_list\":[{\"_type\":\"SymbolExpression\",\"_java_type\":\"java/lang/String\",\"_exp\":\"M_L'String'_1\"},{\"_type\":\"ConStringSymbolExp\",\"_java_type\":\"java/lang/String\",\"_exp\":\"Y_L'String'_AllTooWell\"}]}]", pc);
    }

    @Test
    public void testContains() {
        String val = new String("AllTooWell");
        System.symbolize(val);
        if (val.contains("AllTooWell")) {

        }
        String pc = System.getPathCondition();
        assertEquals("[{\"_type\":\"OpStrExpression\",\"_name\":\"contains\",\"_param_list\":[{\"_type\":\"SymbolExpression\",\"_java_type\":\"java/lang/String\",\"_exp\":\"M_L'String'_1\"},{\"_type\":\"ConStringSymbolExp\",\"_java_type\":\"java/lang/String\",\"_exp\":\"Y_L'String'_AllTooWell\"}]}]", pc);
    }
}
