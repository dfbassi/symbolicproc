// EXPRESS.ccp : Testing program for symbolic expression processing

// Symbolic Manipulation And Replacement Transformation System (2011-16)

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>

#include "def.hh"
#include "token.h"
#include "conv.h"
#include "node.h"
#include "symbol.h"
#include "express.h"
#include "init.h"
#include "pattern.h"

using namespace std;


int main (int argc, char * argv[])
{
	const char * re = regexp[1];
	string * str;
	if(argc>1)                      // Input regular expression, for output...only
		re = argv[1];		
	sys = new CExpress();           // System object (pointed by pexp)
	CNode * in;                     // Input[  ]
	CNode * inst;                   // InputString
	CNode * st;                     // String (atom)
	CNode * ou;                     // Output[ ]
	CNode * exp;                    // Actual expression
	CNode * fin = expr(Getlinecin); // Node with input function
	
	cout << "Symbolic Manipulation And Rule Transformation System\n\n";
	cout << "Output regular expression: \""<< re << "\"\n";
	cout << "Sizesof: CNode=" << sizeof(*exp) << " CSequ=" << sizeof(CSequ) ;
	cout << " CSymbol=" << sizeof(CSymbol) << " CExpress=" << sizeof(*sys)<< endl;

	size_t i,j;
	init();
	cout << "Symbol Table (initial): \n";
	for(i=0; i< sys->symbol_table_size(); ++i)
		cout << sys->pos_sym(i)->symbol_string(i) << "\n";
	cout << "Symbol Transformation Definitions: \n";		
	for(i=0; i< sys->symbol_table_size(); ++i)
		for(j=0; j < sys->pos_sym(i)->size(); ++j)
			cout << "    " << tra(sys->pos_sym(i)->value(j)) << "\n";	
	cout << "Session \n";
	do {
		in=expr(sys->expr("Input"),::expr(++sys->num_line));
		inst=expr(sys->expr("InputString"),::expr(sys->num_line));
		cout << tra(in) << ":= " ;
		cout << tra(inst)<<"= " << (st->text()) << endl;
		sys->eval(exp=expr(sys->expr("Input")));
		exp = evalfun(in,fin);
		sys->eval(def(inst,exp));
		exp = sys->parse(exp);                          // Parsing input
		ou=expr(sys->expr("Output"),expr(sys->num_line));
		sys->eval(def(ou,exp));
		cout << tra(ou)<<"= "<< tra(exp)<< endl;        // Conversion of expression
		cout << "\t" << sys->expr_info() << endl;
	} while (! equival(exp,sys->expr("Quit")));
	cout << "Symbol Table (final): \n";
	for(i=0; i< sys->symbol_table_size(); ++i)
		for(j=0; j < sys->pos_sym(i)->size(); ++j)
			cout << "\t" << tra(sys->pos_sym(i)->value(j)) << "\n";		
	return sys->num_line;
}
