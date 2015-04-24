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
 *    $RCSfile: commands.cc,v $
 *    $Revision: 1.7 $
 *    $Date: 2002/11/21 02:40:15 $
 */


/* commands.cc -
 *
 * This file contains routines to read in the SCAD3 script and
 * process the commands it contains.  The script is read from
 * standard input.  Each line contains a command name followed
 * by additional parameters, if necessary.
 */

#include <fstream.h>
#include <iostream.h>
#include "globals.h"
#include "circuits.h"
#include "acan.h"
#include <sys/times.h>
#include "timer.h"
#include "unistd.h"
#include "mxexp.h"


/* Imports from other SCAD3 modules: */

extern char *RunStats();
extern char *RunStatsSince();
extern int Lookup(char[], char *([]));
extern int ReadMatrix(char *file);
extern int ReadMatrixLabel(char *file);

extern int Help(char*);
extern int QuitCmd(char*);
extern int Load(char*);
extern int Source(char*);
extern int LoadSymbListCmd(char *);
extern int ACAnalysisCmd(char*);
extern int MnaCmd(char*);
extern int SymbCmd(char*);
extern int RunCmd(char*);
extern int SweepCmd(char*);
extern int ReduceBranchCmd(char*);
extern int ReduceBothCmd(char*);
extern int ReduceElemCmd(char*);
extern int ReduceFCoeffElemCmd(char*);
extern int FactorDDDNode(char*);
extern int CoeffCmd(char*);
extern int ExpdddCmd(char*);
extern int ExpsdddCmd(char*);
extern int SimpdddCmd(char*);
extern int SimpcoeffCmd(char*);
extern int SuppCoeffCmd(char*);
extern int DecancelCmd(char*);
extern int PoleReduceCmd(char*);
extern int PoleFactCmd(char*);
extern int SetCmd(char*);
extern int StatisticCmd(char*);
extern int ReadMatrixCmd(char*);
extern int ReadLabeledMatrixCmd(char*);
extern int DumpLabelCmd(char *);
extern int GCCmd(char*);
extern int PrintExprCmd(char*);
extern int EditCmd(char*);
extern int PlotCmd(char*);
extern int CompCmd(char*);
extern int NoiseCmd(char*);
extern int CNoiseCmd(char*);
extern int ToMapleCmd(char*);
extern int TransferFuncCmd(char*);
extern int CompDDDReorderCmd(char*);
extern int SemiSymbCmd(char*);
extern int CompDDDMinimizeCmd(char*);


extern int read_file( char *file);

Timer *theTimer;
vector<SymbBranch *> theSymbBranchList;

/* The following two arrays hold the names of the commands, and
 * the routine to be invoked to process each command.
 */

char *(cmds[]) = {
    "help",
    "load control_file",
    "source spice_file",
	"loadsymb symbolic_list_file", 
    "ac [dec|oct|lin] np fstart fstop",
    "mna (build MNA)",
    "buildsymb (build symbolic expressions)",
    "set  parameter = value ...",
    "run  -- perform the numerical analysis on complex ddd",
    "sweep [ddd|sddd|coeff|scoeff]  -- perform numerical frequence sweep",
    "redbranch   -- reduce circuit branchs(ckt element as a whole)",
    "redboth   -- reduce circuit branchs for both nume and deno",
    "redelem   -- reduce MNA elements",
    "redfcoeff   -- reduce fcoefficient DDD nodes",
    "factddd   -- factor out common DDD node among nume and dene",
    "coeff -- obtain partially expanded ddd coeficient list",
    "expddd [decacel] -- obtain fcoefficient list from complex ddd",
    "expsddd [decacel] -- obtain fcoefficient list from simplified ddd",
    "simpddd -- simplify complex ddd",
    "simpcoeff -- simplify coefficient ddd list",
    "suppress -- suppress coefficients of coefficient list",
    "decancel -- removal all the symbolic cancellation terms",
    "polered [pindex] -- device elemination for pole/zero extraction",
    "polefact [pindex] -- DDD factorization for pole/zero extraction",
    "noise -- noise analysis", 
    "cnoise -- noise analysis by using coeff list",
	"cdddreorder -- complex ddd reordering",
	"semisymb -- semi-sysmbolic analysis",
	"min_cddd -- minimize complex DDDs by dynamic reordering",
	"maple {file} -- maple input file for the noise model construnction",
	"transfer {file} -- dump transfer function ready for matlab",
    "statistic",
    "read {matrix_file} read matrix without variable indices",
    "label {matrix_file} read matrix with variable indices",
    "dumplabel {matrix_file} dump the matrix with variable indices",
    "gc [cddd|sddd|fcoeff|sfcoeff]",
    "printexpr {output_file}",
    "edit {file}",
    "plot {mag|phase|db|real|image}",
    "compare {mag|phase|db|real|image}",
    "quit",
    NULL};

