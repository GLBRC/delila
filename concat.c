/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "concat.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*


*/


#define version         1.08


/*
*/



/*





























*/



Static _TEXT afile, bfile, abfile;


Static Void unlimitln(afile)
_TEXT *afile;
{
  /*


*/
}


Static Void onecopy(sin, sout)
_TEXT *sin, *sout;
{
  Char c;

  while (!BUFEOF(sin->f)) {
    while (!P_eoln(sin->f)) {
      c = getc(sin->f);
      if (c == '\n')
	c = ' ';
      putc(c, sout->f);
    }
    fscanf(sin->f, "%*[^\n]");
    getc(sin->f);
    putc('\n', sout->f);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  abfile.f = NULL;
  strcpy(abfile.name, "abfile");
  bfile.f = NULL;
  strcpy(bfile.name, "bfile");
  afile.f = NULL;
  strcpy(afile.name, "afile");
  printf(" concat %4.2f\n", version);

  if (*afile.name != '\0') {
    if (afile.f != NULL)
      afile.f = freopen(afile.name, "r", afile.f);
    else
      afile.f = fopen(afile.name, "r");
  } else
    rewind(afile.f);
  if (afile.f == NULL)
    _EscIO2(FileNotFound, afile.name);
  RESETBUF(afile.f, Char);
  if (*bfile.name != '\0') {
    if (bfile.f != NULL)
      bfile.f = freopen(bfile.name, "r", bfile.f);
    else
      bfile.f = fopen(bfile.name, "r");
  } else
    rewind(bfile.f);
  if (bfile.f == NULL)
    _EscIO2(FileNotFound, bfile.name);
  RESETBUF(bfile.f, Char);
  if (*abfile.name != '\0') {
    if (abfile.f != NULL)
      abfile.f = freopen(abfile.name, "w", abfile.f);
    else
      abfile.f = fopen(abfile.name, "w");
  } else {
    if (abfile.f != NULL)
      rewind(abfile.f);
    else
      abfile.f = tmpfile();
  }
  if (abfile.f == NULL)
    _EscIO2(FileNotFound, abfile.name);
  SETUPBUF(abfile.f, Char);
  unlimitln(&abfile);

  if (!BUFEOF(afile.f))
    onecopy(&afile, &abfile);
  else
    printf("afile is empty\n");

  if (!BUFEOF(bfile.f))
    onecopy(&bfile, &abfile);
  else {
    printf("bfile is empty\n");

  }
  if (afile.f != NULL)
    fclose(afile.f);
  if (bfile.f != NULL)
    fclose(bfile.f);
  if (abfile.f != NULL)
    fclose(abfile.f);
  exit(EXIT_SUCCESS);
}



/* End. */
