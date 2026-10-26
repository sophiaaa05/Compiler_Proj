
#include <cdk/types/types.h>
#include "targets/type_checker.h"
#include ".auto/all_nodes.h"  // automatically generated
#include <cdk/types/primitive_type.h>

#include "udf_parser.tab.h"
#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------



void udf::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    if (node->node(i)) // <-- check for nullptr
      node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------



void udf::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void udf::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void udf::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
}

void udf::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);

  if(!isInt(node->argument()->type())){ 
    throw std::string("wrong type in not operator");
  }  
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT)); 

}
void udf::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}
void udf::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  do_BooleanLogicalExpression(node, lvl);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------


void udf::type_checker::do_BooleanLogicalExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in binary expression");
  }

  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (!node->argument()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in argument of unary expression");

  // in udf, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_unary_minus_node(cdk::unary_minus_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void udf::type_checker::do_unary_plus_node(cdk::unary_plus_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void udf::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (!node->left()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (!node->right()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of binary expression");


  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void udf::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol) {
    node->type(symbol->type());
  } else {
    throw std::string("undeclared variable '" + id + "'");
  }
}

void udf::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void udf::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
  } catch (const std::string &id) {
    auto symbol = udf::make_symbol(cdk::primitive_type::create(4, cdk::TYPE_INT), id, 0, false, false, false, false, false);    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);
  }

  if (!node->lvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in left argument of assignment expression");

  node->rvalue()->accept(this, lvl + 2);
  if (!node->rvalue()->is_typed(cdk::TYPE_INT)) throw std::string("wrong type in right argument of assignment expression");

  // in udf, expressions are always int
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

//---------------------------------------------------------------------------



void udf::type_checker::do_evaluation_node(udf::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void udf::type_checker::do_input_node(udf::input_node *const node, int lvl) {
  node->type(cdk::primitive_type::create(0, cdk::TYPE_UNSPEC));
}
//---------------------------------------------------------------------------

void udf::type_checker::do_for_node(udf::for_node *const node, int lvl) {
  if (node->init())
    node->init()->accept(this, lvl + 4);
  if (node->condition())
    node->condition()->accept(this, lvl + 4);
  if (node->increment())
    node->increment()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void udf::type_checker::do_if_node(udf::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void udf::type_checker::do_if_else_node(udf::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void udf::type_checker::do_block_node(udf::block_node *const node, int lvl) {
  node->declarations()->accept(this, lvl + 2);
  node->instructions()->accept(this, lvl + 2);
}

void udf::type_checker::do_break_node(udf::break_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_continue_node(udf::continue_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_function_call_node(udf::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;
  

  const std::string &id = node->id();
  auto symbol = _symtab.find(id);
  if (!symbol) throw std::string("symbol '" + id + "' is undeclared.");
  if (!symbol->is_function()) throw std::string("symbol '" + id + "' is not a function.");

  if (symbol->is_typed(cdk::TYPE_STRUCT)) {
    // declare return variable for passing to function call
    const std::string return_var_name = "$return_" + id;
    auto return_symbol = udf::make_symbol(symbol->type(), return_var_name, 0, false, false, false, false, false);
    _symtab.insert(return_var_name, return_symbol); // ignore if already declared
  }

  node->type(symbol->type());

  if (node->args()->size() == symbol->number_of_arguments()) {
    node->args()->accept(this, lvl + 4);
    for (size_t ax = 0; ax < node->args()->size(); ax++) {
      auto arg = dynamic_cast<cdk::expression_node*>(node->args()->node(ax));
      if (!arg) continue;
      if (arg->type() == symbol->argument_type(ax)) continue;
      if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && arg->is_typed(cdk::TYPE_INT)) continue;
      throw std::string("type mismatch for argument " + std::to_string(ax + 1) + " of '" + id + "'.");
    }
  } else {
    throw std::string(
      "number of arguments in call (" + std::to_string(node->args()->size()) + ") must match declaration ("
      + std::to_string(symbol->number_of_arguments()) + ").");
  }
}

void udf::type_checker::do_function_decl_node(udf::function_decl_node * const node, int lvl) {
  std::string id;

  // Rename if needed
  if ( node->identifier() == "udf") {
    id = "_main";
  } else if (node->identifier() == "_main") {
    id = "._main";
  }else
    id  = node->identifier()  ;

  std::shared_ptr<udf::symbol> symbol = _symtab.find(id);
  if (!symbol) {
    std::vector<std::shared_ptr<cdk::basic_type>> params;
    // Use arguments() instead of parameters()
    auto args_node = node->arguments();
    for (size_t i = 0; i < args_node->size(); ++i) {
      auto vardecl = dynamic_cast<udf::variable_declaration_node*>(args_node->node(i));
      params.push_back(vardecl->type());
    }

    symbol = std::make_shared<udf::symbol>(
      node->type(), id, -node->type()->size(), true, node->is_public(), node->is_forward(),
      true, false, params
    );
    _symtab.insert(id, symbol);
  } else {
    throw std::string(id + " redeclared");
  }
}

void udf::type_checker::do_function_def_node(udf::function_def_node * const node, int lvl) {
  std::string id;
  if (node->identifier() == "udf")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  std::shared_ptr<udf::symbol> symbol = _symtab.find(id);
  if (!symbol) {
    std::vector<std::shared_ptr<cdk::basic_type>> params;
    auto args_node = node->arguments();
    for (size_t i = 0; i < args_node->size(); ++i) {
      auto vardecl = dynamic_cast<udf::variable_declaration_node*>(args_node->node(i));
      params.push_back(vardecl->type());
    }

    symbol = std::make_shared<udf::symbol>(
      node->type(), id, -node->type()->size(), true, node->is_public(), node->is_forward(),
      true, false, params
    );
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol); 
  } else {
    throw std::string(id + " redeclared");
  }
}

void udf::type_checker::do_index_node(udf::index_node * const node, int lvl) {
  ASSERT_UNSPEC;
  std::shared_ptr < cdk::reference_type > btype;

  if (node->expr()) {
    node->expr()->accept(this, lvl + 2);
    btype = cdk::reference_type::cast(node->expr()->type());
    if (!node->expr()->is_typed(cdk::TYPE_POINTER)) throw std::string("pointer expression expected in index left-value");
  } else {
    btype = cdk::reference_type::cast(_function->type());
    if (!_function->is_typed(cdk::TYPE_POINTER)) throw std::string("return pointer expression expected in index left-value");
  }

  node->index()->accept(this, lvl + 2);
  if (!node->index()->is_typed(cdk::TYPE_INT)) throw std::string("integer expression expected in left-value index");

  node->type(btype->referenced());
}

void udf::type_checker::do_nullptr_node(udf::nullptr_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::reference_type::create(4, nullptr));
}

void udf::type_checker::do_objects_node(udf::objects_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->positions()->accept(this, lvl + 2);
  if (!node->positions()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in allocation expression");
  }
  auto mytype = cdk::reference_type::create(4, cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
  node->type(mytype);
}

void udf::type_checker::do_return_node(udf::return_node * const node, int lvl) {
  if (node->value()) {
    
    node->value()->accept(this, lvl + 2);

    node->type(node->value()->type());
  } else {
    // No return value: treat as void
    node->type(cdk::primitive_type::create(0, cdk::TYPE_VOID));
  }
}

void udf::type_checker::do_sizeof_node(udf::sizeof_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->arg()->accept(this, lvl + 2);
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_tensor_capacity_node(udf::tensor_capacity_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_tensor_dim_node(udf::tensor_dim_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_tensor_dims_node(udf::tensor_dims_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_tensor_index_node(udf::tensor_index_node * const node, int lvl) {
  ASSERT_UNSPEC;
  // Type check the tensor expression
  node->tensor()->accept(this, lvl + 2);

  // The tensor expression must be a pointer (to int for now)
  auto tensor_type = std::dynamic_pointer_cast<cdk::reference_type>(node->tensor()->type());
  if (!tensor_type || tensor_type->referenced()->name() != cdk::TYPE_INT) {
    throw std::string("tensor_index_node: tensor expression must be a pointer to int");
  }

  // Type check all indices (should be int)
  if (node->index()) {
    node->index()->accept(this, lvl + 2);
    for (size_t i = 0; i < node->index()->size(); ++i) {
      auto idx = dynamic_cast<cdk::expression_node*>(node->index()->node(i));
      if (!idx || !idx->is_typed(cdk::TYPE_INT)) {
        throw std::string("tensor_index_node: all indices must be int");
      }
    }
  }

  // The result of indexing a tensor is an int lvalue
  node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
}

void udf::type_checker::do_tensor_node(udf::tensor_node * const node, int lvl) {
  ASSERT_UNSPEC;
  // Visit all elements to type-check them
  if (node->elements()) {
    node->elements()->accept(this, lvl + 2);
    // Optionally, check that all elements have the same type
    for (size_t i = 0; i < node->elements()->size(); ++i) {
      auto elem = dynamic_cast<cdk::expression_node*>(node->elements()->node(i));
      if (elem && !elem->is_typed(cdk::TYPE_INT)) {
        throw std::string("tensor elements must be int (for now)");
      }
    }
  }
  // Set type as pointer to int (adjust as needed)
  node->type(cdk::reference_type::create(4, cdk::primitive_type::create(4, cdk::TYPE_INT)));
}

void udf::type_checker::do_tensor_rank_node(udf::tensor_rank_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_tensor_reshape_node(udf::tensor_reshape_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_contraction_node(udf::contraction_node * const node, int lvl) {
  //TODO
}

void udf::type_checker::do_write_node(udf::write_node * const node, int lvl) {
  node->args()->accept(this, lvl + 2);
}

void udf::type_checker::do_variable_declaration_node(udf::variable_declaration_node * const node, int lvl) {
;
  if (node->initializer() != nullptr) {
    node->initializer()->accept(this, lvl + 2);


    if (!node->type() || node->is_typed(cdk::TYPE_UNSPEC)) { // auto type
      if (node->initializer()->is_typed(cdk::TYPE_INT)) {
        node->type(cdk::primitive_type::create(4, cdk::TYPE_INT));
      } else if (node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
        node->type(cdk::primitive_type::create(8, cdk::TYPE_DOUBLE));
      } else if (node->initializer()->is_typed(cdk::TYPE_STRING)) {
        node->type(cdk::primitive_type::create(4, cdk::TYPE_STRING));
      } else if (node->initializer()->is_typed(cdk::TYPE_POINTER)) {
        node->type(cdk::reference_type::create(4, cdk::primitive_type::create(0, cdk::TYPE_UNSPEC)));
      } else {
        throw std::string("cannot infer type from initializer for auto variable");
      }
    }
  }

  const std::string &id = node->identifier();
  auto symbol = std::make_shared<udf::symbol>(
    node->type(), id, 0, false, node->is_public(), node->is_forward(), false, node->initializer() != nullptr
  );
  if (_symtab.insert(id, symbol)) {
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
  } else {
    throw std::string("variable '" + id + "' redeclared");
  }
}

void udf::type_checker::do_address_node(udf::address_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(cdk::reference_type::create(4, node->lvalue()->type()));
}

