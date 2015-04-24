/*
*******************************************************

        Symbolic Circuit Analysis With DDDs
                (*** SCAD3 ***)

   Xiang-Dong (Sheldon) Tan, 1998 (c) Copyright

   Electrical Engineering, Univ. of Washington
   Electrical Computer Engineering, Univ. of Iowa

   email: xtan@ee.washington.edu and xtan@eng.uiowa.edu
   date: 10/26/1998 -> %G%
*******************************************************
*/

/*
 *    $RCSfile: spgram.g,v $
 *    $Revision: 1.3 $
 *    $Date: 2002/09/29 17:06:48 $
 */

/* This is test.g which tests a simple DLG-based scanner
 * with user-defined tokens
 */

#header <<#include "globals.h">>

<<
#include "DLGLexer.h"		/* include definition of DLGLexer.
				* This cannot be generated automatically because
				* ANTLR has no idea what you will call this file
				* with the DLG command-line options.
				*/

typedef enum { 
	cPrint,
	cPlot,
	cSubCkt,
	cCktCall,
	cUnknown
} OutputCmd;

static OutputCmd cur_cmd = cUnknown;

#include "globals.h"
#include "circuits.h"
#include "ckt_call.h"
#include "acan.h"

typedef ANTLRCommonToken ANTLRToken;

int
read_input(char *filename )
{
	FILE *fp;
	fp = fopen(filename,"r");
	if(!fp){
		sprintf(_buf,"Can't open file %s",filename);
		error_mesg(INT_ERROR,_buf);
		return -1;
	}
	setbuf(stderr,0);
	ANTLRToken aToken; 
	DLGFileInput in(fp);
	DLGLexer scan(&in);
	ANTLRTokenBuffer pipe(&scan);
	scan.setToken(&aToken);
	sp_parser parser(&pipe);
	parser.init();
	parser.input();
	return 0;
}

>>


#lexclass START
#token TITTLE	"~[\n]*" <<mode(NORMAL); newline();>>

#lexclass PAR_STATE
#token "[\ \t]+"	<<skip();>>
#token VALUE		"{(\+|\-)}[0-9]*{[\.]}[0-9]+{[eE]{(\+ | \-)}[0-9]+}{([Tt]|[Gg]|[Mm][Ee][Gg]|[Kk]|[Mm][Ii][Ll]|[Mm]|[Uu]|[Nn]|[Pp]|[Ff]|[Vv])[a-zA-Z]*}"
#token NEWLINE "\n" <<mode(NORMAL); newline();>>
#token COMMENT		"\*(~[\n])*" 
//#token DOT		"\."
#token IC		"[Ii][Cc]"
#token TEMP		"[Tt][Ee][Mm][Pp]"
#tokclass ONOFF		{"[Oo][Nn]" "[Oo][Ff][Ff]"}
#token AC		"[Aa][Cc]" // AC analysis
#token DC		"[Dd][Cc]" // DC analysis
#token TRAN		"[Tt][Rr][Aa][Nn]" // Transient analysis
#token EQUAL		"="
#token COMMA		","
#token LEFTB		"\("
#token RIGHTB		"\)"
#token VAR_NAME		"[VvIi]{([Rr] | [Ii] | [Mm] | [Pp] | [Dd][Bb])}"
#token IDENTIFIER	"[a-zA-Z][a-zA-Z0-9_\-\+\.]*"


/* general definition */


#lexclass NORMAL
#token "[\ \t]+"	<<skip();>>
#token COMMENT		"\*(~[\n])*" 
#token NEWLINE 		"\n" <<newline();>>
#token VALUE		"{(\+|\-)}[0-9]*{[\.]}[0-9]+{[eE]{(\+ | \-)}[0-9]+}{([Tt]|[Gg]|[Mm][Ee][Gg]|[Kk]|[Mm][Ii][Ll]|[Mm]|[Uu]|[Nn]|[Pp]|[Ff]|[Vv])[a-zA-Z]*}"
#token EQUAL		"="
#token COMMA		","
#token DOT		"\."
#token TEMP		"[Tt][Ee][Mm][Pp]"
/*
#tokclass ONOFF		{"[Oo][Nn]" "[Oo][Ff][Ff]"}
*/


/* analsys */
#token AC		"[Aa][Cc]" <<mode(PAR_STATE);>>// AC analysis
#token DC		"[Dd][Cc]" // DC analysis
#token OP		"[Oo][Pp]" // operation point analysis
#token SENS		"[Ss][Ee][Nn][Ss]" <<mode(PAR_STATE);>> // Sensitivity analysis
#token TF		"[Tt][Ff]" // Transfer Function analysis
#token TRAN		"[Tt][Rr][Aa][Nn]" // Transient analysis
#token NOISE	"[Nn][Oo][Ii][Ss][Ee]" <<mode(PAR_STATE);>> // Noise analysis

