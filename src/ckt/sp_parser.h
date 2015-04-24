/*
 * sp_parser: P a r s e r  H e a d e r 
 *
 * Generated from: spgram.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-1995
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * with AHPCRC, University of Minnesota
 * ANTLR Version 1.33
 */

#ifndef sp_parser_h
#define sp_parser_h
#include "AParser.h"

#include "globals.h"  
class sp_parser : public ANTLRParser {
protected:
	static ANTLRChar *_token_tbl[];
private:
	static SetWordType err1[12];
	static SetWordType err2[12];
	static SetWordType err3[12];
	static SetWordType err4[12];
	static SetWordType setwd1[74];
	static SetWordType err5[12];
	static SetWordType err6[12];
	static SetWordType err7[12];
	static SetWordType err8[12];
	static SetWordType err9[12];
	static SetWordType err10[12];
	static SetWordType err11[12];
	static SetWordType err12[12];
	static SetWordType err13[12];
	static SetWordType ONOFF_set[12];
	static SetWordType err15[12];
	static SetWordType setwd2[74];
	static SetWordType err16[12];
	static SetWordType err17[12];
	static SetWordType err18[12];
	static SetWordType PULSE_set[12];
	static SetWordType SIN_set[12];
	static SetWordType EXP_set[12];
	static SetWordType setwd3[74];
	static SetWordType PWL_set[12];
	static SetWordType SFFM_set[12];
	static SetWordType err24[12];
	static SetWordType err25[12];
	static SetWordType setwd4[74];
	static SetWordType setwd5[74];
private:
	void zzdflthandlers( int _signal, int *_retsignal );

public:
	sp_parser(ANTLRTokenBuffer *input);
	void input(void);
	void title(void);
	void body(void);
	void dot_command(void);
	void analysis(void);
	void indep_source(void);
	void value_list(void);
	void variable_list(void);
	void parameter_list(void);
	void outvar(void);
	void mix_para_list(void);
};

#endif /* sp_parser_h */
