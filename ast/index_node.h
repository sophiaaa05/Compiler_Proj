#pragma once

#include <cdk/ast/lvalue_node.h>

//TODO: este index é necessario?

namespace udf {

  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_expr;
    cdk::expression_node *_index;

  public:
    index_node(int lineno, cdk::expression_node *expr, cdk::expression_node *index) :
        cdk::lvalue_node(lineno), _expr(expr), _index(index) {
    }

  public:
    cdk::expression_node* expr() {return _expr;}
    cdk::expression_node* index() {return _index;}

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // udf
