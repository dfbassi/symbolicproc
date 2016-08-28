// DEF.h: GLOBAL DEFINITIONS

// SymLogical SMARTS V1.01 (2013/11)
// Symbolic Manipulation And Replacement Transformation System

#include <string>
#include <string.h>

using namespace std;

// Class forward declarations

class CTraversal;
class CNode;
class CSequ;
class CSymbol;
class CExpress;
class CPattern;


// Typedef's 
typedef unsigned short typ_t;	// type of content (0 is compound)
typedef unsigned sym_t	;		// symbol number
typedef unsigned count_t;		// type for counters
typedef unsigned short u_dim;
typedef char	 char8 [8];		// Generic (8 bytes)

// Function pointers
typedef void	(*fp0_t)	();
typedef void	(*fp10_t)	(CNode*);
typedef void	(*fp110_t)	(CNode*,CNode*);
typedef void	(*fp20_t)	(string*);
typedef CNode*	(*fp1_t)	();
typedef CNode*	(*fp11_t)	(CNode*);
typedef CNode*	(*fp111_t)	(CNode*,CNode*);
typedef string*	(*fp2_t)	();
typedef string*	(*fp22_t)	(string*);
typedef string*	(*fp222_t)	(string*,string*);
typedef bool	(*fp3_t)	();
typedef bool	(*fp33_t)	(bool);
typedef bool	(*fp333_t)	(bool,bool);
typedef int		(*fp4_t)	();
typedef int		(*fp44_t)	(int);
typedef int		(*fp444_t)	(int,int);
typedef double	(*fp5_t)	();
typedef double	(*fp55_t)	(double);
typedef double	(*fp555_t)	(double,double);



// Enumeration type of fixed symbols constants
enum con_t {Null=0, Symbol, String, Boolean, Integer, Real, 
			Rational, Complex, Compiled, Sparse, 
			Eps, Any, Neg, Alt, Rpt, Rpt0, Nam, Anyrpt, Anyrpt0};

#define Expr		Symbol
#define Compound	Null
#define Pattern		Eps
#define EndPattern	Seq

enum flag_t {NoWFlags=0, NoRValues, ValueFix, HeadFix, 
			 Eval0, Eval1, Eval2n, Num1, Num2n,
			Numeric, Commutative, Associative, Listable};

#define EVAL0	(1<<Eval0)
#define EVAL1	(1<<Eval1)
#define EVAL2N	(1<<Eval2n)
#define EVAL12N	(EVAL1 | EVAL2N)
#define EVAL02N (EVAL0 | EVAL1 | EVAL2N)


// Global variables

// Initial symbol names
string names []={"Null","Symbol","String","Boolean","Integer","Real",
				  "Rational","Complex","Compiled","Sparse",
				  "Eps", "Any", "Neg", "Alt", "Rpt", "Rpt0", "Nam", 
				  "Anyrpt", "Anyrpt0" };

//// Symbolic variables

sym_t Seq, List, Rule, RuleNow, Replace, ReplAll, Eval;
sym_t Define, DefNow, Flags, SetFlag, ResetFlag;

// System pointer

CExpress * sys;


// Numeric classes definition

class CRational {
    int number[2];
  public:
    void normalize();
};

class CComplex {
    double number[2];
public:
    void normalize();
};


