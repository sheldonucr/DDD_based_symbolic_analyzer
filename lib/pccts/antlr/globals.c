/*
 * globals.c	--	File containing all variables/tables visible to all files.
 *
 * $Id: globals.c,v 1.5 95/10/05 11:57:02 parrt Exp $
 * $Revision: 1.5 $
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 * 
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-1995
 */
#include <stdio.h>
#ifdef __cplusplus
#ifndef __STDC__
#define __STDC__
#endif
#endif
#include "set.h"
#include "syn.h"
#include "hash.h"
#include "generic.h"

char Version[] = "1.33" ;	/* PCCTS version number */
char VersionDef[] = "133";  /* same (except int equiv for preproc symbol) */
char LexStartSymbol[] = "START";/* Name of starting lexical class/automaton */

char *RemapFileName = "remap.h";
char *DlgFileName = "parser.dlg";
char *DefFileName = "tokens.h";
char *ErrFileName = "err.c";
char *ModeFileName = "mode.h";

char *ParserName = DefaultParserName;

/* list of PCCTS supplied support symbols; these are renamed when more than
 * one ANTLR-generated parsers are linked together to avoid name conflicts.
 * Can't use '##' ANSIC preprocessor concat operator with K&R and:
 *		#define zzskip	zzparser ## skip
 * will not work for ANSI/C++ as 'zzparserskip' is created w/o zzparser
 * being substituted--ack!!!
 */
char *StandardSymbols[] = {
/* ANTLR stuff */
	"zzStackOvfMsg",
	"zzasp",
	"zzaStack",
	"inf_tokens",
	"inf_text",
	"inf_text_buffer",
	"inf_text_buffer_ptr",
	"inf_text_buffer_size",
 	"inf_labase",
	"inf_last",
	"inf_lap",
	"zztokenLA",
	"zztextLA",
	"zzlap",
	"zzlabase",
	"zztoktext",
	"zztoken",
	"zzdirty",
	"zzguessing",
	"zzguess_start",
	"zzresynch",
	"zzinf_tokens",
	"zzinf_text",
	"zzinf_text_buffer",
	"zzinf_labase",
	"zzinf_last",
	"zzfill_inf_look",
	"zzFAIL",
	"zzsave_antlr_state",
	"zzrestore_antlr_state",
	"zzsyn",
	"zzset_el",
	"zzset_deg",
	"zzedecode",
	"_zzsetmatch",
	"_zzmatch",
	"_inf_zzgettok",
    "zzconsumeUntil", 
    "zzconsumeUntilToken",
    "_zzmatch_wsig",
    "_zzsetmatch_wsig",
    "_zzmatch_wdfltsig",
    "_zzsetmatch_wdfltsig",
	"zzdflthandlers",
/* DLG stuff */
	"zzreal_line",
	"zzcharfull",
	"zzerr",
	"zzlextext",
	"zzbegexpr",
	"zzendexpr",
	"zzbufsize",
	"zzbegcol",
	"zzendcol",
	"zzline",
	"zzchar",
	"zzbufovf",
	"zzrdstream",
	"zzrdfunc",
	"zzrdstr",
	"zzclose_stream",
	"zzsave_dlg_state",
	"zzrestore_dlg_state",
	"zzmode",
	"zzskip",
	"zzmore",
	"zzreplchar",
	"zzreplstr",
	"zzgettok",
	"zzadvance",
	"zzerrstd",
	"zzerr_in",
	"zzconstr_attr",
	"zzempty_attr",
	"zzerraction",
	"zztokens",			/* list of token regular expressions */
	"dfa",
	"accepts",
	"actions",
	NULL		/* must be present */
};

/* list of PCCTS supplied support functions; these are renamed when more than
 * one ANTLR-generated parsers are linked together to avoid name conflicts.
 */
char *ASTSymbols[] = {
	"AST",
	"zzast_sp",
	"zzastStack",
	"zzlink",
	"zzastnew",
	"zzsubchild",
	"zzsubroot",
	"zzpre_ast",
	"zzfree_ast",
	"zztmake",
	"zzdup_ast",
	"zztfree",
	"zzdouble_link",
	NULL		/* must be present */
};

/* Current ambiguity examination information */
int CurAmbigAlt1, CurAmbigAlt2, CurAmbigline, CurAmbigfile;
char *CurAmbigbtype;


						/* M e t h o d  T a b l e s */
/*
 * The following tables are used to fill syntax diagram nodes with the correct
 * function pointers for computing FIRST sets and printing themselves.
 */

/* fpTraverse[node type] == pointer to function that calculates trees
 * representing the FIRST sets for that node (maintains spatial info).
 * We use 'struct _tree' not 'tree' due to a g++ 2.4.3 bug.
 */
