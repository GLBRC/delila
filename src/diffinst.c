/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "diffinst.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*








*/



#define version         1.08
/*





*/
#define updateversion   1.00



/*




















































*/


Static _TEXT insta, instb, data;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}


#define asterisk        '*'
#define leftcurly       '{'
#define leftparen       '('
#define rightcurly      '}'
#define rightparen      ')'



Static Void delilacomment(previous, current, comparen, comcurly)
Char previous, current;
boolean *comparen, *comcurly;
{
  /*

*/
  if (previous == leftparen && current == asterisk)
    *comparen = true;

  if (*comparen && !*comcurly && previous == asterisk && current == rightparen)
    *comparen = false;

  if (!*comparen && current == leftcurly)
    *comcurly = true;

  if (*comcurly && !*comparen && current == rightcurly)
    *comcurly = false;
}

#undef asterisk
#undef leftcurly
#undef leftparen
#undef rightcurly
#undef rightparen






Static Void isitget(thefile, getbool)
_TEXT *thefile;
boolean *getbool;
{
  Char c;

  *getbool = false;
  if (P_eoln(thefile->f)) {
    *getbool = false;
    return;
  }
  while (!P_eoln(thefile->f) && *getbool == false) {
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
    if (c != 'g') {
      *getbool = false;
      continue;
    }
    if (P_eoln(thefile->f)) {
      *getbool = false;
      continue;
    }
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
    if (c != 'e') {
      *getbool = false;
      continue;
    }
    if (P_eoln(thefile->f)) {
      *getbool = false;


      continue;
    }
    c = getc(thefile->f);
    if (c == '\n')
      c = ' ';
    if (c == 't')
      *getbool = true;
    else
      *getbool = false;
  }

}



Static Void finddiff(coorda, coordb, orienta, orientb)
long coorda, coordb, orienta, orientb;
{
  long difference;

  if (orienta <= 0 && orientb <= 0)
    difference = coorda - coordb;
  if (orienta > 0 && orientb > 0)
    difference = coordb - coorda;
  if ((orienta <= 0 || orientb <= 0) && (orienta > 0 || orientb > 0)) {
    printf("Corresponding coordinates need to have the same orientation\n");
    halt();
  }
  fprintf(data.f, "%ld\n", difference);
}



Static Void diffinstloop()
{
  long coorda, coordb, orienta, orientb;
  boolean getboola = false, getboolb = false;
  Char previous = ' ';

  if (*insta.name != '\0') {
    if (insta.f != NULL)
      insta.f = freopen(insta.name, "r", insta.f);
    else
      insta.f = fopen(insta.name, "r");
  } else {
/* p2c: diffinst.p: Note: Eliminated unused assignment statement [338] */
/* p2c: diffinst.p: Note: Eliminated unused assignment statement [338] */
    rewind(insta.f);
  }
  if (insta.f == NULL)
    _EscIO2(FileNotFound, insta.name);
  RESETBUF(insta.f, Char);
  if (*instb.name != '\0') {
    if (instb.f != NULL)
      instb.f = freopen(instb.name, "r", instb.f);
    else
      instb.f = fopen(instb.name, "r");
  } else
    rewind(instb.f);
  if (instb.f == NULL)
    _EscIO2(FileNotFound, instb.name);
  RESETBUF(instb.f, Char);
  if (*data.name != '\0') {
    if (data.f != NULL)
      data.f = freopen(data.name, "w", data.f);
    else
      data.f = fopen(data.name, "w");
  } else {
    if (data.f != NULL)
      rewind(data.f);
    else
      data.f = tmpfile();
  }
  if (data.f == NULL)
    _EscIO2(FileNotFound, data.name);
  SETUPBUF(data.f, Char);
  while ((!BUFEOF(insta.f)) & (!BUFEOF(instb.f))) {
    isitget(&insta, &getboola);
    while ((getboola == false) & (!BUFEOF(insta.f))) {
      fscanf(insta.f, "%*[^\n]");
      getc(insta.f);
      isitget(&insta, &getboola);
    }
    isitget(&instb, &getboolb);
    while ((getboolb == false) & (!BUFEOF(instb.f))) {
      fscanf(instb.f, "%*[^\n]");
      getc(instb.f);
      isitget(&instb, &getboolb);
    }
    if (getboola != true || getboolb != true)
      continue;
    skipcolumn(&insta);
    skipcolumn(&instb);
    fscanf(insta.f, "%ld", &coorda);
    fscanf(insta.f, "%ld%*[^\n]", &orienta);
    getc(insta.f);
    fscanf(instb.f, "%ld", &coordb);
    fscanf(instb.f, "%ld%*[^\n]", &orientb);
    getc(instb.f);
    finddiff(coorda, coordb, orienta, orientb);
  }
}



Static Void themain()
{
  printf("diffinst %4.2f\n", version);
  diffinstloop();
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  data.f = NULL;
  strcpy(data.name, "data");
  instb.f = NULL;
  strcpy(instb.name, "instb");
  insta.f = NULL;
  strcpy(insta.name, "insta");
  themain();
_L1:
  if (insta.f != NULL)
    fclose(insta.f);
  if (instb.f != NULL)
    fclose(instb.f);
  if (data.f != NULL)
    fclose(data.f);
  exit(EXIT_SUCCESS);
}



/* End. */
