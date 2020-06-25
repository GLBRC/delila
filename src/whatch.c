/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "whatch.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*



*/


#define version         1.13
/*


*/



/*

































*/




#define maxchars        255


typedef struct charident {
  Char ch;
  long nu;
} charident;


Static _TEXT fin, fout;

Static charident chars[maxchars + 1];
Static uchar index_;
Static long count;


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


main(argc, argv)
int argc;
Char *argv[];
{
  short TEMP;
  charident *WITH;

  PASCAL_MAIN(argc, argv);
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  printf(" whatch %4.2f\n", version);
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
  fprintf(fout.f, " whatch %4.2f\n", version);
  putc(' ', fout.f);
  if (!BUFEOF(fin.f))
    copyaline(&fin, &fout);
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
  putc('\n', fout.f);


  for (TEMP = 0; TEMP <= maxchars; TEMP++) {
    index_ = TEMP;


    WITH = &chars[index_];
    WITH->ch = ' ';
    WITH->nu = 0;
  }
/* p2c: whatch.p, line 102:
 * Note: Rewritten FOR loop won't work if it meddles with index_ [253] */
  count = 0;
  while (!BUFEOF(fin.f)) {
    count++;
    if (P_peek(fin.f) > maxchars) {
      fprintf(fout.f,
	" character with ord = %d this is greater than maxchars (%ld)\n",
	P_peek(fin.f), (long)maxchars);
      printf(" increase maxchars\n");
    } else if (P_peek(fin.f) < 0)
      fprintf(fout.f, " character with negative ord: %d\n", P_peek(fin.f));
    else {
      WITH = &chars[P_peek(fin.f)];
      if (WITH->nu > 0)
	WITH->nu++;
      else {
	WITH->nu = 1;
	WITH->ch = P_peek(fin.f);
      }
    }

    getc(fin.f);
  }


  fprintf(fout.f, "\n %ld characters found\n", count);
  if (count != 0) {
    fprintf(fout.f, "\n char ord(char) count percent\n");
    for (TEMP = 0; TEMP <= maxchars; TEMP++) {
      index_ = TEMP;
      if (chars[index_].nu != 0) {
	fprintf(fout.f, "  %c", chars[index_].ch);
	fprintf(fout.f, "     %5d", index_);
	fprintf(fout.f, "   %5ld", chars[index_].nu);
	fprintf(fout.f, " %7.2f", 100 * ((double)chars[index_].nu / count));
	if (chars[index_].ch != index_) {
	  fprintf(fout.f, " warning: program error,");
	  fprintf(fout.f, " actual character is different");
	}
	putc('\n', fout.f);
      }
    }
/* p2c: whatch.p, line 149:
 * Note: Rewritten FOR loop won't work if it meddles with index_ [253] */
  }
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
