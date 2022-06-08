package concolic.test.sql;

import com.mysql.jdbc.JDBC42PreparedStatement;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.math.BigDecimal;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;

import static org.junit.jupiter.api.Assertions.assertTrue;

public class SetParameterTest {
    PreparedStatement pstmt;
    @BeforeEach
    public void start() {
        pstmt = new JDBC42PreparedStatement();
    }

    @AfterEach
    public void end() {
        System.endConcolic();
    }

    @Test
    public void testSetTimestamp() throws SQLException {
        System.startConcolic();
        Timestamp ts = new Timestamp(System.currentTimeMillis());
        pstmt.setTimestamp(1, ts);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
//        System.out.println(res);
        // make sure parameters are not empty
        assertTrue(res.contains("parameterExprs\":[{"));
        assertTrue(res.contains("PlaceholderExpression"));
        assertTrue(res.contains("currentTimeMills"));
    }

    @Test
    public void testSetBigDecimalConcrete() throws SQLException {
        System.startConcolic();
        BigDecimal bd = BigDecimal.valueOf(42L);
        pstmt.setBigDecimal(1, bd);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        assertTrue(res.contains("Y_D_42"));
    }

    @Test
    public void testSetBigDecimalSymbolic() throws SQLException {
        System.startConcolic();
        Long val = new Long(42L);
        System.symbolize(val);
        BigDecimal bd = BigDecimal.valueOf(val);
        pstmt.setBigDecimal(1, bd);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        assertTrue(res.contains("_scale"));
        assertTrue(res.contains("_intCompact"));
        assertTrue(res.contains("$BIG_DECIMAL$"));
    }
}
