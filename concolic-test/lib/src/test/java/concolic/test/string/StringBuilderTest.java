package concolic.test.string;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class StringBuilderTest {
    @BeforeEach
    public void begin() {
        System.startConcolic();
    }

    @AfterEach
    public void end() {
        System.endConcolic();
    }

    private String getNull() {
        // prevent inline
        String.class.getClass();
        return null;
    }

    @Test
    public void testAppendNull() {
        StringBuilder stringBuilder = new StringBuilder();
        String val = new String("SJTU");
        System.symbolize(val);
        String res = stringBuilder.append(val).append(getNull()).toString();
        if (res.contains("UTJS")) {}
        assertEquals("[{\"_type\":\"OpStrExpression\",\"_name\":\"contains\",\"_is_not\":true,\"_param_list\":[{\"_type\":\"OpStrExpression\",\"_name\":\"concat\",\"_is_not\":false,\"_param_list\":[{\"_type\":\"OpStrExpression\",\"_name\":\"concat\",\"_is_not\":false,\"_param_list\":[{\"_type\":\"ConStringSymbolExp\",\"_java_type\":\"java/lang/String\",\"_exp\":\"Y_L'String'_\"},{\"_type\":\"SymbolExpression\",\"_java_type\":\"java/lang/String\",\"_exp\":\"M_L'String'_1\"}]},{\"_type\":\"ConStringSymbolExp\",\"_java_type\":\"java/lang/String\",\"_exp\":\"Y_L'String'_null\"}]},{\"_type\":\"ConStringSymbolExp\",\"_java_type\":\"java/lang/String\",\"_exp\":\"Y_L'String'_UTJS\"}]}]", System.getPathCondition());
    }
}
