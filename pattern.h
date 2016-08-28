// PATTERN.h : Definitions for Pattern Manipulation

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation And Replacement Transformation System

#include <vector>
#include <string>
#include <iterator>
#include <numeric>


using namespace std;

///////////////// CPattern ////////////////////////////////////////////////

struct CSymPatt {
	sym_t symbol;
	CNode * first;
	CNode * last;
	CSymPatt(sym_t s,CNode * f, CNode * l): symbol(s), first(f), last(l){};
	CNode * expr();
};

CNode * CSymPatt::expr(){
	CNode * r=0;
	if (first != last) 
		if (first->next()==last) 
			r = first->dup();
		else {
			r = ::expr(sys->expr(Seq));
			CNode * e=first;
			for (CNode * e=first; e!=last; e->next())
				append(r,e->dup());
		}
	return ::expr(sys->expr(Rule),sys->expr(symbol),r);
}

 
class CPattern{
	vector<CSymPatt>	pat_table;
public:
	vector<CSymPatt>::iterator find(sym_t s);
	CNode * expr();
	bool found(vector<CSymPatt>::iterator it){return it != pat_table.end();};
	bool match_pattern(CNode * * e, CNode * p);
	bool match_next_pat(CNode * * e, CNode * p);
	bool match_alt(CNode * * e, CNode * ap, typ_t * ct, typ_t i);
	bool match_rpt(CNode * * e, CNode * rp, typ_t * ct, typ_t i);
	bool match(CNode * * e, CNode * p);
	bool match_next(CNode * * e, CNode * p);
	bool match_seq(CNode * * e, CNode * p);	
	bool match_next_seq(CNode * * e, CNode * p);
	void trans_anyrpt(CNode * e);
	void trans_anyrpt0(CNode * e);
};
	
	
//////////////////// FUNCTIONS /////////////////////////////////////////////

/////////////// Generic functions

inline bool pattern(sym_t n) {return n>=Pattern && n<EndPattern;}

inline bool pattern(CNode * p){return p->is_symbol() ? pattern(p->symbol()):
									  false; }
									  
bool match(CNode * e, CNode * p){
	CPattern pat;
	return pat.match(&e,p);
}

bool match_next(CNode * e, CNode * p){
	CPattern pat;
	return pat.match_next(&e,p);
}

CNode * match_rule(CNode * e, CNode * p){
	CPattern pat;
	cerr << "match_rule(" << tra(e) <<","<< tra(p)<<")"<< endl;
	bool m = pat.match(&e,p);
	cerr << "match : " << m << endl;
	if (m)
		return pat.expr();
	else
		return 0;
}


//////////////// Member functions

vector<CSymPatt>::iterator CPattern::find(sym_t s){ 
	vector<CSymPatt>::iterator it = pat_table.begin();
	while (it != pat_table.end() && it->symbol != s) 
		it++;
	return it;
}

CNode * CPattern::expr(){
	CNode * r = ::expr(sys->expr(List));
	for (vector<CSymPatt>::iterator it=pat_table.begin(); 
		 it!=pat_table.end(); it++)
			append(r,it->expr());
	return r;
}
	
bool CPattern::match_pattern(CNode * * e, CNode * p){
	CNode * f;
	cerr << "match_pattern(" << tra(*e) <<","<< tra(p)<<")"<< endl;
	switch (p->head()->symbol()) {
		case Eps: 
			return true;
		case Any:
			f = *e;
			*e = (*e)->next();			// Prepare next
			return !p->first() || p->first()->symbol()==head_sym(f);
		case Neg:
			return !match(e,p->first());
		case Alt:
			return match_alt(e,p->first(),&(p->head()->sub), 1);
		case Rpt:
			return match_rpt(e,p->first(),&(p->head()->sub),p->next()? 1: 100);
		case Rpt0:
			return match_rpt(e,p->first(),&(p->head()->sub),p->next()? 0: 100);
		case Nam:{
			vector<CSymPatt>::iterator it=find(p->first()->symbol());
			bool m = true;
			if (found(it)) {			// Already saved
				f = it->first;
				while( f!=it->last && (m=match(e,f)) ){
					f=f->next();
					*e=(*e)->next();
				}
			}
			else {
				f=*e;					// First position
				if(m = match(e,p->first()->next()))
					pat_table.push_back(CSymPatt(p->first()->symbol(),f,*e));
			}
			return m;
		}
		case Anyrpt:
			trans_anyrpt(p);			// Transformed
			return match_pattern(e, p);
		case Anyrpt0:
			trans_anyrpt0(p);			// Transformed
			return match_pattern(e, p);
	}
	return false;
}

