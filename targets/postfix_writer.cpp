#include "targets/type_checker.h"
#include <cdk/types/types.h>
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include ".auto/all_nodes.h"  // all_nodes.h is automatically generated
#include "targets/symbol.h"
#include <set>

#include "udf_parser.tab.h"

//---------------------------------------------------------------------------

void udf::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}

void udf::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void udf::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value()); // load number to the stack
  } else {
    _pf.SDOUBLE(node->value());    // double is on the DATA segment
  }
}

void udf::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();

}

void udf::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void udf::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}
//---------------------------------------------------------------------------

void udf::postfix_writer::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    if (node->node(i)) //
      node->node(i)->accept(this, lvl);
  }
}
//---------------------------------------------------------------------------




void udf::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value()); // integer literal is on the stack: push an integer
  } else {
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  }
}

void udf::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;
  /* generate the string literal */
  _pf.RODATA(); // strings are readonly DATA
  _pf.ALIGN(); // make sure the address is aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters
  if (_function) {
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void udf::postfix_writer::do_unary_minus_node(cdk::unary_minus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

void udf::postfix_writer::do_unary_plus_node(cdk::unary_plus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
}

//---------------------------------------------------------------------------

void udf::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.ADD();
}

void udf::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}

void udf::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}

void udf::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}

void udf::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void udf::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}

void udf::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}

void udf::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}

void udf::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}

void udf::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}

void udf::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void udf::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (!symbol) {
    std::cerr << "CODEGEN ERROR: variable '" << node->name() << "' not found in symbol table.\n";
    exit(1);
  }
  if (symbol->offset() != 0) {
    // Local variable or parameter: load address from stack frame
    _pf.LOCAL(symbol->offset());
  } else {
    // Global variable
    _pf.ADDR(node->name());
  }
}

void udf::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
 node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  } else {
    // integers, pointers, and strings
    _pf.LDINT();
  }
}

void udf::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl); // value to assign
  _pf.DUP32();
  node->lvalue()->accept(this, lvl); // address to assign to
  _pf.STINT(); // store value at address
}

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void udf::postfix_writer::do_evaluation_node(udf::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  if (isVoid(node->argument()->type())) {
    
  } 
  else if(isString(node->argument()->type())) {
    _pf.TRASH(4);
  } 
  else if(isDouble(node->argument()->type())){
    _pf.TRASH(8);
  }
  else if(isPointer(node->argument()->type())){
    _pf.TRASH(4);
  }
  else if(isInt(node->argument()->type())){
    _pf.TRASH(4);
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}


//---------------------------------------------------------------------------

void udf::postfix_writer::do_input_node(udf::input_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;


  if (_lvalueType == cdk::TYPE_DOUBLE) {
    _functions_to_declare.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } else if (_lvalueType == cdk::TYPE_INT) {
    _functions_to_declare.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  } else {
    std::cerr << "FATAL: " << node->lineno() << ": cannot read type" << std::endl;
    return;
  }
}

//---------------------------------------------------------------------------

void udf::postfix_writer::do_for_node(udf::for_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _forIni.push(++_lbl); // after init, before body
  _forStep.push(++_lbl); // after instruction
  _forEnd.push(++_lbl); // after for

  os() << "        ;; FOR initialize" << std::endl;
  _symtab.push();
  _inForInit = true;

  // Initialization (may be null)
  if (node->init())
    node->init()->accept(this, lvl + 2);

  os() << "        ;; FOR test" << std::endl;
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forIni.top()));

  // Condition (may be null or empty)
  if (node->condition()) {
    if (node->condition()->size() > 0) {
      node->condition()->node(0)->accept(this, lvl + 2);
      for (size_t i = 1; i < node->condition()->size(); ++i) {
        node->condition()->node(i)->accept(this, lvl + 2);
        _pf.AND();
      }
    } else {
      _pf.INT(1); // empty condition: always true
    }
    _pf.JZ(mklbl(_forEnd.top()));
  } else {
    _pf.INT(1); // no condition: always true
  }

  os() << "        ;; FOR instruction" << std::endl;
  if (node->instruction())
    node->instruction()->accept(this, lvl + 2);

  os() << "        ;; FOR increment" << std::endl;
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forStep.top()));
  if (node->increment() && node->increment()->size() > 0) {
    for (size_t i = 0; i < node->increment()->size(); ++i) {
      node->increment()->node(i)->accept(this, lvl + 2);
      _pf.TRASH(4); 
    }
  }

  os() << "        ;; FOR jump to test" << std::endl;
  _pf.JMP(mklbl(_forIni.top()));

  os() << "        ;; FOR end" << std::endl;
  _pf.ALIGN();
  _pf.LABEL(mklbl(_forEnd.top()));

  _inForInit = false;
  _symtab.pop();

  _forIni.pop();
  _forStep.pop();
  _forEnd.pop();
}

