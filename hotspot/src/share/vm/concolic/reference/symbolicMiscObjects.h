#ifndef SYMBOLIC_MISC_OBJECTS
#define SYMBOLIC_MISC_OBJECTS

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"


class SymbolicMiscObjects: public SymInstance {
public:
    static method_set_t handle_method_names;
    static std::map<std::string, bool> skip_method_names;

public:
    static bool invoke_method_helper(MethodSymbolizerHandle &handle);
    static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

    static void init_register_class(MethodSymbolizer* m_symbolizer);

};
#endif // ENABLE_CONCOLIC


#endif //SYMBOLIC_MISC_OBJECTS