/* Batch output */
#token SAVE		"[Ss][Aa][Vv][Ee]" // save lines
#token PRINT		"[Pp][Rr][Ii][Nn][Tt]" <<mode(PAR_STATE);>>// print lines
#token PLOT		"[Pp][Ll][Oo][Tt]" <<mode(PAR_STATE);>>// print lines

#token END		"[Ee][Nn][Dd]"
#token ENDS		"[Ee][Nn][Dd][Ss]"

/* output control */
#token MODEL	"[Mm][Oo][Dd][Ee][Ll]" <<mode(PAR_STATE);>> 
#token SUBCKT	"[Ss][Uu][Bb][Cc][Kk][Tt]" <<mode(PAR_STATE);>>
#token OPTION	"[Oo][Pp][Tt][Ii][Oo][Nn]{[Ss]}" <<mode(PAR_STATE);>>
#token NODESET	"[Nn][Oo][Dd][Ee][Ss][Ee][Tt]"<<mode(PAR_STATE);>>
#token IC	"[Ii][Cc]" <<mode(PAR_STATE);>>

/* independent source functions */
#tokclass PULSE		{"PULSE" "pulse"}
#tokclass SIN		{"SIN" "sin"}
#tokclass EXP		{"EXP" "exp"}
#tokclass PWL		{"PWL" "pwl"}
#tokclass SFFM		{"SFFM" "sffm"}

/* basic passive elements */
#token RES		"[Rr][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// resistor
#token CAP		"[Cc][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// capacitor
#token IND		"[Ll][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// inductor
#token MUIND    "[Kk][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// mutual inductor

/* independent sources */
#token VOL		"[Vv][a-zA-Z0-9_\-\+]*" // independent voltage
#token CUR		"[Ii][a-zA-Z0-9_\-\+]*" // independent current


/* linear controlled source */
#token	VCCS 	"[Gg][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// VCCS
#token  VCVS	"[Ee][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// VCVS
#token 	CCCS	"[Ff][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// CCCS
#token 	CCVS	"[Hh][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// CCVS

/* basic nonlinear devices */
#token 	DIODE	"[Dd][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// Diodes
#token 	BJT		"[Qq][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// Bipolar Junction Transistor
#token 	JFET	"[Jj][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// Junction Field-Effect Transistor 
#token  MOSFET	"[Mm][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// MOSFET
#token  MESFET	"[Zz][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// MOSFET
#token  CKTCALL	"[Xx][a-zA-Z0-9_\-\+]*" <<mode(PAR_STATE);>>// MOSFET

#token IDENTIFIER	"[a-zA-Z][a-zA-Z0-9_\-\+]*" 

class sp_parser {				/* Define a grammar class */

input	: title (body)+ "@";

title	: tt:TITTLE 
		  <<fprintf(stderr,"%s\n",$tt->getText());
			theCurCkt->add_title($tt->getText());>>
		  NEWLINE
		  ;

body	: NEWLINE 
		| cm:COMMENT NEWLINE

		/* independent voltage source */
		| vk:VOL vn1:VALUE vn2:VALUE 
		  {(DC|TRAN) VALUE} 
		  {AC {vv:VALUE {VALUE}}}
		  << theCurCkt->parse_vi($vk->getText(), 
				$vn1->getText(), $vn2->getText(), 
				($vv != NULL ?$vv->getText():NULL));>>
		  {("DISTOF1"|"distof1") {VALUE {VALUE}}}
		  {("DISTOF2"|"distof2") {VALUE {VALUE}}}
		  {indep_source}
		  NEWLINE 

		/* independent current source */
		| ik:CUR in1:VALUE in2:VALUE 
		  {(DC|TRAN) VALUE} 
		  {AC {iv:VALUE {VALUE}}}
		  << theCurCkt->parse_vi($ik->getText(), 
				$in1->getText(), $in2->getText(), 
				($iv != NULL ? $iv->getText():NULL)); >>
		  {("DISTOF1"|"distof1") {VALUE {VALUE}}}
		  {("DISTOF2"|"distof2") {VALUE {VALUE}}}
		  {indep_source}
		  NEWLINE 

		/* linear controlled source */
		| vccsk:VCCS vccsn1:VALUE vccsn2:VALUE vccscn1:VALUE 
			vccscn2:VALUE (vccsv:VALUE | vccsvar:IDENTIFIER) NEWLINE
		<<theCurCkt->parse_vcs($vccsk->getText(), $vccsn1->getText(), 
			$vccsn2->getText(), $vccscn1->getText(), 
			$vccscn2->getText(), 
			($vccsv != NULL ?$vccsv->getText():NULL),
			($vccsvar != NULL ?$vccsvar->getText():NULL));>>
				  
