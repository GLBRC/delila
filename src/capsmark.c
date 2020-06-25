/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "capsmark.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*
*/
#define updateversion   1.00



/*









































*/


Static _TEXT sequ, capsmarkp, searchp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Char capitalize(c)
Char c;
{
  long n = c;

  if (n >= 'a' && n <= 'z')
    c = _toupper(n);

  return c;
}



Static Char decapitalize(c)
Char c;
{
  long n = c;

  if (n >= 'A' && n <= 'Z')
    c = _tolower(n);
  else
    c = (Char)n;

  return c;
}


#define maxnamelength   100



Static Void themain(sequ, capsmarkp, searchp)
_TEXT *sequ, *capsmarkp, *searchp;
{
  Char c;
  Char name[maxnamelength];
  long namelength = 0;
  long n;
  double parameterversion;
  long position = 0;
  boolean waslower = true;

  printf("capsmark %4.2f\n", version);
  if (*capsmarkp->name != '\0') {
    if (capsmarkp->f != NULL)
      capsmarkp->f = freopen(capsmarkp->name, "r", capsmarkp->f);
    else
      capsmarkp->f = fopen(capsmarkp->name, "r");
  } else
    rewind(capsmarkp->f);
  if (capsmarkp->f == NULL)
    _EscIO2(FileNotFound, capsmarkp->name);
  RESETBUF(capsmarkp->f, Char);
  fscanf(capsmarkp->f, "%lg%*[^\n]", &parameterversion);
  getc(capsmarkp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  while (!P_eoln(capsmarkp->f)) {
    namelength++;
    name[namelength-1] = getc(capsmarkp->f);
    if (name[namelength-1] == '\n')
      name[namelength-1] = ' ';
  }

  if (*sequ->name != '\0') {
    if (sequ->f != NULL)
      sequ->f = freopen(sequ->name, "r", sequ->f);
    else
      sequ->f = fopen(sequ->name, "r");
  } else
    rewind(sequ->f);
  if (sequ->f == NULL)
    _EscIO2(FileNotFound, sequ->name);
  RESETBUF(sequ->f, Char);
  if (*searchp->name != '\0') {
    if (searchp->f != NULL)
      searchp->f = freopen(searchp->name, "w", searchp->f);
    else
      searchp->f = fopen(searchp->name, "w");
  } else {
    if (searchp->f != NULL)
      rewind(searchp->f);
    else
      searchp->f = tmpfile();
  }
  if (searchp->f == NULL)
    _EscIO2(FileNotFound, searchp->name);
  SETUPBUF(searchp->f, Char);
  fprintf(searchp->f, "* capsmark %4.2f\n", version);


  while (!BUFEOF(sequ->f)) {
    if (P_eoln(sequ->f)) {
      fscanf(sequ->f, "%*[^\n]");
      /*

*/
      getc(sequ->f);
      continue;
    }
    c = getc(sequ->f);
    if (c == '\n')
      c = ' ';
    if (c == 'n' || c == 't' || c == 'g' || c == 'c' || c == 'a') {
      position++;
      if (!waslower)
	putc('\n', searchp->f);
      waslower = true;
      /*

*/
      continue;
    }
    if (c != 'N' && c != 'T' && c != 'G' && c != 'C' && c != 'A') {
      printf("unidentified character: %c\n", c);
      continue;
    }
    position++;
    if (waslower) {
      putc('"', searchp->f);
      for (n = 0; n < namelength; n++)
	putc(name[n], searchp->f);
      fprintf(searchp->f, ".%ld\"\n", position);
    }
    waslower = false;
    fputc(decapitalize(c), searchp->f);

    /*

*/
  }

  fprintf(searchp->f, "q\n");

  /*







*/

}

#undef maxnamelength


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  searchp.f = NULL;
  strcpy(searchp.name, "searchp");
  capsmarkp.f = NULL;
  strcpy(capsmarkp.name, "capsmarkp");
  sequ.f = NULL;
  strcpy(sequ.name, "sequ");
  themain(&sequ, &capsmarkp, &searchp);
_L1:
  if (sequ.f != NULL)
    fclose(sequ.f);
  if (capsmarkp.f != NULL)
    fclose(capsmarkp.f);
  if (searchp.f != NULL)
    fclose(searchp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
