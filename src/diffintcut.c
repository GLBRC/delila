/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "diffintcut.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.10
/*




*/
#define updateversion   1.00



/*










































































*/


Static _TEXT diffintcutp;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define tab             9






Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == tab);
}

#undef tab


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}


#define showparameters  false



Static Void themain(diffintcutp)
_TEXT *diffintcutp;
{
  long current, previous, c, column, difference, width, lower, upper;
  boolean haveprevious = false;
  double parameterversion;
  _TEXT TEMP;

  if (showparameters)
    printf("* diffintcut %4.2f\n", version);
  if (*diffintcutp->name != '\0') {
    if (diffintcutp->f != NULL)
      diffintcutp->f = freopen(diffintcutp->name, "r", diffintcutp->f);
    else
      diffintcutp->f = fopen(diffintcutp->name, "r");
  } else
    rewind(diffintcutp->f);
  if (diffintcutp->f == NULL)
    _EscIO2(FileNotFound, diffintcutp->name);
  RESETBUF(diffintcutp->f, Char);
  fscanf(diffintcutp->f, "%lg%*[^\n]", &parameterversion);
  getc(diffintcutp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(diffintcutp->f, "%ld%*[^\n]", &column);
  getc(diffintcutp->f);
  if (column < 1) {
    printf("column (%ld) < 1\n", column);
    halt();
  }

  fscanf(diffintcutp->f, "%ld%ld%*[^\n]", &lower, &upper);
  getc(diffintcutp->f);
  if (lower > upper) {
    printf("lower (%ld) > upper (%ld) \n", lower, upper);
    halt();
  }

  fscanf(diffintcutp->f, "%ld%*[^\n]", &width);
  getc(diffintcutp->f);
  if (width < 1) {
    printf("width (%ld) < 1\n", width);
    halt();
  }

  if (showparameters) {
    printf("* parameterversion: %4.2f\n", parameterversion);
    printf("* column: %ld\n", column);
    printf("* lower: %ld\n", lower);
    printf("* upper: %ld\n", upper);
    printf("* width: %ld\n", width);
  }

  while (!P_eof(stdin)) {
    if (P_peek(stdin) == '*') {
      while (!P_eoln(stdin)) {
	putchar(P_peek(stdin));
	getc(stdin);
      }
      haveprevious = false;
      scanf("%*[^\n]");
      getchar();
      putchar('\n');
      continue;
    }

    c = 1;
    while (c < column) {
      TEMP.f = stdin;
      *TEMP.name = '\0';
      skipcolumn(&TEMP);
      c++;
    }
    scanf("%ld%*[^\n]", &current);
    getchar();

    if (haveprevious) {
      difference = current - previous;
      if (difference >= lower) {
	if (difference <= upper)
	  printf("%*ld %*ld %*ld\n",
		 (int)width, current - previous, (int)width, previous,
		 (int)width, current);
      }
    }

    haveprevious = true;
    previous = current;
  }

}

#undef showparameters


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  diffintcutp.f = NULL;
  strcpy(diffintcutp.name, "diffintcutp");
  themain(&diffintcutp);
_L1:
  if (diffintcutp.f != NULL)
    fclose(diffintcutp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
