/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "roundfile.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.03
/*

*/



/*






























*/



#define maxline         500



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  Char line[maxline];
  long i;
  long characters = 0;

  do {
    if (P_eoln(fin->f)) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      if (!BUFEOF(fin->f)) {
	for (i = 0; i < characters; i++)
	  putc(line[i], fout->f);
	putc('\n', fout->f);
      }
      characters = 0;
    } else {
      characters++;
      line[characters-1] = getc(fin->f);
      if (line[characters-1] == '\n')
	line[characters-1] = ' ';
    }
  } while (!BUFEOF(fin->f));
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
