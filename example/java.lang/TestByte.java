/**
 * Found problems by this program:
 * /=============================================================\
 * | Description                        |     where?   | Solved? |
 * |------------------------------------|--------------|---------|
 * \====================================|==============|=========/
 * 
 */
public class TestByte {
    static void testNonStatic() {
        Byte b1 = new Byte((byte)16);
        Byte b2 = new Byte((byte)32);
        
        System.startConcolic();
        System.symbolize(b1);   
    
        // test non-static method
        Byte b3 = new Byte(b1.byteValue());
        if (b3 > 1) {
            b3 = new Byte((byte)8);
        }
        byte byte_value = b1.byteValue();
        int compare_to = b1.compareTo(b2);
        double double_value = b1.doubleValue();
        boolean equals = b1.equals(b2);
        int hash_code = b1.hashCode();
        int int_value = b1.intValue();
        long long_value = b1.longValue();
        short short_value = b1.shortValue();
        // String to_string = b1.toString();


        System.endConcolic();
        
        System.out.println(b3);
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
        Byte b1 = new Byte((byte)16);
        Byte b2 = new Byte((byte)32);

        Byte.valueOf(b1); // this is a hack to avoid <cinit>

        System.startConcolic();
        System.symbolize(b1);
    
        // // test static method
        int compare = Byte.compare(b1, b2);
        // Byte decode = Byte.decode("123");
        int hash_code = Byte.hashCode(b1);
        int to_unsigned_int = Byte.toUnsignedInt(b1);
        long to_unsigned_long = Byte.toUnsignedLong(b1);

        // TODO: test `parseInteger` and `parseUnsignedInteger`
        // TODO: test `to*String`

        Byte value_of = Byte.valueOf(b1);

        // TODO: test `valueOf(String)`
        
        System.endConcolic();

        System.out.println(compare);
        System.out.println(hash_code);
        System.out.println(to_unsigned_int);
        System.out.println(to_unsigned_long);

    }

    public static void main(String[] args) {
        testNonStatic();
        testStatic();
    }
}