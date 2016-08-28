// NODE.h : Definitions for Node and Sequence Classes

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation And Replacement Transformation System

#include <iostream>
#include <string>
#include <cstring>
#include <string.h>

using namespace std;

///////////// Class CNode ////////////////////////////////////////////////

class CNode {
    typ_t		typ;            // 0: composite, 1,2,...: atom types
    typ_t		sub;            // subtype
    union {
		CSequ *  cmp;           // Compound expression: points the head
		// Atomic expressions:
		sym_t		sym;		// Symbol (serial number), others
		string    * str;		// String (pointer)
		bool        boo;		// Boolean (local)
		int         itg;		// Integer (local)
		double      rel;		// Real (local)
		CRational   rat;		// Rational (local)
		CComplex  * com;		// Complex (pointer)
		union {					// Pointer to compiled function
			fp0_t	f0		;
			fp10_t	f10		;
			fp110_t	f110	;
			fp20_t	f20		;			
			fp1_t	f1		;
			fp11_t	f11		;
			fp111_t	f111	;
			fp2_t	f2		;
			fp22_t	f22		;
			fp222_t	f222	;
			fp3_t	f3		;
			fp33_t	f33		;
			fp333_t	f333	;
			fp4_t	f4		;
			fp44_t	f44		;
			fp444_t	f444	;
			fp5_t	f5		;
			fp55_t	f55		;
			fp555_t	f555	;
		};
	};
	CNode      *  lnk;          // Sequence: next expression of a list
public:
	// Constructors & Destructors
    CNode(CSequ * s)	  : typ(Compound), cmp(s),  lnk(0) {};
    CNode(sym_t u)		  : typ(Symbol),sym(u), lnk(0) {};
    CNode(string * t)	  : typ(String),str(t), lnk(0) {};
	CNode(bool   a)		  : typ(Boolean),boo(a),lnk(0) {};
    CNode(int    n)		  : typ(Integer),itg(n),lnk(0) {};
    CNode(double n)		  : typ(Real),   rel(n),lnk(0) {};
    CNode(fp0_t f,typ_t t): typ(Compiled), sub(t),f0(f), lnk(0) {};
	CNode(sym_t n,typ_t t,typ_t s ):typ(t),sub(s),sym(n),lnk(0) {};
	
	// Structure information: E = H[E1,E2,..,En] | A (atom)
	typ_t	type()		{ return typ; };
	typ_t	subtype()	{ return sub; };
	sym_t   symbol()	{ return sym; };
	string * text()		{ return str; };
	bool    boolean()	{ return boo; };
	int		integer()	{ return itg; };
	double  real()		{ return rel; };
    bool is_atom()		{ return typ!=Compound; };
	bool is_comp()		{ return typ==Compound; };
	bool is_symbol()	{ return typ==Symbol; };
	bool is_symbol(sym_t a)	{ return typ==Symbol && sym==a; };	
	bool is_com_sym()	{ return typ<=Symbol; };
	bool is_number()	{ return typ>=Integer && typ<=Complex; };
	bool is_string()	{ return typ==String; };
	bool is_bool()		{ return typ==Boolean; };
	bool is_int()		{ return typ==Integer; };
	bool is_real()		{ return typ==Real; };
	
	CSequ  * seq() {return is_comp() ?  cmp : 0;};
	CNode * next() {return lnk; };
	unsigned symb(){return is_symbol() ? sym:0;};
	short depth();
	int length() ;
	CNode * head();
	CNode * first();
	CNode * second();
	// Structure modification
	CSequ * reset();
	CNode * join(CNode * s) ;				// : [...En,F]
	CNode * dup(int l);						// : F == E
	CNode * dup() { return dup(0); };
	void swap(CNode * n);
	void replace(CNode * n);
	CNode * uplevel();						// : Sequence and node
	friend class CTraversal;
	friend class CExpress;
	friend class CPattern;
};


///////////// Class CSequ ////////////////////////////////////////////////

class CSequ{
	CNode * hed;		// Pointer to head
	CNode * end;		// Pointer to end
	int len;			// Length, elements of sequence
	short cpy;			// Copies...
public:
	CSequ(CNode * n) : hed(n), end(n), len(1), cpy(0){};
	int length()     { return len; };
	CNode * head()   { return hed; };
	CNode * last()   { return end; };
	CNode * first()  { return hed->next();};
    CNode * index(u_dim i);						// : Ei 	
	CSequ * append (CNode * n);
	CSequ * prepend(CNode * n);
	CNode * takefirst();	
	CSequ * join(CSequ * s);
	CSequ * dup(int lv);
	CSequ * del();
	CSequ * reset(){ end = hed; len = 0; return this;};
	CNode * node();
};



////////////////////////// CNode FUNCTIONS ///////////////////////////////////

// Information member functions

short CNode::depth() { 
	if (is_atom())
		return 1;
	short dep = 2;
	CNode * e = head();
	while ( e=e->next() )
		dep = max(dep, (short)(1+e->depth()));
	return dep;
}

inline int CNode::length()   { return is_atom() ? 0 : cmp->length(); };

inline CNode * CNode::head() { return is_atom() ? 0 : cmp->head(); };

