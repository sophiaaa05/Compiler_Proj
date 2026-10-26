#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include ".auto/all_nodes.h"  // automatically generated

//---------------------------------------------------------------------------

void udf::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  // EMPTY
}
void udf::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << " size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++) {
    cdk::basic_node *n = node->node(i);
    if (n == NULL) break;
    n->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}
//---------------------------------------------------------------------------

void udf::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void udf::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

void udf::xml_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void udf::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void udf::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void udf::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void udf::xml_writer::do_evaluation_node(udf::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_input_node(udf::input_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
//---------------------------------------------------------------------------

void udf::xml_writer::do_for_node(udf::for_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("init", lvl + 2);
  node->init()->accept(this, lvl + 4);
  closeTag("init", lvl + 2);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("increment", lvl + 2);
  node->increment()->accept(this, lvl + 4);
  closeTag("increment", lvl + 2);
  openTag("instruction", lvl + 2);
  node->instruction()->accept(this, lvl + 4);
  closeTag("instruction", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void udf::xml_writer::do_if_node(udf::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_if_else_node(udf::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_block_node(udf::block_node* const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);

  openTag("declarations", lvl);
  if (node->declarations() != nullptr) {node->declarations()->accept(this, lvl + 4);};
  closeTag("declarations", lvl);

  openTag("instructions", lvl);
  if (node->instructions() != nullptr) {node->instructions()->accept(this, lvl + 4);};
  closeTag("instructions", lvl);

  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_break_node(udf::break_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_continue_node(udf::continue_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_function_call_node(udf::function_call_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->id() << "'>" << std::endl;
  ASSERT_SAFE_EXPRESSIONS;
  openTag("arguments", lvl);
  if (node->args()) node->args()->accept(this, lvl + 4);
  closeTag("arguments", lvl);
  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_function_decl_node(udf::function_decl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  reset_new_symbol();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "'>" << std::endl;

  openTag("arguments", lvl);
  if (node->arguments()) {
    _symtab.push();
    node->arguments()->accept(this, lvl + 4);
    _symtab.pop();
  }
  closeTag("arguments", lvl);
  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_function_def_node(udf::function_def_node * const node, int lvl) {

  ASSERT_SAFE_EXPRESSIONS;
  reset_new_symbol();

  _symtab.push();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() << "'>" << std::endl;

  openTag("arguments", lvl);
  if (node->arguments()) node->arguments()->accept(this, lvl + 4);
  closeTag("arguments", lvl);
  node->block()->accept(this, lvl + 2);
  closeTag(node, lvl);

  _symtab.pop();
}

/*****************************************************************************/

void udf::xml_writer::do_index_node(udf::index_node * const node, int lvl) {

  openTag(node, lvl);
  openTag("expr", lvl);
  node->expr()->accept(this, lvl + 2);
  closeTag("expr", lvl);
  openTag("index", lvl);
  node->index()->accept(this, lvl + 2);
  closeTag("index", lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_nullptr_node(udf::nullptr_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_objects_node(udf::objects_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_return_node(udf::return_node * const node, int lvl) {
  
  //TODO
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if (node->value()) node->value()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void udf::xml_writer::do_sizeof_node(udf::sizeof_node * const node, int lvl) {
  //TODO review
  ASSERT_SAFE_EXPRESSIONS
  openTag(node, lvl);
  node->arg()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_capacity_node(udf::tensor_capacity_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_dim_node(udf::tensor_dim_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("tensor", lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag("tensor", lvl);
  openTag("index", lvl);
  node->index()->accept(this, lvl + 2);
  closeTag("index", lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_dims_node(udf::tensor_dims_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_index_node(udf::tensor_index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("tensor", lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag("tensor", lvl);
  openTag("index", lvl);
  node->index()->accept(this, lvl + 2);
  closeTag("index", lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_node(udf::tensor_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->elements()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_rank_node(udf::tensor_rank_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void udf::xml_writer::do_tensor_reshape_node(udf::tensor_reshape_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("tensor", lvl);
  node->tensor()->accept(this, lvl + 2);
  closeTag("tensor", lvl);
  openTag("dimensions", lvl);
  node->dimensions()->accept(this, lvl + 2);
  closeTag("dimensions", lvl);
  closeTag(node, lvl);
}

void udf::xml_writer::do_contraction_node(udf::contraction_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_write_node(udf::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->args()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

/*****************************************************************************/

void udf::xml_writer::do_variable_declaration_node(udf::variable_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  reset_new_symbol();

  os() << std::string(lvl, ' ') << "<" << node->label() << " name='" << node->identifier() 
   << "'>" << std::endl;

  if (node->initializer() != nullptr ) {
    openTag("initializer", lvl);
    node->initializer()->accept(this, lvl + 4);
    closeTag("initializer", lvl);
  }
  closeTag(node, lvl);
}


void udf::xml_writer::do_address_node(udf::address_node * const node, int lvl) {
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
 }


