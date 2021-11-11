    /**
 * Found problems by this program:
 * /=============================================================\
 * | Description                        |     where?   | Solved? |
 * |------------------------------------|--------------|---------|
 * \====================================|==============|=========/
 * 
 */
public class TestDouble {
    static void testNonStatic() {
        Double d1 = new Double(1.2);
        Double d2 = new Double(4.3);
        
        System.startConcolic();
        System.symbolize(d1);
    
        // test non-static method
        Double d3 = new Double(d1.doubleValue());
        if (d3 > 1.0) {
            d3 = new Double(3.3);
        }
        byte byte_value = d1.byteValue();
        int compare_to = d1.compareTo(d2);
        double double_value = d1.doubleValue();
        boolean equals = d1.equals(d2);
        float float_value = d1.floatValue();
        int hash_code = d1.hashCode();
        int int_value = d1.intValue();
        boolean is_infinite = d1.isInfinite();
        boolean is_nan = d1.isNaN();
        long long_value = d1.longValue();
        short short_value = d1.shortValue();
        // String to_string = d1.toString();


        System.endConcolic();
        
        System.out.println(d3);
        System.out.println(byte_value);
        System.out.println(compare_to);
        System.out.println(double_value);
        System.out.println(equals);
        System.out.println(double_value);
        System.out.println(hash_code);
        System.out.println(int_value);
        System.out.println(is_infinite);
        System.out.println(is_nan);
        System.out.println(long_value);
        System.out.println(short_value);
        // System.out.println(to_string);
    }

    static void testStatic() {
        Double d1 = new Double(1.2);
        Double d2 = new Double(4.3);
        
        System.startConcolic();
        System.symbolize(d1);
    
        // test static method
        int compare = Double.compare(d1, d2);
        long double_to_long_bits = Double.doubleToLongBits(d1);
        long double_to_raw_long_bits = Double.doubleToRawLongBits(d1);
        int hash_code = Double.hashCode(d1);
        double long_bits_to_double = Double.longBitsToDouble(0x99999);
        boolean is_finite = Double.isFinite(d1);
        boolean is_infinite = Double.isInfinite(d1);
        boolean is_nan = Double.isNaN(d1);
        double sum = Double.sum(d1, d2);
        double max = Double.max(d1, d2);
        double min = Double.min(d1, d2);
        //TODO: test String-related

        System.endConcolic();

        System.out.println(compare);
        System.out.println(double_to_long_bits);
        System.out.println(double_to_raw_long_bits);
        System.out.println(hash_code);
        System.out.println(long_bits_to_double);
        System.out.println(is_finite);
        System.out.println(is_infinite);
        System.out.println(is_nan);
        System.out.println(sum);
        System.out.println(max);
        System.out.println(min);
    }

    public static void main(String[] args) {
        testNonStatic();
        testStatic();
    }
}