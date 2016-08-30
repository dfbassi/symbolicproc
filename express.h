// EXPRESS.h: Definitions for Expression Class

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation and Replacement Transformation System SMARTS

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <string.h>
#include <numeric>


using namespace std;

///////////////// CExpress ////////////////////////////////////////////////////

class CExpress{
    vector<CSymbol>			 sym_table;		// Table of symbols, and definitions
	vector<CSymbol>::iterator curr_sym;		// Current iterator for symbol table
    vector<CNode*>			 exp_table;		// Table of expressions
	vector<CNode*>::iterator curr_exp;		// Current iterator for expression table
public:	
	sym_t num_sym;							// Number of initial symbols
	int	  num_line;							// Number of current line
    bool  find_symbol(string s) ;
    sym_t str_sym(string s) ;
	sym_t str_sym(const char * s) {return str_sym((string) s);};
    size_t insert_expr(CNode * e){ 
		exp_table.push_back( e ); 
		return exp_table.size();
	};
	vector<CSymbol>::iterator pos_sym(sym_t i) {return sym_table.begin()+i;};
	CNode * del_expr(CNode * e) ;					   // Removing an expression	
    CExpress(){ for(num_sym=0; num_sym< sizeof(names)/sizeof(names[0]);num_sym++) 
					str_sym( names[num_sym]);}
    CNode * expr(sym_t i){ pos_sym(i)->ref_plus();
								return new CNode(i);}; // Atom creation (Symbol)
	CNode * expr(const char * s){return expr(str_sym(s));};							
	void replace_symbol(CNode * e, sym_t s);
	CNode * add_rule(CNode * r, sym_t ref);
	bool eval(CNode * e);
    CNode * parse (CToken * tok, CNode * e) ;
	CNode * parse (CToken * tok) { return parse(tok,0); };
    CNode * parse (CNode * e);	
    CNode * parse_atom (CToken * tok) ;
	size_t symbol_table_size() {return sym_table.size(); };
	string sym_str(sym_t i)    { return pos_sym(i)->name; };
	string expr_info(){ stringstream ss; 
		ss << "Symbol table (size:" << sym_table.size();
		ss << ", references:"<< sym_table[0].cnt<< ")  Expression table (size: ";
		ss << exp_table.size()<<  ")"; return ss.str(); };
};
	
	
//////////////////// FUNCTIONS ////////////////////////////////////////////

//// Static Functions & Variables

const char * regexp [] = {"$[?( $)*]","$[?(,$)*]","[s?( s $)*]"};

string symbol_str(unsigned u) { return sys->sym_str(u); }

string tra(CNode * e) { return traverse(e,(string)regexp[1],symbol_str);}


//////////////// Member functions

// find: look for position of a symbol, given its name
	
bool CExpress::find_symbol(string s) {
  curr_sym = sym_table.begin();
  while( curr_sym != sym_table.end() && curr_sym->name != s )
    ++curr_sym;
  return curr_sym != sym_table.end();
}

// str_sym: interpretes a string as a symbol (finding or inserting), returning index

unsigned CExpress::str_sym(string s) {
	unsigned size = sym_table.size();
	if(find_symbol( s ))					
		return (curr_sym - sym_table.begin());	// Found (in curr_sym)
	sym_table.push_back( CSymbol( s ) );		// Not found: Insert at the end
	return size;								// Position is initial size of vector
}

// Deleting an expression

CNode * CExpress::del_expr(CNode * e) {
	CNode * s;	
	if( e->is_comp() ){						// Compound expression
		s = e->head();
		while ((s = del_expr(s)) != NULL)	// Delete subexpressions
			;
		s = e->next();
	}
	else {									// Atomic expression
		s = e->next();
		if ( e->is_symbol() )
			pos_sym(e->symbol())->ref_minus();	// Update of counter
		if (e->type() == String)
			delete e->text();					// Remove string
	}
	delete e;								// Actual delete
	return s;
}

// Replacing a symbol

void CExpress::replace_symbol(CNode * e, sym_t s){
	if (e->is_symbol()) {
		pos_sym(e->symbol())->ref_minus();
		pos_sym(s)->ref_plus();
		e->sym = s;
	}
}

/////////////// Generic functions

// Information

sym_t head_sym(CNode * e) { 
	if (e->is_atom())
		return e->type();
	return e->head()->is_symbol() ? e->head()->symbol():0; 
}

sym_t final_head_sym(CNode * e){
	if ( e->is_symbol() )
		return e->symbol();
	return e->is_comp() ? final_head_sym(e->head()) : 0;
}



// Expression modification and creation

// Atomic Expressions creation

