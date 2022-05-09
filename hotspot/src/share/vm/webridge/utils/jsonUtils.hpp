//
// Created by novemser on 5/9/22.
//

#ifndef OPENJDK8_JSONUTILS_HPP
#define OPENJDK8_JSONUTILS_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "utils/rapidjson/writer.h"
#include "utils/rapidjson/stringbuffer.h"

class jsonUtils {
public:
  static std::string statementsToJsonString(const std::vector<std::pair<SymStmt *, jlong> > &sym_stmt_list) {

    using namespace rapidjson;
    StringBuffer s;
    Writer <StringBuffer> writer(s);
    writer.StartArray();
    // traverse and add all the sql template
    for (size_t index = 0; index < sym_stmt_list.size(); ++index) {
      const std::pair<SymStmt *, jlong> &elem = sym_stmt_list[index];
      SymStmt *stmt = elem.first;
      writer.StartObject();
      writer.Key("connNo");
      writer.Int64(elem.second);
      writer.Key("sqlTemplate");
      writer.String(stmt->get_sql_template().c_str());
      // parameters
      exp_map_t param_exprs = stmt->get_param_exps();
      exp_map_t::iterator iter;
      std::vector<int> expr_position_index;
      for (iter = param_exprs.begin(); iter != param_exprs.end(); ++iter) {
        expr_position_index.push_back(iter->first);
      }
      std::sort(expr_position_index.begin(), expr_position_index.end());
      if (!expr_position_index.empty()) {
        if (expr_position_index[0] != 0) {
          tty->print_cr("Parameter index starts from %d rather than 0. SQL:%s", expr_position_index[0],
                        stmt->get_sql_template().c_str());
          assert(false, "Parameter index must starts from 0!");
        }
      }
      for (size_t param_index = 0; param_index < expr_position_index.size(); ++param_index) {
        int pos = expr_position_index[param_index];
        Expression * expr = param_exprs[pos];
      }
      // path condition

      writer.EndObject();
    }
    writer.EndArray();

    return std::string(s.GetString());
  }
};


#endif //OPENJDK8_JSONUTILS_HPP