/* Compiler barfs if command procedures aren't declared here. */

int (*(rtns[]))(char*) = 
{
        Help,   
		Load,
		Source,
		LoadSymbListCmd,
		ACAnalysisCmd,
		MnaCmd,
		SymbCmd,
		SetCmd,
		RunCmd,
		SweepCmd,
		ReduceBranchCmd,
		ReduceBothCmd,
		ReduceElemCmd,
		ReduceFCoeffElemCmd,
		FactorDDDNode,
		CoeffCmd,
		ExpdddCmd,
		ExpsdddCmd,
		SimpdddCmd,
		SimpcoeffCmd,
		SuppCoeffCmd,
		DecancelCmd,
		PoleReduceCmd,
		PoleFactCmd,
		NoiseCmd,
		CNoiseCmd,
		CompDDDReorderCmd,
		SemiSymbCmd,
        CompDDDMinimizeCmd,
		ToMapleCmd,
		TransferFuncCmd,
		StatisticCmd,
		ReadMatrixCmd,	
		ReadLabeledMatrixCmd,
		DumpLabelCmd,
		GCCmd,
		PrintExprCmd,
		EditCmd,
		PlotCmd,
		CompCmd,
		QuitCmd,
		NULL
};

int CmdDo(char* line)
{
    int  index, nargs;
    char cmd[128], residue[256];

#ifdef _DEBUG
    struct timeval mytime;
#endif

    nargs = sscanf(line, " %127s %[^]", cmd, residue);

    if ((nargs <= 0) || (cmd[0] == '!')) 
	{
		return -1;
	}

    if (nargs == 1) 
	{ 
		residue[0] = 0;
	}

    index = Lookup(cmd, cmds);
 
	if (index == -1)
	{
        printf("Ambiguous command: %s\n", cmd);
	}
    else if (index == -2)
	{
        printf("Unknown command: %s\n", cmd);
	}
    else 
	{

#ifdef _DEBUG
        gettimeofday(&mytime, NULL);
        double time1 = mytime.tv_sec + mytime.tv_usec/CLOCKS_PER_SEC;
        double time1 = 1.0 * clock() / CLOCKS_PER_SEC;
        theTimer->Start();
#endif
        if (rtns[index] != NULL)
		{
			(*(rtns[index]))(residue);
		}

#ifdef _DEBUG
        gettimeofday(&mytime, NULL);
        double time2 = mytime.tv_sec + mytime.tv_usec/CLOCKS_PER_SEC;
        double time2 = 1.0 * clock() / CLOCKS_PER_SEC;
        printf("%lf\n",time2-time1);
		//theTimer->Stop();
        //theTimer->Print(cout);
#endif        
        cout.flush();
        printf("%s\n", RunStatsSince());
        (void) fflush(stdout);
    }

    return 0;
}


void Command(FILE* f)

/*---------------------------------------------------------
 *    Command just reads commands from the script, and
 *    calls the corresponding routines.
 *
 *    Results:    None.
 *
 *    Side Effects:
 *    Whatever the commands do.  This procedure continues
 *    until it hits end-of-file.
 *---------------------------------------------------------
 */

{
    char line[800], *p;
    int tty, length;

    tty = isatty(fileno(f));
    while (TRUE)
    {
		if (tty)
		{
			fputs(": ", stdout);
			(void) fflush(stdout);
		}
		p = line;
		*p = 0;
		length = 799;
		while (TRUE)
		{
			if (fgets(p, length, f) == NULL) return;

			/* Get rid of the stupid newline character.  If we didn't
			 * get a whole line, keep reading.
			 */

			for ( ; (*p != 0) && (*p != '\n'); p++);
			if (*p == '\n')
			{
				*p = 0;
				if (p == line) break;
				if (*(p-1) != '\\') break;
				else p--;
			}
			length = 799 - strlen(line);
			if (length <= 0) break;
		}
		if (line[0] == 0) continue;
		if (!tty) printf(": %s\n", line);
		CmdDo(line);
    }
}