inline CNode * expr(string * s){return new CNode(s);}	// String
inline CNode * expr(bool a)    {return new CNode(a);}	// Boolean
inline CNode * expr(int n)     {return new CNode(n);}	// Integer
inline CNode * expr(double f)  {return new CNode(f);}	// Real
inline CNode * expr(fp0_t f, typ_t t) {return new CNode(f,t);}// function
inline CNode * expr(fp222_t f, typ_t t){return new CNode((fp0_t)f,92);}
inline CNode * expr(fp333_t f, typ_t t){return new CNode((fp0_t)f,93);}
inline CNode * expr(fp444_t f, typ_t t){return new CNode((fp0_t)f,94);}
inline CNode * expr(fp555_t f, typ_t t){return new CNode((fp0_t)f,95);}
inline CNode * expr(fp0_t   f)	{return new CNode(f, 0);}
inline CNode * expr(fp10_t  f)	{return new CNode((fp0_t)f, 10);}
inline CNode * expr(fp110_t f)  {return new CNode((fp0_t)f, 110);}
inline CNode * expr(fp20_t  f)	{return new CNode((fp0_t)f, 20);}
inline CNode * expr(fp1_t   f)	{return new CNode((fp0_t)f, 1);}
inline CNode * expr(fp11_t  f)	{return new CNode((fp0_t)f, 11);}
inline CNode * expr(fp111_t f)  {return new CNode((fp0_t)f, 111);}
inline CNode * expr(fp2_t   f)	{return new CNode((fp0_t)f, 2);}
inline CNode * expr(fp22_t  f)	{return new CNode((fp0_t)f, 22);}
inline CNode * expr(fp222_t f)  {return new CNode((fp0_t)f, 222);}
inline CNode * expr(fp3_t   f)	{return new CNode((fp0_t)f, 3);}
inline CNode * expr(fp33_t  f)	{return new CNode((fp0_t)f, 33);}
inline CNode * expr(fp333_t f)  {return new CNode((fp0_t)f, 333);}
inline CNode * expr(fp4_t   f)	{return new CNode((fp0_t)f, 4);}
inline CNode * expr(fp44_t  f)	{return new CNode((fp0_t)f, 44);}
inline CNode * expr(fp444_t f)  {return new CNode((fp0_t)f, 444);}
inline CNode * expr(fp5_t   f)	{return new CNode((fp0_t)f, 5);}
inline CNode * expr(fp55_t  f)	{return new CNode((fp0_t)f, 55);}
inline CNode * expr(fp555_t f)  {return new CNode((fp0_t)f, 555);}

// Appending & Prepending subexpressions

inline CNode * append(CNode * e, CNode * s){
	if( s )
		e->seq()->append( s );
	return e;
}

inline CNode * prepend(CNode * e, CNode * s){
	if ( s ) 
		e->seq()->prepend( s );
	return e;
}

// Compound Expressions (using append)

CNode * expr(CNode * e) {							// E=E0[]
	if( e && e->is_com_sym() )
		return e->uplevel();
	return 0;}
	
CNode * expr(CNode * e0, CNode * e1) {				// E=E0[E1]
	if (e0 && e0->is_com_sym())
		return append(e0->uplevel(),e1);
	return 0;
}

CNode * expr(CNode * e0, CNode * e1, CNode * e2) {	// E=E0[E1,E2]
	if (e0 && e0->is_com_sym())
		return append(append(e0->uplevel(),e1),e2);
	return 0;
}
													// E=E0[E1,E2,E3]
CNode * expr(CNode * e0, CNode * e1, CNode * e2, CNode * e3) {	
	if (e0 && e0->is_com_sym())
		return append(append(append(e0->uplevel(),e1),e2),e3);
	return 0;
}


// Taking first subexpression from an expression

inline CNode * takefirst(CNode * e ){
	return e->seq()->takefirst();
}


// Sequence generation & manipulation

inline CSequ * sequ(CNode * e) { return e ? new CSequ(e) : 0; }

inline CSequ * sequ(CNode * e0, CNode * e1) { 
		return e1 ? (new CSequ(e0))->append(e1) : sequ(e1); }

inline CSequ * sequ( CSequ * s, CNode * e) { 
		return s ? s->append(e) : sequ(e); }

// Expression from a sequence

inline CNode * expr(CSequ * s) { return s ? s->node() : 0; }	


////// Comparison ///////////////////////////////////////////////

// Numeric conversion

double num_double(CNode * e) {
	switch (e->type()) {
		case Integer:
			return (double) e->integer();
		case Real:
				return e->real();			
		default:
			return 0.0;
	}
}

// Sign function

inline int sign(double v) { return (v>0) - (v<0) ;}

// Compare two numbers

inline int compare_number(CNode * e1, CNode * e2){
	return sign( num_double(e1) - num_double(e2) );	
}

// Compare two atoms

