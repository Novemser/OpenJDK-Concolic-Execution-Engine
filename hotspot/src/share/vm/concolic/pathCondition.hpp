#ifndef SHARE_VM_CONCOLIC_PATHCONDITION_HPP
#define SHARE_VM_CONCOLIC_PATHCONDITION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"

#include <vector>

class PathCondition {
  struct Condition {
    std::string _code_pos;
    Expression *_exp;

    Condition(std::string code_pos, Expression *exp)
        : _code_pos(code_pos), _exp(exp) {
      exp->inc_ref();
    }

    ~Condition() { Expression::gc(_exp); }

    Expression *getExp() const {
      return _exp;
    }
  };

private:
  std::vector<Condition *> _conds;
  char * _pc_str;

public:
  PathCondition() : _pc_str(NULL) {}

  PathCondition(const PathCondition& original) : _pc_str(NULL) {
    for (size_t index = 0; index < original._conds.size(); ++index) {
      this->_conds.push_back(original._conds[index]);
    }
  }

  const std::vector<Condition *> &getConds() const {
    return _conds;
  }

  void add(Expression *exp);

  void gc();

  void print();

  void serialize(rapidjson::Writer<rapidjson::StringBuffer> &writer) const {
    writer.StartArray();
    for (size_t index = 0; index < _conds.size(); index++) {
      Condition *cond = _conds[index];
      Expression *expr = cond->_exp;
      if (expr) {
        expr->serialize(writer);
      }
    }
    writer.EndArray();
  }

  const char* toString() {
    if (_pc_str != NULL) {
      return _pc_str;
    }

    using namespace rapidjson;
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    serialize(writer);
    const char* res = s.GetString();
    guarantee(_pc_str == NULL, "should be null");
    _pc_str = new char[strlen(res)];
    strcpy(_pc_str, res);
    return _pc_str;
  }
};

#endif

#endif // SHARE_VM_CONCOLIC_PATHCONDITION_HPP