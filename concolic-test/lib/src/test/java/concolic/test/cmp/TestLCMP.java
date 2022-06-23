package concolic.test.cmp;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TestLCMP {
    // FIXME: don't use @BeforeEach since this does not work due to some bugs.
    public void setUp() {
        System.startConcolic();
    }

    @AfterEach
    public void finish() {
        System.endConcolic();
    }

    private Long prepareVal(long v) {
        setUp();
        Long val = new Long(v);
        System.symbolize(val);
        return val;
    }

    @Test
    public void testNE() {
        // ifeq
        if (prepareVal(10) != 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testNE2() {
        if (prepareVal(3) != 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\"==\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testGT() {
        // ifle
        if (prepareVal(10) > 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testGT2() {
        if (prepareVal(2) > 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\"<\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testLT() {
        // ifge
        if (prepareVal(1) < 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\"<\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testLT2() {
        if (prepareVal(4) < 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testEQ() {
        // ifne
        if (prepareVal(3) == 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\"==\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testEQ2() {
        if (prepareVal(4) == 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testLE() {
        // ifgt
        if (prepareVal(3) <= 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\"==\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testLE2() {
        if (prepareVal(4) <= 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testGE() {
        // iflt
        if (prepareVal(31) >= 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }

    @Test
    public void testGE2() {
        if (prepareVal(0) >= 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_java_lang_Long_value\"},\"_op\":\"<\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_3\"}}]", System.getPathCondition());
    }
}