bool CPattern::match_next_pat(CNode * * e, CNode * p){
	CNode * f;
	switch (p->head()->symbol()) {
		case Eps: 
		case Any:
		case Neg:
			return false;
		case Alt:
			return match_alt(e,p->first(),&(p->head()->sub),p->head()->sub + 1);
		case Rpt:
		case Rpt0:
			return match_rpt(e,p->first(),&(p->head()->sub),
							 p->next()? p->head()->sub + 1: 100);
		case Nam:
			return match_next(e,p->first()->next());
	}
	return false;
}


bool CPattern::match_alt(CNode * * e, CNode * ap, typ_t * ct, typ_t i){
	*ct = 1;
	while( *ct < i  && (ap=ap->next()) )		// Skipping
		(*ct)++ ;
	while ( ! match(e,ap) && (ap=ap->next()) )	// Searching
		(*ct)++ ;
	return ap!=0;
}

bool CPattern::match_rpt(CNode * * e, CNode * rp, typ_t * ct, typ_t i){
	bool m = true;
	*ct = 0;				// initialize counter
	while ((++(*ct)<=i) && *e && (m=match(e,rp)) ) 
		;
	return m;
}
	

bool CPattern::match(CNode * * e, CNode * p){
	CNode * f = *e;
	cerr << "match(" << tra(f) <<","<< tra(p)<<") "<< e << endl;
	if ( !p )						// Empty pattern
		return f==0 ;				// Empty matches
	if ( f  && p->is_atom() ){		// Matching atom
			*e = f->next();			// Prepare next
			return compare_atom(f,p)==0;
		}
	else{
		cerr << "\tpattern ?:" << pattern(p->head()) << endl;
		if ( pattern(p->head()) )		// Matching a pattern
			return match_pattern(e, p);
		else						// Compound e & p
			if ( f && f->is_comp()){
				CNode * n = f->head();
				*e = f->next();		// Prepare next
				return match_seq(&n, p->head() );
			}
			else			
				return false;		// No match
	}
}

bool CPattern::match_next(CNode * * e, CNode * p){
	CNode * f = *e;
	if ( !p || p->is_atom() || !f || f->is_atom() )
		return false;
	if ( pattern(p->head()) )				// Matching pattern
		return match_next_pat(e,p);
	else {							// Compound
		CNode * n = f->head();	 
		*e = f->next();				// Prepare next
		return match_next_seq( &n, p->head() );
	}
}


bool CPattern::match_seq(CNode * * e, CNode * p){
	CNode * f = *e;
	cerr << "match_seq(" << tra(f) <<","<< tra(p)<<")"<< endl;
	if ( match(e , p) )
		if ( !p || match_seq(e,p->next()) )
			return true;
		else {
			do{						// Backtracking
				*e = f;
				if( ! match_next(e,p) )
					return false;
			} while ( ! match_seq(e,p) );
			return true;
		}
	return false;
}
	
bool CPattern::match_next_seq(CNode * * e, CNode * p){
	if ( match(e, p) )
		if ( match_next_seq(e,p->next()) )
			return true;
		else 
			return match_next(e, p) && match_seq (e, p);
	return false;
}
	
void CPattern::trans_anyrpt(CNode * e){		// e = anyrpt[e1] -> rpt[any[e1]]
	sys->replace_symbol(e->head(),Rpt);	// anyrpt -> rpt
	prepend(e,append(::expr(sys->expr(Any)),takefirst(e)));	// any[e1]
}

void CPattern::trans_anyrpt0(CNode * e){	// e = anyrpt0[e1] -> rpt0[any[e1]]
	sys->replace_symbol(e->head(),Rpt);	// anyrpt -> rpt
	prepend(e,append(::expr(sys->expr(Any)),takefirst(e)));
}	






