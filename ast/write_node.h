#pragma once

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>


namespace udf { 

  class write_node: public cdk::basic_node {
    cdk::sequence_node *_args;
    bool _is_end;

  public:
    inline write_node(int lineno, cdk::sequence_node *args, bool is_end) :
        cdk::basic_node(lineno), _args(args) ,_is_end(is_end)  {
    }

    cdk::sequence_node *args() {
      return _args;
    }
    bool is_end() {
      return _is_end;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_write_node(this, level);
    }

  };
  

} // udf
