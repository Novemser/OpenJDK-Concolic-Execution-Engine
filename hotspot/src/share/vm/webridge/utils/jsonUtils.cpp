//
// Created by novemser on 5/9/22.
//
#include "jsonUtils.hpp"
#ifdef ENABLE_WEBRIDGE
#include <ctime>

#include "webridge/utils/rapidjson/writer.h"
#include "webridge/utils/rapidjson/stringbuffer.h"
#include <algorithm>

void toUpperCase(std::string &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

std::string jsonUtils::statementsToJsonString(const std::vector<std::pair<SymStmt *, jlong> > &sym_stmt_list) {
  using namespace rapidjson;
  StringBuffer s;
  Writer<StringBuffer> writer(s);
  writer.StartArray();
  // traverse and add all the sql template
  std::vector<double> elp;
  for (size_t index = 0; index < sym_stmt_list.size(); ++index) {
    std::clock_t start;
    start = std::clock();
    const std::pair<SymStmt *, jlong> &elem = sym_stmt_list[index];
    SymStmt *stmt = elem.first;
    std::string templateStr = stmt->get_sql_template();
    writer.StartObject();
    writer.Key("connNo");
    writer.Int64(elem.second);
    writer.Key("sqlTemplate");
    writer.String(templateStr.c_str());
    writer.Key("isTxnControl");
    writer.Bool(stmt->is_txn_control());
    writer.Key("sqlId");
    writer.Int(stmt->get_query_id());
    writer.Key("rowCountExp");
    if (stmt->get_row_count_exp()) {
      stmt->get_row_count_exp()->serialize(writer);
    } else {
      writer.Null();
    }
    writer.Key("rowCount");
    toUpperCase(templateStr);
    if (!stmt->is_txn_control() && templateStr != "SELECT 1") {
      writer.Int(stmt->get_concrete_row_count());
    } else {
      // txn control stmt returns 0 rows
      writer.Int(0);
    }

    // SQL parameters
    writer.Key("parameterExprs");
    writer.StartArray();
    exp_map_t param_exprs = stmt->get_param_exps();
    exp_map_t::iterator iter;
    std::vector<int> expr_position_index;
//    tty->print_cr("param_exprs size:%lu", param_exprs.size());
    for (iter = param_exprs.begin(); iter != param_exprs.end(); ++iter) {
      expr_position_index.push_back(iter->first);
    }
    std::sort(expr_position_index.begin(), expr_position_index.end());
    if (!expr_position_index.empty()) {
      if (expr_position_index[0] != 1) {
        tty->print_cr("Parameter index starts from %d rather than 0. SQL:%s", expr_position_index[0],
                      stmt->get_sql_template().c_str());
        guarantee(false, "Parameter index must starts from 1!");
      }
      if (expr_position_index[expr_position_index.size() - 1] != (int) expr_position_index.size()) {
        tty->print_cr("Parameter index should be consequent. SQL:%s", stmt->get_sql_template().c_str());
        std::vector<int>::iterator iter_idx = expr_position_index.begin();
        for (; iter_idx != expr_position_index.end(); iter_idx++) {
          tty->print("%d", *iter_idx);
          if (iter_idx + 1 != expr_position_index.end()) {
            tty->print(",");
          }
        }
        guarantee(false, "Non-consequent parameter index");
      }
    }
    for (size_t param_index = 0; param_index < expr_position_index.size(); ++param_index) {
      int pos = expr_position_index[param_index];
      Expression *expr = param_exprs[pos];
      if (expr) {
        expr->serialize(writer);
      } else {
        writer.Null();
      }
    }
    writer.EndArray();
    // path conditions
    PathCondition pc = stmt->getPc();
    writer.Key("pathCondition");
    pc.serialize(writer);

    writer.EndObject();
    double elapsed = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
    tty->print_cr("SQL %lu used %f ms to serialize", index, elapsed);
    elp.push_back(elapsed);
  }
  writer.EndArray();

  tty->print_cr("[WeBridge] Finished serialization");
//  for (size_t index = 0; index < elp.size(); ++index) {
//    tty->print_cr("EPTIME: %lu %f", index, elp[index]);
//  }
  return std::string(s.GetString());
}


jstring jsonUtils::stringToOop(std::string input, JNIEnv *env, Thread *thread) {
  Klass *strClz = SystemDictionary::resolve_or_fail(
      vmSymbols::java_lang_String(),
      true,
      thread
  );
  assert(strClz, "We should find string class...");
//  gss_buffer_desc_struct outNameBuf = {
//      input.length(), (void*)input.c_str()
//  };
  assert(strClz->oop_is_instance(), "Should be InstanceKlass");
  InstanceKlass *ik = (InstanceKlass *) strClz;
  assert(ik->is_initialized(), "Should be initialized");
  // TODO: try initialize the String object
  return NULL;
}
#endif