int Help(char*)

/*---------------------------------------------------------
 *    This command routine just prints out the legal commands.
 *
 *    Results:    None.
 *
 *    Side Effects:
 *    Valid command names are printed one per line on standard
 *    output.
 *---------------------------------------------------------
 */

{
    char **p;

    p = cmds;
    printf("Valid commands are:\n");
    while (*p != NULL)
    {
		printf("  %s\n", *p);
		p++;
    }

    return 0;
}

int QuitCmd(char*)

/*---------------------------------------------------------
 *    This routine just returns from SCAD3 to the shell.
 *
 *    Results:    None -- it never returns.
 *
 *    Side Effects:    SCAD3 exits.
 *---------------------------------------------------------
 */

{
    printf("%s SCAD3 done. Bye!\n", RunStats());
    if(theCkt)
	{
		delete theCkt;
	}
    exit(0);
    return 0;
}


int Load(char* name)

/*---------------------------------------------------------
 *    This command routine merely reads other commands
 *    from a given file.
 *
 *    Results:    None.
 *
 *    Side Effects:
 *    Whatever the commands in the file do.
 *---------------------------------------------------------
 */

{
    FILE *f;
    char string[100];

    if (name[0] == 0) 
	{
        printf("No command file name given.\n");
        return -1;
    }
    sscanf(name, "%s", string);
    f = fopen(string, "r");
    if (f == NULL) 
	{
        printf("Can't find file %s.\n", string);
        return -1;
    }
    
	Command(f);
    fclose(f);
    
	return 0;
}

int Source(char* name)
{
    char string[256];

    if (name[0] == 0) 
	{
        print_mesg("No circuit file name given.");
        return -1;
    }
    sscanf(name, "%s", string);
    if(theCkt)
	{
        print_raw("Release memory ... ");
        delete theCkt;
        print_mesg("done.");
    }

    theCkt = new class Circuit(name);
    if(theCkt->read_spice_file(name) == -1)
	{
		delete theCkt;
		theCkt = NULL;
    }
    
	return 0;
}



/*
 * read the information regarding which branch is symbolic branch and
 * their vairance for semi-symbolic analysis.
 *
 */
int LoadSymbListCmd(char *file_name)
{    
	if(!theCkt)
	{
        error_mesg(INT_ERROR,"Load spice file first.");
        return -1;
    }

	ifstream SymbolicFile(file_name,ios::in);
	if(SymbolicFile)
	{
		//if_symbolic_analysis=1;                         

		char  symName[128];		
		double startValue,endValue;
		
		/* Format of the symbolic file:
		 * branch_name1, start_value, end_value unused_str
		 * branch_name2, start_value, end_value unused_str
		 * ...
		 * If the first charater is a '*' or space, this line is ignored.
		 */
		char buf[1024];		
		while(!SymbolicFile.eof())
		{
			
			buf[0] = '\0';
			
			SymbolicFile.getline(buf, 256, '\n');						
			
			//cout << buf <<endl;
			
			// ignore the 
			if((buf[0] == '*') || (strlen(buf) < 2) || (buf[0] == '\n'))
			{				
				continue;
			}
					
			sscanf(buf, "%s %lg %lg", symName, &startValue, &endValue);			
			SymbBranch *new_bch = new SymbBranch(symName, startValue, endValue);
			theSymbBranchList.push_back(new_bch);	

			cout <<symName <<" " << startValue <<" " << endValue<< endl;
		}
	}

	cout <<"number of symbolic branches defined: " <<  theSymbBranchList.size() << endl;	
	
	theCkt->mark_symbolic_branchs();
	
	return 0;
	
}



