#pragma once

#include <cdk/ast/typed_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/types/basic_type.h>

namespace udf {

  class variable_declaration_node: public cdk::typed_node {
    
    bool _is_public;
    bool _is_forward;
    std::string _identifier;
    cdk::expression_node *_initializer;

  public:
    variable_declaration_node(int lineno, bool is_public,bool is_forward, std::shared_ptr<cdk::basic_type> varType, const std::string &identifier,
                              cdk::expression_node *initializer) :
        cdk::typed_node(lineno), _is_public(is_public),_is_forward(is_forward), _identifier(identifier), _initializer(initializer) {
      this->type(varType);
    }

  public:
    bool is_public() const {
      return _is_public;
    }
    bool is_forward() const {
      return _is_forward;
    }
    const std::string& identifier() {return _identifier;}
    cdk::expression_node* initializer() {return _initializer;}

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_declaration_node(this, level);
    }

  };

} // udf