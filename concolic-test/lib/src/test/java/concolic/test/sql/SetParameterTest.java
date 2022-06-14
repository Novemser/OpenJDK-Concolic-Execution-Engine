package concolic.test.sql;

import com.alibaba.fastjson2.JSONArray;
import com.alibaba.fastjson2.JSONObject;
import com.mysql.jdbc.JDBC42PreparedStatement;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.math.BigDecimal;
import java.sql.*;

import static org.junit.jupiter.api.Assertions.assertEquals;
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

    @Test
    public void testSetSQLDate() throws SQLException {
        System.startConcolic();
        Date dt = new Date(System.currentTimeMillis());
        pstmt.setDate(1, dt);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        assertTrue(res.contains("TO_TIMESTAMP"));
        assertTrue(res.contains("currentTimeMills"));
    }

    @Test
    public void testSetEnum() throws Exception {
        System.startConcolic();
        pstmt.setObject(1, "ENUM_STR", Types.VARCHAR);
        Integer intVal = new Integer(42);
        System.symbolize(intVal);
        pstmt.setObject(2, intVal, Types.INTEGER);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        JSONArray jArr = JSONArray.parseArray(res);
        assertEquals(1, jArr.size());
        JSONObject jRes = jArr.getJSONObject(0);
        JSONArray params = jRes.getJSONArray("parameterExprs");
        assertEquals(2, params.size());
        JSONObject pam1 = params.getJSONObject(0);
        assertEquals("ConStringSymbolExp", pam1.getString("_type"));
        assertEquals("java/lang/String", pam1.getString("_java_type"));
        JSONObject pam2 = params.getJSONObject(1);
        assertEquals("SymbolExpression", pam2.getString("_type"));
        assertEquals("I", pam2.getString("_java_type"));
    }
}
