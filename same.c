/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "same.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/


#define version         1.12


/*
*/



/*




























*/




Static _TEXT a, b;
Static long counter;
Static boolean match, empty;



Static boolean complines(a, b)
_TEXT *a, *b;
{
  /*







*/
  boolean Result;
  Char chara, charb;
  boolean done = false;

  if (BUFEOF(a->f) & BUFEOF(b->f))
    return true;
  if (BUFEOF(a->f) | BUFEOF(b->f))
    return false;
  while (!done) {
    if (P_eoln(a->f))
      chara = ' ';
    else {
      chara = getc(a->f);
      if (chara == '\n')
	chara = ' ';
    }

    if (P_eoln(b->f))
      charb = ' ';
    else {
      charb = getc(b->f);
      if (charb == '\n')
	charb = ' ';
    }

    if (P_eoln(a->f) & P_eoln(b->f)) {
      done = true;
      Result = true;
    } else if (chara != charb) {
      done = true;
      Result = false;
    }
  }
  fscanf(a->f, "%*[^\n]");
  getc(a->f);
  fscanf(b->f, "%*[^\n]");
  getc(b->f);
  return Result;
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  b.f = NULL;
  strcpy(b.name, "b");
  a.f = NULL;
  strcpy(a.name, "a");
  printf(" same %4.2f\n", version);
  if (*a.name != '\0') {
    if (a.f != NULL)
      a.f = freopen(a.name, "r", a.f);
    else
      a.f = fopen(a.name, "r");
  } else
    rewind(a.f);
  if (a.f == NULL)
    _EscIO2(FileNotFound, a.name);
  RESETBUF(a.f, Char);
  if (*b.name != '\0') {
    if (b.f != NULL)
      b.f = freopen(b.name, "r", b.f);
    else
      b.f = fopen(b.name, "r");
  } else
    rewind(b.f);
  if (b.f == NULL)
    _EscIO2(FileNotFound, b.name);
  RESETBUF(b.f, Char);
  match = false;

  counter = 0;
  empty = false;

  if (BUFEOF(a.f) | BUFEOF(b.f))
    empty = true;





  if (BUFEOF(a.f) & BUFEOF(b.f))
    printf(" both files are empty\n");
  else if (BUFEOF(a.f))
    printf(" file a is empty\n");
  else if (BUFEOF(b.f))
    printf(" file b is empty\n");
  else {
    match = true;

    while (match & (!(BUFEOF(a.f) | BUFEOF(b.f)))) {
      match = complines(&a, &b);
      if (match)
	counter++;
    }
  }
  if (match) {
    if (BUFEOF(a.f) & BUFEOF(b.f)) {
      printf(" files a and b are identical.\n");
      printf(" each contains %ld line(s).\n", counter);
    } else {
      if (BUFEOF(a.f) & (!BUFEOF(b.f)))
	printf(" file a is shorter than file b\n");
      else if (BUFEOF(b.f) & (!BUFEOF(a.f)))
	printf(" file b is shorter than file a\n");
      printf(" and ends at line %ld.  up to and including line %ld,\n",
	     counter, counter);
      printf(" the two files are identical.\n");
    }
  } else {
    if (counter > 0) {
      printf(" files a and b are different from each other \n");
      printf(" after the first %ld lines, which are identical.\n", counter);
    } else if (counter == 0 && !empty)
      printf(" files a and b differ from each other in the first line.\n");
  }
  if (a.f != NULL)
    fclose(a.f);
  if (b.f != NULL)
    fclose(b.f);
  exit(EXIT_SUCCESS);
}



/* End. */
