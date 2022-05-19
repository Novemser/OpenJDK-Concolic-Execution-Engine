import java.util.List;
import java.util.ArrayList;
import java.util.function.Function;
import java.util.stream.Collectors;

public class TestLambda {
  public static void main(String[] args) {
    System.startConcolic();
List<String> strs = new ArrayList<>();
        strs.add("1");
        strs.add("2");
        strs.add("3");
        List<String> res = strs.stream().mapToInt(String::hashCode)
                .mapToObj(val -> String.valueOf(val))
                .map(new Function<String, String>() {
                    @Override
                    public String apply(String s) {
                        return s.toLowerCase();
                    }
                })
                .collect(Collectors.toList());
        System.out.println(res);
    System.endConcolic();
  }
}