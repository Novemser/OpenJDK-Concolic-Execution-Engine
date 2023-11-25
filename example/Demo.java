import java.util.*;

public class Demo {
	public static void main(String[] args) throws Exception {
        Integer val = new Integer(1);
        String str = new String("ipads");

		// initialize the concolic execution, and symbolize the parameters that you like
		System.startConcolic();
		System.symbolize(val);
		System.symbolize(str);
        if (val == 3) {
			System.out.println("Value = 3");
		}

		if (str.startsWith("ip")) {
			System.out.println("Str starts with ip");
		}
        System.out.println("Path condition=" + System.getPathCondition());
        System.endConcolic();
	}
}
