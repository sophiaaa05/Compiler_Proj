#pragma once

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace udf {

    /**
     * Class for describing block nodes.
     */
    class block_node: public cdk::basic_node {
        cdk::sequence_node* _declarations;
        cdk::sequence_node* _instructions;


    public:
        inline block_node(int lineno, cdk::sequence_node* decl, cdk::sequence_node* inst) :
            cdk::basic_node(lineno), _declarations(decl), _instructions(inst) {}

        cdk::sequence_node *declarations() { return _declarations; }
        cdk::sequence_node *instructions() { return _instructions; }


    void accept(basic_ast_visitor* sp, int level) override {
        sp->do_block_node(this, level);
    }
    
    };

}; // udf