		| vcvsk:VCVS vcvsn1:VALUE vcvsn2:VALUE vcvscn1:VALUE 
			vcvscn2:VALUE (vcvsv:VALUE | vcvsvar:IDENTIFIER) NEWLINE

		<<theCurCkt->parse_vcs($vcvsk->getText(), $vcvsn1->getText(), 
			$vcvsn2->getText(), $vcvscn1->getText(),
			$vcvscn2->getText(), 
			($vcvsv != NULL ?$vcvsv->getText():NULL),
			($vcvsvar != NULL ?$vcvsvar->getText():NULL));>>

		| cccsk:CCCS cccsn1:VALUE  cccsn2:VALUE  cccscid:IDENTIFIER 
			(cccsv:VALUE | cccsvar:IDENTIFIER) NEWLINE
		<<theCurCkt->parse_ccs($cccsk->getText(), $cccsn1->getText(), 
			$cccsn2->getText(), $cccscid->getText(), 
			($cccsv != NULL ?$cccsv->getText():NULL),
			($cccsvar != NULL ?$cccsvar->getText():NULL));>>

		| ccvsk:CCVS ccvsn1:VALUE ccvsn2:VALUE ccvscid:IDENTIFIER 
			(ccvsv:VALUE | ccvsvar:IDENTIFIER) NEWLINE

		<<theCurCkt->parse_ccs($ccvsk->getText(), $ccvsn1->getText(), 
			$ccvsn2->getText(), $ccvscid->getText(), 
			($ccvsv != NULL ?$ccvsv->getText():NULL),
			($ccvsvar != NULL ?$ccvsvar->getText():NULL));>>

		/* basic linear device */
		| rk:RES rn1:VALUE rn2:VALUE (rv:VALUE | rvar:IDENTIFIER) NEWLINE
		  <<	theCurCkt->parse_rlc($rk->getText(), $rn1->getText(), 
			$rn2->getText(), 
			($rv != NULL ?$rv->getText():NULL), 
			($rvar != NULL ?$rvar->getText():NULL)); >>

		| ck:CAP cn1:VALUE cn2:VALUE (cv:VALUE | cvar:IDENTIFIER) NEWLINE
		  <<	theCurCkt->parse_rlc($ck->getText(), $cn1->getText(), 
			$cn2->getText(), 
			($cv != NULL ?$cv->getText():NULL), 
			($cvar != NULL ?$cvar->getText():NULL)); >>

		| indk:IND indn1:VALUE indn2:VALUE (indv:VALUE | indvar:IDENTIFIER) 
			NEWLINE
		  <<	theCurCkt->parse_rlc($indk->getText(), 
			$indn1->getText(), $indn2->getText(), 
			($indv != NULL ?$indv->getText():NULL),
			($indvar != NULL ?$indvar->getText():NULL)); >>

		/* for the mutual indutance */
		| muindk:MUIND L1id:IDENTIFIER L2id:IDENTIFIER Kval:VALUE
		  << theCurCkt->parse_mutual_inductor($muindk->getText(),
	         $L1id->getText(), $L2id->getText(), $Kval->getText()); >>

		/* basic nonlinear device */
		| dk:DIODE dn1:VALUE dn2:VALUE dm:IDENTIFIER {VALUE} {ONOFF}
		{IC EQUAL VALUE} {TEMP EQUAL VALUE} NEWLINE
		<<	theCurCkt->parse_diode($dk->getText(), 
			$dn1->getText(), $dn2->getText(), $dm->getText());>>

		| bjk:BJT bjn1:VALUE bjn2:VALUE bjn3:VALUE {bjn4:VALUE} bjm:IDENTIFIER
		  {VALUE} {ONOFF} {IC EQUAL VALUE COMMA VALUE} 
		  {TEMP EQUAL VALUE } NEWLINE
		<<	theCurCkt->parse_bjt($bjk->getText(), $bjn1->getText(), 
			$bjn2->getText(), $bjn3->getText(), 
			($bjn4 != NULL ? $bjn4->getText():NULL), 
			$bjm->getText());>>

		| jfk:JFET	jfn1:VALUE jfn2:VALUE jfn3:VALUE jfm:IDENTIFIER
		  {VALUE} {ONOFF} {IC EQUAL VALUE COMMA VALUE}
		  {TEMP EQUAL VALUE } NEWLINE
		<<	theCurCkt->parse_jfet($jfk->getText(), 
			$jfn1->getText(), $jfn2->getText(),
		  	$jfn3->getText(), $jfm->getText());>>

