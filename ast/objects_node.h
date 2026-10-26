#pragma once

#include <cdk/ast/typed_node.h>
#include <cdk/ast/expression_node.h>

namespace udf {

 class objects_node: public cdk::expression_node {
    cdk::expression_node *_positions;

  public:
    objects_node(int lineno, std::shared_ptr<cdk::basic_type> type, cdk::expression_node *positions) :
    cdk::expression_node(lineno), _positions(positions) {
    this->type(type);
    }

    cdk::expression_node *positions() { return _positions; }

    void accept(basic_ast_visitor *sp, int level) { sp->do_objects_node(this, level);}
  };

} // udf