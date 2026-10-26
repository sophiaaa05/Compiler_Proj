#include <cdk/ast/unary_operation_node.h>

namespace udf {

   class address_node: public cdk::expression_node {
     cdk::lvalue_node *_lvalue;

   public:
     address_node(int lineno, cdk::lvalue_node *argument) :
         cdk::expression_node(lineno), _lvalue(argument) {
     }

   public:
     cdk::lvalue_node* lvalue() {
       return _lvalue;
     }

   public:
     void accept(basic_ast_visitor *sp, int level) {
       sp->do_address_node(this, level);
     }

   };

 } // udf

