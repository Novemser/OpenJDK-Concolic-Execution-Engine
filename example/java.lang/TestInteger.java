/**
 * Found problems by this program:
 * /=============================================================\
 * | Description                        |     where?   | Solved? |
 * |------------------------------------|--------------|---------|
 * \====================================|==============|=========/
 * 
 */
public class TestInteger {
    static void testNonStatic() {
        Integer i1 = new Integer(16);
        Integer i2 = new Integer(32);
        
        System.startConcolic();
        System.symbolize(i1);   
    
        // test non-static method
        Integer i3 = new Integer(i1.intValue());
        if (i3 > 1) {
            i3 = new Integer(8);
        }
        byte byte_value = i1.byteValue();
        int compare_to = i1.compareTo(i2);
        double double_value = i1.doubleValue();
        boolean equals = i1.equals(i2);
        int hash_code = i1.hashCode();
        int int_value = i1.intValue();
        long long_value = i1.longValue();
        short short_value = i1.shortValue();
        // String to_string = i1.toString();


        System.endConcolic();
        
        System.out.println(i3);
        System.out.println(byte_value);
        System.out.println(compare_to);
        System.out.println(double_value);
        System.out.println(equals);
        System.out.println(hash_code);
        System.out.println(int_value);
        System.out.println(long_value);
        System.out.println(short_value);
        // System.out.println(to_string);
    }

    static void testStatic() {
        Integer i1 = new Integer(16);
        Integer i2 = new Integer(32);
        Integer i3 = new Integer(-16);

        Integer.valueOf(10); // this is a hack to avoid <cinit>

        System.startConcolic();
        System.symbolize(i1);
        System.symbolize(i3);
    
        // // test static method
        int bit_count = Integer.bitCount(i1);
        int compare = Integer.compare(i1, i2);
        int compare_unsigned = Integer.compareUnsigned(i1, i3);
        // Integer decode = Integer.decode("123");

        // TODO: test `getInteger`

        int divide_unsigned = Integer.divideUnsigned(i2, i1);
        int hash_code = Integer.hashCode(i1);
        int highest_one_bit = Integer.highestOneBit(i3);
        int lowest_one_bit = Integer.lowestOneBit(i1 - 1);
        int max = Integer.max(i1, i2);
        int min = Integer.min(i1, i2);
        int number_of_leading_zeros = Integer.numberOfLeadingZeros(i1);
        int number_of_trailing_zeros = Integer.numberOfTrailingZeros(i1);

        // TODO: test `parseInteger` and `parseUnsignedInteger`

        int reminder_unsigned = Integer.remainderUnsigned(i2, i1);
        int reverse = Integer.reverse(i1);
        int reverse_bytes = Integer.reverseBytes(i1);
        int rotate_left = Integer.rotateLeft(i1, 2);
        int rotate_right = Integer.rotateRight(i1, 1);
        int signum = Integer.signum(i1);
        int sum = Integer.sum(i1, i2);

        // TODO: test `to*String`

        Integer value_of = Integer.valueOf(10);

        // TODO: test `valueOf(String)`
        
        System.endConcolic();

        System.out.println(bit_count);
        System.out.println(compare);
        System.out.println(compare_unsigned);
        System.out.println(divide_unsigned);
        System.out.println(hash_code);
        System.out.println(highest_one_bit);
        System.out.println(lowest_one_bit);
        System.out.println(max);
        System.out.println(min);
        System.out.println(number_of_leading_zeros);
        System.out.println(number_of_trailing_zeros);
        System.out.println(reminder_unsigned);
        System.out.println(reverse);
        System.out.println(reverse_bytes);
        System.out.println(rotate_left);
        System.out.println(rotate_right);
        System.out.println(signum);
        System.out.println(sum);
        System.out.println(value_of);

    }

    public static void main(String[] args) {
        testNonStatic();
        testStatic();
    }
}