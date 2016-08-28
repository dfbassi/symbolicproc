// SYMBOL: definition of all symbolic elements

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation And Replacement Transformation System

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <cstring>
#include <iterator>

using namespace std;

struct CBitvector{
	unsigned int val;
	CBitvector() : val(0) {};
	CBitvector(unsigned n) : val(n) {};
	void set(size_t i)  { val |= 1 << i;};
	void flip(size_t i) { val ^= 1 << i;};
	void reset(size_t i){ val |= 1 << i; val ^= 1 << i;};
	bool bit(size_t i){ return val & (1 << i);};
	bool mask(unsigned n){ return val & n;};
};

class CSymbol {
	static const char * fstr;
  public: 
    string	name;			// Name of symbol (unique)
	sym_t	 num;			// Symbol number
	unsigned cnt;			// Reference counter
 private:
    vector<CNode*>  val;	// Vector of list of definitions
    CBitvector flg;			// Control bit vector	
  public:
    CSymbol(string s)  : name(s), cnt(0), flg(EVAL12N) {};
    CSymbol(string s, unsigned f)  : name(s), cnt(0), flg(f) {};
	unsigned flag(){ return flg.val; };
	void set(unsigned i){ flg.set(i);};
	void reset(unsigned i){ flg.reset(i);};
    void ref_plus(){ ++cnt; };
    void ref_minus(){ --cnt; };
	CNode *  add_rule(CNode * e, sym_t ref);// New rule
	void del_rule(CNode * e);			// Delete a definition
	void undefine();
	string symbol_string() ;
    string symbol_string(unsigned i) ;
	size_t size(){ return val.size(); };
	CNode * value(unsigned i){ return val[i]; };
	friend class CExpress;
};

const char * CSymbol::fstr = "%-20s count:%4d flag:%08x def:%4i";

string CSymbol::symbol_string() { 
	char buf[100];
	sprintf(buf,fstr, name.c_str(), cnt, flg.val, val.size() ); 
	return (string) buf;
}

string CSymbol::symbol_string(unsigned i) { 
	char buf[10];
	sprintf(buf,"%6u ",i);
	return (string) buf + symbol_string();
}


// Add rule for ref symbol


CNode * CSymbol::add_rule(CNode * r, sym_t ref){
	CNode * e = r->first();
	val.push_back(r);
	cerr << name << " (" << ref << ") :" << endl;
	return r;
}
		

void del_rule(CNode * e);// Erases a definition
	
void undefine();