int compare_atom(CNode * e1, CNode * e2){
	if( e1->is_number()  && e2->is_number())	
		return 	compare_number(e1,e2);		// Numeric comparison
	if (e1->type()!=e2->type())				// Mixed, non-numeric
		return (int)e2->type() - (int)e1->type() ;
	if (e1->is_symbol())					// Two symbols
		return (int)e1->symbol() - (int)e2->symbol();		   
	if (e1->type() == String)				// Two strings
		return (e1->text())->compare(*(e2->text()));
	if (e1->type() == Boolean)				// Two booleans
		return int(e1->boolean()) -  int(e2->boolean());
	return int(e1 - e2);
}

// Compare two expressions (canonical order)

int compare(CNode * e1, CNode * e2){
	if (e1 == e2)
		return 0;						// Two identical expressions
	if (!e1)								
		return  1;						// Assuming e2!= 0
	if (!e2)
		return -1;						// Assuming e1!= 0
	if (e1->is_comp() && e2->is_comp() ) {	// Two compound expressions
		int c ;								// comparison value
		e1=e1->head();	
		e2=e2->head();
		while( (c=compare(e1,e2))==0 && e1 && e2 ){
			e1=e1->next();
			e2=e2->next();
		}						
		return c;
	}
	return compare_atom(e1, e2);		// At least one atom
}


inline bool before(CNode * e1, CNode * e2) { return compare(e1, e2) < 0 ; };
inline bool equival(CNode * e1, CNode * e2){ return compare(e1, e2) == 0; };
inline bool before2(CSequ * e1, CSequ * e2){ return 
						before(e1->head()->next(),e2->head()->next()); };
inline bool equival2(CSequ * e1, CSequ * e2) { return 
						equival(e1->head()->next(),e2->head()->next()); };



//// RULES ////	

CNode * CExpress::add_rule(CNode * r, sym_t ref){
	if (!ref)
		ref = final_head_sym(r->first());	// Ref is defined here 
	return sym_table[ref].add_rule(r,ref);
}

//// EVALUATION ////


// Evaluation of compiled functions

inline typ_t ftype(con_t t)  { return t; }
inline typ_t ftype(con_t t1, con_t t2) { return t2*10+t1; }
inline typ_t ftype(con_t t1, con_t t2, con_t t3) { return (t3*10+t2)*10+t1; }
inline typ_t ftype(con_t t1, con_t t2, con_t t3, con_t t4) { return ((t4*10+t3)*10+t2)*10+t1; }

CNode * evalfun(CNode * e, CNode * fu){
	cerr << "evalfun(" << tra(e) << "," << tra(fu) << ")" << endl;
	CNode * e1 ;			// First argument
	CNode * o = 0;			// Output pointer
	switch (fu->subtype() % 10) {	// Takes least digit
		case 0:
			switch (fu->subtype()) {
				case 0:			// void (*) ()
					fu->f0();
					break;
				case 10:		// void (*) (CNode*)
					fu->f10( e );
					break;
				case 110:		// void (*) (CNode*,CNode*)
					e1 = takefirst(e);
					fu->f110(e1, takefirst(e));
					break;
			}
			break;
		case 1:
			switch (fu->subtype()) {
				case 1:
					return fu->f1();
					break;
				case 11:
					o = fu->f11( e );
					break;
				case 111:
					e1 = takefirst(e);
					o = fu->f111(e1, takefirst(e));
					break;
				default: ;
			}
			break;					
		case 2:
			e1 = e->first();	
			switch (fu->subtype()) {
				case 2:
					o = ::expr(fu->f2());
					break;
				case 22:
					o = ::expr(fu->f22( e1->text() ));
					break;
				case 222:
					o = ::expr(fu->f222( e1->text(), e1->next()->text() ));
					break;
				case 92:	{
				    string * s = e1->text();
					while ((e1=e1->next()) != NULL )
						fu->f222( s, e1->text() );
					o = ::expr(s);
					}
					break;
				default: ;
			}
			break;
		case 3:
			e1 = e->first();	
			switch (fu->subtype()) {
				case 3:
					o = expr(fu->f3());
					break;
				case 33:
					o = expr(fu->f33( e1->boolean() ));
					break;
				case 333:
					o = expr(fu->f333( e1->boolean(), e1->next()->boolean() ));
					break;
				case 93:	{
				    bool b = e1->boolean();
					while ( (e1=e1->next()) != NULL )
						b = fu->f333( b, e1->boolean() );
					o = expr(b);
					}
					break;
				default: ;
			}
			break;
		case 4:
			e1 = e->first();	
			switch (fu->subtype()) {
				case 4:
					o = expr(fu->f4());
					break;
				case 44:
					o = expr(fu->f44( e1->integer() ));
					break;
				case 444:
					o = expr(fu->f444( e1->integer(), e1->next()->integer() ));
					break;
				case 94:	{
					int n = e1->integer();
					while ( (e1=e1->next()) != NULL )
						n = fu->f444( n, e1->integer() );
					o = expr(n);
					}
					break;
				default: ;
			}
			break;
		case 5:
			e1 = e->first();	
			switch (fu->subtype()) {
				case 5:
					o = expr(fu->f5());
					break;
				case 55:
					o = expr(fu->f55( e1->real() ));
					break;
				case 555:
					o = expr(fu->f555( e1->real(), e1->next()->real() ));
					break;
				case 95:	{
					double r = e1->real();
					while ( (e1=e1->next()) != NULL )
						r = fu->f555( r, e1->real() );
					o = expr(r);
					}
					break;
				default: ;
			}
			break;			
		default: ;
	}
	cerr << "\t" << tra(o) << endl;
	return o;			// Output is returned
}

