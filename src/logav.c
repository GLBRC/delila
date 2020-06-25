/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "logav.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*


*/
#define updateversion   1.00



/*









































*/


Static _TEXT data, logavp, logavreport;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for themain: */
struct LOC_themain {
  double sumlogs;
  long count;
} ;

Local Void report(f, LINK)
_TEXT *f;
struct LOC_themain *LINK;
{
  fprintf(f->f, "for %ld numbers, the average of the log2 is %10.5f\n",
	  LINK->count, LINK->sumlogs / LINK->count);
}



Static Void themain(data, logavp, logavreport)
_TEXT *data, *logavp, *logavreport;
{
  struct LOC_themain V;
  double parameterversion, n, m;
  long shift;
  _TEXT TEMP;

  printf("logav %4.2f\n", version);
  if (*logavp->name != '\0') {
    if (logavp->f != NULL)
      logavp->f = freopen(logavp->name, "r", logavp->f);
    else
      logavp->f = fopen(logavp->name, "r");
  } else
    rewind(logavp->f);
  if (logavp->f == NULL)
    _EscIO2(FileNotFound, logavp->name);
  RESETBUF(logavp->f, Char);
  fscanf(logavp->f, "%lg%*[^\n]", &parameterversion);
  getc(logavp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(logavp->f, "%ld", &shift);

  V.sumlogs = 0.0;
  V.count = 0;

  /*


*/
  shift = 20;

  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else
    rewind(data->f);
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);
  while (!BUFEOF(data->f)) {
    if (P_peek(data->f) == '*') {
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      continue;
    }
    fscanf(data->f, "%lg%*[^\n]", &n);
    getc(data->f);
    m = n - shift;
    if (m > 0) {
      V.sumlogs += log(m) / log(2.0);
      V.count++;
    } else {
      printf(
	"The value n = %1.1f cannot be shifted by %ld because that is not positive  so the log cannot be taken.\n",
	n, shift);
      printf("  This value was dropped from the computation\n");
    }
  }

  if (*logavreport->name != '\0') {
    if (logavreport->f != NULL)
      logavreport->f = freopen(logavreport->name, "w", logavreport->f);
    else
      logavreport->f = fopen(logavreport->name, "w");
  } else {
    if (logavreport->f != NULL)
      rewind(logavreport->f);
    else
      logavreport->f = tmpfile();
  }
  if (logavreport->f == NULL)
    _EscIO2(FileNotFound, logavreport->name);
  SETUPBUF(logavreport->f, Char);
  report(logavreport, &V);
  TEMP.f = stdout;
  *TEMP.name = '\0';
  report(&TEMP, &V);

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  logavreport.f = NULL;
  strcpy(logavreport.name, "logavreport");
  logavp.f = NULL;
  strcpy(logavp.name, "logavp");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&data, &logavp, &logavreport);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (logavp.f != NULL)
    fclose(logavp.f);
  if (logavreport.f != NULL)
    fclose(logavreport.f);
  exit(EXIT_SUCCESS);
}



/* End. */
