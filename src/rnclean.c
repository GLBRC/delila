/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rnclean.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.04


/*
*/



/*

































































*/


Static _TEXT rnin, rncleanp, rnout;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(rnin, rncleanp, rnout)
_TEXT *rnin, *rncleanp, *rnout;
{
  Char c;
  long number;
  boolean wipe = false;

  /*
*/
  printf("rnclean %4.2f\n", version);
  if (*rnin->name != '\0') {
    if (rnin->f != NULL)
      rnin->f = freopen(rnin->name, "r", rnin->f);
    else
      rnin->f = fopen(rnin->name, "r");
  } else
    rewind(rnin->f);
  if (rnin->f == NULL)
    _EscIO2(FileNotFound, rnin->name);
  RESETBUF(rnin->f, Char);
  if (*rncleanp->name != '\0') {
    if (rncleanp->f != NULL)
      rncleanp->f = freopen(rncleanp->name, "r", rncleanp->f);
    else
      rncleanp->f = fopen(rncleanp->name, "r");
  } else
    rewind(rncleanp->f);
  if (rncleanp->f == NULL)
    _EscIO2(FileNotFound, rncleanp->name);
  RESETBUF(rncleanp->f, Char);
  if (!BUFEOF(rncleanp->f)) {
    if (P_peek(rncleanp->f) == 'w')
      wipe = true;
  }
  if (wipe)
    printf("highest news article number is now set to 1\n");
  if (*rnout->name != '\0') {
    if (rnout->f != NULL)
      rnout->f = freopen(rnout->name, "w", rnout->f);
    else
      rnout->f = fopen(rnout->name, "w");
  } else {
    if (rnout->f != NULL)
      rewind(rnout->f);
    else
      rnout->f = tmpfile();
  }
  if (rnout->f == NULL)
    _EscIO2(FileNotFound, rnout->name);
  SETUPBUF(rnout->f, Char);

  while (!BUFEOF(rnin->f)) {
    do {
      c = getc(rnin->f);
      if (c == '\n')
	c = ' ';
      putc(c, rnout->f);
    } while (c != '!' && c != ':');


    number = 0;
    while (!P_eoln(rnin->f)) {
      c = getc(rnin->f);
      if (c == '\n')
	c = ' ';
      if (c != '9' && c != '8' && c != '7' && c != '6' && c != '5' &&
	  c != '4' && c != '3' && c != '2' && c != '1' && c != '0' &&
	  c != '-' && c != ',' && c != ' ') {
	printf("ignoring strange character found: \"%c\"", c);
	printf(", ord(%c) = %d\n", c, c);
	continue;
      }
      switch (c) {

      case ' ':
	number = 0;
	break;

      case ',':
	number = 0;
	break;

      case '-':
	number = 0;
	break;

      case '0':
	number *= 10;
	break;

      case '1':
	number = number * 10 + 1;
	break;

      case '2':
	number = number * 10 + 2;
	break;

      case '3':
	number = number * 10 + 3;
	break;

      case '4':
	number = number * 10 + 4;
	break;

      case '5':
	number = number * 10 + 5;
	break;

      case '6':
	number = number * 10 + 6;
	break;

      case '7':
	number = number * 10 + 7;
	break;

      case '8':
	number = number * 10 + 8;
	break;

      case '9':
	number = number * 10 + 9;
	break;
      }
    }
    fscanf(rnin->f, "%*[^\n]");

    getc(rnin->f);
    if (number < 1)
      number = 1;
    if (wipe)
      number = 1;
    fprintf(rnout->f, " 1-%ld\n", number);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  rnout.f = NULL;
  strcpy(rnout.name, "rnout");
  rncleanp.f = NULL;
  strcpy(rncleanp.name, "rncleanp");
  rnin.f = NULL;
  strcpy(rnin.name, "rnin");
  themain(&rnin, &rncleanp, &rnout);
_L1:
  if (rnin.f != NULL)
    fclose(rnin.f);
  if (rncleanp.f != NULL)
    fclose(rncleanp.f);
  if (rnout.f != NULL)
    fclose(rnout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
