#pragma once

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include <vector>

namespace udf {


    //TODO: REVER PESSOMO
class tensor_index_node : public cdk::lvalue_node {
    cdk::expression_node *_tensor;                   // Expressão do tensor
    cdk::sequence_node * _index;    // Índices (coordenadas)

public:
    tensor_index_node(int lineno, cdk::expression_node *tensor, cdk::sequence_node *index) :
        cdk::lvalue_node(lineno), _tensor(tensor), _index(index) {}

    cdk::expression_node* tensor() const { return _tensor; }

    cdk::sequence_node *index() { return _index; }


    void accept(basic_ast_visitor *sp, int level) {
        sp->do_tensor_index_node(this, level);
    }
};

} // udf
