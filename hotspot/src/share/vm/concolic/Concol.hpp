#ifndef SHARE_VM_CONCOLIC_CONCOL_HPP
#define SHARE_VM_CONCOLIC_CONCOL_HPP

#include "oops/klass.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/oop.hpp"
#include "oops/oop.inline.hpp"
#include "jvmtifiles/jvmti.h"
#include "runtime/fieldDescriptor.hpp"
#include "runtime/handles.hpp"
#include "utilities/top.hpp"


class FieldSymbolizer: public FieldClosure {
private:
    int _depth;
    oop _obj;
public:
    FieldSymbolizer(oop obj, int depth) : _obj(obj), _depth(depth) {}
    void do_field(fieldDescriptor* fd);
};

class Concol {
public:
    Concol(oop o);
    static void symbolize(oop obj, int depth);

};

#endif