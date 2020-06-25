/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "bookends.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*
*/
#define updateversion   1.00



/*
































*/


Static _TEXT book, pairs;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void getcoords(afile, bfile)
_TEXT *afile, *bfile;
{
  /*
*/
  Char ch = ' ';
  long firstnum, lastnum;


  while (!BUFEOF(afile->f) && ch != 'p') {
    fscanf(afile->f, "%c%*[^\n]", &ch);
    getc(afile->f);
    if (ch == '\n')
      ch = ' ';
  }

  if (BUFEOF(afile->f))
    return;


  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  fscanf(afile->f, "%*[^\n]");

  /*
*/
  getc(afile->f);
  fscanf(afile->f, "%c%*[^\n]", &ch);
  getc(afile->f);
  if (ch == '\n')
    ch = ' ';
  if (ch == 'n') {
    do {
      fscanf(afile->f, "%c%*[^\n]", &ch);
      getc(afile->f);
      if (ch == '\n')
	ch = ' ';
    } while (ch != 'n');
    fscanf(afile->f, "%c%*[^\n]", &ch);
    getc(afile->f);
    if (ch == '\n')
      ch = ' ';
  }


  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  fscanf(afile->f, "%*[^\n]");


  getc(afile->f);
  fscanf(afile->f, "%c%ld%*[^\n]", &ch, &firstnum);
  getc(afile->f);
  if (ch == '\n')
    ch = ' ';
  fscanf(afile->f, "%c%ld%*[^\n]", &ch, &lastnum);
  getc(afile->f);


  if (ch == '\n')
    ch = ' ';
  fprintf(bfile->f, "%ld %ld\n", firstnum, lastnum);


  do {
    fscanf(afile->f, "%c%*[^\n]", &ch);
    getc(afile->f);

    if (ch == '\n')
      ch = ' ';
  } while (ch != 'p');
}



Static Void themain(book, pairs)
_TEXT *book, *pairs;
{

  /*


*/
  printf("bookends %4.2f\n", version);
  /*






*/

  if (*book->name != '\0') {
    if (book->f != NULL)
      book->f = freopen(book->name, "r", book->f);
    else
      book->f = fopen(book->name, "r");
  } else
    rewind(book->f);
  if (book->f == NULL)
    _EscIO2(FileNotFound, book->name);
  RESETBUF(book->f, Char);
  if (*pairs->name != '\0') {
    if (pairs->f != NULL)
      pairs->f = freopen(pairs->name, "w", pairs->f);
    else
      pairs->f = fopen(pairs->name, "w");
  } else {
    if (pairs->f != NULL)
      rewind(pairs->f);
    else
      pairs->f = tmpfile();
  }
  if (pairs->f == NULL)
    _EscIO2(FileNotFound, pairs->name);
  SETUPBUF(pairs->f, Char);

  while (!BUFEOF(book->f))
    getcoords(book, pairs);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  pairs.f = NULL;
  strcpy(pairs.name, "pairs");
  book.f = NULL;
  strcpy(book.name, "book");
  themain(&book, &pairs);
_L1:
  if (book.f != NULL)
    fclose(book.f);
  if (pairs.f != NULL)
    fclose(pairs.f);
  exit(EXIT_SUCCESS);
}



/* End. */
