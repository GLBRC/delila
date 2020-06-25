/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "csv2ssv.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.10
/*











*/
#define updateversion   1.00


Static jmp_buf _JL1;



/*































































*/



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(infile, outfile)
_TEXT *infile, *outfile;
{
  Char cin, cout;
  Char previouscin = ' ';
  boolean inquote = false;
  boolean beginline;

  if (*infile->name != '\0') {
    if (infile->f != NULL)
      infile->f = freopen(infile->name, "r", infile->f);
    else
      infile->f = fopen(infile->name, "r");
  } else
    rewind(infile->f);
  if (infile->f == NULL)
    _EscIO2(FileNotFound, infile->name);
  RESETBUF(infile->f, Char);


  /*
*/
  if (!BUFEOF(infile->f)) {
    if (!P_eoln(infile->f)) {
      if (P_peek(infile->f) == ',')
	fprintf(outfile->f, "- ");
    }
  }

  while (!BUFEOF(infile->f)) {
    if (BUFEOF(infile->f))
      fprintf(outfile->f, "EOF");

    if (P_eoln(infile->f)) {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
      /*




*/
      if (!BUFEOF(infile->f))
	putc('\n', outfile->f);
      /*

*/
      previouscin = ' ';
      inquote = false;
      beginline = true;
      continue;
    }
    cin = getc(infile->f);
    if (cin == '\n')
      cin = ' ';
    if (cin == 13)
      continue;
    if (cin == '"')
      inquote = !inquote;

    if (cin == ',' && beginline)
      fprintf(outfile->f, "- ");

    else {
      cout = cin;
      if (cin == ' ')
	cout = '_';
      else {
	if (!inquote) {
	  if (cin == ',') {
	    if (previouscin == ',')
	      putc('-', outfile->f);
	    cout = ' ';

	  }

	}

      }


      if (cin == '"' && previouscin == '"')
	fprintf(outfile->f, "--");


      if (cin != '"')
	putc(cout, outfile->f);
    }


    previouscin = cin;
    beginline = false;
    /*

*/



  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
