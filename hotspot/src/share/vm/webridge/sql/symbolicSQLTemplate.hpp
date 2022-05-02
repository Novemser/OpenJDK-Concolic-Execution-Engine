//
// Created by gansen on 5/2/22.
//

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
  unsigned sqlNo;
  unsigned connectionId;

  SqlType sqlType;
  std::string sqlTemplate;
  std::vector<std::string> parameterExprs;
  std::vector<Expression> parameterSymbolics;
  std::vector<std::string> concreteParameters;

  PathCondition pc;
};


#endif //JDK8_CDB_SYMBOLICSQLTEMPLATE_HPP
