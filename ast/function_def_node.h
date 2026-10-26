#pragma once

#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include "ast/block_node.h"

namespace udf {

  /**
   * Class for describing function def nodes.
   */

  class function_def_node: public cdk::typed_node {
    bool _is_public;
    bool _is_forward;
    std::string _identifier;
    cdk::sequence_node *_arguments;
    udf::block_node *_block;

  public:
    function_def_node(int lineno, bool is_public,bool is_forward, const std::string &identifier, cdk::sequence_node *arguments,
                             udf::block_node *block) :
        cdk::typed_node(lineno), _is_public(is_public),_is_forward(is_forward), _identifier(identifier), _arguments(arguments), _block(block) {
      type(cdk::primitive_type::create(0, cdk::TYPE_VOID));
    }

    function_def_node(int lineno, bool is_public,bool is_forward, std::shared_ptr<cdk::basic_type> funType, const std::string &identifier,
                             cdk::sequence_node *arguments, udf::block_node *block) :
        cdk::typed_node(lineno), _is_public(is_public),_is_forward(is_forward), _identifier(identifier), _arguments(arguments), _block(block) {
      type(funType);
    }

  public:
    
    bool is_public() const {
      return _is_public;
    }
    bool is_forward() const {
      return _is_forward;
    }
    const std::string& identifier() const {
      return _identifier;
    }
    cdk::sequence_node* arguments() {
      return _arguments;
    }
    udf::block_node* block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_def_node(this, level);
    }

  };

} // udf