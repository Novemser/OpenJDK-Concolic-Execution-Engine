#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "symbolicHibernateMethod.hpp"


std::set<std::string> SymHibernateMethod::handle_method_name_sigs = init_handle_method_name_sigs();

std::set<std::string> SymHibernateMethod::init_handle_method_name_sigs() {
  std::set<std::string> name_sigs;
  name_sigs.insert(std::string("org.hibernate.id.enhanced.TableGenerator.generate(Lorg/hibernate/engine/spi/SharedSessionContractImplementor;Ljava/lang/Object;)Ljava/io/Serializable;"));
  return name_sigs;
}

bool SymHibernateMethod::target(const std::string& name_sig) {
  return handle_method_name_sigs.find(name_sig) != handle_method_name_sigs.end();
}


#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
