/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dnamwt.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*




*/



#define version         1.08
/*
*/



/*
















































































*/



/*
*/

#define awt             313.2
#define cwt             289.2
#define gwt             329.2
#define twt             304.2



Static _TEXT report;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Local Void getbase(report, xchar, xno)
_TEXT *report;
Char xchar;
long *xno;
{
  printf("number of %c's in strand 1 of the sequence: ", xchar);
  scanf("%ld%*[^\n]", xno);
  getchar();
  fprintf(report->f,
	  "the number of %c's in strand 1 of the sequence is: %ld\n",
	  xchar, *xno);
}



Static Void themain(report)
_TEXT *report;
{
  long ano, cno, gno, tno, total;

  Char c;
  double strand1, strand2;

  printf("dnamwt %4.2f\n", version);
  if (*report->name != '\0') {
    if (report->f != NULL)
      report->f = freopen(report->name, "w", report->f);
    else
      report->f = fopen(report->name, "w");
  } else {
    if (report->f != NULL)
      rewind(report->f);
    else
      report->f = tmpfile();
  }
  if (report->f == NULL)
    _EscIO2(FileNotFound, report->name);
  SETUPBUF(report->f, Char);
  fprintf(report->f, "dnamwt %4.2f\n", version);

  getbase(report, 'A', &ano);
  getbase(report, 'C', &cno);
  getbase(report, 'G', &gno);
  getbase(report, 'T', &tno);

  strand1 = ano * awt + cno * cwt + gno * gwt + tno * twt + 18.0;
  strand2 = ano * twt + cno * gwt + gno * cwt + tno * awt + 18.0;
  total = ano + cno + gno + tno;
  fprintf(report->f, "total number of bases: %12ld\n", total);
  fprintf(report->f, "strand 1 molecular weight: %4.1f daltons\n", strand1);
  fprintf(report->f, "strand 2 molecular weight: %4.1f daltons\n", strand2);
  fprintf(report->f, "total molecular weight: %4.1f daltons\n",
	  strand1 + strand2);
  fprintf(report->f, "%% GC: %4.1f\n",
	  (gno + cno) * 100.0 / (ano + cno + gno + tno));


  putchar('\n');
  if (*report->name != '\0') {
    if (report->f != NULL)
      report->f = freopen(report->name, "r", report->f);
    else
      report->f = fopen(report->name, "r");
  } else
    rewind(report->f);
  if (report->f == NULL)
    _EscIO2(FileNotFound, report->name);
  RESETBUF(report->f, Char);
  while (!BUFEOF(report->f)) {
    while (!P_eoln(report->f)) {
      c = getc(report->f);
      if (c == '\n')
	c = ' ';
      putchar(c);
    }
    fscanf(report->f, "%*[^\n]");
    getc(report->f);
    putchar('\n');
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  report.f = NULL;
  strcpy(report.name, "report");
  themain(&report);
_L1:
  if (report.f != NULL)
    fclose(report.f);
  exit(EXIT_SUCCESS);
}



/* End. */
