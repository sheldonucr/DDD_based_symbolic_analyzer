/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-1994
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33
 */
#include <stdio.h>
#define ANTLR_VERSION	133
#include "tokens.h"
#include "globals.h"  
#include "AParser.h"
#include "sp_parser.h"
#include "DLexerBase.h"
#include "ATokPtr.h"
#ifndef PURIFY
#define PURIFY(r,s)
#endif

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

  

void
sp_parser::input(void)
{
	zzRULE;
	title();
	{
		int zzcnt=1;
		do {
			body();
		} while ( (setwd1[LA(1)]&0x1) );
	}
	zzmatch(1);
	 consume();
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x2);
}

void
sp_parser::title(void)
{
	zzRULE;
	ANTLRTokenPtr tt=NULL;
	zzmatch(TITTLE);
	
	tt = (ANTLRTokenPtr)LT(1);

	fprintf(stderr,"%s\n", tt->getText());
	theCurCkt->add_title( tt->getText());  
 consume();
	zzmatch(NEWLINE);
	 consume();
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x4);
}

void
sp_parser::body(void)
{
	zzRULE;
	ANTLRTokenPtr cm=NULL, vk=NULL, vn1=NULL, vn2=NULL, vv=NULL, ik=NULL, in1=NULL, in2=NULL, iv=NULL, vccsk=NULL, vccsn1=NULL, vccsn2=NULL, vccscn1=NULL, vccscn2=NULL, vccsv=NULL, vccsvar=NULL, vcvsk=NULL, vcvsn1=NULL, vcvsn2=NULL, vcvscn1=NULL, vcvscn2=NULL, vcvsv=NULL, vcvsvar=NULL, cccsk=NULL, cccsn1=NULL, cccsn2=NULL, cccscid=NULL, cccsv=NULL, cccsvar=NULL, ccvsk=NULL, ccvsn1=NULL, ccvsn2=NULL, ccvscid=NULL, ccvsv=NULL, ccvsvar=NULL, rk=NULL, rn1=NULL, rn2=NULL, rv=NULL, rvar=NULL, ck=NULL, cn1=NULL, cn2=NULL, cv=NULL, cvar=NULL, indk=NULL, indn1=NULL, indn2=NULL, indv=NULL, indvar=NULL, muindk=NULL, L1id=NULL, L2id=NULL, Kval=NULL, dk=NULL, dn1=NULL, dn2=NULL, dm=NULL, bjk=NULL, bjn1=NULL, bjn2=NULL, bjn3=NULL, bjn4=NULL, bjm=NULL, jfk=NULL, jfn1=NULL, jfn2=NULL, jfn3=NULL, jfm=NULL, mosk=NULL, mosn1=NULL, mosn2=NULL, mosn3=NULL, mosn4=NULL, mosm=NULL, callid=NULL, callnd=NULL, callnm=NULL;
	if ( (LA(1)==NEWLINE) ) {
		zzmatch(NEWLINE);
		 consume();
	}
	else {
		if ( (LA(1)==COMMENT) ) {
			zzmatch(COMMENT);
			
			cm = (ANTLRTokenPtr)LT(1);
 consume();
			zzmatch(NEWLINE);
			 consume();
		}
		else {
			if ( (LA(1)==VOL) ) {
				zzmatch(VOL);
				
				vk = (ANTLRTokenPtr)LT(1);
 consume();
				zzmatch(VALUE);
				
				vn1 = (ANTLRTokenPtr)LT(1);
 consume();
				zzmatch(VALUE);
				
				vn2 = (ANTLRTokenPtr)LT(1);
 consume();
				{
					if ( (setwd1[LA(1)]&0x8) ) {
						{
							if ( (LA(1)==DC) ) {
								zzmatch(DC);
								 consume();
							}
							else {
								if ( (LA(1)==TRAN) ) {
									zzmatch(TRAN);
									 consume();
								}
								else {FAIL(1,err1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
							}
						}
						zzmatch(VALUE);
						 consume();
					}
				}
				{
					if ( (LA(1)==AC) ) {
						zzmatch(AC);
						 consume();
						{
							if ( (LA(1)==VALUE) ) {
								zzmatch(VALUE);
								
								vv = (ANTLRTokenPtr)LT(1);
 consume();
								{
									if ( (LA(1)==VALUE) ) {
										zzmatch(VALUE);
										 consume();
									}
								}
							}
						}
					}
				}
				theCurCkt->parse_vi( vk->getText(), 
				vn1->getText(),  vn2->getText(), 
				( vv != NULL ? vv->getText():NULL));  
				{
					if ( (setwd1[LA(1)]&0x10) ) {
						{
							if ( (LA(1)==67) ) {
								zzmatch(67);
								 consume();
							}
							else {
								if ( (LA(1)==68) ) {
									zzmatch(68);
									 consume();
								}
								else {FAIL(1,err2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
							}
						}
						{
							if ( (LA(1)==VALUE) ) {
								zzmatch(VALUE);
								 consume();
								{
									if ( (LA(1)==VALUE) ) {
										zzmatch(VALUE);
										 consume();
									}
								}
							}
						}
					}
				}
				{
					if ( (setwd1[LA(1)]&0x20) ) {
						{
							if ( (LA(1)==69) ) {
								zzmatch(69);
								 consume();
							}
							else {
								if ( (LA(1)==70) ) {
									zzmatch(70);
									 consume();
								}
								else {FAIL(1,err3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
							}
						}
						{
							if ( (LA(1)==VALUE) ) {
								zzmatch(VALUE);
								 consume();
								{
									if ( (LA(1)==VALUE) ) {
										zzmatch(VALUE);
										 consume();
									}
								}
							}
						}
					}
				}
				{
					if ( (setwd1[LA(1)]&0x40) ) {
						indep_source();
					}
				}
				zzmatch(NEWLINE);
				 consume();
			}
			else {
				if ( (LA(1)==CUR) ) {
					zzmatch(CUR);
					
					ik = (ANTLRTokenPtr)LT(1);
 consume();
					zzmatch(VALUE);
					
					in1 = (ANTLRTokenPtr)LT(1);
 consume();
					zzmatch(VALUE);
					
					in2 = (ANTLRTokenPtr)LT(1);
 consume();
					{
						if ( (setwd1[LA(1)]&0x80) ) {
							{
								if ( (LA(1)==DC) ) {
									zzmatch(DC);
									 consume();
								}
								else {
									if ( (LA(1)==TRAN) ) {
										zzmatch(TRAN);
										 consume();
									}
									else {FAIL(1,err4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
							zzmatch(VALUE);
							 consume();
						}
					}
					{
						if ( (LA(1)==AC) ) {
							zzmatch(AC);
							 consume();
							{
								if ( (LA(1)==VALUE) ) {
									zzmatch(VALUE);
									
									iv = (ANTLRTokenPtr)LT(1);
 consume();
									{
										if ( (LA(1)==VALUE) ) {
											zzmatch(VALUE);
											 consume();
										}
									}
								}
							}
						}
					}
					theCurCkt->parse_vi( ik->getText(), 
					in1->getText(),  in2->getText(), 
					( iv != NULL ?  iv->getText():NULL));   
					{
						if ( (setwd2[LA(1)]&0x1) ) {
							{
								if ( (LA(1)==67) ) {
									zzmatch(67);
									 consume();
								}
								else {
									if ( (LA(1)==68) ) {
										zzmatch(68);
										 consume();
									}
									else {FAIL(1,err5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
							{
								if ( (LA(1)==VALUE) ) {
									zzmatch(VALUE);
									 consume();
									{
										if ( (LA(1)==VALUE) ) {
											zzmatch(VALUE);
											 consume();
										}
									}
								}
							}
						}
					}
					{
						if ( (setwd2[LA(1)]&0x2) ) {
							{
								if ( (LA(1)==69) ) {
									zzmatch(69);
									 consume();
								}
								else {
									if ( (LA(1)==70) ) {
										zzmatch(70);
										 consume();
									}
									else {FAIL(1,err6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
							{
								if ( (LA(1)==VALUE) ) {
									zzmatch(VALUE);
									 consume();
									{
										if ( (LA(1)==VALUE) ) {
											zzmatch(VALUE);
											 consume();
										}
									}
								}
							}
						}
					}
					{
						if ( (setwd2[LA(1)]&0x4) ) {
							indep_source();
						}
					}
					zzmatch(NEWLINE);
					 consume();
				}
				else {
					if ( (LA(1)==VCCS) ) {
						zzmatch(VCCS);
						
						vccsk = (ANTLRTokenPtr)LT(1);
 consume();
						zzmatch(VALUE);
						
						vccsn1 = (ANTLRTokenPtr)LT(1);
 consume();
						zzmatch(VALUE);
						
						vccsn2 = (ANTLRTokenPtr)LT(1);
 consume();
						zzmatch(VALUE);
						
						vccscn1 = (ANTLRTokenPtr)LT(1);
 consume();
						zzmatch(VALUE);
						
						vccscn2 = (ANTLRTokenPtr)LT(1);
 consume();
						{
							if ( (LA(1)==VALUE) ) {
								zzmatch(VALUE);
								
								vccsv = (ANTLRTokenPtr)LT(1);
 consume();
							}
							else {
								if ( (LA(1)==IDENTIFIER) ) {
									zzmatch(IDENTIFIER);
									
									vccsvar = (ANTLRTokenPtr)LT(1);
 consume();
								}
								else {FAIL(1,err7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
							}
						}
						zzmatch(NEWLINE);
						
						theCurCkt->parse_vcs( vccsk->getText(),  vccsn1->getText(), 
						vccsn2->getText(),  vccscn1->getText(), 
						vccscn2->getText(), 
						( vccsv != NULL ? vccsv->getText():NULL),
						( vccsvar != NULL ? vccsvar->getText():NULL));  
 consume();
					}
					else {
						if ( (LA(1)==VCVS) ) {
							zzmatch(VCVS);
							
							vcvsk = (ANTLRTokenPtr)LT(1);
 consume();
							zzmatch(VALUE);
							
							vcvsn1 = (ANTLRTokenPtr)LT(1);
 consume();
							zzmatch(VALUE);
							
							vcvsn2 = (ANTLRTokenPtr)LT(1);
 consume();
							zzmatch(VALUE);
							
							vcvscn1 = (ANTLRTokenPtr)LT(1);
 consume();
							zzmatch(VALUE);
							
							vcvscn2 = (ANTLRTokenPtr)LT(1);
 consume();
							{
								if ( (LA(1)==VALUE) ) {
									zzmatch(VALUE);
									
									vcvsv = (ANTLRTokenPtr)LT(1);
 consume();
								}
								else {
									if ( (LA(1)==IDENTIFIER) ) {
										zzmatch(IDENTIFIER);
										
										vcvsvar = (ANTLRTokenPtr)LT(1);
 consume();
									}
									else {FAIL(1,err8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
							zzmatch(NEWLINE);
							
							theCurCkt->parse_vcs( vcvsk->getText(),  vcvsn1->getText(), 
							vcvsn2->getText(),  vcvscn1->getText(),
							vcvscn2->getText(), 
							( vcvsv != NULL ? vcvsv->getText():NULL),
							( vcvsvar != NULL ? vcvsvar->getText():NULL));  
 consume();
						}
						else {
							if ( (LA(1)==CCCS) ) {
								zzmatch(CCCS);
								
								cccsk = (ANTLRTokenPtr)LT(1);
 consume();
								zzmatch(VALUE);
								
								cccsn1 = (ANTLRTokenPtr)LT(1);
 consume();
								zzmatch(VALUE);
								
								cccsn2 = (ANTLRTokenPtr)LT(1);
 consume();
								zzmatch(IDENTIFIER);
								
								cccscid = (ANTLRTokenPtr)LT(1);
 consume();
								{
									if ( (LA(1)==VALUE) ) {
										zzmatch(VALUE);
										
										cccsv = (ANTLRTokenPtr)LT(1);
 consume();
									}
									else {
										if ( (LA(1)==IDENTIFIER) ) {
											zzmatch(IDENTIFIER);
											
											cccsvar = (ANTLRTokenPtr)LT(1);
 consume();
										}
										else {FAIL(1,err9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
									}
								}
								zzmatch(NEWLINE);
								
								theCurCkt->parse_ccs( cccsk->getText(),  cccsn1->getText(), 
								cccsn2->getText(),  cccscid->getText(), 
								( cccsv != NULL ? cccsv->getText():NULL),
								( cccsvar != NULL ? cccsvar->getText():NULL));  
 consume();
							}
							else {
								if ( (LA(1)==CCVS) ) {
									zzmatch(CCVS);
									
									ccvsk = (ANTLRTokenPtr)LT(1);
 consume();
									zzmatch(VALUE);
									
									ccvsn1 = (ANTLRTokenPtr)LT(1);
 consume();
									zzmatch(VALUE);
									
									ccvsn2 = (ANTLRTokenPtr)LT(1);
 consume();
									zzmatch(IDENTIFIER);
									
									ccvscid = (ANTLRTokenPtr)LT(1);
 consume();
									{
										if ( (LA(1)==VALUE) ) {
											zzmatch(VALUE);
											
											ccvsv = (ANTLRTokenPtr)LT(1);
 consume();
										}
										else {
											if ( (LA(1)==IDENTIFIER) ) {
												zzmatch(IDENTIFIER);
												
												ccvsvar = (ANTLRTokenPtr)LT(1);
 consume();
											}
											else {FAIL(1,err10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
										}
									}
									zzmatch(NEWLINE);
									
									theCurCkt->parse_ccs( ccvsk->getText(),  ccvsn1->getText(), 
									ccvsn2->getText(),  ccvscid->getText(), 
									( ccvsv != NULL ? ccvsv->getText():NULL),
									( ccvsvar != NULL ? ccvsvar->getText():NULL));  
 consume();
								}
								else {
									if ( (LA(1)==RES) ) {
										zzmatch(RES);
										
										rk = (ANTLRTokenPtr)LT(1);
 consume();
										zzmatch(VALUE);
										
										rn1 = (ANTLRTokenPtr)LT(1);
 consume();
										zzmatch(VALUE);
										
										rn2 = (ANTLRTokenPtr)LT(1);
 consume();
										{
											if ( (LA(1)==VALUE) ) {
												zzmatch(VALUE);
												
												rv = (ANTLRTokenPtr)LT(1);
 consume();
											}
											else {
												if ( (LA(1)==IDENTIFIER) ) {
													zzmatch(IDENTIFIER);
													
													rvar = (ANTLRTokenPtr)LT(1);
 consume();
												}
												else {FAIL(1,err11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
											}
										}
										zzmatch(NEWLINE);
										
										theCurCkt->parse_rlc( rk->getText(),  rn1->getText(), 
										rn2->getText(), 
										( rv != NULL ? rv->getText():NULL), 
										( rvar != NULL ? rvar->getText():NULL));   
 consume();
									}
									else {
										if ( (LA(1)==CAP) ) {
											zzmatch(CAP);
											
											ck = (ANTLRTokenPtr)LT(1);
 consume();
											zzmatch(VALUE);
											
											cn1 = (ANTLRTokenPtr)LT(1);
 consume();
											zzmatch(VALUE);
											
											cn2 = (ANTLRTokenPtr)LT(1);
 consume();
											{
												if ( (LA(1)==VALUE) ) {
													zzmatch(VALUE);
													
													cv = (ANTLRTokenPtr)LT(1);
 consume();
												}
												else {
													if ( (LA(1)==IDENTIFIER) ) {
														zzmatch(IDENTIFIER);
														
														cvar = (ANTLRTokenPtr)LT(1);
 consume();
													}
													else {FAIL(1,err12,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
												}
											}
											zzmatch(NEWLINE);
											
											theCurCkt->parse_rlc( ck->getText(),  cn1->getText(), 
											cn2->getText(), 
											( cv != NULL ? cv->getText():NULL), 
											( cvar != NULL ? cvar->getText():NULL));   
 consume();
										}
										else {
											if ( (LA(1)==IND) ) {
												zzmatch(IND);
												
												indk = (ANTLRTokenPtr)LT(1);
 consume();
												zzmatch(VALUE);
												
												indn1 = (ANTLRTokenPtr)LT(1);
 consume();
												zzmatch(VALUE);
												
												indn2 = (ANTLRTokenPtr)LT(1);
 consume();
												{
													if ( (LA(1)==VALUE) ) {
														zzmatch(VALUE);
														
														indv = (ANTLRTokenPtr)LT(1);
 consume();
													}
													else {
														if ( (LA(1)==IDENTIFIER) ) {
															zzmatch(IDENTIFIER);
															
															indvar = (ANTLRTokenPtr)LT(1);
 consume();
														}
														else {FAIL(1,err13,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
													}
												}
												zzmatch(NEWLINE);
												
												theCurCkt->parse_rlc( indk->getText(), 
												indn1->getText(),  indn2->getText(), 
												( indv != NULL ? indv->getText():NULL),
												( indvar != NULL ? indvar->getText():NULL));   
 consume();
											}
											else {
												if ( (LA(1)==MUIND) ) {
													zzmatch(MUIND);
													
													muindk = (ANTLRTokenPtr)LT(1);
 consume();
													zzmatch(IDENTIFIER);
													
													L1id = (ANTLRTokenPtr)LT(1);
 consume();
													zzmatch(IDENTIFIER);
													
													L2id = (ANTLRTokenPtr)LT(1);
 consume();
													zzmatch(VALUE);
													
													Kval = (ANTLRTokenPtr)LT(1);

													theCurCkt->parse_mutual_inductor( muindk->getText(),
													L1id->getText(),  L2id->getText(),  Kval->getText());   
 consume();
												}
												else {
													if ( (LA(1)==DIODE) ) {
														zzmatch(DIODE);
														
														dk = (ANTLRTokenPtr)LT(1);
 consume();
														zzmatch(VALUE);
														
														dn1 = (ANTLRTokenPtr)LT(1);
 consume();
														zzmatch(VALUE);
														
														dn2 = (ANTLRTokenPtr)LT(1);
 consume();
														zzmatch(IDENTIFIER);
														
														dm = (ANTLRTokenPtr)LT(1);
 consume();
														{
															if ( (LA(1)==VALUE) ) {
																zzmatch(VALUE);
																 consume();
															}
														}
														{
															if ( (setwd2[LA(1)]&0x8) ) {
																zzsetmatch(ONOFF_set);
																 consume();
															}
														}
														{
															if ( (LA(1)==IC) ) {
																zzmatch(IC);
																 consume();
																zzmatch(EQUAL);
																 consume();
																zzmatch(VALUE);
																 consume();
															}
														}
														{
															if ( (LA(1)==TEMP) ) {
																zzmatch(TEMP);
																 consume();
																zzmatch(EQUAL);
																 consume();
																zzmatch(VALUE);
																 consume();
															}
														}
														zzmatch(NEWLINE);
														
														theCurCkt->parse_diode( dk->getText(), 
														dn1->getText(),  dn2->getText(),  dm->getText());  
 consume();
													}
													else {
														if ( (LA(1)==BJT) ) {
															zzmatch(BJT);
															
															bjk = (ANTLRTokenPtr)LT(1);
 consume();
															zzmatch(VALUE);
															
															bjn1 = (ANTLRTokenPtr)LT(1);
 consume();
															zzmatch(VALUE);
															
															bjn2 = (ANTLRTokenPtr)LT(1);
 consume();
															zzmatch(VALUE);
															
															bjn3 = (ANTLRTokenPtr)LT(1);
 consume();
															{
																if ( (LA(1)==VALUE) ) {
																	zzmatch(VALUE);
																	
																	bjn4 = (ANTLRTokenPtr)LT(1);
 consume();
																}
															}
															zzmatch(IDENTIFIER);
															
															bjm = (ANTLRTokenPtr)LT(1);
 consume();
															{
																if ( (LA(1)==VALUE) ) {
																	zzmatch(VALUE);
																	 consume();
																}
															}
															{
																if ( (setwd2[LA(1)]&0x10) ) {
																	zzsetmatch(ONOFF_set);
																	 consume();
																}
															}
															{
																if ( (LA(1)==IC) ) {
																	zzmatch(IC);
																	 consume();
																	zzmatch(EQUAL);
																	 consume();
																	zzmatch(VALUE);
																	 consume();
																	zzmatch(COMMA);
																	 consume();
																	zzmatch(VALUE);
																	 consume();
																}
															}
															{
																if ( (LA(1)==TEMP) ) {
																	zzmatch(TEMP);
																	 consume();
																	zzmatch(EQUAL);
																	 consume();
																	zzmatch(VALUE);
																	 consume();
																}
															}
															zzmatch(NEWLINE);
															
															theCurCkt->parse_bjt( bjk->getText(),  bjn1->getText(), 
															bjn2->getText(),  bjn3->getText(), 
															( bjn4 != NULL ?  bjn4->getText():NULL), 
															bjm->getText());  
 consume();
														}
														else {
															if ( (LA(1)==JFET) ) {
																zzmatch(JFET);
																
																jfk = (ANTLRTokenPtr)LT(1);
 consume();
																zzmatch(VALUE);
																
																jfn1 = (ANTLRTokenPtr)LT(1);
 consume();
																zzmatch(VALUE);
																
																jfn2 = (ANTLRTokenPtr)LT(1);
 consume();
																zzmatch(VALUE);
																
																jfn3 = (ANTLRTokenPtr)LT(1);
 consume();
																zzmatch(IDENTIFIER);
																
																jfm = (ANTLRTokenPtr)LT(1);
 consume();
																{
																	if ( (LA(1)==VALUE) ) {
																		zzmatch(VALUE);
																		 consume();
																	}
																}
																{
																	if ( (setwd2[LA(1)]&0x20) ) {
																		zzsetmatch(ONOFF_set);
																		 consume();
																	}
																}
																{
																	if ( (LA(1)==IC) ) {
																		zzmatch(IC);
																		 consume();
																		zzmatch(EQUAL);
																		 consume();
																		zzmatch(VALUE);
																		 consume();
																		zzmatch(COMMA);
																		 consume();
																		zzmatch(VALUE);
																		 consume();
																	}
																}
																{
																	if ( (LA(1)==TEMP) ) {
																		zzmatch(TEMP);
																		 consume();
																		zzmatch(EQUAL);
																		 consume();
																		zzmatch(VALUE);
																		 consume();
																	}
																}
																zzmatch(NEWLINE);
																
																theCurCkt->parse_jfet( jfk->getText(), 
																jfn1->getText(),  jfn2->getText(),
																jfn3->getText(),  jfm->getText());  
 consume();
															}
															else {
																if ( (LA(1)==MOSFET) ) {
																	zzmatch(MOSFET);
																	
																	mosk = (ANTLRTokenPtr)LT(1);
 consume();
																	zzmatch(VALUE);
																	
																	mosn1 = (ANTLRTokenPtr)LT(1);
 consume();
																	zzmatch(VALUE);
																	
																	mosn2 = (ANTLRTokenPtr)LT(1);
 consume();
																	zzmatch(VALUE);
																	
																	mosn3 = (ANTLRTokenPtr)LT(1);
 consume();
																	zzmatch(VALUE);
																	
																	mosn4 = (ANTLRTokenPtr)LT(1);
 consume();
																	zzmatch(IDENTIFIER);
																	
																	mosm = (ANTLRTokenPtr)LT(1);
 consume();
																	{
																		while ( (setwd2[LA(1)]&0x40) ) {
																			parameter_list();
																		}
																	}
																	zzmatch(NEWLINE);
																	
																	theCurCkt->parse_mosfet( mosk->getText(), 
																	mosn1->getText(),  mosn2->getText(),
																	mosn3->getText(),  mosn4->getText(), 
																	mosm->getText());  
 consume();
																}
																else {
																	if ( (LA(1)==CKTCALL) ) {
																		zzmatch(CKTCALL);
																		
																		callid = (ANTLRTokenPtr)LT(1);

																		cout <<"begin cktcall -- "<<endl;
																		theCurCkt->parse_ckt_call( callid->getText());
																		cur_cmd = cCktCall;  
 consume();
																		{
																			int zzcnt=1;
																			do {
																				zzmatch(VALUE);
																				
																				callnd = (ANTLRTokenPtr)LT(1);

																				theCurCkt->parse_ckt_call_node( callnd->getText());  
 consume();
																			} while ( (LA(1)==VALUE) );
																		}
																		zzmatch(IDENTIFIER);
																		
																		callnm = (ANTLRTokenPtr)LT(1);

																		theCurCkt->cur_call->match_sub_ckt(
																		callnm->getText(), theCkt->sub_ckt);   
 consume();
																		{
																			if ( (LA(1)==IDENTIFIER) ) {
																				{
																					int zzcnt=1;
																					do {
																						variable_list();
																					} while ( (LA(1)==IDENTIFIER) );
																				}
																			}
																		}
																		zzmatch(NEWLINE);
																		
																		cur_cmd = cUnknown;  
 consume();
																	}
																	else {
																		if ( (LA(1)==DOT) ) {
																			zzmatch(DOT);
																			 consume();
																			dot_command();
																		}
																		else {FAIL(1,err15,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x80);
}

void
sp_parser::dot_command(void)
{
	zzRULE;
	ANTLRTokenPtr subid=NULL, internd=NULL, acvar=NULL, acnp=NULL, acfb=NULL, acfe=NULL, ns=NULL;
	if ( (LA(1)==MODEL) ) {
		zzmatch(MODEL);
		 consume();
		zzmatch(IDENTIFIER);
		 consume();
		zzmatch(IDENTIFIER);
		 consume();
		zzmatch(71);
		 consume();
		{
			int zzcnt=1;
			do {
				parameter_list();
			} while ( (setwd3[LA(1)]&0x1) );
		}
		zzmatch(72);
		 consume();
		zzmatch(NEWLINE);
		 consume();
	}
	else {
		if ( (LA(1)==SUBCKT) ) {
			zzmatch(SUBCKT);
			 consume();
			zzmatch(IDENTIFIER);
			
			subid = (ANTLRTokenPtr)LT(1);

			cout <<"subckt: "<< subid->getText()<<endl; 
			theCurCkt = theCurCkt->parse_subckt(
			subid->getText());
			cur_cmd = cSubCkt;  
 consume();
			{
				int zzcnt=1;
				do {
					zzmatch(VALUE);
					
					internd = (ANTLRTokenPtr)LT(1);

					theCurCkt->parse_inter_node( internd->getText());   
 consume();
				} while ( (LA(1)==VALUE) );
			}
			{
				if ( (LA(1)==IDENTIFIER) ) {
					{
						int zzcnt=1;
						do {
							variable_list();
						} while ( (LA(1)==IDENTIFIER) );
					}
				}
			}
			zzmatch(NEWLINE);
			
			cur_cmd = cUnknown;   
 consume();
		}
		else {
			if ( (LA(1)==OPTION) ) {
				zzmatch(OPTION);
				 consume();
				{
					int zzcnt=1;
					do {
						mix_para_list();
					} while ( (LA(1)==IDENTIFIER) );
				}
				zzmatch(NEWLINE);
				 consume();
			}
			else {
				if ( (LA(1)==AC) ) {
					zzmatch(AC);
					 consume();
					zzmatch(IDENTIFIER);
					
					acvar = (ANTLRTokenPtr)LT(1);
 consume();
					zzmatch(VALUE);
					
					acnp = (ANTLRTokenPtr)LT(1);
 consume();
					zzmatch(VALUE);
					
					acfb = (ANTLRTokenPtr)LT(1);
 consume();
					zzmatch(VALUE);
					
					acfe = (ANTLRTokenPtr)LT(1);
 consume();
					zzmatch(NEWLINE);
					
					theCurCkt->parse_ac_cmd( acvar->getText(),
					acnp->getText(),
					acfb->getText(),
					acfe->getText());  
 consume();
				}
				else {
					if ( (LA(1)==NOISE) ) {
						zzmatch(NOISE);
						 consume();
						zzmatch(IDENTIFIER);
						
						ns = (ANTLRTokenPtr)LT(1);
 consume();
						zzmatch(NEWLINE);
						
						theCurCkt->read_nz_sources( ns->getText());   
 consume();
					}
					else {
						if ( (LA(1)==DC) ) {
							zzmatch(DC);
							 consume();
							{
								if ( (LA(1)==VOL) ) {
									zzmatch(VOL);
									 consume();
								}
								else {
									if ( (LA(1)==CUR) ) {
										zzmatch(CUR);
										 consume();
									}
									else {FAIL(1,err16,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
							zzmatch(VALUE);
							 consume();
							zzmatch(VALUE);
							 consume();
							zzmatch(VALUE);
							 consume();
							zzmatch(NEWLINE);
							 consume();
						}
						else {
							if ( (LA(1)==OP) ) {
								zzmatch(OP);
								 consume();
								zzmatch(NEWLINE);
								 consume();
							}
							else {
								if ( (LA(1)==SENS) ) {
									zzmatch(SENS);
									 consume();
									outvar();
									zzmatch(AC);
									 consume();
									zzmatch(IDENTIFIER);
									 consume();
									zzmatch(VALUE);
									 consume();
									zzmatch(VALUE);
									 consume();
									zzmatch(VALUE);
									 consume();
									zzmatch(NEWLINE);
									 consume();
								}
								else {
									if ( (LA(1)==TF) ) {
										zzmatch(TF);
										 consume();
										outvar();
										zzmatch(NEWLINE);
										 consume();
									}
									else {
										if ( (LA(1)==TRAN) ) {
											zzmatch(TRAN);
											 consume();
											zzmatch(VALUE);
											 consume();
											zzmatch(VALUE);
											 consume();
											{
												if ( (LA(1)==VALUE) ) {
													zzmatch(VALUE);
													 consume();
													{
														if ( (LA(1)==VALUE) ) {
															zzmatch(VALUE);
															 consume();
														}
													}
												}
											}
											zzmatch(NEWLINE);
											 consume();
										}
										else {
											if ( (LA(1)==SAVE) ) {
												zzmatch(SAVE);
												 consume();
												{
													int zzcnt=1;
													do {
														zzmatch(IDENTIFIER);
														 consume();
													} while ( (LA(1)==IDENTIFIER) );
												}
												zzmatch(NEWLINE);
												 consume();
											}
											else {
												if ( (LA(1)==PRINT) ) {
													zzmatch(PRINT);
													
													cur_cmd = cPrint;   
 consume();
													{
														if ( (setwd3[LA(1)]&0x2) ) {
															analysis();
														}
													}
													{
														int zzcnt=1;
														do {
															outvar();
														} while ( (LA(1)==VAR_NAME) );
													}
													zzmatch(NEWLINE);
													
													cur_cmd = cUnknown;   
 consume();
												}
												else {
													if ( (LA(1)==PLOT) ) {
														zzmatch(PLOT);
														
														cur_cmd = cPlot;   
 consume();
														{
															if ( (setwd3[LA(1)]&0x4) ) {
																analysis();
															}
														}
														{
															int zzcnt=1;
															do {
																outvar();
															} while ( (LA(1)==VAR_NAME) );
														}
														zzmatch(NEWLINE);
														
														cur_cmd = cUnknown;   
 consume();
													}
													else {
														if ( (LA(1)==ENDS) ) {
															zzmatch(ENDS);
															 consume();
															{
																if ( (LA(1)==IDENTIFIER) ) {
																	zzmatch(IDENTIFIER);
																	 consume();
																}
															}
															zzmatch(NEWLINE);
															
															cout << "change current ckt"<<endl;
															theCurCkt = theCurCkt->parent; 
															if(!theCurCkt)
															error_mesg(SYN_ERROR,"Invalid .ENDS command");
 consume();
														}
														else {
															if ( (LA(1)==END) ) {
																zzmatch(END);
																 consume();
																zzmatch(NEWLINE);
																 consume();
															}
															else {FAIL(1,err17,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x8);
}

void
sp_parser::analysis(void)
{
	zzRULE;
	if ( (LA(1)==AC) ) {
		zzmatch(AC);
		 consume();
	}
	else {
		if ( (LA(1)==DC) ) {
			zzmatch(DC);
			 consume();
		}
		else {
			if ( (LA(1)==TRAN) ) {
				zzmatch(TRAN);
				 consume();
			}
			else {FAIL(1,err18,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
		}
	}
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x10);
}

void
sp_parser::indep_source(void)
{
	zzRULE;
	if ( (setwd3[LA(1)]&0x20) ) {
		zzsetmatch(PULSE_set);
		 consume();
		zzmatch(71);
		 consume();
		zzmatch(VALUE);
		 consume();
		zzmatch(VALUE);
		 consume();
		zzmatch(VALUE);
		 consume();
		zzmatch(VALUE);
		 consume();
		zzmatch(VALUE);
		 consume();
		zzmatch(VALUE);
		 consume();
		zzmatch(VALUE);
		 consume();
		zzmatch(72);
		 consume();
	}
	else {
		if ( (setwd3[LA(1)]&0x40) ) {
			zzsetmatch(SIN_set);
			 consume();
			zzmatch(71);
			 consume();
			zzmatch(VALUE);
			 consume();
			zzmatch(VALUE);
			 consume();
			zzmatch(VALUE);
			 consume();
			zzmatch(VALUE);
			 consume();
			zzmatch(VALUE);
			 consume();
			zzmatch(72);
			 consume();
		}
		else {
			if ( (setwd3[LA(1)]&0x80) ) {
				zzsetmatch(EXP_set);
				 consume();
				zzmatch(71);
				 consume();
				zzmatch(VALUE);
				 consume();
				zzmatch(VALUE);
				 consume();
				zzmatch(VALUE);
				 consume();
				zzmatch(VALUE);
				 consume();
				zzmatch(VALUE);
				 consume();
				zzmatch(VALUE);
				 consume();
				zzmatch(72);
				 consume();
			}
			else {
				if ( (setwd4[LA(1)]&0x1) ) {
					zzsetmatch(PWL_set);
					 consume();
					zzmatch(71);
					 consume();
					{
						int zzcnt=1;
						do {
							value_list();
						} while ( (LA(1)==VALUE) );
					}
					zzmatch(72);
					 consume();
				}
				else {
					if ( (setwd4[LA(1)]&0x2) ) {
						zzsetmatch(SFFM_set);
						 consume();
						zzmatch(71);
						 consume();
						zzmatch(VALUE);
						 consume();
						zzmatch(VALUE);
						 consume();
						zzmatch(VALUE);
						 consume();
						zzmatch(VALUE);
						 consume();
						zzmatch(VALUE);
						 consume();
						zzmatch(72);
						 consume();
					}
					else {FAIL(1,err24,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
				}
			}
		}
	}
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x4);
}

void
sp_parser::value_list(void)
{
	zzRULE;
	zzmatch(VALUE);
	 consume();
	zzmatch(VALUE);
	 consume();
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x8);
}

void
sp_parser::variable_list(void)
{
	zzRULE;
	ANTLRTokenPtr varnm=NULL, varval=NULL;
	zzmatch(IDENTIFIER);
	
	varnm = (ANTLRTokenPtr)LT(1);
 consume();
	zzmatch(EQUAL);
	 consume();
	zzmatch(VALUE);
	
	varval = (ANTLRTokenPtr)LT(1);

	if(cur_cmd == cSubCkt)
	theCurCkt->parse_deflt_value(
	varnm->getText(),  varval->getText());
	else if(cur_cmd == cCktCall)
	theCurCkt->parse_ckt_call_param(
	varnm->getText(),  varval->getText());
 consume();
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x10);
}

void
sp_parser::parameter_list(void)
{
	zzRULE;
	if ( (LA(1)==IDENTIFIER) ) {
		zzmatch(IDENTIFIER);
		 consume();
		zzmatch(EQUAL);
		 consume();
		zzmatch(VALUE);
		 consume();
	}
	else {
		if ( (LA(1)==IC) ) {
			zzmatch(IC);
			 consume();
			zzmatch(EQUAL);
			 consume();
			zzmatch(VALUEA);
			 consume();
			{
				if ( (LA(1)==COMMA) ) {
					zzmatch(COMMA);
					 consume();
					zzmatch(VALUEA);
					 consume();
					{
						if ( (LA(1)==COMMA) ) {
							zzmatch(COMMA);
							 consume();
							zzmatch(VALUEA);
							 consume();
						}
					}
				}
			}
		}
		else {
			if ( (setwd4[LA(1)]&0x20) ) {
				zzsetmatch(ONOFF_set);
				 consume();
			}
			else {
				if ( (LA(1)==TEMP) ) {
					zzmatch(TEMP);
					 consume();
					zzmatch(EQUAL);
					 consume();
					zzmatch(VALUE);
					 consume();
				}
				else {FAIL(1,err25,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
			}
		}
	}
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x40);
}

void
sp_parser::outvar(void)
{
	zzRULE;
	ANTLRTokenPtr varnm=NULL, varn1=NULL, varn2=NULL;
	zzmatch(VAR_NAME);
	
	varnm = (ANTLRTokenPtr)LT(1);
 consume();
	zzmatch(LEFTB);
	 consume();
	zzmatch(VALUE);
	
	varn1 = (ANTLRTokenPtr)LT(1);
 consume();
	{
		if ( (LA(1)==COMMA) ) {
			zzmatch(COMMA);
			 consume();
			zzmatch(VALUE);
			
			varn2 = (ANTLRTokenPtr)LT(1);
 consume();
		}
	}
	zzmatch(RIGHTB);
	
	if(cur_cmd == cPrint || cur_cmd == cPlot){
		// we only allow one output now
		if(!theCurCkt->ac_mgr->get_output_num()){
			theCurCkt->parse_print( varnm->getText(), 
			varn1->getText(), 
			( varn2 != NULL?  varn2->getText():NULL));
		} 
	}
 consume();
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x80);
}

void
sp_parser::mix_para_list(void)
{
	zzRULE;
	zzmatch(IDENTIFIER);
	 consume();
	{
		if ( (LA(1)==EQUAL) ) {
			{
				zzmatch(EQUAL);
				 consume();
				zzmatch(VALUE);
				 consume();
			}
		}
	}
	return;
fail:
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd5, 0x1);
}
