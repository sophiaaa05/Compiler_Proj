#pragma once

#include <cdk/ast/expression_node.h>

namespace udf {

  /**
   * Class for describing function calls nodes.
   */
  class function_call_node : public cdk::expression_node {
    std::string _id;
    cdk::sequence_node * _args;

    public:
        /**
         * Constructor for a function call without args.
         */
        function_call_node(int lineno, const std::string &id) :
            cdk::expression_node(lineno), _id(id), _args(new cdk::sequence_node(lineno)) {
        }

        /**
         * Constructor for a function call with args.
         */
        function_call_node(int lineno, const std::string &id, cdk::sequence_node *args) :
            cdk::expression_node(lineno), _id(id), _args(args) {
    }

    const std::string& id() {return _id;}
    cdk::sequence_node* args() {return _args;}

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_call_node(this, level);
    }

  };

} // udf
