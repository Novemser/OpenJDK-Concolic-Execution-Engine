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
        assertTrue(res.contains("\"M_J_field_java_lang_Long_value\""));
    }

    @Test
    public void testSetBigDecimalAddSymbolic() throws SQLException {
        System.startConcolic();
        Double d1 = new Double(1.0);
        Double d2 = new Double(2.0);
        System.symbolize(d1);
        System.symbolize(d2);
        BigDecimal bd1 = new BigDecimal(d1);
        BigDecimal bd2 = new BigDecimal(d2);
        BigDecimal bdRes = bd1.add(bd2);
        pstmt.setBigDecimal(1, bdRes);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        JSONArray jArr = JSONArray.parseArray(res);
        assertEquals(1, jArr.size());
        JSONObject jRes = jArr.getJSONObject(0);
        JSONArray params = jRes.getJSONArray("parameterExprs");
        assertEquals(1, params.size());
        JSONObject paramJson = params.getJSONObject(0);
        assertEquals("BigDecimalExpression", paramJson.getString("_type"));
        JSONObject binaryExprJson = paramJson.getJSONObject("_intCompact");
        assertEquals("BinaryExpression", binaryExprJson.getString("_type"));
        assertEquals("+", binaryExprJson.getString("_op"));
        assertTrue(binaryExprJson.toString().contains("\"$BIG_DECIMAL$M_D_field_java_lang_Double_value#intCompact$BIG_DECIMAL$\""));
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

    @Test
    public void testConcreteSetDate() throws Exception {
        System.startConcolic();
        PreparedStatement pstmt = new JDBC42PreparedStatement();
        PreparedStatement pstmt2 = new JDBC42PreparedStatement();
        Date dt = new Date(1655274444828L);
        pstmt.setDate(1, dt);
        ResultSet rs = pstmt.executeQuery();
        Timestamp preTs = rs.getTimestamp(1);

        pstmt2.setTimestamp(1, preTs);
        pstmt2.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        JSONArray jArr = JSONArray.parseArray(res);
        assertEquals(2, jArr.size());
        assertTrue(res.contains("1655274444828"));
        assertTrue(res.contains("#fastTime"));
        assertTrue(res.contains("TO_TIMESTAMP"));
    }
}
