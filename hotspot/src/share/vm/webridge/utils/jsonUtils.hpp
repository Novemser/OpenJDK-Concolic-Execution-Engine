//
// Created by novemser on 5/9/22.
//

#ifndef OPENJDK8_JSONUTILS_HPP
#define OPENJDK8_JSONUTILS_HPP
#ifdef ENABLE_WEBRIDGE
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "concolic/jdbc/reference/symbolicStatement.hpp"

class jsonUtils {
public:
  static std::string statementsToJsonString(const std::vector<std::pair<SymStmt *, jlong> > &sym_stmt_list);

  static jstring stringToOop(std::string input, JNIEnv *env, Thread* thread);
};

#endif
#endif //OPENJDK8_JSONUTILS_HPP