#ifdef __cplusplus
struct _tree *(*fpTraverse[NumNodeTypes+1])(... /* Node *, int, set * */) = {
	NULL,
	(struct _tree *(*)(...)) tJunc,
	(struct _tree *(*)(...)) tRuleRef,
	(struct _tree *(*)(...)) tToken,
	(struct _tree *(*)(...)) tAction
};
#else
Tree *(*fpTraverse[NumNodeTypes+1])() = {
	NULL,
	tJunc,
	tRuleRef,
	tToken,
	tAction
};
#endif

/* fpReach[node type] == pointer to function that calculates FIRST set for
 * that node. (r stands for reach).  We use 'struct _set' not 'set'
 * due to a g++ 2.4.3 bug.
 */
#ifdef __cplusplus
struct _set (*fpReach[NumNodeTypes+1])(... /* Node *, int, set * */) = {
	NULL,
	(struct _set (*)(...)) rJunc,
	(struct _set (*)(...)) rRuleRef,
	(struct _set (*)(...)) rToken,
	(struct _set (*)(...)) rAction
};
#else
set (*fpReach[NumNodeTypes+1])() = {
	NULL,
	rJunc,
	rRuleRef,
	rToken,
	rAction
};
#endif

/* fpPrint[node type] == pointer to function that knows how to print that node. */
#ifdef __cplusplus
void (*fpPrint[NumNodeTypes+1])(... /* Node * */) = {
	NULL,
	(void (*)(...)) pJunc,
	(void (*)(...)) pRuleRef,
	(void (*)(...)) pToken,
	(void (*)(...)) pAction
};
#else
void (*fpPrint[NumNodeTypes+1])() = {
	NULL,
	pJunc,
	pRuleRef,
	pToken,
	pAction
};
#endif

char *decodeJType[] = {
	"invalid",
	"aSubBlk",
	"aOptBlk",
	"aLoopBlk",
	"EndBlk",
	"RuleBlk",
	"Generic",
	"EndRule",
	"aPlusBlk",
	"aLoopBegin"
};


							/* H a s h  T a b l e s */

Entry	**Tname,			/* Table of all token names (maps name to tok num)*/
		**Texpr,			/* Table of all token expressions
							   (maps expr to tok num) */
		**Rname,			/* Table of all Rules (has ptr to start of rule) */
		**Fcache,			/* Cache of First/Follow Computations */
		**Tcache;			/* Tree cache; First/Follow for permute trees */
Entry	**Elabel;			/* Table of all element label names */
Entry	**Sname;			/* Signal names */


							/* V a r i a b l e s */

int		EpToken=0;			/* Imaginary Epsilon token number */
int		WildCardToken=0;
int		CurFile= -1;		/* Index into FileStr table */
char	*CurRule=NULL;		/* Pointer to current rule name */
RuleEntry *CurRuleNode=NULL;/* Pointer to current rule node in syntax tree */
char	*CurRetDef=NULL;	/* Pointer to current return type definition */
char	*CurParmDef=NULL;	/* Pointer to current parameter definition */
Junction *CurRuleBlk=NULL;	/* Pointer to current block node for enclosing block */
ListNode *CurExGroups=NULL;	/* Current list of exception groups for rule/alts */
ListNode *CurElementLabels=NULL;
int		CurBlockID=0;		/* Unique int for each block */
int		CurAltNum=0;
Junction *CurAltStart = NULL;	/* Junction node that starts the alt */
int		NumRules=0;			/* Rules are from 1 to n */
FILE	*output=NULL;		/* current parser output file */
FILE	*input=NULL;		/* current grammar input file */
char	*FileStr[MaxNumFiles];/* Ptr to array of file names on command-line */
int		NumFiles=0;			/* current grammar file number */
#ifdef __cplusplus
void	(**fpTrans)(...),	/* array of ptrs to funcs that translate nodes */
	 	(**fpJTrans)(...);	/*  ... that translate junctions */
#else
void	(**fpTrans)(),		/* array of ptrs to funcs that translate nodes */
	 	(**fpJTrans)();		/*  ... that translate junctions */
#endif
int		**FoStack;			/* Array of LL_k ptrs to stacks of rule numbers */
int		**FoTOS;			/* FOLLOW stack top-of-stack pointers */
Junction *SynDiag = NULL;	/* Pointer to start of syntax diagram */
int		BlkLevel=1;			/* Current block level.  Set by antlr.g, used by
							 * scanner to translate $i.j attributes */
set		reserved_positions;	/* set of token positions reserved by '#token T=i' cmds */
set		all_tokens;			/* set of all token types */
set		imag_tokens;		/* set of all imaginary token types (EpToken, errclasses...) */
set		tokclasses;			/* set of all token class token types */
ListNode *ForcedTokens = 0;	/* list of token_id/token_num pairs to remap */
ListNode *MetaTokenNodes=NULL; /* list of meta token refs such as token classes etc... */
int		*TokenInd=NULL;		/* an indirection level between token num and position
							 * of that token def in TokenStr and ExprStr */
