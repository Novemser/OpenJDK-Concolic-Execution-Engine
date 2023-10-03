import java.io.InputStream;
import java.io.Reader;
import java.math.BigDecimal;
import java.net.URL;
import java.sql.Date;
import java.util.*;
import java.text.*;
import java.math.RoundingMode;
import java.nio.charset.StandardCharsets;
import java.sql.*;

public class TestBigDecimal {
static class StubResultSet implements ResultSet {

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean next() throws SQLException {
        // TODO Auto-generated method stub
        this.getClass();
        return false;
    }

    @Override
    public void close() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean wasNull() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public String getString(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return new String("en");
    }

    @Override
    public boolean getBoolean(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        System.out.println("in getBoolean");
        return false;
    }

    @Override
    public byte getByte(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public short getShort(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getInt(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public long getLong(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public float getFloat(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public double getDouble(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public BigDecimal getBigDecimal(int columnIndex, int scale) throws SQLException {
        // TODO Auto-generated method stub
        return new BigDecimal("1.99");
    }

    @Override
    public byte[] getBytes(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Date getDate(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return new Date(1);
    }

    @Override
    public Time getTime(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Timestamp getTimestamp(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return new Timestamp(1545274444828L);
    }

    @Override
    public InputStream getAsciiStream(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public InputStream getUnicodeStream(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public InputStream getBinaryStream(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String getString(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return new String("en");
    }

    @Override
    public boolean getBoolean(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return getBoolean(1);
    }

    @Override
    public byte getByte(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public short getShort(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getInt(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public long getLong(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public float getFloat(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public double getDouble(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public BigDecimal getBigDecimal(String columnLabel, int scale) throws SQLException {
        // TODO Auto-generated method stub
        return new BigDecimal("1.99");
    }

    @Override
    public byte[] getBytes(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Date getDate(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return new Date(1);
    }

    @Override
    public Time getTime(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Timestamp getTimestamp(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return new Timestamp(1545274444828L);
    }

    @Override
    public InputStream getAsciiStream(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public InputStream getUnicodeStream(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public InputStream getBinaryStream(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public SQLWarning getWarnings() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void clearWarnings() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public String getCursorName() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public ResultSetMetaData getMetaData() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Object getObject(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Object getObject(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public int findColumn(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public Reader getCharacterStream(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Reader getCharacterStream(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public BigDecimal getBigDecimal(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return new BigDecimal("1.99");
    }

    @Override
    public BigDecimal getBigDecimal(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return new BigDecimal("1.99");
    }

    @Override
    public boolean isBeforeFirst() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean isAfterLast() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean isFirst() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean isLast() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void beforeFirst() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void afterLast() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean first() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean last() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public int getRow() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean absolute(int row) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean relative(int rows) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean previous() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void setFetchDirection(int direction) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getFetchDirection() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setFetchSize(int rows) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getFetchSize() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getType() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getConcurrency() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean rowUpdated() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean rowInserted() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean rowDeleted() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void updateNull(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBoolean(int columnIndex, boolean x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateByte(int columnIndex, byte x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateShort(int columnIndex, short x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateInt(int columnIndex, int x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateLong(int columnIndex, long x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateFloat(int columnIndex, float x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateDouble(int columnIndex, double x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBigDecimal(int columnIndex, BigDecimal x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateString(int columnIndex, String x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBytes(int columnIndex, byte[] x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateDate(int columnIndex, Date x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateTime(int columnIndex, Time x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateTimestamp(int columnIndex, Timestamp x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateAsciiStream(int columnIndex, InputStream x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBinaryStream(int columnIndex, InputStream x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateCharacterStream(int columnIndex, Reader x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateObject(int columnIndex, Object x, int scaleOrLength) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateObject(int columnIndex, Object x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNull(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBoolean(String columnLabel, boolean x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateByte(String columnLabel, byte x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateShort(String columnLabel, short x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateInt(String columnLabel, int x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateLong(String columnLabel, long x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateFloat(String columnLabel, float x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateDouble(String columnLabel, double x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBigDecimal(String columnLabel, BigDecimal x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateString(String columnLabel, String x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBytes(String columnLabel, byte[] x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateDate(String columnLabel, Date x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateTime(String columnLabel, Time x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateTimestamp(String columnLabel, Timestamp x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateAsciiStream(String columnLabel, InputStream x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBinaryStream(String columnLabel, InputStream x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateCharacterStream(String columnLabel, Reader reader, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateObject(String columnLabel, Object x, int scaleOrLength) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateObject(String columnLabel, Object x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void insertRow() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateRow() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void deleteRow() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void refreshRow() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void cancelRowUpdates() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void moveToInsertRow() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void moveToCurrentRow() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public Statement getStatement() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Object getObject(int columnIndex, Map<String, Class<?>> map) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Ref getRef(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Blob getBlob(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Clob getClob(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Array getArray(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Object getObject(String columnLabel, Map<String, Class<?>> map) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Ref getRef(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Blob getBlob(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Clob getClob(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Array getArray(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Date getDate(int columnIndex, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        return new Date(1);
    }

    @Override
    public Date getDate(String columnLabel, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        return new Date(1);
    }

    @Override
    public Time getTime(int columnIndex, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Time getTime(String columnLabel, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Timestamp getTimestamp(int columnIndex, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        return new Timestamp(1545274444828L);
    }

    @Override
    public Timestamp getTimestamp(String columnLabel, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        return new Timestamp(1545274444828L);
    }

    @Override
    public URL getURL(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public URL getURL(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void updateRef(int columnIndex, Ref x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateRef(String columnLabel, Ref x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBlob(int columnIndex, Blob x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBlob(String columnLabel, Blob x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateClob(int columnIndex, Clob x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateClob(String columnLabel, Clob x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateArray(int columnIndex, Array x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateArray(String columnLabel, Array x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public RowId getRowId(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public RowId getRowId(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void updateRowId(int columnIndex, RowId x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateRowId(String columnLabel, RowId x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getHoldability() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean isClosed() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void updateNString(int columnIndex, String nString) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNString(String columnLabel, String nString) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNClob(int columnIndex, NClob nClob) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNClob(String columnLabel, NClob nClob) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public NClob getNClob(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public NClob getNClob(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public SQLXML getSQLXML(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public SQLXML getSQLXML(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void updateSQLXML(int columnIndex, SQLXML xmlObject) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateSQLXML(String columnLabel, SQLXML xmlObject) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public String getNString(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String getNString(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Reader getNCharacterStream(int columnIndex) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Reader getNCharacterStream(String columnLabel) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void updateNCharacterStream(int columnIndex, Reader x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNCharacterStream(String columnLabel, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateAsciiStream(int columnIndex, InputStream x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBinaryStream(int columnIndex, InputStream x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateCharacterStream(int columnIndex, Reader x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateAsciiStream(String columnLabel, InputStream x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBinaryStream(String columnLabel, InputStream x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateCharacterStream(String columnLabel, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBlob(int columnIndex, InputStream inputStream, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBlob(String columnLabel, InputStream inputStream, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateClob(int columnIndex, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateClob(String columnLabel, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNClob(int columnIndex, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNClob(String columnLabel, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNCharacterStream(int columnIndex, Reader x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNCharacterStream(String columnLabel, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateAsciiStream(int columnIndex, InputStream x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBinaryStream(int columnIndex, InputStream x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateCharacterStream(int columnIndex, Reader x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateAsciiStream(String columnLabel, InputStream x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBinaryStream(String columnLabel, InputStream x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateCharacterStream(String columnLabel, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBlob(int columnIndex, InputStream inputStream) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateBlob(String columnLabel, InputStream inputStream) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateClob(int columnIndex, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateClob(String columnLabel, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNClob(int columnIndex, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void updateNClob(String columnLabel, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public <T> T getObject(int columnIndex, Class<T> type) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public <T> T getObject(String columnLabel, Class<T> type) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

}

static class StubPreparedStmt implements PreparedStatement {
ResultSet rs = new StubResultSet();

    @Override
    public ResultSet executeQuery(String sql) throws SQLException {
        // TODO Auto-generated method stub
        System.out.println("CALL ME");
        return rs;
    }

    @Override
    public int executeUpdate(String sql) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void close() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getMaxFieldSize() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setMaxFieldSize(int max) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getMaxRows() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setMaxRows(int max) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setEscapeProcessing(boolean enable) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getQueryTimeout() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setQueryTimeout(int seconds) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void cancel() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public SQLWarning getWarnings() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void clearWarnings() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setCursorName(String name) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean execute(String sql) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public ResultSet getResultSet() throws SQLException {
        // TODO Auto-generated method stub
        System.out.println("[java] calling getResultSet");
        return rs;
    }

    @Override
    public int getUpdateCount() throws SQLException {
        // TODO Auto-generated method stub
        System.out.println("[java] calling getUpdateCount");
        return 0;
    }

    @Override
    public boolean getMoreResults() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void setFetchDirection(int direction) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getFetchDirection() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setFetchSize(int rows) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getFetchSize() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getResultSetConcurrency() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getResultSetType() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void addBatch(String sql) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void clearBatch() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int[] executeBatch() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Connection getConnection() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean getMoreResults(int current) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public ResultSet getGeneratedKeys() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public int executeUpdate(String sql, int autoGeneratedKeys) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int executeUpdate(String sql, int[] columnIndexes) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int executeUpdate(String sql, String[] columnNames) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean execute(String sql, int autoGeneratedKeys) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean execute(String sql, int[] columnIndexes) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean execute(String sql, String[] columnNames) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public int getResultSetHoldability() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean isClosed() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void setPoolable(boolean poolable) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean isPoolable() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void closeOnCompletion() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean isCloseOnCompletion() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public ResultSet executeQuery() throws SQLException {
        // TODO Auto-generated method stub
        // System.out.println("INSIDE PSTMT EXEQUERY");
        rs.hashCode();
        return rs;
    }

    @Override
    public int executeUpdate() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setNull(int parameterIndex, int sqlType) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBoolean(int parameterIndex, boolean x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setByte(int parameterIndex, byte x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setShort(int parameterIndex, short x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setInt(int parameterIndex, int x) throws SQLException {
        // TODO Auto-generated method stub
        System.out.println("inside set int");
    }

    @Override
    public void setLong(int parameterIndex, long x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setFloat(int parameterIndex, float x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setDouble(int parameterIndex, double x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBigDecimal(int parameterIndex, BigDecimal x) throws SQLException {
        // TODO Auto-generated method stub
        if (x != null) x.hashCode();
    }

    @Override
    public void setString(int parameterIndex, String x) throws SQLException {
        // TODO Auto-generated method stub
        x.hashCode();
    }

    @Override
    public void setBytes(int parameterIndex, byte[] x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setDate(int parameterIndex, Date x) throws SQLException {
        // TODO Auto-generated method stub
        x.hashCode();
    }

    @Override
    public void setTime(int parameterIndex, Time x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setTimestamp(int parameterIndex, Timestamp x) throws SQLException {
        // TODO Auto-generated method stub
        if (x != null) x.hashCode();
    }

    @Override
    public void setAsciiStream(int parameterIndex, InputStream x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setUnicodeStream(int parameterIndex, InputStream x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBinaryStream(int parameterIndex, InputStream x, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void clearParameters() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setObject(int parameterIndex, Object x, int targetSqlType) throws SQLException {
        // TODO Auto-generated method stub
        switch (targetSqlType) {
            case java.sql.Types.VARCHAR:
                System.out.println("MEMEM");
                setString(parameterIndex, (String)x);
                break;
            default:
                break;
        }
        x.hashCode();
    }

    @Override
    public void setObject(int parameterIndex, Object x) throws SQLException {
        // TODO Auto-generated method stub
        x.hashCode();
        if (x instanceof Integer) {
            System.out.println("Inside setObject to int ----");
            setInt(parameterIndex, (int) x);
        }
    }

    @Override
    public boolean execute() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void addBatch() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setCharacterStream(int parameterIndex, Reader reader, int length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setRef(int parameterIndex, Ref x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBlob(int parameterIndex, Blob x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setClob(int parameterIndex, Clob x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setArray(int parameterIndex, Array x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public ResultSetMetaData getMetaData() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void setDate(int parameterIndex, Date x, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setTime(int parameterIndex, Time x, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setTimestamp(int parameterIndex, Timestamp x, Calendar cal) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setNull(int parameterIndex, int sqlType, String typeName) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setURL(int parameterIndex, URL x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public ParameterMetaData getParameterMetaData() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void setRowId(int parameterIndex, RowId x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setNString(int parameterIndex, String value) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setNCharacterStream(int parameterIndex, Reader value, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setNClob(int parameterIndex, NClob value) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setClob(int parameterIndex, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBlob(int parameterIndex, InputStream inputStream, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setNClob(int parameterIndex, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setSQLXML(int parameterIndex, SQLXML xmlObject) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setObject(int parameterIndex, Object x, int targetSqlType, int scaleOrLength) throws SQLException {
        // TODO Auto-generated method stub
        System.out.println();

    }

    @Override
    public void setAsciiStream(int parameterIndex, InputStream x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBinaryStream(int parameterIndex, InputStream x, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setCharacterStream(int parameterIndex, Reader reader, long length) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setAsciiStream(int parameterIndex, InputStream x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBinaryStream(int parameterIndex, InputStream x) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setCharacterStream(int parameterIndex, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setNCharacterStream(int parameterIndex, Reader value) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setClob(int parameterIndex, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setBlob(int parameterIndex, InputStream inputStream) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setNClob(int parameterIndex, Reader reader) throws SQLException {
        // TODO Auto-generated method stub
        
    }

}

static class StubStmt implements Statement {
    private ResultSet rs;

    StubStmt() {
        rs = new StubResultSet();
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public ResultSet executeQuery(String sql) throws SQLException {
        // TODO Auto-generated method stub
        return rs;
    }

    @Override
    public int executeUpdate(String sql) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void close() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getMaxFieldSize() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setMaxFieldSize(int max) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getMaxRows() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setMaxRows(int max) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setEscapeProcessing(boolean enable) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getQueryTimeout() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setQueryTimeout(int seconds) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void cancel() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public SQLWarning getWarnings() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public void clearWarnings() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void setCursorName(String name) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean execute(String sql) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public ResultSet getResultSet() throws SQLException {
        // TODO Auto-generated method stub
        return rs;
    }

    @Override
    public int getUpdateCount() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean getMoreResults() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void setFetchDirection(int direction) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getFetchDirection() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void setFetchSize(int rows) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int getFetchSize() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getResultSetConcurrency() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int getResultSetType() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public void addBatch(String sql) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void clearBatch() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public int[] executeBatch() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public Connection getConnection() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public boolean getMoreResults(int current) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public ResultSet getGeneratedKeys() throws SQLException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public int executeUpdate(String sql, int autoGeneratedKeys) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int executeUpdate(String sql, int[] columnIndexes) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public int executeUpdate(String sql, String[] columnNames) throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean execute(String sql, int autoGeneratedKeys) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean execute(String sql, int[] columnIndexes) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean execute(String sql, String[] columnNames) throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public int getResultSetHoldability() throws SQLException {
        // TODO Auto-generated method stub
        return 0;
    }

    @Override
    public boolean isClosed() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void setPoolable(boolean poolable) throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean isPoolable() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public void closeOnCompletion() throws SQLException {
        // TODO Auto-generated method stub
        
    }

    @Override
    public boolean isCloseOnCompletion() throws SQLException {
        // TODO Auto-generated method stub
        return false;
    }

}

static class InternalLong {
    long value;
    String fld = new String("sss");

    public InternalLong(int value) {
        this.value = value;
    }
}

    public static void main(String[] args) throws Exception {
        // System.startConcolic();
        // Integer val = new Integer(1);
        // System.symbolize(val);
        // BigDecimal bd = new BigDecimal(val.intValue());
        // if (bd.intValue() == 3) {}
        // System.out.println("PC3="+System.getPathCondition());
        // System.endConcolic();
        TestBigDecimal main = new TestBigDecimal();
        // System.out.println("=======================4 Params=========================");
        main.testExecute();
    }

    public void testExecute() throws SQLException {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        String val = new String("SJTU");
        System.symbolize(val);
        pstmt.setString(1, val);
        pstmt.execute();
        ResultSet rs = pstmt.getResultSet();
        while (rs.next()) {
            System.out.println("Calling next");
        }
        System.endConcolic();
    }

    public void testSetNumericObject3Params() throws SQLException {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();

        Integer val1 = new Integer(2);
        System.symbolize(val1);
        pstmt.setObject(1, val1, Types.NUMERIC);
        Integer val2 = new Integer(2);
        System.symbolize(val2);
        pstmt.setObject(2, val2, Types.NUMERIC);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        System.out.println(res);
        System.endConcolic();
    }

    public void testSetNumericObject4Params() throws SQLException {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        Integer val1 = new Integer(2);
        System.symbolize(val1);
        pstmt.setObject(1, val1);
        Integer val2 = new Integer(2);
//         System.symbolize(val2);
//         pstmt.setObject(2, val2, Types.NUMERIC, 0);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        System.out.println(res);
        System.endConcolic();
    }

    public void testUTF8() {
        System.startConcolic();
        String val = new String("SJTU");
        System.symbolize(val);
        int length = val.getBytes(StandardCharsets.UTF_8).length;
        // int length = val.getBytes().length;
        if (length > 0) {}
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }

    public void testBigdecimalZeroConcrete() throws Exception {
        System.startConcolic();
        Integer val = new Integer(10);
        System.symbolize(val);
        BigDecimal bd = BigDecimal.valueOf(val);
        StubPreparedStmt pstmt = new StubPreparedStmt();
        pstmt.setBigDecimal(1, bd);
        pstmt.executeQuery();

        // System.out.println(System.getPathCondition());
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));
        System.endConcolic();
    }

    public void testIntCmpLost() throws Exception {
        System.startConcolic();
        Integer val = new Integer(10);
        System.symbolize(val);
        BigDecimal bd = BigDecimal.valueOf(val);
        BigDecimal totalAllItems = BigDecimal.valueOf(val);
        totalAllItems.setScale(4);
        BigDecimal orderAdjAmt = BigDecimal.valueOf(val * 2);
        orderAdjAmt.setScale(4);
        BigDecimal fgItemAmount = bd;
        BigDecimal proratedAdjAmt = totalAllItems.compareTo(BigDecimal.ZERO) == 0 ? 
            totalAllItems : orderAdjAmt.multiply(fgItemAmount).divide(totalAllItems, RoundingMode.FLOOR);
        proratedAdjAmt.setScale(2);
        StubPreparedStmt pstmt = new StubPreparedStmt();
        pstmt.setBigDecimal(1, proratedAdjAmt);
        pstmt.executeQuery();

        // System.out.println(System.getPathCondition());
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));
        System.endConcolic();
    }

    public void testSetScaleBigger() {
        System.startConcolic();
        Integer val = new Integer(10);
        System.symbolize(val);
        BigDecimal bd = BigDecimal.valueOf(val);
        BigDecimal newBd = bd.setScale(15);
        if (newBd.intValue() == 10) {
        }
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }
    public void testSetScaleSmaller() {
        System.startConcolic();
        Integer val = new Integer(10);
        System.symbolize(val);
        BigDecimal bd = BigDecimal.valueOf(val);
        BigDecimal newBd = bd.setScale(3);
        if (newBd.intValue() == 10) {
        }
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }

    public void testNotPc() throws Exception {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        ResultSet rc = pstmt.executeQuery();
        rc.next();
        String str = rc.getString(1);
        if (!str.startsWith("qwe")) {
            str = "^" + str;
        }

        if (!str.endsWith("suffix")) {
        }
        System.out.println(System.getPathCondition());
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));

        System.endConcolic();
    }

    public void testWhat() {
        System.startConcolic();
        String val = new String("htop");
        System.symbolize(val, "user_name");
        if (val.equals("150141.user")) {

        }
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }

    private static class Order {
        private String comments = new String("cmmmm");
        private String currency = new String("currency");
        private Long val = new Long(11l);
    }

    public void testName() throws Exception {
        System.startConcolic();
        Order od = new Order();
        String nm = new String("COVID19");
        // System.symbolize(od, "prefix");
        System.symbolize(nm, "IDIDID");
        StubPreparedStmt pstmt = new StubPreparedStmt();
        pstmt.setString(1, od.comments);
        pstmt.executeQuery();
        // if (od.val == 11l) {}
        if (nm.equals("ID")){}
        System.out.println(System.getPathCondition());
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));
        System.endConcolic();
    }
    public void testLocale() throws Exception {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        ResultSet rs = pstmt.executeQuery();
        // String val = rs.getString(1);
        String val = new String("en");
        String us = "US";
        InternalLong il = new InternalLong(10);
        // System.symbolize(val);
        System.symbolize(il);
        if (il.fld.contains("en")) {}
        if (il.value == 10) {}
        // new java.util.Locale(val, us);
        // new java.util.Locale(val, us);
        // pstmt2.setTimestamp(1, preTs);
        // pstmt2.executeQuery();
        System.out.println(System.getPathCondition());
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));
        System.endConcolic();
    }

    public void testBigDecimalAdd() throws Exception {
        System.startConcolic();

        // Integer val = new Integer(10);
        // System.symbolize(val);
        // Double d1 = new Double(1.0);
        // Double d2 = new Double(2.0);
        // System.symbolize(d1);
        // System.symbolize(d2);
        // BigDecimal bd1 = new BigDecimal(d1);
        // BigDecimal bd2 = new BigDecimal(d2);
        // BigDecimal bdRes = bd1.add(bd2);
        // BigDecimal res = bd.add(bd);
        // .multiply(bd)
        // .divide(bd);
        StubPreparedStmt pstmt0 = new StubPreparedStmt();
        ResultSet rc = pstmt0.executeQuery();
        rc.next();
        BigDecimal price = rc.getBigDecimal(1);

        // BigDecimal grandTotal = new BigDecimal(0);
        // grandTotal.setScale(2, java.math.RoundingMode.HALF_UP);

        BigDecimal subTotal = new BigDecimal(0);
        subTotal.setScale(2, java.math.RoundingMode.HALF_UP);
        Integer quantity = new Integer(10);
        // Double price = new Double(1.99d);
        // String price = new String("1.99");
        // System.symbolize(price, "price");
        System.symbolize(quantity, "quantity");
        BigDecimal qty = new BigDecimal(quantity);
        BigDecimal st = price.multiply(qty);
        // st = new BigDecimal("19.9");

        subTotal = subTotal.add(st);
        // subTotal = st.add(subTotal);
        // grandTotal=grandTotal.add(subTotal);
        // String submitedAmount = getAdminFormatedAmount(grandTotal);
        // if (submitedAmount.equals(new BigDecimal(1.99d))) {

        // }
        StubPreparedStmt pstmt = new StubPreparedStmt();
        // pstmt.setString(1, submitedAmount);
        // pstmt.setBigDecimal(2, grandTotal);
        pstmt.setBigDecimal(1, st);
        ResultSet rs = pstmt.executeQuery();
        // BigDecimal bdFromRs = rs.getBigDecimal(1);
        // pstmt = new StubPreparedStmt();
        // pstmt.setBigDecimal(1, bdFromRs.multiply(grandTotal));
        // pstmt.executeQuery();
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));
        System.endConcolic();
    }
	private final static char DECIMALCOUNT = '2';

	public String getAdminFormatedAmount(BigDecimal amount) throws Exception {
			
		if(amount==null) {
			return "";
		}
		
		NumberFormat nf = null;

			
		nf = NumberFormat.getInstance(Locale.US);

		nf.setMaximumFractionDigits(Integer.parseInt(Character
					.toString(DECIMALCOUNT)));
		nf.setMinimumFractionDigits(Integer.parseInt(Character
					.toString(DECIMALCOUNT)));

		return nf.format(amount);
	}

    public void testSetEnum() throws Exception {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        pstmt.setObject(1, "ENUM_STR", java.sql.Types.VARCHAR);
        pstmt.executeQuery();
        // System.symbolize(val);
        // System.out.println(System.getPathCondition());
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));
        System.endConcolic();
    }

    public void testSetDate() throws Exception {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        StubPreparedStmt pstmt2 = new StubPreparedStmt();
        Date dt = new Date(1655274444828L);
        pstmt.setDate(1, dt);
        ResultSet rs = pstmt.executeQuery();
        Timestamp preTs = rs.getTimestamp(1);
        // System.symbolize(val);
        pstmt2.setTimestamp(1, preTs);
        pstmt2.executeQuery();
        System.out.println(System.getPathCondition());
        System.out.println(System.weBridgeAnalysis(getClass().getClassLoader()));
        System.endConcolic();
    }

	private final static String LINE_BREAK = "<br/>";

    private String getFName() {
        String name = new String("FNAME");
        System.symbolize(name);
        return null;
    }

    public void testAppendSb() {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        StringBuilder sb = new StringBuilder();
        String val = new String("value");
        System.symbolize(val);
        
        // val = null;
        String str = sb.append(val).append(getFName()).append(" ").append(LINE_BREAK).toString();
        if (str.contains("fuck")) {}
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }

    public void testSetDateCorrectly() throws Exception {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        pstmt.setTimestamp(1, new Timestamp(System.currentTimeMillis()));
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        System.out.println(res);
        System.endConcolic();
    }

    public void testStringEQ() throws Exception {
        System.startConcolic();
        String val = new String("AllTooWell");
        System.symbolize(val);
        if (val.equals("AllTooWell")) {

        }
        System.endConcolic();
    }

    public void testTSSetByDate() throws SQLException {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        ResultSet rs = pstmt.executeQuery();
        java.util.Date value = rs.getTimestamp(1);
        if ( value instanceof Timestamp ) {
            Timestamp orig = (Timestamp) value;
            Timestamp ts = new Timestamp( orig.getTime() );
            ts.setNanos( orig.getNanos() );
        }
        String res = System.getPathCondition();
        System.out.println(res);

        System.endConcolic();
    }

    public void testGetBigDecimal() throws SQLException {
        StubPreparedStmt pstmt = new StubPreparedStmt();

        System.startConcolic();
        ResultSet rs = pstmt.executeQuery();
        BigDecimal bd = rs.getBigDecimal(1);
        if (bd.longValue() == 1L) {
        }
        // String pc = System.getPathCondition();
        // System.out.println(pc);
        System.endConcolic();
    }

    public void testSetBigDecimalSymbolic() throws SQLException {
        System.startConcolic();
        Long val = new Long(42L);
        StubPreparedStmt pstmt = new StubPreparedStmt();
        System.symbolize(val);
        BigDecimal bd = BigDecimal.valueOf(val);
        pstmt.setBigDecimal(1, bd);
        pstmt.executeQuery();
        String res = System.weBridgeAnalysis(getClass().getClassLoader());
        System.out.println(res);
        System.endConcolic();
    }

    public void testSetTs() throws SQLException {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        Long val = new Long(42);
        System.symbolize(val);
        Timestamp ts = new Timestamp(val);
        new Timestamp(ts.getTime());
        // pstmt.setTimestamp(1, ts);
        // ResultSet rs = pstmt.executeQuery();
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }

    public void testInternPc() {
        System.startConcolic();
        String val = new String("SJTU");
        System.symbolize(val);
        String internStr = val.intern();
        if (internStr.contains("SJTU")) {}
        String pc = System.getPathCondition();
        System.out.println(pc);
        System.endConcolic();
    }

    public void testIntern() {
        System.startConcolic();
        String val = new String("ME");
        System.symbolize(val);
        String internStr = val.intern();
        if (internStr.contains("ME")) {}
        System.endConcolic();
    }

    public void testStubRs() throws Exception {
        System.startConcolic();
        StubPreparedStmt pstmt = new StubPreparedStmt();
        // pstmt.execute("SQL");
        ResultSet rs = pstmt.executeQuery();
        // rs.next();
        BigDecimal bd = rs.getBigDecimal(1);
        if (bd.longValue() == 1L) {}
        // String s = rs.getString(1);
        // if (s.startsWith("n")) {}
        // int i = rs.getInt(1);
        // if (i == 1) {}
        // Timestamp iVal = rs.getTimestamp(1);
        // if (iVal.getTime() > 0) {}
        // iVal.hashCode();
        // System.out.println("DDDD");
        // rs.getInt(1);
        System.out.println(System.getPathCondition());
        // String res = System.weBridgeAnalysis(getClass().getClassLoader());
        // System.out.println("FUCK");
        // System.out.println(res);
        System.endConcolic();
    }

    private void testI2F() {
        System.startConcolic();
        Integer val = new Integer(10);
        System.symbolize(val);
        Float dVal = new Float((float)val.intValue());
        if (dVal.floatValue() == 10d) {}
        System.endConcolic();
    }

    private void testValOf() {
        System.startConcolic();
        Integer i = Integer.valueOf(5);
        System.endConcolic();
    }

    private Float prepareVal(float v) {
        Float val = new Float(v);
        System.symbolize(val);
        return val;
    }

    private Long prepareVal(long v) {
        Long val = new Long(v);
        System.symbolize(val);
        return val;
    }
    public void testEQ() {
        System.startConcolic();
        // ifne
        if (prepareVal(3.0f) == 10.0f) {
        }
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }
    public void testlcmp() {
        System.startConcolic();
        Long value = new Long(42);
        System.symbolize(value);
        if (value.longValue() == 42) {}
        System.out.println(System.getPathCondition());

        System.endConcolic();
    }

    public void testConcrete() {
        System.startConcolic();
        Integer val = new Integer(10);
        BigDecimal bd = BigDecimal.valueOf(val);
        BigDecimal newBd = bd.setScale(15);
        if (newBd.intValue() == 10) {}
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }

    public void testSetScaleConcrete() {
        System.startConcolic();
        Integer val = new Integer(10);
        System.symbolize(val);
        BigDecimal bd = BigDecimal.valueOf(val);
        BigDecimal newBd = bd.setScale(3);
        if (newBd.intValue() == 10) {}
        System.out.println(System.getPathCondition());
        System.endConcolic();
    }

    public void testStringInit() {
        System.startConcolic();
        String val = new String("10");
        System.symbolize(val);
        BigDecimal bd = new BigDecimal(val);
        if (bd.intValue() == 42) {}
        System.out.println("PC=" + System.getPathCondition());
        System.endConcolic();
    }

    public void testDoubleInit() {
        System.startConcolic();
        Double val = new Double(1.0);
        System.symbolize(val);
        BigDecimal bd = new BigDecimal(val.doubleValue());
        if (bd.doubleValue() == 3.2d) {}
        System.out.println("PC2=" + System.getPathCondition());
        System.endConcolic();
    }

    public void testIntInit() {
        System.startConcolic();
        Integer val = new Integer(1);
        System.symbolize(val);
        BigDecimal bd = new BigDecimal(val.intValue());
        if (bd.intValue() == 3) {}
        System.out.println("PC3="+System.getPathCondition());
        System.endConcolic();
    }
}
