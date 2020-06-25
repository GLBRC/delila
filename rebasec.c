/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rebasec.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.00
/*

*/



/*


















































*/


#define bufmax          100


typedef Char linebuffer[bufmax];


Static _TEXT enzymelist, data;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void copyaline(fin, fout)
_TEXT *fin, *fout;
{
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', fout->f);
}


#define wid             5
#define dec             2



Static Void themain(infile, outfile)
_TEXT *infile, *outfile;
{
  boolean bad;
  long b, blength;

  long w;
  linebuffer buffer;
  Char c;
  double Rs, T = 2 - log(3.0) / log(2.0);

  /*





*/

  printf("rebasec %4.2f\n", version);

  if (*outfile->name != '\0') {
    if (outfile->f != NULL)
      outfile->f = freopen(outfile->name, "w", outfile->f);
    else
      outfile->f = fopen(outfile->name, "w");
  } else {
    if (outfile->f != NULL)
      rewind(outfile->f);
    else
      outfile->f = tmpfile();
  }
  if (outfile->f == NULL)
    _EscIO2(FileNotFound, outfile->name);
  SETUPBUF(outfile->f, Char);
  fprintf(outfile->f, "* rebasec %4.2f\n", version);
  fprintf(outfile->f, "* columns:\n");
  fprintf(outfile->f, "*   DNA sequence\n");
  fprintf(outfile->f, "*   b bases\n");
  fprintf(outfile->f, "*   R bits\n");
  fprintf(outfile->f, "*   D dimensionality\n");

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


  while (!BUFEOF(infile->f)) {
    if (P_eoln(infile->f)) {
      fscanf(infile->f, "%*[^\n]");
      getc(infile->f);
    } else {
      blength = 0;
      while (!P_eoln(infile->f) && blength < bufmax) {
	blength++;
	buffer[blength-1] = getc(infile->f);
	if (buffer[blength-1] == '\n')
	  buffer[blength-1] = ' ';
      }
      fscanf(infile->f, "%*[^\n]");



      getc(infile->f);
      Rs = 0.0;
      b = 1;
      bad = false;
      while (b <= blength) {
	c = buffer[b-1];

	if (c == 'A')
	  Rs += 2;
	if (c == 'C')
	  Rs += 2;
	if (c == 'G')
	  Rs += 2;
	if (c == 'T')
	  Rs += 2;

	if (c == 'R')
	  Rs++;
	if (c == 'Y')
	  Rs++;

	if (c == 'M')
	  Rs++;
	if (c == 'K')
	  Rs++;

	if (c == 'S')
	  Rs++;
	if (c == 'W')
	  Rs++;

	if (c == 'B')
	  Rs += T;
	if (c == 'D')
	  Rs += T;
	if (c == 'H')
	  Rs += T;
	if (c == 'V')
	  Rs += T;


	if (c != ')' && c != '(' && c != '/' && c != '-' && c != '^' &&
	    c != ' ' && c != 'N' && c != '9' && c != '8' && c != '7' &&
	    c != '6' && c != '5' && c != '4' && c != '3' && c != '2' &&
	    c != '1' && c != '0' && c != 'V' && c != 'H' && c != 'D' &&
	    c != 'B' && c != 'W' && c != 'S' && c != 'K' && c != 'M' &&
	    c != 'Y' && c != 'R' && c != 'T' && c != 'G' && c != 'C' &&
	    c != 'A')
/* p2c: rebasec.p, line 217: 
 * Note: Line breaker spent 0.0 seconds, 5000 tries on line 241 [251] */
	  bad = true;
	b++;
      }
      if (Rs == 0)
	bad = true;


      if (bad)
	fprintf(outfile->f, "* ");
      /*

*/
      putc(' ', outfile->f);
      for (w = 0; w < blength; w++)
	putc(buffer[w], outfile->f);
      fprintf(outfile->f, " %*.*f", wid, dec, Rs / 2);
      fprintf(outfile->f, " %*.*f", wid, dec, Rs);
      fprintf(outfile->f, " %*.*f", wid, dec, 2 * Rs);
    }



    fprintf(outfile->f, " %*.*fDIMENSION\n", wid, dec, 2 * Rs);
  }
}

#undef wid
#undef dec


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  data.f = NULL;
  strcpy(data.name, "data");
  enzymelist.f = NULL;
  strcpy(enzymelist.name, "enzymelist");
  themain(&enzymelist, &data);
_L1:
  if (enzymelist.f != NULL)
    fclose(enzymelist.f);
  if (data.f != NULL)
    fclose(data.f);
  exit(EXIT_SUCCESS);
}



/* End. */