//---------------------------------------------------------------------------

void udf::postfix_writer::do_if_node(udf::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void udf::postfix_writer::do_if_else_node(udf::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

void udf::postfix_writer::do_block_node(udf::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
   _symtab.push(); 
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();

}

void udf::postfix_writer::do_continue_node(udf::continue_node * const node, int lvl) {
  if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forStep.top())); // jump to next cycle
  } else
    error(node->lineno(), "'restart' outside 'for'");
}

void udf::postfix_writer::do_break_node(udf::break_node * const node, int lvl) {
    if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forEnd.top())); // jump to for end
  } else
    error(node->lineno(), "'break' outside 'for'");
}



void udf::postfix_writer::do_function_call_node(udf::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto symbol = _symtab.find(node->id());

  if (!symbol) {
    // If the function is not defined, mark it for extern declaration
    _functions_to_declare.insert(node->id());
  }

  size_t argsSize = 0;
  if (node->args()->size() > 0) {
    for (int ax = node->args()->size() - 1; ax >= 0; ax--) {

      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->args()->node(ax));
      arg->accept(this, lvl + 2);
      
      if (symbol->argument_is_typed(ax, cdk::TYPE_DOUBLE) && arg->is_typed(cdk::TYPE_INT)) {
        _pf.I2D();
      }
      argsSize += symbol->argument_size(ax);
    }
  }
  _pf.CALL(node->id());
  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (symbol->is_typed(cdk::TYPE_INT) || symbol->is_typed(cdk::TYPE_POINTER) || symbol->is_typed(cdk::TYPE_STRING)) {
    _pf.LDFVAL32();
  } else if (symbol->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.LDFVAL64();
  } else if (symbol->is_typed(cdk::TYPE_VOID)) {
    
  } else {
    error(node->lineno(), "function call returns unknown type");
  }
}

void udf::postfix_writer::do_function_decl_node(udf::function_decl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot declare function in body or in args");
    return;
  }

  if (!new_symbol()) return;

  auto function = new_symbol();
  _functions_to_declare.insert(function->name());
  reset_new_symbol();
}

void udf::postfix_writer::do_function_def_node(udf::function_def_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (_inFunctionBody || _inFunctionArgs) {
    error(node->lineno(), "cannot define function in body or in arguments");
    return;
  }

  _function = new_symbol();
  if (!_function) {
    error(node->lineno(), "internal error: no symbol for function definition");
    return;
  }
  _functions_to_declare.erase(_function->name());
  reset_new_symbol();

  _currentBodyRetLabel = mklbl(++_lbl);

  // --- ARGUMENTS ---
  _offset = 8; // arguments start at +8 (assuming 4 bytes for return address, 4 for old base pointer)
  _symtab.push(); // scope for arguments

  if (node->arguments() && node->arguments()->size() > 0) {
    _inFunctionArgs = true;
    for (size_t ix = 0; ix < node->arguments()->size(); ix++) {
      auto vardecl = dynamic_cast<udf::variable_declaration_node*>(node->arguments()->node(ix));
      if (!vardecl) continue;
      auto symbol = std::make_shared<udf::symbol>(
        vardecl->type(), vardecl->identifier(), _offset, false, vardecl->is_public(), vardecl->is_forward(), false, false
      );
      _symtab.insert(vardecl->identifier(), symbol);
      _offset += vardecl->type()->size();
    }
    _inFunctionArgs = false;
  }

  // --- CODEGEN ---
  _pf.TEXT();
  _pf.ALIGN();
  std::string label = _function->name();
  if (node->is_public()) _pf.GLOBAL(label, _pf.FUNC());
  _pf.LABEL(label);

  // --- FRAME SIZE ---
  frame_size_calculator lsc(_compiler, _symtab,_function);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize());
  std::cout << " frame size: " << lsc.localsize() << std::endl;

  if (label == "_main") { 
    _functions_to_declare.insert("mem_init");
    _pf.CALL("mem_init");
  }

  // --- LOCALS ---
  _offset = 0; // locals go below base pointer

  _inFunctionBody = true;
  os() << "        ;; before body " << std::endl;
  node->block()->accept(this, lvl + 4); // block has its own scope
  os() << "        ;; after body " << std::endl;
  _inFunctionBody = false;
  _returnSeen = false;

  _pf.LABEL(_currentBodyRetLabel);
  _pf.LEAVE();
  _pf.RET();

  _symtab.pop();

  if (node->identifier() == "udf") {
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }
}



