// INIT: input of initial definitions

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation And Replacement Transformation System

#include <math.h>

CNode * defdef;
CNode * defdeftg;


//// FUNCTIONS

//// Input/Output Functions

string * Getlinecin(){
	string str;
	getline(cin,str);
	return new string(str);
}

void Streamcout(string * s){
	cout << s;
}


//// Symbolic Processing Functions

CNode * define(CNode * r){
	cerr << "define(" << tra(r) << ")" << endl;
	sys->replace_symbol(r->head(),Rule);
	return sys->add_rule(r,0);
}

CNode * definetag(CNode * r){
	sys->replace_symbol(r->head(),Rule);
	sym_t sym   = r->first()->symbol();
	sys->del_expr(takefirst(r));
	return sys->add_rule(r,sym);
}


//// Arithmentic Functions

int AddInt( int a, int b){ 
	cerr << "Add(" << a << "," << b << ")" << endl;
	return a+b; }
int NegateInt( int a){ return -a; }
int MultiplyInt(int a, int b){ return a*b; }
int DivideInt(int a, int b) {return a/b;}

double Add( double a, double b){ return a+b; }
double Negate( double a){ return -a; }
double Multiply(double a, double b){ return a*b; }
double Divide(double a, double b) {return a/b;}
inline double Power(double a, double n) {return pow(a,n);}
inline double Exp(double a) {return exp(a);}
inline double Log(double a) {return log(a);}

//// Logic functions

bool Not(bool a){ return !a; }
bool And(bool a, bool b){ return a && b;}
bool Or (bool a, bool b){ return a || b;}
bool Implies(bool a, bool b){ return !a || b;}

#define _(h)		expr(sys->expr(Any),h)
#define __(h)		expr(sys->expr(Anyrpt),h)
#define def(a,b)	expr(sys->expr(Define), a, b )
#define def3(a,b,c)	expr(sys->expr(Define), a, b, c )
#define rul(a,b)	expr(sys->expr(Rule), a, b )


// Execution of initialization

void init(){
// Assigning symbol values
	Seq		= sys->str_sym("Seq");
	List	= sys->str_sym("List");
	Rule	= sys->str_sym("Rule");
	RuleNow	= sys->str_sym("RuleNow");
	Replace = sys->str_sym("Replace");
	ReplAll = sys->str_sym("ReplAll");
	Define	= sys->str_sym("Define");
	DefNow	= sys->str_sym("DefNow");
	
// Defining compiled functions

	define(def(def(_(0),_(0)),expr(define)));
	define(def(def3(_(sys->expr(Symbol)),_(0),_(0)), expr(definetag)));
	define(def(expr(sys->expr("Add"),__(sys->expr(Integer))),expr(AddInt,9)));
	define(def(expr(sys->expr("Add"),__(sys->expr(Real))),expr(Add,9)));
	define(def(expr(sys->expr("Input")),expr(Getlinecin)));
	
	
}