package concolic.test.symbolic;

import org.junit.jupiter.api.Test;

import java.math.BigDecimal;

public class BigDecimalTest {
    @Test
    public void testStringInit() {
        System.startConcolic();
        String val = new String("10");
        System.symbolize(val);
        BigDecimal bd = new BigDecimal(val);
        if (bd.intValue() == 42) {}
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
