/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "ll.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/


#define version         1.04


/*


*/



/*

























*/



Static Void themain(a, b)
_TEXT *a, *b;
{
  /*
*/
  long count;

  while (!BUFEOF(a->f)) {
    count = 0;
    while (!P_eoln(a->f)) {
      count++;
      getc(a->f);
    }
    fscanf(a->f, "%*[^\n]");
    getc(a->f);
    fprintf(b->f, "%ld\n", count);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
  exit(EXIT_SUCCESS);
}



/* End. */
