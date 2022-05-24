//
// Created by gansen on 5/2/22.
//
#ifdef ENABLE_WEBRIDGE
#ifndef JDK8_CDB_SYMBOLICSQLTEMPLATE_HPP
#define JDK8_CDB_SYMBOLICSQLTEMPLATE_HPP

#include "concolic/pathCondition.hpp"
#include <string>
#include <vector>

class symbolicSQLTemplate {
public:
  enum SqlType {
    SELECT,
    UPDATE,
    DELETE,
    INSERT,
    ENABLE_AUTO_COMMIT,
    DISABLE_AUTO_COMMIT,
    COMMIT,
  };

  static bool isTxnControl(SqlType sqlType) {
    return sqlType == ENABLE_AUTO_COMMIT || sqlType == DISABLE_AUTO_COMMIT || sqlType == COMMIT;
  }

  static bool isWriteSql(SqlType sqlType) {
    return sqlType == UPDATE || sqlType == INSERT || sqlType == DELETE;
  }

private:
  uint _sqlNo;
  uint _connectionId;

  SqlType _sqlType;
  std::string _sqlTemplate;
  std::vector<std::string> _parameterExprs;
  std::vector<Expression*> _parameterSymbolics;
  std::vector<std::string> _concreteParameters;

  PathCondition _pc;

public:
  inline void setSqlNo(uint sqlNo) { _sqlNo = sqlNo; }

  inline void setConnectionId(uint id) { _connectionId = id; }

  inline void setSqlTemplate(const std::string &sqlTemplate) { _sqlTemplate = sqlTemplate; }

  uint getSqlNo() const {
    return _sqlNo;
  }

  uint getConnectionId() const {
    return _connectionId;
  }

  SqlType getSqlType() const {
    return _sqlType;
  }

  const std::string &getSqlTemplate() const {
    return _sqlTemplate;
  }

  const std::vector<std::string> &getParameterExprs() const {
    return _parameterExprs;
  }

  const std::vector<Expression*> &getParameterSymbolics() const {
    return _parameterSymbolics;
  }

  const std::vector<std::string> &getConcreteParameters() const {
    return _concreteParameters;
  }

  const PathCondition &getPc() const {
    return _pc;
  }
};

#endif //JDK8_CDB_SYMBOLICSQLTEMPLATE_HPP
#endif