int ACAnalysisCmd(char* string)
{
    VariationType    type;
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Load spice file first.");
        return -1;
    }
    char *str = ToLower(string);
    char *cmd;
    cmd = NextField(&str);
    if(!cmd)
	{
        error_mesg(PARSE_ERROR,"Miss variation");
        return -1;
    }

    if(!strcmp(cmd,"dec"))
	{
        type = vDEC;
	}
    else if(!strcmp(cmd,"lin"))
	{
        type = vLIN;
	}
    else if(!strcmp(cmd,"oct"))
	{
        type = vOCT;
	}
    else
	{
        sprintf(_buf,"Unknown variation type: %s",cmd);
        error_mesg(PARSE_ERROR,_buf);
        return -1;
    }

    cmd = NextField(&str);
    if(!cmd)
	{
        sprintf(_buf,"Syntex error: %s",string);
        error_mesg(PARSE_ERROR,_buf);
        return -1;
    }
    double _np = TransValue(cmd);

    cmd = NextField(&str);
    if(!cmd)
	{
        sprintf(_buf,"Syntex error: %s",string);
        error_mesg(PARSE_ERROR,_buf);
        return -1;
    }
    double _fstart = TransValue(cmd);

    cmd = NextField(&str);
    if(!cmd)
	{
        sprintf(_buf,"Syntex error: %s",string);
        error_mesg(PARSE_ERROR,_buf);
        return -1;
    }
    
	double _fstop = TransValue(cmd);

    ACAnalysis *ac_cmd = new ACAnalysis(type, (int)_np, _fstart, _fstop);
    theCkt->ac_mgr->new_command(ac_cmd); 
    //theCkt->ac_mgr->ac_analysis(); 
    
	return 0;
}

int MnaCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    
	theCkt->build_mna();
    
	return 0;
}

int SymbCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->build_expressions();
    
	return 0;
}

int RunCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->ac_analysis();
    
	return 0;
}

int SweepCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    if(string[0])
	{
        theCkt->ac_mgr->new_freq_sweep(string);
	}
    else
	{
        theCkt->ac_mgr->new_freq_sweep(NULL);
	}
    
	return 0;
}

int ReduceElemCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    
	theCkt->ac_mgr->elem_elimination();
    
	return 0;
}

int ReduceBranchCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->whole_elem_elim();
    
	return 0;
}

int ReduceBothCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->whole_elem_elim_both();
    
	return 0;
}

int ReduceFCoeffElemCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->fcoeff_elem_elimination();
    
	return 0;
}

int FactorDDDNode(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->factor_ddd_node();
    
	return 0;
}

int CoeffCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->coeff_ac_analysis();
    
	return 0;
}

int ExpdddCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }

	bool decancel = 0;

    if (string[0] && (!strcmp(string,"decancel")))
	{
		decancel = 1;			
	}
    theCkt->ac_mgr->build_fcoeff_from_ddd(decancel);
    
	return 0;
}

int ExpsdddCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }

	bool decancel = 0;

    if (string[0] && (!strcmp(string,"decancel")))
	{
		decancel = 1;			
	}
    theCkt->ac_mgr->build_fcoeff_from_sddd(decancel);
        
	return 0;
}

int SimpdddCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->simplify_ddd();
    
	return 0;
}

int SimpcoeffCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->simplify_coefflist();
    
	return 0;
}

int SuppCoeffCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->ac_mgr->fcoeff_suppress();
    
	return 0;
}

int DecancelCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    cout<<"begin decancellation"<<endl;
    theCkt->fcoeff_decancel();

    return 0;
}

int PoleReduceCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    cout<<"beginning device elemination in pole/zero extration"<<endl;
    if(string[0] == 0)
	{
        theCkt->whole_elem_elim_polezero(0);
	}
    else
	{
        theCkt->whole_elem_elim_polezero(atoi(string));
	}

    return 0;
}

int PoleFactCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    cout<<"beginning DDD factorization in pole/zero extration"<<endl;
    
	if(string[0] == 0)
	{
        theCkt->ac_mgr->factor_polezero(0);
	}
    else
	{
        theCkt->ac_mgr->factor_polezero(atoi(string));
	}

    return 0;
}

int NoiseCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
	
    theCkt->ac_mgr->noise_analysis();
    return 0;
}

int CNoiseCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }

    theCkt->ac_mgr->coeff_noise_analysis();
    return 0;
}



int SemiSymbCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }

    theCkt->ac_mgr->fcoeff_semi_symbolic_analysis();
    return 0;
}

int CompDDDMinimizeCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }

    theCkt->ac_mgr->complex_ddd_minimization_via_reordering();
    return 0;
}

int CompDDDReorderCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }

    theCkt->ac_mgr->complex_ddd_semi_variable_reordering();
    return 0;
}

