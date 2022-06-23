package concolic.test.cmp;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class TestDCMP {
    // FIXME: don't use @BeforeEach since this does not work due to some bugs.
    public void setUp() {
        System.startConcolic();
    }

    @AfterEach
    public void finish() {
        System.endConcolic();
    }

    private Double prepareVal(double v) {
        setUp();
        Double val = new Double(v);
        System.symbolize(val);
        return val;
    }

    @Test
    public void testNE() {
        if (prepareVal(10) != 3.0) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"D\",\"_exp\":\"M_D_field_java_lang_Double_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"D\",\"_exp\":\"Y_D_3.000000e+00\"}}]", System.getPathCondition());
    }

    @Test
    public void testGT() {
        if (prepareVal(10) > 3.0) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"D\",\"_exp\":\"M_D_field_java_lang_Double_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"D\",\"_exp\":\"Y_D_3.000000e+00\"}}]", System.getPathCondition());
    }

    @Test
    public void testEQ() {
        if (prepareVal(3) != 3.0) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"D\",\"_exp\":\"M_D_field_java_lang_Double_value\"},\"_op\":\"==\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"D\",\"_exp\":\"Y_D_3.000000e+00\"}}]", System.getPathCondition());
    }

    @Test
    public void testLE() {
        if (prepareVal(3) != 3.0) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"D\",\"_exp\":\"M_D_field_java_lang_Double_value\"},\"_op\":\"==\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"D\",\"_exp\":\"Y_D_3.000000e+00\"}}]", System.getPathCondition());
    }

    @Test
    public void testGE() {
        if (prepareVal(23) >= 3.0) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"D\",\"_exp\":\"M_D_field_java_lang_Double_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"D\",\"_exp\":\"Y_D_3.000000e+00\"}}]", System.getPathCondition());
    }

    @Test
    public void testLT() {
        if (prepareVal(1) != 3.0) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"D\",\"_exp\":\"M_D_field_java_lang_Double_value\"},\"_op\":\"<\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"D\",\"_exp\":\"Y_D_3.000000e+00\"}}]", System.getPathCondition());
    }
}