int		LastTokenCounted=0;	/* ==TokenNum if no token renumbering (same as old TokenNum) */
int		TokenNum=TokenStart;
char	**TokenStr=NULL;	/* map token # to token name */
char	**ExprStr=NULL;		/* map token # to expr */
Junction **RulePtr=NULL;	/* map rule # to RuleBlk node of rule */
ListNode *ExprOrder=NULL;	/* list of exprs as they are found in grammar */
ListNode *BeforeActions=NULL;/* list of grammar actions before rules */
ListNode *AfterActions=NULL;/* list of grammar actions after rules */
ListNode *LexActions=NULL;	/* list of lexical actions */
ListNode **Cycles=NULL;		/* list of cycles (for each k) found when
							   doing FOLLOWs */
ListNode *eclasses=NULL;	/* list of error classes */
ListNode *tclasses=NULL;	/* list of token classes */
LClass	 lclass[MaxLexClasses]; /* array of lex class definitions */
int		 CurrentLexClass;	/* index into lclass */
int		 NumLexClasses=0;	/* in range 1..MaxLexClasses (init 0) */

char	*HdrAction=NULL;	/* action defined with #header */
FILE	*ErrFile;			/* sets and error recovery stuff */
FILE	*DefFile=NULL;		/* list of tokens, return value structs, setwd defs */
int		CannotContinue=FALSE;
int		OutputLL_k = 1;		/* LL_k for parsing must be power of 2 */
int		action_file;		/* used to track start of action */
int		action_line;
int		FoundGuessBlk=0;	/* there is a (...)? block somewhere in grammar */
int		FoundException=0;	/* there is an exception somewhere in grammar */
int		pLevel=0;			/* print Level */
int		pAlt1,pAlt2;		/* print "==>" in front of these alts */

/* C++ output stuff */
FILE	*Parser_h,			/* where subclass of ANTLRParser goes */
		*Parser_c;			/* where code for subclass of ANTLRParser goes */
char	Parser_h_Name[MaxFileName+1] = "";
char	Parser_c_Name[MaxFileName+1] = "";

/* list of actions inside the #class {...} defs */
ListNode *class_before_actions=NULL;
ListNode *class_after_actions=NULL;

char	CurrentClassName[MaxRuleName]="";
int		no_classes_found=1;
char	*UserTokenDefsFile;
int		UserDefdTokens=0;	/* found #tokdefs? */
char	*OutputDirectory=TopDirectory;
ExceptionGroup *DefaultExGroup = NULL;
int		NumSignals = NumPredefinedSignals;
int		ContextGuardTRAV=0;


					/* C m d - L i n e  O p t i o n s */

int		LL_k=1;				/* how many tokens of full lookahead */
int		CLL_k= -1;			/* how many tokens of compressed lookahead */
int		PrintOut = FALSE;	/* print out the grammar */
int		PrintAnnotate = FALSE;/* annotate printout with FIRST sets */
int		CodeGen=TRUE;		/* Generate output code? */
int		LexGen=TRUE;		/* Generate lexical files? (tokens.h, parser.dlg) */
int		GenAST=FALSE;		/* Generate AST's? */
int		GenANSI=FALSE;		/* Generate ANSI code where necessary */
int		GenExprSets=TRUE;	/* use sets not (LA(1)==tok) expression lists */
int		GenCR=FALSE;		/* Generate cross reference? */
int		GenLineInfo=FALSE;	/* Generate # line "file" stuff? */
int		TraceGen=FALSE;		/* Generate code to trace rule invocation */
int		elevel=1;			/* error level for ambiguity messages */
int		GenEClasseForRules=0;/* don't generate eclass for each rule */
int		TreeResourceLimit= -1;/* don't limit tree resource */
int		DemandLookahead = 0;/* demand/delayed lookahead or not */
char	*RulePrefix = "";	/* prefix each generated rule with this */
char	*stdpccts = "stdpccts.h";/* where to generate std pccts include file */
int		GenStdPccts = 0;	/* don't gen stdpccts.h? */
int		ParseWithPredicates = 1;
int		WarningLevel = 1;
int		HoistPredicateContext = 0;
int		GenCC = 0;			/* Generate C++ output */

/* DontCopyTokens and Pragma_DupLabeledTokens were a bad idea.  I've just
   turned them off rather than backpatching the code.  Who knows?  We
   may need them in the future.
 */
int		DontCopyTokens = 1;	/* in C++, don't copy ANTLRToken passed to ANTLR */