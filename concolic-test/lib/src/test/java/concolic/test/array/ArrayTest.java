package concolic.test.array;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class ArrayTest {
    static class InternalLong {
        long value;

        public InternalLong(int value) {
            this.value = value;
        }
    }

    @AfterEach
    public void finish() {
        System.endConcolic();
    }

    @Test
    void testArrayStoreSymbolicValue() {
        long[] a = new long[]{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        InternalLong i = new InternalLong(42);
        System.startConcolic();
        System.symbolize(i);
        a[5] = i.value;
        if (a[5] > 0) {
        }
        String pathCondition = System.getPathCondition();
        assertEquals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_concolic_test_array_ArrayTest__InternalLong_value\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_0\"}}]", pathCondition);
    }

    @Test
    void testSymbolicArrayStoreSymbolicValue() {
        long[] a = new long[]{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        InternalLong i = new InternalLong(42);
        System.startConcolic();
        System.symbolize(i);
        System.symbolize(a);
        a[5] = i.value;
        if (a[5] > 0) {
        }
        String pathCondition = System.getPathCondition();
        assertEquals("[{\"_type\":\"array_init_expr\",\"_arr_str\":\"AJ_2\",\"_arr_exps\":[{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_10\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_20\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_30\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_40\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_50\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_60\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_70\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_80\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_90\"},{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_100\"}]},{\"_type\":\"_array_expr\",\"_arr_str\":\"AJ_2\",\"_is_load\":false,\"_index_exp\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_5\"},\"_value_exp\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_field_concolic_test_array_ArrayTest__InternalLong_value\"}},{\"_type\":\"_array_expr\",\"_arr_str\":\"AJ_2\",\"_is_load\":true,\"_index_exp\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_exp\":\"Y_I_5\"},\"_value_exp\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"EJ_2-1-1\"}},{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"EJ_2-1-1\"},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_exp\":\"Y_J_0\"}}]", pathCondition);
    }
}
