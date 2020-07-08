//**************************//
/* fileParsing.c */
/* Mike Place June 2020 */

#include <getopt.h>  /* getopt API */ 
#include <stdio.h> /* printf */
#include <stdlib.h> 
#include <string.h> /* strtok_r */
#include </home/mplace/bin/p2c/src/p2c.h>

int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0; 
  	/* flags marking arguments passed */
	int fflag=0;       /* file flag */
  	char *fName = "filename.txt";
  	static char usage[] = "usage: %s -f <parameter file>\n";

/* Process command line arguments  */
while ((c = getopt(argc, argv, "f:")) != -1)
		switch (c) {
		case 'f':
      		fflag = 1;
			fName = optarg;
			break;
		case '?':
			err = 1;
			break;
		}
  /* Is the Output file name present */  
	if (fflag == 0) {	/* -o was mandatory */ 
		fprintf(stderr, "%s: missing -f parameter file\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} 
	
	// Open input file
	FILE *fp = fopen(fName, "r");
	char buf[1024];
	
	// Check if input file exists
	if (!fp){
		printf("Can't open file\n");
		return 1;
	}
	
	// Declaration of delimiter 
	const char d[4] = "=";
	char *rest = NULL;
	char *param;
	char *fname;

	// Loop through file
	while (fgets(buf, 1024, fp)){
	
		param = strtok_r(buf, d, &rest);
		//printf("param = %s \t", param);
		fname = strtok_r(NULL, d, &rest);
		//printf(" value is %s \n", fname);

		if (strcmp(param, "l1") == 0){
			printf("l1 file name is %s\n", fname);
		} else if (strcmp(param, "l2") == 0){
			printf("l2 file name is %s\n", fname);
		} 

	}

	// Close file
	fclose(fp);
    
return 0;
}

