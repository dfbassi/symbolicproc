// Token.h : Definitions of tokenization procedures

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation And Replacement Transformation System

#include <iostream>
#include <string>
#include <cstring>
#include <string.h>

using namespace std;

// Tokenizer

enum token_t {null=0, delim_t, under_t, name_t, integer_t, real_t, text_t};
const char delimiters[] = "[](){},;:";
const char space[] = " \t\n";
const char dani[] = "Danilo Bassi";

class CToken {
    const char * chp, * str;
    string s;
    bool rtnd;
  public:
    token_t typ; 
    CToken () : str(""), chp(""), rtnd(false), typ(null) {};
    CToken (const char * sp) : str(sp), chp(sp), rtnd(false), typ(null) {};
    void next();
    void ret_token(){ rtnd = true; };
    string val() { return s.assign(str,chp-str); };
    bool is_space()   { return  strchr(space,*chp)!=0; };
    bool is_delimit() { return  strchr(delimiters,*chp)!=0; };
    bool is_number() { return(*chp>='0' && *chp<='9'); };
    bool is_letter() { return(*chp>='A' && *chp<='Z' || *chp>='a' && *chp<='z'); };
    bool is_underscore() { return *chp == '_'; };
    int reminder() { return chp-str; };
};

void CToken::next(){
    if( rtnd ){
      rtnd = false;
      return;		// Do nothing
    }
    if( *chp )
      while( is_space() ) // Skipped...
	chp++;
    if( *chp==0 ){	  // End of input
      typ = null;
      return ;
    }
//  Analysis of non-null char
    str = chp;  
// Delimeter    
    if( is_delimit() ){
      chp++;
      typ = delim_t;
      return;
    }
// Text
    if( *chp == '\"' ){ 	// Literal text
      chp++;			// Searches next
      while( *chp != '\"' || *(chp-1) == '\\' )    // escape sequences
        chp++;
      if( chp )
	chp++;
      typ = text_t;		// Text
      return; 
      }
 // Number
    if( is_number() ){
      typ = integer_t;       
      while( is_number() ||( typ==integer_t &&  *chp=='.' && (typ=real_t)))
        chp++;
      return ;
      }
// Name
    if( is_letter() ){
      chp++;   
      while( is_letter() || is_number() )
        chp++;
      typ = name_t;
      return ;
      }
// Underscore
    if( is_underscore() ){
      int i=3;    
      while( i-- && is_underscore() )
        chp++;
      typ = under_t;
      return ;
      }
// Junk: not valid char...
    chp++;
    typ =  null;
}
