/**
 * Found problems by this program:
 * /=============================================================\
 * | Description                        |     where?   | Solved? |
 * |------------------------------------|--------------|---------|
 * | return value of native?            |   equals     |    x    |
 * |------------------------------------|--------------|---------|
 * | return value location              |   equals     |    √    |
 * |------------------------------------|--------------|---------|
 * | free error (sym str too long)      |   isFinite   |    √    |
 * \====================================|==============|=========/
 * 
 */
public class TestFloat {
    static void testNonStatic() {
        Float f1 = new Float(1.2);
        Float f2 = new Float(4.3);
        
        System.startConcolic();
        System.symbolize(f1);
    
        // test non-static method
        Float f3 = new Float(f1.floatValue());
        if (f3 > 1.0) {
            f3 = new Float(3.3);
        }
        byte byte_value = f1.byteValue();
        int compare_to = f1.compareTo(f2);
        double double_value = f1.doubleValue();
        boolean equals = f1.equals(f2);
        float float_value = f1.floatValue();
        int hash_code = f1.hashCode();
        int int_value = f1.intValue();
        boolean is_infinite = f1.isInfinite();
        boolean is_nan = f1.isNaN();
        long long_value = f1.longValue();
        short short_value = f1.shortValue();
        // String to_string = f1.toString();


        System.endConcolic();
        
        System.out.println(f3);
        System.out.println(byte_value);
        System.out.println(compare_to);
        System.out.println(double_value);
        System.out.println(equals);
        System.out.println(float_value);
        System.out.println(hash_code);
        System.out.println(int_value);
        System.out.println(is_infinite);
        System.out.println(is_nan);
        System.out.println(long_value);
        System.out.println(short_value);
        // System.out.println(to_string);
    }

    static void testStatic() {
        Float f1 = new Float(1.2);
        Float f2 = new Float(4.3);
        
        System.startConcolic();
        System.symbolize(f1);
    
        // test static method
        int compare = Float.compare(f1, f2);
        int float_to_int_bits = Float.floatToIntBits(f1);
        int float_to_raw_int_bits = Float.floatToRawIntBits(f1);
        int hash_code = Float.hashCode(f1);
        float int_bits_to_float = Float.intBitsToFloat(0x99999);
        boolean is_finite = Float.isFinite(f1);
        boolean is_infinite = Float.isInfinite(f1);
        boolean is_nan = Float.isNaN(f1);
        float sum = Float.sum(f1, f2);
        float max = Float.max(f1, f2);
        float min = Float.min(f1, f2);
        //TODO: test String-related

        System.endConcolic();
    }

    public static void main(String[] args) {
        testNonStatic();
        testStatic();
    }
}