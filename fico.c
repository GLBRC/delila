/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "fico.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*








































*/


Static _TEXT ficop;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define HIGH            127



Static Void themain(fin, ficop)
_TEXT *fin, *ficop;
{
  Char c;
  long controlsfound = 0, linenumber = 1;
  Char s;

  printf("fico %4.2f\n", version);
  printf("Characters with ord > %ld will be shown.\n", (long)HIGH);
  /*






*/

  while (!BUFEOF(fin->f)) {
    if (P_eoln(fin->f)) {
      linenumber++;
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      continue;
    }
    c = getc(fin->f);
    if (c == '\n')
      c = ' ';
    if (c > HIGH) {
      printf("line %ld\n", linenumber);
      controlsfound++;
    }
  }
  printf("number of lines in the input file: %ld\n", linenumber);
  if (controlsfound == 1)
    s = ' ';
  else
    s = 's';
  printf("%ld control character%c found in input file\n", controlsfound, s);

}

#undef HIGH


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  ficop.f = NULL;
  strcpy(ficop.name, "ficop");
  TEMP.f = stdin;
  *TEMP.name = '\0';
  themain(&TEMP, &ficop);
_L1:
  if (ficop.f != NULL)
    fclose(ficop.f);
  exit(EXIT_SUCCESS);
}



/* End. */
