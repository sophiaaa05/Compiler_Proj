#pragma once

#include "cdk/ast/basic_node.h"
#include <cdk/ast/expression_node.h>

//TODO: basic or typed

namespace udf {

class return_node : public cdk::typed_node {
    cdk::expression_node* _value;

  public:
    inline return_node(int lineno, cdk::expression_node* value = nullptr): 
    cdk::typed_node(lineno), _value(value) {}

    cdk::expression_node* value() const {return _value;}

    void accept(basic_ast_visitor* sp, int level) override{
        sp->do_return_node(this, level);
    }
};

} // udf