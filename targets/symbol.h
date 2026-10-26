#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cdk/types/basic_type.h>

namespace udf {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::string _name;

    int _offset;
    bool _is_global;
    bool _is_public;
    bool _is_forward;
    bool _is_function;
    bool _constant; // is it a constant?
    std::vector<std::shared_ptr<cdk::basic_type>> _params;
    std::vector<std::shared_ptr<cdk::basic_type>> _argument_types;

  public:

    
    symbol(std::shared_ptr<cdk::basic_type> type,const std::string &name,int offset,bool is_global,bool is_public,
    bool is_forward,bool is_function,bool constant): _type(type),
      _name(name),_offset(offset),_is_global(is_global), _is_public(is_public), _is_forward(is_forward), _is_function(is_function), _constant(constant) {}

    
    symbol(std::shared_ptr<cdk::basic_type> type,const std::string &name,int offset,bool is_global,bool is_public,
    bool is_forward,bool is_function,bool constant,const std::vector<std::shared_ptr<cdk::basic_type>> &params): _type(type),
    _name(name),_offset(offset),_is_global(is_global),_is_public(is_public),_is_forward(is_forward),_is_function(is_function),_constant(constant),_params(params) {}

    virtual ~symbol() = default;

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }

    const std::string &name() const {
      return _name;
    }

    int offset() const {
      return _offset;
    }

    bool is_global() const {
      return _is_global;
    }

    bool is_public() const {
      return _is_public;
    }

    bool is_forward() const {
      return _is_forward;
    }

    bool is_function() const {
      return _is_function;
    }

    bool is_constant() const {
      return _constant;
    }

    inline int offset(int o) {
      return _offset = o;
    }
    inline void is_global(bool global){
      _is_global = global;
    }
    
    const std::vector<std::shared_ptr<cdk::basic_type>> &params() const {
      return _params;
    }

    bool is_typed(cdk::typename_type name) const {
      return _type && _type->name() == name;
    }
    void set_argument_types(const std::vector<std::shared_ptr<cdk::basic_type>> &types) {
      _argument_types = types;
    }

    bool argument_is_typed(size_t ax, cdk::typename_type name) const {
      return _argument_types[ax]->name() == name;
    }

    void set_offset(int offset) {
      _offset = offset;
    }
    std::shared_ptr<cdk::basic_type> argument_type(size_t ax) const {
      return _argument_types[ax];
    }

    size_t argument_size(size_t ax) const {
      return _argument_types[ax]->size();
    }

    size_t number_of_arguments() const {
      return _argument_types.size();
    }
    void set_type(std::shared_ptr<cdk::basic_type> t) {
      _type = t;
    }
  };

 inline auto make_symbol(std::shared_ptr<cdk::basic_type> type, const std::string &name,
  int offset, bool is_global, bool is_public, bool is_forward,bool is_function, bool constant) {
  return std::make_shared<symbol>(type, name, offset, is_global, is_public, is_forward, is_function, constant);
}

} // namespace udf
