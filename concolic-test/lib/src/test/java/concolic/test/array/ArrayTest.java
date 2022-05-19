package concolic.test.array;

import org.junit.jupiter.api.Test;

public class ArrayTest {
    static class InternalLong {
        long value;
        public InternalLong(int value) {
            this.value = value;
        }
    }

    @Test
    void testConcreteArrayStoreSymbolicValue() {
        long a[] = new long[]{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        InternalLong i = new InternalLong(42);
        System.startConcolic();
        System.symbolize(i);
        a[5] = i.value;
        if (a[5] > 0) {}
        String pathCondition = System.getPathCondition();
        if (!pathCondition.equals("[{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"BinaryExpression\",\"_left\":{\"_type\":\"SymbolExpression\",\"_java_type\":\"J\",\"_exp\":\"M_J_1_field24\"},\"_op\":\"--\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"J\",\"_expr\":\"Y_J_0\"}},\"_op\":\">\",\"_right\":{\"_type\":\"ConstExpr\",\"_java_type\":\"I\",\"_expr\":\"Y_I_0\"}}]")) {
            throw new RuntimeException("Unexpected path condition:" + pathCondition);
        }
        System.endConcolic();
    }
}
