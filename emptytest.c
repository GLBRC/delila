/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "emptytest.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         2.03
/*






*/
#define updateversion   1.00



/*

































































































































*/


Static _TEXT empty, notempty;



Static boolean strictemptyfile(afile)
_TEXT *afile;
{
  /*







*/
  boolean Result;
  long lines = 0, chars = 0;
  Char ch;

  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  while (!BUFEOF(afile->f) && chars < 1) {
    if (P_eoln(afile->f)) {
      lines++;
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
    } else {
      ch = getc(afile->f);
      if (ch == '\n')
	ch = ' ';
      chars++;
    }
  }
  if (chars == 0)
    Result = true;
  else
    Result = false;
  printf("lines = %ld\n", lines);
  printf("chars = %ld\n", chars);
  return Result;
}


#define debugging       false

#define boundary        2



Static boolean emptyfile(afile)
_TEXT *afile;
{
  /*










*/
  boolean Result;
  /*
*/
  long lines = 0, chars = 0;
  Char ch;

  if (*afile->name != '\0') {
    if (afile->f != NULL)
      afile->f = freopen(afile->name, "r", afile->f);
    else
      afile->f = fopen(afile->name, "r");
  } else
    rewind(afile->f);
  if (afile->f == NULL)
    _EscIO2(FileNotFound, afile->name);
  RESETBUF(afile->f, Char);
  while (!BUFEOF(afile->f) && chars < boundary) {
    if (P_eoln(afile->f)) {
      lines++;
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
      continue;
    }
    ch = getc(afile->f);
    if (ch == '\n')
      ch = ' ';
    if (debugging) {
      printf("emptyfile: ord(ch) = %12d\n", ch);
      printf("emptyfile:     ch  = %c\n", ch);
    }
    chars++;
  }
  if (chars < boundary)
    Result = true;
  else
    Result = false;
  if (!debugging)
    return Result;
  printf("emptyfile: lines = %ld\n", lines);
  printf("emptyfile: chars = %ld\n", chars);
  if (chars < boundary)
    printf("emptyfile: chars < boundary, %ld<%ld\n", chars, (long)boundary);
  else
    printf("emptyfile: chars < boundary, %ld>=%ld\n", chars, (long)boundary);
  return Result;
}

#undef debugging
#undef boundary



Static Void themain(empty, noempty)
_TEXT *empty, *noempty;
{
  printf("emptytest %4.2f\n", version);

  printf("----  emptytest: -------------------------\n");

  printf("----  test eof():\n");
  if (*empty->name != '\0') {
    if (empty->f != NULL)
      empty->f = freopen(empty->name, "r", empty->f);
    else
      empty->f = fopen(empty->name, "r");
  } else
    rewind(empty->f);
  if (empty->f == NULL)
    _EscIO2(FileNotFound, empty->name);
  RESETBUF(empty->f, Char);
  if (BUFEOF(empty->f))
    printf("empty    file is     eof ....... OK!\n");
  else
    printf("empty    file is not eof ..... OOPS!\n");
  if (*notempty.name != '\0') {
    if (notempty.f != NULL)
      notempty.f = freopen(notempty.name, "r", notempty.f);
    else
      notempty.f = fopen(notempty.name, "r");
  } else {

    rewind(notempty.f);
  }
  if (notempty.f == NULL)
    _EscIO2(FileNotFound, notempty.name);
  RESETBUF(notempty.f, Char);
  if (BUFEOF(notempty.f))
    printf("notempty file is     eof ...... OPS!\n");
  else
    printf("notempty file is not eof ....... OK!\n");

  printf("----  test emptyfile():\n");
  if (emptyfile(empty))
    printf("empty    file is     empty ..... OK!\n");
  else
    printf("empty    file is not empty ... OOPS!\n");
  if (emptyfile(&notempty))
    printf("notempty file is     empty ... OOPS!\n");
  else
    printf("notempty file is not empty ..... OK!\n");

  printf("------------------------------------------\n");

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  notempty.f = NULL;
  strcpy(notempty.name, "notempty");
  empty.f = NULL;
  strcpy(empty.name, "empty");
  themain(&empty, &notempty);
  if (empty.f != NULL)
    fclose(empty.f);
  if (notempty.f != NULL)
    fclose(notempty.f);
  exit(EXIT_SUCCESS);
}



/* End. */
