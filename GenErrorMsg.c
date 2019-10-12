/*
 * GenErrorMsg.c
 *
 *  Created on: Oct 12, 2019
 *      Author: dad
 *      Version     :
 *      Copyright   : Your copyright notice
 *      Description : a simple c program to read a list of error messsages
 *                    and generate a snipet of opsi script to check error codes.
 *                    The snipet can be pasted into the standard opsi Sub_check_exitcode
 *                    script.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int 	stripnl(char * firstline);

#define NAXCHAR   10025

int main(void) {
	FILE *fp, *fpo;
	char firstline[NAXCHAR];
	char errmsg[NAXCHAR];
	char * tp1;
	fp = fopen("errcodes.txt", "r");
	fpo = fopen("exitcode.opsiscript", "w+");
	if (fp == NULL) {
	  fprintf(stderr, "Can't open input file!\n");
	  return(1);
	}
	firstline[0] = '\0';
//	fgets(firstline, NAXCHAR, fp);
//	stripnl(firstline);
//	printf("%s\n",firstline);
	int linenum = 0;
	int ierrornum;

	while ( fgets(firstline, NAXCHAR, fp) != NULL ) {
		stripnl(firstline);
//		printf("%s\n",firstline);
		linenum++;
		sscanf(firstline,"%i",&ierrornum);
		errmsg[0] = '\0';
		tp1 = strchr(firstline, '\t');
		if ( tp1 == NULL ) {
			fprintf(stderr, "line %i missing tab character!\n",linenum);
			return(1);
		} else {
			tp1++;
		}
		strcat(errmsg,tp1);
		fprintf(fpo,"\tif ($ExitCode$ = \"%i\")\n\t\tset $ErrorMsg$ = \"ExitCode = \"+$ExitCode$+\"   %s\"\n\tendif\n",ierrornum,errmsg);
		firstline[0] = '\0';
	}// end of while on EOF

	fclose(fp);
	fclose(fpo);
	printf(" %i lines processed\n", linenum);

	return EXIT_SUCCESS;
}



int 	stripnl(char * firstline)
{
	int lenline=0;
	int i;
	lenline = strlen(firstline);
	if(lenline == 0)return(0);
	for(i=lenline;i>=0;i--) {
		if ( firstline[i] == '\n' || firstline[i] == '\r') {
			firstline[i]='\000';
		}
	}
	return 0;
}
