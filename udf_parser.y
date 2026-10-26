%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include ".auto/all_nodes.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;          /* integer value */
  std::string          *s;          /* symbol name or string literal */
  double d; /* double value */
  cdk::basic_node      *node;       /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  udf::block_node      *block;
  udf::tensor_node     *tensor;
  std::vector<size_t> *dims;
};

%token <i> tINTEGER
%token<d> tREAL
%token <s> tIDENTIFIER tSTRING
%token tFOR tIF tELSEIF tWRITE tWRITELN tPUBLIC tTYPEINT tTYPESTRING tTYPEREAL tAUTO tSIZEOF tTYPE_POINTER tTYPETENSOR
%token tBLOCK tRETURN tCONTINUE tDOUBLE tBREAK tFORWARD tDO tVOID
%token tOBJECTS
%token tDIM tDIMS tRANK tCAPACITY  tRESHAPE
%token<expression> tNULLPTR


%nonassoc tIFX
%nonassoc tELSEIF
%nonassoc tELSE

%right '='
%left tOR
%left tAND
%nonassoc '~'
%left tNE tEQ
%left '<' tLE tGE '>'
%left '+' '-'
%left '*' '/' '%' 
%nonassoc tUNARY
%nonassoc '(' '[' '@' 

%nonassoc tDIM tDIMS tRANK tCAPACITY tCONTRACT tRESHAPE     


%type <node> stmt file var decl  ifs func_decl func_def arg fordec var_decl return
%type <sequence> list  stmts args call_args fordecs init var_decls exprs
%type <expression> expr integer real
%type <lvalue> lval
%type <type> type void_type
%type<s> string
%type <dims> dimensions


%type <block> block

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file : /* empty */  { compiler->ast($$ = new cdk::sequence_node(LINE)); }
     |list           { compiler->ast($$ = $1); }
     ;


list : decl      { $$ = new cdk::sequence_node(LINE, $1); }
     | list decl { $$ = new cdk::sequence_node(LINE, $2, $1); }
     ;


decl : var ';'         { $$ = $1; }
     | func_decl       { $$ = $1; }
     | func_def        { $$ = $1; }
     ;

var_decl : var ';' { $$ = $1; }
         ;

var_decls : var_decl           { $$ = new cdk::sequence_node(LINE, $1); }
          | var_decls var_decl { $$ = new cdk::sequence_node(LINE, $2, $1); }
          ;



var  : type tIDENTIFIER                                   { $$ = new udf::variable_declaration_node(LINE, false, false, $1, *$2, nullptr); delete $2; }
     | type tIDENTIFIER '=' expr                          { $$ = new udf::variable_declaration_node(LINE, false, false, $1, *$2, $4); delete $2; }
     | tPUBLIC type tIDENTIFIER                           { $$ = new udf::variable_declaration_node(LINE, true, false, $2, *$3, nullptr); delete $3; }
     | tPUBLIC type tIDENTIFIER '=' expr                  { $$ = new udf::variable_declaration_node(LINE, true, false, $2, *$3, $5); delete $3; }
     | tFORWARD type tIDENTIFIER                          { $$ = new udf::variable_declaration_node(LINE, false, true, $2, *$3, nullptr); delete $3; }
     | tPUBLIC  tAUTO tIDENTIFIER '=' expr                { $$ = new udf::variable_declaration_node(LINE, true, false, nullptr, *$3, $5); delete $3; }
     |          tAUTO tIDENTIFIER '=' expr                { $$ = new udf::variable_declaration_node(LINE, false, false, nullptr, *$2, $4); delete $2; }
     
     ;

func_decl:  tFORWARD type  tIDENTIFIER '(' args ')'       { $$ = new udf::function_decl_node(LINE, false, true,  $2, *$3, $5); }
          | tPUBLIC  type  tIDENTIFIER '(' args ')'       { $$ = new udf::function_decl_node(LINE, true, false,  $2, *$3, $5); }
          |          tAUTO tIDENTIFIER '(' args ')'       { $$ = new udf::function_decl_node(LINE, false, false, nullptr, *$2, $4); }
          | tFORWARD tAUTO tIDENTIFIER '(' args ')'       { $$ = new udf::function_decl_node(LINE, false, true,  nullptr, *$3, $5); }
          | tPUBLIC  tAUTO tIDENTIFIER '(' args ')'       { $$ = new udf::function_decl_node(LINE, true, false,  nullptr, *$3, $5); }
          |          type  tIDENTIFIER '(' args ')'       { $$ = new udf::function_decl_node(LINE, false, false, $1, *$2, $4); }
          |          void_type  tIDENTIFIER '(' args ')'  { $$ = new udf::function_decl_node(LINE, false, false, $1, *$2, $4); }
          | tFORWARD void_type  tIDENTIFIER '(' args ')'  { $$ = new udf::function_decl_node(LINE, false,true,  $2, *$3, $5); }
          | tPUBLIC void_type  tIDENTIFIER '(' args ')'   { $$ = new udf::function_decl_node(LINE, true,false,  $2, *$3, $5); }
          ;

