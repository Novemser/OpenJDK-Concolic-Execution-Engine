package concolic.test.symbolic;

import org.junit.jupiter.api.Test;

import java.math.BigDecimal;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class BigDecimalTest {
    @Test
    public void testStringInit() {
        System.startConcolic();
        String val = new String("10");
        System.symbolize(val);
        BigDecimal bd = new BigDecimal(val);
        if (bd.intValue() == 42) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"\",\"_exp\":\"$BIG_DECIMAL$M_L'String'_1intCompact$BIG_DECIMAL$\"},\"_op\":\"--\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_-9223372036854775808\"}},\"_op\":\"!=\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_0\"}},{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"\",\"_exp\":\"$BIG_DECIMAL$M_L'String'_1scale$BIG_DECIMAL$\"},\"_op\":\"==\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_0\"}},{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":null,\"_op\":\"2i\",\"_right\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"\",\"_exp\":\"$BIG_DECIMAL$M_L'String'_1intCompact$BIG_DECIMAL$\"}},\"_op\":\"!=\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_42\"}}]", System.getPathCondition());
        System.endConcolic();
    }

    @Test
    public void testDoubleInit() {
        System.startConcolic();
        Double val = new Double(1.0);
        System.symbolize(val);
        BigDecimal bd = new BigDecimal(val.doubleValue());
        if (bd.doubleValue() == 3.2d) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"\",\"_exp\":\"$BIG_DECIMAL$M_D_1_field24intCompact$BIG_DECIMAL$\"},\"_op\":\"--\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_-9223372036854775808\"}},\"_op\":\"!=\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_0\"}},{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"\",\"_exp\":\"$BIG_DECIMAL$M_D_1_field24scale$BIG_DECIMAL$\"},\"_op\":\"==\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_0\"}},{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":null,\"_op\":\"2d\",\"_right\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"\",\"_exp\":\"$BIG_DECIMAL$M_D_1_field24intCompact$BIG_DECIMAL$\"}},\"_op\":\"--l\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"D\",\"_exp\":\"Y_D_3.200000e+00\"}},\"_op\":\"!=\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_0\"}}]", System.getPathCondition());
        System.endConcolic();
    }

    @Test
    public void testIntInit() {
        System.startConcolic();
        Integer val = new Integer(1);
        System.symbolize(val);
        BigDecimal bd = new BigDecimal(val.intValue());
        if (bd.intValue() == 3) {
        }
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":null,\"_op\":\"2l\",\"_right\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"I\",\"_exp\":\"M_I_1_field24\"}},\"_op\":\"--\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_-9223372036854775808\"}},\"_op\":\"!=\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_0\"}},{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":null,\"_op\":\"2i\",\"_right\":{\"_type\":\"BinaryExpression\",\"_left\":null,\"_op\":\"2l\",\"_right\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"I\",\"_exp\":\"M_I_1_field24\"}}},\"_op\":\"!=\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_3\"}}]", System.getPathCondition());
        System.endConcolic();
    }
//    @Test
//    public void testArithmetic() {
//        Integer val = new Integer(10);
//        System.startConcolic();
//        System.symbolize(val);
//        BigDecimal bd = new BigDecimal(val);
////        System.symbolize(bd);
//        BigDecimal res = bd.add(bd).multiply(bd).divide(bd);
//        if (res.intValue() == 11) {
//        }
//        System.endConcolic();
//    }
//
//    @Test
//    public void testValueOf() {
//        Integer val = new Integer(10);
//        System.startConcolic();
//        System.symbolize(val);
//        BigDecimal bd = BigDecimal.valueOf(val);
//        if (bd.longValue() == 10) {
//        }
//        System.endConcolic();
//    }
//
//    @Test
//    public void testCompareToConcrete() {
//        System.startConcolic();
//        Integer val = new Integer(10);
//        System.symbolize(val);
//        BigDecimal bd = BigDecimal.valueOf(val);
//        bd.compareTo(BigDecimal.valueOf(42));
//        System.endConcolic();
//    }
//
//    @Test
//    public void testCompareToSymbolic() {
//        System.startConcolic();
//        Integer val = new Integer(10);
//        System.symbolize(val);
//        BigDecimal bd = BigDecimal.valueOf(val);
//        bd.compareTo(bd);
//        System.endConcolic();
//    }
//
//    @Test
//    public void testSetScaleConcrete() {
//        System.startConcolic();
//        Integer val = new Integer(10);
//        System.symbolize(val);
//        BigDecimal bd = BigDecimal.valueOf(val);
//        BigDecimal newBd = bd.setScale(3);
//        System.endConcolic();
//    }

}