int ToMapleCmd(char* name)
{
    if(!theCkt){
        error_mesg(IO_ERROR,"no file name given");
        return -1;
    }
    if (name[0] == 0)
	{
		theCkt->ac_mgr->maple_cmd(cout);
	}
    else
	{
        ofstream outfile(name);
        if(!outfile)
		{
            error_mesg(IO_ERROR,"Can't open the file to write");
            return -1;
        }
        theCkt->ac_mgr->maple_cmd(outfile);
        outfile.close();
    }
    return 0;
}

int TransferFuncCmd(char* name)
{
    if(!theCkt)
	{
        error_mesg(IO_ERROR,"no file name given");
        return -1;
    }
	
    if (name[0] == 0)
	{
		theCkt->ac_mgr->transfer_func_cmd(cout);
	}
    else
	{
        ofstream outfile(name);
        if(!outfile)
		{
            error_mesg(IO_ERROR,"Can't open the file to write");
            return -1;
        }
        theCkt->ac_mgr->transfer_func_cmd(outfile);
        outfile.close();
    }
    return 0;
}


int SetCmd(char *str)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    Set *set_aux = new Set; 
    set_aux->analyze_command(str);
    delete set_aux;
    return 0; 
}

int PlotCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    if(string[0])
        theCkt->ac_mgr->graph_display(string);
    else
        theCkt->ac_mgr->graph_display(NULL);
    return 0;
}

int CompCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    if(string[0])
	{
        theCkt->ac_mgr->graph_compare(string);
	}
    else
	{
        theCkt->ac_mgr->graph_compare(NULL);
	}
    return 0;
}

    
int StatisticCmd(char* string)
{
    if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
    theCkt->statistic();

    return 0;
}

int ReadMatrixCmd(char* name)
{
    ReadMatrix(name);
    
	return 0;
}

int ReadLabeledMatrixCmd(char* name)
{
    ReadMatrixLabel(name);

    return 0;
}

int DumpLabelCmd(char* name)
{
	 if(!theCkt)
	{
        error_mesg(INT_ERROR,"Source spice file first.");
        return -1;
    }
	if (name[0] == 0)
	{
        error_mesg(IO_ERROR,"No file name given.");
        return -1;
    }
	else
	{
        ofstream outfile(name);
        if(!outfile)
		{
            error_mesg(IO_ERROR,"Can't open the file to write");
            return -1;
        }
		//theCkt->ac_mgr->print_expression(outfile);
		if(theCkt->get_mna_matrix())
		{			
			theCkt->get_mna_matrix()->get_imatrix()->dump_index(outfile);
		}
				
        outfile.close();
    }    

    return 0;
}

int
GCCmd(char* type)
{
	if (type[0] == 0)
	{
        error_mesg(IO_ERROR,"No file name given.");
        return -1;
    }
    theCkt->ac_mgr->forceGC(type);
	
	return 0;
}

int PrintExprCmd(char* name)
{
    if(!theCkt)
	{
        error_mesg(IO_ERROR,"No simulator file name given.");
        return -1;
    }
    if (name[0] == 0)
	{
		//theCkt->ac_mgr->print_expression(cout);
		theCkt->ac_mgr->print_fcoeff_list(cout);
	}
    else
	{
        ofstream outfile(name);
        if(!outfile)
		{
            error_mesg(IO_ERROR,"Can't open the file to write");
            return -1;
        }
		//theCkt->ac_mgr->print_expression(outfile);
        theCkt->ac_mgr->print_fcoeff_list(outfile);
        outfile.close();
    }

    return 0;
}

int EditCmd(char* name)
{
    FILE *f;
    char str[256];
    char line[256];

    if (name[0] == 0) 
	{
        if(!theCkt)
		{
            error_mesg(IO_ERROR,"No simulator file name given.");
            return -1;
        }
        sprintf(line,"vi %s",theCkt->get_spice_file());
        system(line);
        sprintf(str,theCkt->get_spice_file());
        Source(str);
        return 0;
    }

    sscanf(name, "%s", str);
    f = fopen(str, "r");
    if (f == NULL) 
	{
        sprintf(_buf,"Cann't find file: %s.",str);
        error_mesg(IO_ERROR,_buf);
        return -1;
    }
    fclose(f);
    sprintf(line,"vi %s",name);
    system(line);
    Source(str);

    return 0;
}
