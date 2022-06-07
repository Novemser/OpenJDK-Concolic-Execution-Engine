package concolic.test.sql;

import com.mysql.jdbc.JDBC42PreparedStatement;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import java.math.BigDecimal;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class GetParameterTest {
    final PreparedStatement pstmt = new JDBC42PreparedStatement();

    @AfterEach
    public void end() {
        System.endConcolic();
    }

    @Test
    public void testGetBigDecimal() throws SQLException {
        System.startConcolic();
        ResultSet rs = pstmt.executeQuery();
        BigDecimal bd = rs.getBigDecimal(1);
        if (bd.longValue() == 1L) {
        }
        String pc = System.getPathCondition();
        assertFalse(pc.isEmpty());
        assertTrue(pc.contains("Y_J_1"));
    }

    @Test
    public void testGetTimestamp() throws SQLException {
        System.startConcolic();
        ResultSet rs = pstmt.executeQuery();
        Timestamp iVal = rs.getTimestamp(1);
        if (iVal.getTime() > 42) {
        }
        String pc = System.getPathCondition();
        assertFalse(pc.isEmpty());
        assertTrue(pc.contains("Y_J_42"));
//        System.weBridgeAnalysis(this.getClass().getClassLoader());
    }
}
