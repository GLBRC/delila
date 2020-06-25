/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "lenin.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.24


/*
*/



/*





















































*/


Static _TEXT lengths, leninp, finst, linst;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void gettitle(lengths, fout)
_TEXT *lengths, *fout;
{
  /*








*/
  Char a, b;
  boolean found = false;

  if (*lengths->name != '\0') {
    if (lengths->f != NULL)
      lengths->f = freopen(lengths->name, "r", lengths->f);
    else
      lengths->f = fopen(lengths->name, "r");
  } else
    rewind(lengths->f);
  if (lengths->f == NULL)
    _EscIO2(FileNotFound, lengths->name);
  RESETBUF(lengths->f, Char);

  while (!found) {
    if (BUFEOF(lengths->f)) {
      printf("delila instructions not found in lengths file!\n");
      halt();
    }
    fscanf(lengths->f, "%c%c%*[^\n]", &a, &b);
    getc(lengths->f);
    if (a == '\n')
      a = ' ';
    if (b == '\n')
      b = ' ';
    if (a != '*') {
      printf("While locating Delila instructions\n");
      printf("Missing * in lengths file!  Found \"%c\"\n", a);
      halt();
    }
    if (b == '*')
      found = true;
  }


  found = false;
  while (!found) {
    a = getc(lengths->f);
    b = getc(lengths->f);
    if (a == '\n')
      a = ' ';
    if (b == '\n')
      b = ' ';
    if (a != '*') {
      printf("While copying Delila instructions\n");
      printf("Missing * in lengths file!  Found \"%c\"\n", a);
      halt();
    }
    if (b == '*') {
      found = true;
      fscanf(lengths->f, "%*[^\n]");
      getc(lengths->f);
      continue;
    }
    while (!P_eoln(lengths->f)) {
      putc(P_peek(lengths->f), fout->f);
      getc(lengths->f);
    }
    fscanf(lengths->f, "%*[^\n]");
    getc(lengths->f);
    putc('\n', fout->f);
  }


  /*

*/
}



Static Void copytitle(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  fprintf(fout->f, "title \"");
  while (!P_eoln(fin->f)) {
    putc(P_peek(fin->f), fout->f);
    getc(fin->f);
  }
  fprintf(fout->f, "\";\n");
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}



Static Void copytotwo(fin, fout1, fout2)
_TEXT *fin, *fout1, *fout2;
{
  /*
*/
  Char c;

  while (!P_eoln(fin->f)) {
    c = getc(lengths.f);
    if (c == '\n')
      c = ' ';
    putc(c, fout1->f);
    putc(c, fout2->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}



Static Void sign(thefile, number)
_TEXT *thefile;
long number;
{
  if (number >= 0)
    fprintf(thefile->f, "+%ld", number);
  else
    fprintf(thefile->f, "%ld", number);
}



Static Void themain(lengths, leninp, finst, linst)
_TEXT *lengths, *leninp, *finst, *linst;
{
  Char c;
  long count = 0;
  long firstposition, lastposition, length, thefromfirst, thetofirst,
       thefromlast, thetolast;

  printf("lenin %4.2f\n", version);

  if (*lengths->name != '\0') {
    if (lengths->f != NULL)
      lengths->f = freopen(lengths->name, "r", lengths->f);
    else
      lengths->f = fopen(lengths->name, "r");
  } else
    rewind(lengths->f);
  if (lengths->f == NULL)
    _EscIO2(FileNotFound, lengths->name);
  RESETBUF(lengths->f, Char);
  if (*leninp->name != '\0') {
    if (leninp->f != NULL)
      leninp->f = freopen(leninp->name, "r", leninp->f);
    else
      leninp->f = fopen(leninp->name, "r");
  } else
    rewind(leninp->f);
  if (leninp->f == NULL)
    _EscIO2(FileNotFound, leninp->name);
  RESETBUF(leninp->f, Char);
  if (*finst->name != '\0') {
    if (finst->f != NULL)
      finst->f = freopen(finst->name, "w", finst->f);
    else
      finst->f = fopen(finst->name, "w");
  } else {
    if (finst->f != NULL)
      rewind(finst->f);
    else
      finst->f = tmpfile();
  }
  if (finst->f == NULL)
    _EscIO2(FileNotFound, finst->name);
  SETUPBUF(finst->f, Char);
  fprintf(finst->f, "(* lenin %4.2f *)\n", version);
  if (*linst->name != '\0') {
    if (linst->f != NULL)
      linst->f = freopen(linst->name, "w", linst->f);
    else
      linst->f = fopen(linst->name, "w");
  } else {
    if (linst->f != NULL)
      rewind(linst->f);
    else
      linst->f = tmpfile();
  }
  if (linst->f == NULL)
    _EscIO2(FileNotFound, linst->name);
  SETUPBUF(linst->f, Char);
  fprintf(linst->f, "(* lenin %4.2f *)\n", version);

  gettitle(lengths, finst);
  gettitle(lengths, linst);

  fscanf(leninp->f, "%ld%ld%*[^\n]", &thefromfirst, &thetofirst);
  getc(leninp->f);
  fscanf(leninp->f, "%ld%ld%*[^\n]", &thefromlast, &thetolast);
  getc(leninp->f);
  printf("First: From %ld to %ld\n", thefromfirst, thetofirst);
  printf(" Last: From %ld to %ld\n", thefromlast, thetolast);
  /*


*/

  while (!BUFEOF(lengths->f)) {
    while (P_peek(lengths->f) == '*') {
      c = getc(lengths->f);
      if (c == '\n')
	c = ' ';
      if (P_peek(lengths->f) != '*') {
	fscanf(lengths->f, "%*[^\n]");
	getc(lengths->f);
	continue;
      }

      c = getc(lengths->f);
      if (c == '\n')
	c = ' ';
      copytotwo(lengths, finst, linst);
      putc('\n', finst->f);
      putc('\n', linst->f);
    }

    fscanf(lengths->f, "%ld%ld%ld", &length, &firstposition, &lastposition);

    putc('\n', finst->f);
    putc('\n', linst->f);
    fprintf(finst->f, "piece ");
    fprintf(linst->f, "piece ");


    copytotwo(lengths, finst, linst);
    fprintf(finst->f, ";\n");
    fprintf(linst->f, ";\n");


    count++;
    fprintf(finst->f, "get from %ld ", firstposition);
    sign(finst, thefromfirst);
    fprintf(finst->f, " to %ld ", firstposition);
    sign(finst, thetofirst);
    fprintf(finst->f, "; (* %ld *)\n", count);

    fprintf(linst->f, "get from %ld ", lastposition);
    sign(linst, thefromlast);
    fprintf(linst->f, " to %ld ", lastposition);
    sign(linst, thetolast);
    fprintf(linst->f, "; (* %ld *)\n", count);
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  linst.f = NULL;
  strcpy(linst.name, "linst");
  finst.f = NULL;
  strcpy(finst.name, "finst");
  leninp.f = NULL;
  strcpy(leninp.name, "leninp");
  lengths.f = NULL;
  strcpy(lengths.name, "lengths");
  themain(&lengths, &leninp, &finst, &linst);
_L1:
  if (lengths.f != NULL)
    fclose(lengths.f);
  if (leninp.f != NULL)
    fclose(leninp.f);
  if (finst.f != NULL)
    fclose(finst.f);
  if (linst.f != NULL)
    fclose(linst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