func_def
     :         type  tIDENTIFIER '(' args ')' block       { $$ = new udf::function_def_node(LINE, false, false, $1, *$2, $4, $6); }
     | tPUBLIC type  tIDENTIFIER '(' args ')' block       { $$ = new udf::function_def_node(LINE, true, false,  $2, *$3, $5, $7); }
     |         tAUTO tIDENTIFIER '(' args ')' block       { $$ = new udf::function_def_node(LINE, false, false, nullptr, *$2, $4, $6); }
     | tPUBLIC tAUTO tIDENTIFIER '(' args ')' block       { $$ = new udf::function_def_node(LINE, true, false,  nullptr, *$3, $5, $7); }
     |         void_type  tIDENTIFIER '(' args ')' block  { $$ = new udf::function_def_node(LINE, false,false, $1, *$2, $4, $6); }
     | tPUBLIC void_type  tIDENTIFIER '(' args ')' block  { $$ = new udf::function_def_node(LINE, true,false,  $2, *$3, $5, $7); }
     ;
     ;

args
     : /*empty*/    { $$ = new cdk::sequence_node(LINE); }
     | arg          { $$ = new cdk::sequence_node(LINE, $1); }
     | args ',' arg { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;

arg : type tIDENTIFIER { $$ = new udf::variable_declaration_node(LINE, false, false, $1, *$2, nullptr); delete $2; }


//
fordec    :type tIDENTIFIER '=' expr { $$ = new udf::variable_declaration_node(LINE, false,false,  $1, *$2, $4); }
          ;
              
fordecs   :             fordec       { $$ = new cdk::sequence_node(LINE, $1);     }
          | fordecs ',' fordec       { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;

init : /**/     { $$ = new cdk::sequence_node(LINE, nullptr); }
     | fordecs  { $$ = $1; }
     ;


stmts
    : stmt           { $$ = new cdk::sequence_node(LINE, $1); }
    | stmts stmt     { $$ = new cdk::sequence_node(LINE, $2, $1); }
    ;


block
    : '{' var_decls stmts '}' { $$ = new udf::block_node(LINE, $2, $3); }
    | '{' var_decls '}'       { $$ = new udf::block_node(LINE, $2, new cdk::sequence_node(LINE)); }
    | '{' stmts '}'           { $$ = new udf::block_node(LINE, new cdk::sequence_node(LINE), $2); }
    | '{' '}'                 { $$ = new udf::block_node(LINE, new cdk::sequence_node(LINE), new cdk::sequence_node(LINE)); }
    ;


stmt : expr ';'                                                          { $$ = new udf::evaluation_node(LINE, $1); }
     | tWRITE call_args ';'                                                   { $$ = new udf::write_node(LINE, $2, false); }
     | tWRITELN call_args ';'                                                 { $$ = new udf::write_node(LINE, $2, true); }
     | tFOR '(' init ';' call_args ';' call_args ')' stmt                { $$ = new udf::for_node(LINE, $3, $5, $7, $9); }
     | block                                                             { $$ = $1; }     
     | return                                                            { $$ = $1; } 
     | tIFX '(' expr ')' stmt %prec tIFX                                 { $$ = new udf::if_node(LINE, $3, $5); }
     | tIFX '(' expr ')' stmt ifs                                        { $$ = new udf::if_else_node(LINE, $3, $5, $6); }
     | tBREAK                                                            { $$ = new udf::break_node(LINE);}
     | tCONTINUE                                                         { $$ = new udf::continue_node(LINE);}
     ;

ifs
     : tELSEIF '(' expr ')' stmt ifs %prec tIFX   { $$ = new udf::if_else_node(LINE, $3, $5, $6); }
     | tELSE stmt                                 { $$ = $2; }
     ;


lval : tIDENTIFIER                                      { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
     | expr      '[' expr ']'                           { $$ = new udf::index_node(LINE, $1, $3); }
     | expr '@'  '('  exprs ')'                        { $$ = new udf::tensor_index_node(LINE, $1, $4); }
     ;

expr : integer                               { $$ = $1; }
     | real                                  { $$ = $1; }
     | string                                { $$ = new cdk::string_node(LINE, $1); }
     | tNULLPTR                              { $$ = new udf::nullptr_node(LINE); }
     | expr '+' expr                         { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr                         { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr                         { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr                         { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr                         { $$ = new cdk::mod_node(LINE, $1, $3); }
     | expr '<' expr                         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr                         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr                         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr                         { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr                         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr                         { $$ = new cdk::eq_node(LINE, $1, $3); }
     | expr tOR expr                         { $$ = new cdk::or_node(LINE, $1, $3); }
     | expr tAND expr                        { $$ = new cdk::and_node(LINE, $1, $3); }
     | '-' expr %prec tUNARY                 { $$ = new cdk::unary_minus_node(LINE, $2); }
     | '+' expr %prec tUNARY                 { $$ = new cdk::unary_plus_node(LINE, $2); }
     | '~' expr                              { $$ = new cdk::not_node(LINE, $2);}
     | tIDENTIFIER '(' call_args ')'         { $$ = new udf::function_call_node(LINE, *$1, $3); delete $1; }
     | tSIZEOF '(' expr ')'                  { $$ = new udf::sizeof_node(LINE, $3); }
     | tOBJECTS '(' expr ')'                 { $$ = new udf::objects_node(LINE, nullptr, $3); }
     | '(' expr ')'                          { $$ = $2; }
     | lval                                  { $$ = new cdk::rvalue_node(LINE, $1); }
     | lval '?'                              { $$ = new udf::address_node(LINE, $1);}
     | lval '=' expr                         { $$ = new cdk::assignment_node(LINE, $1, $3); }    
     |'['call_args ']'                       { $$ = new udf::tensor_node(LINE, new cdk::sequence_node(LINE, $2)); }
     | expr tRANK                            { $$ = new udf::tensor_rank_node(LINE, $1); }
     | expr tCONTRACT expr                   { $$ = new udf::contraction_node(LINE, $1, $3); }
     | expr tDIM  '(' expr')'                { $$ = new udf::tensor_dim_node(LINE, $1,$4); }
     | expr tDIMS                            { $$ = new udf::tensor_dims_node(LINE, $1); }
     | expr tCAPACITY                        { $$ = new udf::tensor_capacity_node(LINE, $1); }
     | expr tRESHAPE '(' call_args ')'       { $$ = new udf::tensor_reshape_node(LINE, $1, $4); }
     ;


call_args : /* empty */                        { $$ = new cdk::sequence_node(LINE); }
          | expr                               { $$ = new cdk::sequence_node(LINE, $1); }
          | call_args ',' expr                 { $$ = new cdk::sequence_node(LINE, $3, $1); }


exprs : expr                                 { $$ = new cdk::sequence_node(LINE, $1); }   
     | exprs ',' expr                        { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;

integer   : tINTEGER                      { $$ = new cdk::integer_node(LINE, $1); };
real      : tREAL                         { $$ = new cdk::double_node(LINE, $1); };
string    : tSTRING                       { $$ = $1; }
          | string tSTRING                { $$ = $1; $$->append(*$2); delete $2; }
          ;


type : tTYPEINT                         { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT); }
     | tTYPEREAL                        { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE); }
     | tTYPESTRING                      { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING); }
     | tTYPE_POINTER '<' type '>'       { $$ = cdk::reference_type::create(4, $3); }
     | tTYPE_POINTER '<' tAUTO '>'      { $$ = cdk::reference_type::create(4, nullptr); }
     | tTYPETENSOR '<' dimensions '>'   { $$ = cdk::tensor_type::create(*$3); }
     ;

return          : tRETURN             ';' { $$ = new udf::return_node(LINE, nullptr); }
                | tRETURN expr ';'      { $$ = new udf::return_node(LINE, $2); }
                ;

dimensions
    : tINTEGER                           { $$ = new std::vector<size_t>() ;$$-> push_back($1); }
    | dimensions ',' tINTEGER            { $$ = $1;$$->push_back($3);  }
;

     
void_type   : tVOID { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID);   }
            ;

%%
