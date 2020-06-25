/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "chacha.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         3.12
/*
*/



/*























































*/


#define top             100


typedef struct buffer {
  Char place[top];
  long length;
} buffer;


Static _TEXT fin, fout, chachap;

Static buffer chafrom, chato, b;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static Void readintobuffer(fin, b)
_TEXT *fin;
buffer *b;
{
  long i = 0;

  b->length = 0;

  while ((i <= top) & (!(P_eoln(fin->f) | BUFEOF(fin->f)))) {
    i++;
    b->place[i-1] = getc(fin->f);
    if (b->place[i-1] == '\n')
      b->place[i-1] = ' ';
    b->length = i;
  }

  if (!BUFEOF(fin->f)) {
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }

}


Static Void removeblanks(b)
buffer *b;
{
  if (b->length == 0) {
    printf("\n no blank lines allowed in chachap.\n\n");
    halt();
  }
  if (b->place[b->length - 1] == ' ') {
    do {
      b->length--;
    } while (b->place[b->length - 1] == ' ');
  }
}


Static Void checkchachap()
{
  if (chafrom.length <= 0 || chato.length <= 0) {
    printf("\n no blank lines allowed in chachap.\n\n");
    halt();
  }

  if (chafrom.length == chato.length)
    return;
  printf("\n the 2 lines in chachap do not contain");
  printf(" the same number of characters.\n\n");
  halt();
}


Static Void translate(fin, fout)
_TEXT *fin, *fout;
{
  long i;
  Char inptchar, tranchar;

  /*

*/

  while (!BUFEOF(fin->f)) {
    while (!P_eoln(fin->f)) {
      inptchar = getc(fin->f);
      if (inptchar == '\n')
	inptchar = ' ';
      i = 0;
      do {
	i++;
      } while (chafrom.place[i-1] != inptchar && chafrom.length != i);

      if (chafrom.place[i-1] != inptchar)
	putc(inptchar, fout->f);
      else {
	tranchar = chato.place[i-1];
	putc(tranchar, fout->f);
      }
    }
    putc('\n', fout->f);
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  chachap.f = NULL;
  strcpy(chachap.name, "chachap");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  printf(" chacha %4.2f\n", version);
  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  if (*chachap.name != '\0') {
    if (chachap.f != NULL)
      chachap.f = freopen(chachap.name, "r", chachap.f);
    else
      chachap.f = fopen(chachap.name, "r");
  } else
    rewind(chachap.f);
  if (chachap.f == NULL)
    _EscIO2(FileNotFound, chachap.name);
  RESETBUF(chachap.f, Char);
  if (*fout.name != '\0') {
    if (fout.f != NULL)
      fout.f = freopen(fout.name, "w", fout.f);
    else
      fout.f = fopen(fout.name, "w");
  } else {
    if (fout.f != NULL)
      rewind(fout.f);
    else
      fout.f = tmpfile();
  }
  if (fout.f == NULL)
    _EscIO2(FileNotFound, fout.name);
  SETUPBUF(fout.f, Char);

  readintobuffer(&chachap, &chafrom);
  removeblanks(&chafrom);
  readintobuffer(&chachap, &chato);
  removeblanks(&chato);
  checkchachap();

  translate(&fin, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (chachap.f != NULL)
    fclose(chachap.f);
  exit(EXIT_SUCCESS);
}



/* End. */