// Replace total

bool replace_all(CNode * e, CNode * r){
	bool m;
	CNode * f;
	if (r->head()->symbol()==Rule){	// Single rule
		f=r;
		m=equival(e,f->first());
	}
	else							// List of rules (assumed)	
		for(f=e->first(); f!=0 && !(m=equival(e,f->first()));
			f=f->next()) ;
	if(m)
		e->replace(f->second());
	if (e->is_comp())	
		for (f=e->head(); f!=0; f=f->next() )
				m=m||replace_all(f,r);
	return m;
}

// Evaluation of an expression

CNode * match_rule(CNode * e, CNode * p);

bool CExpress::eval(CNode * e) {
	cerr << "eval(" << tra(e) << ")  symbolic:" << e->is_com_sym() << endl;
	if (!e->is_com_sym())
		return false;			// No change
	CNode * f;
	bool m=false;
	unsigned flag = EVAL0;
	if (e->is_comp()){			// Evaluation of subexpressions E0,E1,...
		f = e->head();
		flag = f->is_symbol() ? sym_table[f->symbol()].flag() : EVAL02N;
		if( flag & EVAL0)		// Evaluation of head of expression
			m=m||eval(f);
		if( (f=f->next()) && (flag & EVAL1)) // Evaluation of first
			m=m||eval(f);
		while((f=f->next()) && (flag & EVAL2N))// Evaluation of rest
			m=m||eval(f);
	}
	sym_t h = final_head_sym(e);// Rules under this symbol
	CNode * r = 0;
	vector<CNode*>::iterator it;// Matching main expression (table of rules)
	if( e->is_comp() || sym_table[h].flg.mask(EVAL0) )
		for (it=sym_table[h].val.begin(); it!=sym_table[h].val.end(); it++)
			if ( (r=match_rule(e,(*it)->first())) != 0 )	
				break;
	cerr << "\tRules(" << tra(e) << ") : " << tra(r) << endl;
	if ( !r )					// No matches for main expression
		return m;
	cerr << "eval : " << h << "  " << tra(*it) << endl;
	switch ((*it)->second()->type()) {
		case Compound:
		case Symbol:
			replace_all(f=(*it)->second()->dup(), r);
			break;
		case Compiled: 
			f = evalfun(e,(*it)->second());
		default:
			f = (*it)->second()->dup();
			break;
	}
	swap(e,f);
	del_expr(f);
	return m;
}


// Parsing full expression: E = E0[E1,E2,...] 
	
CNode * CExpress::parse (CToken * tok, CNode * e) {
	if( !e && (e = parse_atom(tok)) && !e->is_symbol() )
		return e ;				// Returns if empty or not symbol
// Look at subexpressions [E1,E2,...]
	tok->next();						// Looking next token...
	if( tok->val()=="["){			// There are subexpressions
		e = ::expr( e );		// Compound expression
		do{							// Next expression is sought
			append(e, parse(tok));
			tok->next();				// Looking next token...      
			if( tok->val()=="]" )	// End subexpressions
				return parse(tok,e); 
		}while( tok->val()=="," );	// Continue next
  }
  tok->ret_token();				// Token is returned (not used)
  return e;						// Expression is returned (untouched)
}

CNode * CExpress::parse (CNode * e) { 
	if(e && e->is_string()){
		CToken tok = CToken(e->text()->c_str());
		return parse(&tok,0);
	}
	return 0;
}


// Parsing an atom
	
CNode * CExpress::parse_atom (CToken * tok) {
    tok->next();				// Looking for one token (atom)
    switch (tok->typ) {			// Specific conversions
		case name_t:	
			return expr(str_sym(tok->val()));
		case under_t:	
			return expr(str_sym(tok->val()));
		case text_t:
			return ::expr( new string (tok->val()) );
		case integer_t:
			return ::expr(atoi( tok->val().c_str()));	
		case real_t:
			return ::expr(atof( tok->val().c_str()));   
        default :
			tok->ret_token();	// Token is returned (not used)
			return 0;
	}
}






