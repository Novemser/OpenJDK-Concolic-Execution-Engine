package concolic.test.sql;

import com.mysql.jdbc.JDBC42PreparedStatement;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

import static org.junit.jupiter.api.Assertions.assertEquals;

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
        Timestamp ts = new Timestamp(4200);
        pstmt.setTimestamp(1, ts);
        ResultSet rs = pstmt.executeQuery();
        String pc = System.getPathCondition();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
    }
}