		| mosk:MOSFET mosn1:VALUE mosn2:VALUE mosn3:VALUE mosn4:VALUE 
			mosm:IDENTIFIER (parameter_list)* NEWLINE
		<<	theCurCkt->parse_mosfet($mosk->getText(), 
				$mosn1->getText(), $mosn2->getText(),
				$mosn3->getText(), $mosn4->getText(), 
				$mosm->getText());>>

		// subcircuit call
		| callid:CKTCALL 
		  << cout <<"begin cktcall -- "<<endl;
			theCurCkt->parse_ckt_call($callid->getText());
			cur_cmd = cCktCall;>>
		  (callnd:VALUE 
		  << theCurCkt->parse_ckt_call_node($callnd->getText());>>
		  )+
			callnm:IDENTIFIER 
		  << theCurCkt->cur_call->match_sub_ckt(
					$callnm->getText(), theCkt->sub_ckt); >>
			{(variable_list)+}
			NEWLINE
		  << cur_cmd = cUnknown;>>
		
		/* dot command */
		| DOT dot_command
		;

dot_command :	

		MODEL IDENTIFIER IDENTIFIER 
		  "\(" (parameter_list)+ "\)" NEWLINE

		| SUBCKT subid:IDENTIFIER 
		<<  cout <<"subckt: "<<$subid->getText()<<endl; 
			theCurCkt = theCurCkt->parse_subckt(
						$subid->getText());
			cur_cmd = cSubCkt;>>
		(internd:VALUE 
		<< theCurCkt->parse_inter_node($internd->getText()); >>
		)+

		{(variable_list)+}
		NEWLINE
		<< cur_cmd = cUnknown; >>

		/* simulation variable */
		| OPTION (mix_para_list)+ NEWLINE

		/* initial condition */

		| AC acvar:IDENTIFIER acnp:VALUE acfb:VALUE acfe:VALUE NEWLINE
			<<theCurCkt->parse_ac_cmd($acvar->getText(),
			$acnp->getText(),
			$acfb->getText(),
			$acfe->getText());>>

		| NOISE ns:IDENTIFIER NEWLINE
		   <<theCurCkt->read_nz_sources($ns->getText()); >>

		| DC (VOL | CUR) VALUE VALUE VALUE NEWLINE

		| OP NEWLINE

		| SENS outvar AC IDENTIFIER VALUE VALUE VALUE NEWLINE

		| TF outvar NEWLINE

		| TRAN VALUE VALUE {VALUE {VALUE}} NEWLINE


		/* batch output */
		| SAVE (IDENTIFIER)+ NEWLINE

		| PRINT << cur_cmd = cPrint; >>
			{analysis}  (outvar)+ NEWLINE
			<< cur_cmd = cUnknown; >>

		| PLOT << cur_cmd = cPlot; >> 
			{analysis} (outvar)+ NEWLINE
			<< cur_cmd = cUnknown; >>

		| ENDS {IDENTIFIER} NEWLINE
		  << cout << "change current ckt"<<endl;
			theCurCkt = theCurCkt->parent; 
			if(!theCurCkt)
				error_mesg(SYN_ERROR,"Invalid .ENDS command");
		  >>

		| END NEWLINE

		;
analysis	: AC
		|DC
		|TRAN
		;

indep_source	:
		PULSE "\(" VALUE VALUE VALUE VALUE VALUE VALUE VALUE "\)"
		|SIN "\(" VALUE VALUE VALUE VALUE VALUE "\)"
		|EXP "\(" VALUE VALUE VALUE VALUE VALUE VALUE"\)"
		|PWL "\(" (value_list)+ "\)"
		|SFFM "\(" VALUE VALUE VALUE VALUE VALUE "\)" 
		;

value_list	: VALUE	VALUE
		;

variable_list	:varnm:IDENTIFIER EQUAL varval:VALUE
		<< if(cur_cmd == cSubCkt)
			theCurCkt->parse_deflt_value(
				$varnm->getText(), $varval->getText());
		    else if(cur_cmd == cCktCall)
			theCurCkt->parse_ckt_call_param(
				$varnm->getText(), $varval->getText());
		>>
		;

parameter_list 	:IDENTIFIER EQUAL VALUE
		|IC EQUAL VALUEA {COMMA VALUEA {COMMA VALUEA}}
		|ONOFF
		|TEMP EQUAL	VALUE
		;

outvar          :varnm:VAR_NAME LEFTB varn1:VALUE {COMMA varn2:VALUE} RIGHTB
		<< if(cur_cmd == cPrint || cur_cmd == cPlot){
		    // we only allow one output now
		    if(!theCurCkt->ac_mgr->get_output_num()){
			theCurCkt->parse_print($varnm->getText(), 
			$varn1->getText(), 
			($varn2 != NULL? $varn2->getText():NULL));
		    } 
		}
		>>
		;
					
mix_para_list	: IDENTIFIER {(EQUAL VALUE)}
				;

}