inline CNode * CNode::first(){ return cmp->head()->next(); };

inline CNode * CNode::second(){return cmp->head()->next()->next(); };



// Modification member functions

inline CNode * CNode::join(CNode * s){	// Primitive join function, assuming current node is last 
	return lnk = s;
}

CNode * CNode::dup(int lv) {
	CNode * npt = new CNode (*this);	// Copy of current object node
	if( is_comp() && --lv ) {			// Compound expression and lv!=0
		npt->cmp = cmp->dup(lv);;		// Copy recursively
	}
	return npt ;						// Return ptr to new object
}

CNode * CNode::uplevel() {					// : Classes embedding
	return (new CSequ( this ))->node();
	
}

void CNode::swap(CNode * n){
	CNode aux = *n;
	n->rel = rel;
	n->typ = typ;
	n->sub = sub;
	rel = aux.rel;
	typ = aux.typ;
	sub = aux.sub;
}



void CNode::replace(CNode * n){			// actual -> n 
	rel = n->rel;
	typ = n->typ;
	sub = n->sub;
};



// Generic sequence functions

void insert(CNode * s, CNode * pv) {	// : [E1,..Ei-1,F,Ei..,En]	
	if( s && pv ){
		s->join(pv->next());
		pv->join(s);
	}
};

CNode * delet(CNode * pv) {				// : [E1,..Ei-1,Ei+1,En]
	CNode * s = pv ? pv->next() : 0;
	if ( s )
		pv->join(s->next());
	return s;							// Deleted node is returned
};


//// CSequ Functions  ////////////////////////////////////

// Information member functions

CNode * CSequ::index(u_dim i){
	if ( i<1 || i>len )							// 0 < i <= len
		return 0;
	CNode * s = hed;
	while (i-- && s)
		s = s->next();
	return s;
}

// Modification member functions

CSequ * CSequ::prepend(CNode * n){
	n->join(hed->next());			// Attaches the rest to node inserted
	hed->join(n);
	len++;
	return this;
}

CSequ * CSequ::append(CNode * n){
	end = end->join(n);
	n->join(0);					// Effective end.
	len++;
	return this;
}

CNode * CSequ::takefirst(){
	CNode * n = hed->next();
	if (n) {					// There is first subexpr.
		hed->join(n->next());
		len--;
		n->join(0);				// Node is dettached
	}
	return n;
}


CSequ * CSequ::join(CSequ * s){		// Join by attaching tails (not heads)
	end->join(s->first());
	end = s->last();
	len += s->length();
	return s->reset();				// Return a minimal sequence
}

CSequ * CSequ::dup(int lv){
	CNode * s = head();				// Points to current head
	CSequ * npt = new CSequ (s) ;	// Points to new sequence initialized
	while( s = s->next() )
		npt->append( s->dup(lv) );
	return npt;
}


CNode * CSequ::node(){ 
	return new CNode(this); 
}


typedef CNode CNodeLkd ;
typedef CSequ CSeqLkd;


///////////// Class CTraversal ////////////////////////////////////////////////

class CTraversal {
	string regexp;
	string (*f_sym) (unsigned) ;
	string (*f_text) (string*) ;
	string (*f_bool)  (bool)   ;
	string (*f_int)   (int)    ;
	string (*f_real)  (double) ;
	string (*f_comp)  (void*,typ_t) ;	
public:
	CTraversal() : regexp(FFMath),f_sym(sym_str), f_text(txt_str), f_bool(bool_str), f_int(int_str), f_real(real_str), f_comp(comp_str) {};
	CTraversal(string re) : regexp(re),f_sym(sym_str), f_text(txt_str), f_bool(bool_str), f_int(int_str), f_real(real_str), f_comp(comp_str) {};	
	CTraversal(string re, string (* f_s)(unsigned)) : regexp(re), f_sym(f_s), f_text(txt_str), f_bool(bool_str), f_int(int_str), f_real(real_str), f_comp(comp_str) {};	
	CTraversal(string re, string (* f_s)(unsigned), string (* f_t)(string*), string (* f_b)(bool), string (* f_i)(int),string (* f_r)(double), string (* f_c)(void*,typ_t)) : 
	regexp(re), f_sym(f_s), f_text(f_t), f_bool(f_b),f_int(f_i), f_real(f_r), f_comp(f_c) {};
	string trav(CNode * e);
};


string CTraversal::trav(CNode * e) {
	if (!e)
		return "";
	switch (e->typ) {
		case Compound :
		{
			CStrexpr re = CStrexpr(regexp);
			e = e->head();						// Head
			while( re.eval( trav( e ) ) ) 
				e=e->next();
//			cerr << "trav: " << re.strout << endl;
			return re.strout;					// Result
		}
		case Symbol  : return f_sym(e->sym);			
		case String  : return f_text(e->str);
		case Boolean : return f_bool(e->boo);
		case Integer : return f_int(e->itg); 
		case Real    : return f_real(e->rel); 
		case Compiled: return f_comp((void*)e->f0,e->sub); 
		default      : return (string) "";
	}
}

string traverse(CNode * e, string reg, string (* f_s)(unsigned)){
	CTraversal tra = CTraversal(reg, f_s);
	return tra.trav(e);
}
	
	
	