void udf::postfix_writer::do_index_node(udf::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // Evaluate the pointer expression
  node->expr()->accept(this, lvl);
  // Evaluate the index
  node->index()->accept(this, lvl);
  // Multiply index by 4 (size of int)
  int size = 4;
  auto ref_type = std::dynamic_pointer_cast<cdk::reference_type>(node->type());
  if (ref_type && ref_type->referenced())
    size = ref_type->referenced()->size();
  _pf.INT(size);
  _pf.MUL();

  // Add to pointer
  _pf.ADD();
}

void udf::postfix_writer::do_nullptr_node(udf::nullptr_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
   // a pointer is a 32-bit integer
  if (_inFunctionBody) {
    _pf.INT(0);
  } else {
    _pf.SINT(0);
  }
}

void udf::postfix_writer::do_objects_node(udf::objects_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  // Evaluate the number of objects
  node->positions()->accept(this, lvl);
  // Multiply by the size of the base type (here, int = 4)
  int size = 4;
  auto ref_type = std::dynamic_pointer_cast<cdk::reference_type>(node->type());
  if (ref_type && ref_type->referenced())
    size = ref_type->referenced()->size();
  _pf.INT(size);
  _pf.MUL();

  // Reserve space on the stack
  _pf.ALLOC(); // ALLOC: allocate n bytes, returns pointer in eax
}

void udf::postfix_writer::do_return_node(udf::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  // Only valid inside a function
  if (!_function) {
    std::cerr << node->lineno() << ": 'return' outside of function" << std::endl;
    return;
  }

  if (_function->type()->name() == cdk::TYPE_VOID) {
    // Void function: should not return a value
    if (node->value()) {
      std::cerr << node->lineno() << ": cannot return a value from a void function" << std::endl;
      return;
    }
    // No value to store, just jump to function epilogue
  } else {
    // Non-void function: must return a value
    if (!node->value()) {
      std::cerr << node->lineno() << ": missing return value in non-void function" << std::endl;
      return;
    }
    node->value()->accept(this, lvl + 2);

    if (_function->type()->name() == cdk::TYPE_INT ||
        _function->type()->name() == cdk::TYPE_STRING ||
        _function->type()->name() == cdk::TYPE_POINTER) {
      _pf.STFVAL32();
    } else if (_function->type()->name() == cdk::TYPE_DOUBLE) {
      if (node->value()->type()->name() == cdk::TYPE_INT) _pf.I2D();
      _pf.STFVAL64();
    } else {
      std::cerr << node->lineno() << ": should not happen: unknown return type" << std::endl;
    }
  }

  _pf.JMP(_currentBodyRetLabel);
  _returnSeen = true;
}

void udf::postfix_writer::do_sizeof_node(udf::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.INT(node->arg()->type()->size());
}

void udf::postfix_writer::do_tensor_capacity_node(udf::tensor_capacity_node * const node, int lvl) {
  //TODO
}

void udf::postfix_writer::do_tensor_dim_node(udf::tensor_dim_node * const node, int lvl) {
  //TODO
}

void udf::postfix_writer::do_tensor_dims_node(udf::tensor_dims_node * const node, int lvl) {
  //TODO
}

void udf::postfix_writer::do_tensor_index_node(udf::tensor_index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  // Evaluate the tensor expression (should leave the base address on the stack)
  node->tensor()->accept(this, lvl);

  // For now, assume a flat 1D tensor (pointer to int)
  // Evaluate the index expression(s)
  if (node->index() && node->index()->size() > 0) {
    // Only support 1D for now: use the first index
    auto idx = dynamic_cast<cdk::expression_node*>(node->index()->node(0));
    if (idx) {
      idx->accept(this, lvl);
    } else {
      _pf.INT(0); // fallback to 0 if not an expression
    }
  } else {
    _pf.INT(0); // default index 0
  }

  // Multiply index by 4 (size of int)
  _pf.INT(4);
  _pf.MUL();

  // Add to base address
  _pf.ADD();
}

void udf::postfix_writer::do_tensor_node(udf::tensor_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  size_t n = node->elements() ? node->elements()->size() : 0;
  if (n == 0) {
    // Empty tensor: just allocate 0 bytes
    _pf.INT(0);
    _pf.ALLOC();
    return;
  }

  // Allocate space for n ints
  _pf.INT(n * 4);
  _pf.ALLOC();

  // Duplicate the address to keep it for later
  _pf.DUP32();

  // Initialize each element
  for (size_t i = 0; i < n; ++i) {
    // Duplicate the base address
    _pf.DUP32();
    // Compute offset
    _pf.INT(i * 4);
    _pf.ADD();
    // Evaluate element value
    auto elem = dynamic_cast<cdk::expression_node*>(node->elements()->node(i));
    if (elem) elem->accept(this, lvl + 2);
    else _pf.INT(0); // default to 0 if not an expression
    // Store value
    _pf.STINT();
  }
  // After loop, leave only the base address on the stack
  _pf.TRASH(4 * n);
}

