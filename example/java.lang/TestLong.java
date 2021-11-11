/**
 * Found problems by this program:
 * /==============================================================\
 * | Description                         |    where?    | Solved? |
 * |-------------------------------------|--------------|---------|
 * | EXP_NAME_LENGTH still to small      |              |    √    |
 * |-------------------------------------|--------------|---------|
 * | method call frame maybe entry_frame |   valueOf    |    x    |
 * \=====================================|==============|=========/
 * 
 */
public class TestLong {
    static void testNonStatic() {
        Long l1 = new Long(16);
        Long l2 = new Long(32);
        
        System.startConcolic();
        System.symbolize(l1);
    
        // test non-static method
        Long l3 = new Long(l1.longValue());
        if (l3 > 1) {
            l3 = new Long(8);
        }
        byte byte_value = l1.byteValue();
        int compare_to = l1.compareTo(l2);
        double double_value = l1.doubleValue();
        boolean equals = l1.equals(l2);
        int hash_code = l1.hashCode();
        int int_value = l1.intValue();
        long long_value = l1.longValue();
        short short_value = l1.shortValue();
        // String to_string = l1.toString();


        System.endConcolic();
        
        System.out.println(l3);
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
        Long l1 = new Long(16);
        Long l2 = new Long(32);
        Long l3 = new Long(-16);

        Long.valueOf(10); // this is a hack to avoid <cinit>

        System.startConcolic();
        System.symbolize(l1);
        System.symbolize(l3);
    
        // // test static method
        int bit_count = Long.bitCount(l1);
        int compare = Long.compare(l1, l2);
        int compare_unsigned = Long.compareUnsigned(l1, l3);
        // Long decode = Long.decode("123");

        // TODO: test `getLong`

        long divide_unsigned = Long.divideUnsigned(l2, l1);
        int hash_code = Long.hashCode(l1);
        long highest_one_bit = Long.highestOneBit(l3);
        long lowest_one_bit = Long.lowestOneBit(l1 - 1);
        long max = Long.max(l1, l2);
        long min = Long.min(l1, l2);
        int number_of_leading_zeros = Long.numberOfLeadingZeros(l1);
        int number_of_trailing_zeros = Long.numberOfTrailingZeros(l1);

        // TODO: test `parseLong` and `parseUnsignedLong`

        long reminder_unsigned = Long.remainderUnsigned(l2, l1);
        long reverse = Long.reverse(l1);
        long reverse_bytes = Long.reverseBytes(l1);
        long rotate_left = Long.rotateLeft(l1, 2);
        long rotate_right = Long.rotateRight(l1, 1);
        int signum = Long.signum(l1);
        long sum = Long.sum(l1, l2);

        // TODO: test `to*String`

        Long value_of = Long.valueOf(10);

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