%option c++ prefix="udf_scanner_" outfile="udf_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "udf_parser.tab.h"

#include <sstream>
static std::ostringstream strlit;


// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT X_HEX_INT X_STRING_IGNORE  X_BACKSLASH
%%
  yydebug=1; set_debug(1);

"//".*$                /* match the whole line (it is ignored) */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>.|"\n"      ;  /* ignore inside comments */




">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 
"||"                   return tOR;
"&&"                   return tAND;


"int"				   		     return tTYPEINT;		   
"real"				   		   return tTYPEREAL;
"string"			    	   return tTYPESTRING;
"ptr"                  return tTYPE_POINTER;

"nullptr"              return tNULLPTR;
"void"                 return tVOID;

"auto"				   		   return tAUTO;
"public"			   			 return tPUBLIC;
"forward"				   		 return tFORWARD;
"for"                  return tFOR; 
"do"                   return tDO;
"if"                   return tIFX; 
"else"                 return tELSE; 
"elif"                 return tELSEIF;
"write"                return tWRITE;  
"writeln"              return tWRITELN;
"return"               return tRETURN;
"break"                return tBREAK;
"continue"             return tCONTINUE;
"sizeof"               return tSIZEOF;


"tensor"               return tTYPETENSOR;
".dim"                 return tDIM;
".dims"                return tDIMS;
".reshape"             return tRESHAPE;
".rank"                return tRANK;
".capacity"            return tCAPACITY;
"**"                   return tCONTRACT;





"0x"                                yy_push_state(X_HEX_INT);

<X_HEX_INT>[[:xdigit:]]+            { yylval.i = strtoul(yytext, NULL, 16); yy_pop_state(); return tINTEGER; }
<X_HEX_INT>.|\n                     std::cerr << "WARNING: line " << lineno() << ": bad hexadecimal data!" << std::endl; exit(1);

[A-Za-z][A-Za-z0-9_]*               { yylval.s = new std::string(yytext); return tIDENTIFIER;}
[0-9]+\.[0-9]+([eE][-+]?[0-9]+)?    { yylval.d = atof(yytext); return tREAL; }

   

"\""                           yy_push_state(X_STRING);
<X_STRING>\\                   yy_push_state(X_BACKSLASH);
<X_STRING>"\""                 {
                                 yylval.s = new std::string(strlit.str());
                                 strlit.str("");
                                 yy_pop_state();
                                 return tSTRING;
                               }
<X_STRING>.                    strlit << *yytext;

<X_BACKSLASH>n                 strlit << '\n'; yy_pop_state();
<X_BACKSLASH>r                 strlit << '\r'; yy_pop_state();
<X_BACKSLASH>t                 strlit << '\t'; yy_pop_state();
<X_BACKSLASH>\\                strlit << '\\'; yy_pop_state();

<X_BACKSLASH>[[:xdigit:]]{1,2} {
                                 strlit << (char)(unsigned char)strtoul(yytext, NULL, 16);
                                 yy_pop_state();
                               }

<X_BACKSLASH>.                 strlit << *yytext; yy_pop_state();

<X_STRING_IGNORE>" "              { strlit << *yytext; yy_pop_state(); yy_push_state(X_STRING); }

<X_STRING_IGNORE>"\""               {
                                 yylval.s = new std::string(strlit.str());
                                 strlit.str("");
                                 yy_pop_state();
                                 return tSTRING;
                               } 
<X_STRING_IGNORE>.   

[0-9]+                 yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;


[-%\]{}()<>!*/~+&@|[;?,:=]        return *yytext;
"#"        ;   // ignore single #
[ \r\n\t]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");
%%