void udf::postfix_writer::do_tensor_reshape_node(udf::tensor_reshape_node * const node, int lvl) {
  //TODO
}

void udf::postfix_writer::do_tensor_rank_node(udf::tensor_rank_node * const node, int lvl) {
  //TODO
}

void udf::postfix_writer::do_write_node(udf::write_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto args = node->args()->nodes();

  for (auto a : args) {
      auto arg = dynamic_cast<cdk::expression_node*>(a);
      arg->accept(this, lvl + 2);

      switch (arg->type()->name()) {
      case cdk::TYPE_INT:
          _functions_to_declare.insert("printi");
          _pf.CALL("printi");
          _pf.TRASH(4);
          break;
      case cdk::TYPE_STRING:
          _functions_to_declare.insert("prints");
          _pf.CALL("prints");
          _pf.TRASH(4);
          break;
      case cdk::TYPE_DOUBLE:
          _functions_to_declare.insert("printd");
          _pf.CALL("printd");
          _pf.TRASH(8);
          break;
      default:
          std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
          exit(1);
      }
  }

  // Always print newline for writeln
  if (node->is_end()) {
      _functions_to_declare.insert("println");
      _pf.CALL("println");
  }
}


void udf::postfix_writer::do_variable_declaration_node(udf::variable_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifier();
  int offset = 0, typesize = node->type()->size();

  if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } else if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } else {
    offset = 0; // global variable
  }

  auto symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  // --- TENSOR SUPPORT ---
 bool is_tensor = node->is_typed(cdk::TYPE_POINTER)
  && std::dynamic_pointer_cast<cdk::reference_type>(node->type())
  && std::dynamic_pointer_cast<cdk::reference_type>(node->type())->referenced()->name() == cdk::TYPE_INT;
  if (_inFunctionBody) {
    if (node->initializer()) {
      node->initializer()->accept(this, lvl);
      _pf.LOCAL(symbol->offset());
      _pf.STINT();
    } else if (is_tensor) {
      // Uninitialized tensor: store null pointer
      _pf.INT(0);
      _pf.LOCAL(symbol->offset());
      _pf.STINT();
    }
  } else {
    // Global variable
    if (!_function) {
      if (node->initializer() == nullptr) {
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(id);
        _pf.SALLOC(typesize);
      } else {
        if (is_tensor) {
          // Global tensor with initializer
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);
          node->initializer()->accept(this, lvl);
          _pf.STINT();
        } else if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_POINTER)) {
          _pf.DATA();
          _pf.ALIGN();
          _pf.LABEL(id);
          if (node->is_typed(cdk::TYPE_INT)) {
            node->initializer()->accept(this, lvl);
          } else if (node->is_typed(cdk::TYPE_POINTER)) {
            node->initializer()->accept(this, lvl);
          } else if (node->is_typed(cdk::TYPE_DOUBLE)) {
            if (node->initializer()->is_typed(cdk::TYPE_DOUBLE)) {
              node->initializer()->accept(this, lvl);
            } else if (node->initializer()->is_typed(cdk::TYPE_INT)) {
              cdk::integer_node *dclini = dynamic_cast<cdk::integer_node*>(node->initializer());
              cdk::double_node ddi(dclini->lineno(), dclini->value());
              ddi.accept(this, lvl);
            } else {
              std::cerr << node->lineno() << ": '" << id << "' has bad initializer for real value\n";
              _errors = true;
            }
          }
        } else if (node->is_typed(cdk::TYPE_STRING)) {
          int litlbl;
          _pf.RODATA();
          _pf.ALIGN();
          _pf.LABEL(mklbl(litlbl = ++_lbl));
          _pf.SSTRING(dynamic_cast<cdk::string_node*>(node->initializer())->value());
          _pf.ALIGN();
          _pf.LABEL(id);
          _pf.SADDR(mklbl(litlbl));
        } else {
          std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
          _errors = true;
        }
      }
    }
  }
}


void udf::postfix_writer::do_address_node(udf::address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  // since the argument is an lvalue, it is already an address
  node->lvalue()->accept(this, lvl + 2);
}

void udf::postfix_writer::do_contraction_node(udf::contraction_node * const node, int lvl) {
   //TODO
}