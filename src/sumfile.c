/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sumfile.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.00


/*
*/



/*




































*/



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



Static Void themain()
{
  long s;
  long sum = 0;
  _TEXT TEMP, TEMP1;

  printf("sumfile %4.2f\n", version);

  while (!P_eof(stdin)) {
    scanf("%ld", &s);
    sum += s;
    printf("%10ld %10ld ", s, sum);
    TEMP.f = stdin;
    *TEMP.name = '\0';
    TEMP1.f = stdout;
    *TEMP1.name = '\0';
    copyaline(&TEMP, &TEMP1);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  themain();
_L1:
  exit(EXIT_SUCCESS);
}



/* End. */
