// conv.h : Definition of conversion functions

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation And Replacement Transformation System

#include <string>
#include <cstring>
#include <string.h>
using namespace std;

// Predefined regular expression strings

string FFMath = "$[?(,$)*]" ;
string FLISP  = "\\($ ($ )*\\)" ;

// Specific type conversion functions

string sym_str(unsigned s){
	char buf [15];
	sprintf(buf,"<%u>",s);
	return (string) buf;
}

string txt_str(string * s){
	return *s;
}

string int_str(int n){
	char buf [15];
	sprintf(buf,"%i",n);
	return (string) buf;
}

string real_str(double n){
	char buf [15];
	sprintf(buf,"%f",n);
	return (string) buf;
}

string bool_str(bool a){
	return string (a ? "true" : "false");
}

string comp_str(void * f, typ_t s){
	char buf [20];
	sprintf(buf,"fun<%p,%u>",f,s);	
	return (string) buf;
}

///////////// Class CStrexpr //////////////////////////////////////////////////

class CStrexpr {
	string regexpr;			// Regular string
	string strpnd;			// Pending string
	string s;				// Variable
	size_t p;				// Actual position
	size_t r;				// Position to return
	bool fail;				// String match failed...
public:
	string strout;			// Output accumulating string
	CStrexpr(string re) : regexpr(re), strout(""),strpnd(""),s(""),p(0),r(0),fail(false) {};
	bool eval(string str);
};

bool CStrexpr::eval(string str){
	char ch;
	bool esc   = false;
	while (p < regexpr.length()) {
		ch = regexpr[p++];				// Current char at regexpr
		if (esc){
			strpnd += ch;				// Litteral
			esc = false;
			continue;
		}
		if (str != "" && (ch == '$' || ch == '?') ) {	// Accepted string
			strpnd += str ;								// Produces an output
			return true;								// Returns immediately
		}
		switch ( ch ) {
			case '?':
				break;			// Continue (empty string)
			case '$':
				fail = true;	// Expected string (but empty)
				break;
			case '\\' :				// Escape (wait for next char.)
				esc = true;
				break;
			case '(' :				// Group
				r = p;				// repeat position saved
				strout += strpnd;
				strpnd = "";
				break;
			case ')':
				break;
			case '*':
				if (fail)				// Break repetition
					fail=false;			// Reset, continue forward
				else {
					p = r;				// Repeat pattern
					strout += strpnd;	// Validates output
				}
				strpnd = "";			// Erases pending string
				break;
			case 's' :
				if( s=="" ) { 		// Empty: s = input
					s = str;
					return true;	// Returns immediately
				}
				else
					strpnd += s;	// String is added
				break;
			default  :
				strpnd += ch;		// Accumulates string.
				break;
		}
	}
	if (! fail ) {
		strout += strpnd;			// Flush
		strpnd = "";			// Erases pending string
	}
	return false;					// End
	
}



