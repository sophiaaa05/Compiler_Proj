#pragma once

#include <cdk/ast/binary_operation_node.h>
#include <cdk/ast/expression_node.h>

namespace udf {

  /**
   * Class for describing the contraction operator
   */
  class contraction_node : public cdk::binary_operation_node {
  public:
    contraction_node(int lineno, cdk::expression_node *left, cdk::expression_node *right) noexcept:
        cdk::binary_operation_node(lineno, left, right) {
    }

    void accept(basic_ast_visitor *sp, int level) override { sp->do_contraction_node(this, level); }
  };

} // udf