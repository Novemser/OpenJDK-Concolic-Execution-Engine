/**
 * Found problems by this program:
 * /=============================================================\
 * | Description                        |     where?   | Solved? |
 * |------------------------------------|--------------|---------|
 * \====================================|==============|=========/
 * 
 */
public class TestBoolean {
    static void testNonStatic() {
        Boolean b1 = new Boolean(true);
        Boolean b2 = new Boolean(false);
        
        System.startConcolic();
        System.symbolize(b1);

        int compare_to = b1.compareTo(b2);
        boolean equals = b1.equals(b2);
        int hash_code = b1.hashCode();
        // TODO: test `toString`

        System.endConcolic();
        
        System.out.println(compare_to);
        System.out.println(equals);
        System.out.println(hash_code);
        // System.out.println(to_string);
    }

    static void testStatic() {
        Boolean b1 = new Boolean(true);
        Boolean b2 = new Boolean(false);

        Boolean.valueOf(b1); // this is a hack to avoid <cinit>

        System.startConcolic();
        System.symbolize(b1);
    
        // // test static method
        int compare = Boolean.compare(b1, b2);
        // TODO: test `getBoolean`
        int hash_code = Boolean.hashCode(b1);
        boolean logical_and = Boolean.logicalAnd(b1, b2);
        boolean logical_or = Boolean.logicalOr(b1, b2);
        boolean logical_xor = Boolean.logicalXor(b1, b2);

        // TODO: test `toString`, `valueOf(String)`, `parseBoolean`
        Boolean value_of = Boolean.valueOf(b1);
        
        System.endConcolic();

        System.out.println(compare);
        System.out.println(hash_code);
        System.out.println(logical_and);
        System.out.println(logical_or);
        System.out.println(logical_xor);
        System.out.println(value_of);

    }

    public static void main(String[] args) {
        // testNonStatic();
        testStatic();
    }
}