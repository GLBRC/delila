/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "calhnb.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         2.29


/*








*/


/*


*/


/*


































































*/


Static _TEXT fin, fout;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define maxsize         200
#define accuracy        10000



Static Void calehnb(n, gna, gnc, gng, gnt, hg, ehnb, varhnb)
long n, gna, gnc, gng, gnt;
double *hg, *ehnb, *varhnb;
{
  /*









*/
  /*
*/

  double log2Delila = log(2.0);
  double logn, nlog2;

  long gn;
  double logpa, logpc, logpg, logpt;

  /*
*/
  double logfact[maxsize + 1];

  /*
*/
  double mplog2p[maxsize + 1];

  long i;
  double logi;

  long na;
  long nc = 0, ng = 0, nt = 0;
  boolean done = false;

  double pnb;
  /*
*/
  double hnb, pnbhnb;
  double sshnb = 0.0;


  double total = 0.0;
  /*
*/
  long counter = 0;

  /*
*/


  if (n > maxsize) {
    printf(" procedure calehnb: n > maxsize (%ld>%ld)\n", n, (long)maxsize);
    halt();
  }

  logn = log((double)n);
  nlog2 = n * log2Delila;


  gn = gna + gnc + gng + gnt;
  logpa = log((double)gna / gn);
  logpc = log((double)gnc / gn);
  logpg = log((double)gng / gn);
  logpt = log((double)gnt / gn);


  *hg = -((gna * logpa + gnc * logpc + gng * logpg + gnt * logpt) /
	  (gn * log2Delila));

  *ehnb = 0.0;

  /*
*/
  logfact[0] = 0.0;
  mplog2p[0] = 0.0;
  for (i = 1; i <= n; i++) {
    logi = log((double)i);
    logfact[i] = logfact[i-1] + logi;
    mplog2p[i] = i * (logn - logi) / nlog2;
  }


  na = n;

  /*

























*/

  do {
    /*









*/

    pnb = exp(logfact[n] - logfact[na] - logfact[nc] - logfact[ng] -
	      logfact[nt] + na * logpa + nc * logpc + ng * logpg + nt * logpt);

    hnb = mplog2p[na] + mplog2p[nc] + mplog2p[ng] + mplog2p[nt];

    pnbhnb = pnb * hnb;

    *ehnb += pnbhnb;

    sshnb += pnbhnb * hnb;

    /*
*/
    counter++;
    /*







*/
    total += pnb;

    /*



*/
    if (nt > 0) {
      if (ng > 0) {
	ng--;
	nt++;
      } else if (nc > 0) {
	/*
*/
	nc--;
	ng = nt + 1;
	nt = 0;
      } else if (na > 0) {
	/*
*/
	na--;
	nc = nt + 1;
	nt = 0;
      } else
	done = true;
    } else {
      if (ng > 0) {
	ng--;
	nt++;
      } else if (nc > 0) {
	nc--;
	ng++;
      } else {
	na--;
	nc++;
      }
    }
  } while (!done);


  *varhnb = sshnb - *ehnb * *ehnb;

  /*
*/
  if (accuracy != (long)floor(accuracy * total + 0.5)) {
    printf(" procedure calehnb: the sum of probabilities is\n");
    printf(" not accurate to one part in %ld\n", (long)accuracy);
    printf(" the sum of the probabilities is %10.8f\n", total);
  }

  /*

*/
  if (counter == (long)floor((n + 1.0) * (n + 2) * (n + 3) / 6 + 0.5))
    return;
  /*


*/
  printf(" procedure calehnb: program error, the number of\n");
  printf(" calculations is in error\n");
  halt();
}

#undef maxsize
#undef accuracy



Static Void calaehnb(n, gna, gnc, gng, gnt, hg, aehnb, avarhnb)
long n, gna, gnc, gng, gnt;
double *hg, *aehnb, *avarhnb;
{
  /*











*/
  double log2Delila = log(2.0);

  long gn;
  double pa, pc, pg, pt, e;


  gn = gna + gnc + gng + gnt;
  pa = (double)gna / gn;
  pc = (double)gnc / gn;
  pg = (double)gng / gn;
  pt = (double)gnt / gn;

  *hg = -((pa * log(pa) + pc * log(pc) + pg * log(pg) + pt * log(pt)) / log2Delila);

  e = 3 / (2 * log2Delila * n);

  *aehnb = *hg - e;

  *avarhnb = e * e;
}


#define inf             9
#define ind             5
/*

*/
#define mintell         25
/*
*/
#define maxsize         200


Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  /*
*/
  long gna, gnc, gng, gnt, n;
  double h, ha, ehnb, aehnb, varhnb, avarhnb;
  boolean flag;
  boolean flags = false;
  long power = (long)floor(exp(ind * log(10.0)) + 0.5);

  /*


*/

  printf(" calhnb %4.2f\n", version);

  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  fprintf(fout->f, "* calhnb %4.2f calculate statistics of hnb\n", version);
  fprintf(fout->f, "*\n");


  if (*fin->name != '\0') {
    if (fin->f != NULL)
      fin->f = freopen(fin->name, "r", fin->f);
    else
      fin->f = fopen(fin->name, "r");
  } else
    rewind(fin->f);
  if (fin->f == NULL)
    _EscIO2(FileNotFound, fin->name);
  RESETBUF(fin->f, Char);
  if (BUFEOF(fin->f)) {
    printf(" file fin is empty\n");
    halt();
  }
  fscanf(fin->f, "%ld%ld%ld%ld%*[^\n]", &gna, &gnc, &gng, &gnt);
  getc(fin->f);
  fprintf(fout->f,
	  "* genomic composition:  a = %ld,  c = %ld,  g = %ld,  t = %ld\n",
	  gna, gnc, gng, gnt);

  calaehnb(1L, gna, gnc, gng, gnt, &ha, &aehnb, &avarhnb);
  fprintf(fout->f, "* genomic uncertainty, hg = %*.*f bits\n", inf, ind, ha);
  printf("* genomic uncertainty, hg = %*.*f bits\n", inf, ind, ha);

  fprintf(fout->f, "*\n");
  fprintf(fout->f, "* n          is the number of sequence examples\n");
  fprintf(fout->f,
    "* e(hnb)     is the expectation of the uncertainty hnb calculated from n examples\n");
  fprintf(fout->f,
	  "* ae(hnb)    an approximation of e(hnb) that is calculated\n");
  fprintf(fout->f, "*            more rapidly than e(hnb) for large n\n");
  fprintf(fout->f, "* e diff     e(hnb)-ae(hnb)\n");
  fprintf(fout->f, "* var(hnb)   is the variance of hnb\n");
  fprintf(fout->f, "* avar(hnb)  is the approximate variance of hnb\n");
  fprintf(fout->f,
	  "* std diff   is the difference between the standard deviations\n");
  fprintf(fout->f, "*            (square roots of) var(hnb) and avar(hnb)\n");
  fprintf(fout->f, "* e(n)       hg - e(hnb), the sampling error.\n");
  fprintf(fout->f, "* sd(n)      square root of var(hnb).\n");
  fprintf(fout->f, "*\n");
  fprintf(fout->f, "* units are bits/base, except for the variances which\n");
  fprintf(fout->f, "* are the square of these.\n");
  fprintf(fout->f, "*\n");

  fprintf(fout->f, "*%3c %*s %*s %*s %*s %*s %*s %*s %*s\n",
	  'n', inf, "e(hnb)", inf, "ae(hnb)", inf, "e diff", inf, "var(hnb)",
	  inf, "avar(hnb)", inf, "std diff", inf, "e(n)", inf, "sd(n)");
  fprintf(fout->f, "*\n");



  while (!BUFEOF(fin->f)) {
    fscanf(fin->f, "%ld%*[^\n]", &n);
    getc(fin->f);
    if (n > mintell)
      printf(" calculating n = %4ld\n", n);

    if (n <= maxsize) {
      calehnb(n, gna, gnc, gng, gnt, &h, &ehnb, &varhnb);
      flag = false;
    } else {
      flag = true;
      flags = true;
    }
    calaehnb(n, gna, gnc, gng, gnt, &ha, &aehnb, &avarhnb);

    fprintf(fout->f, " %3ld %*.*f %*.*f %*.*f",
	    n, inf, ind, ehnb, inf, ind, aehnb, inf, ind, ehnb - aehnb);

    fprintf(fout->f, " %*.*f %*.*f %*.*f %*.*f",
	    inf, ind, varhnb, inf, ind, avarhnb, inf, ind,
	    sqrt(varhnb) - sqrt(avarhnb), inf, ind, h - ehnb);
    fprintf(fout->f, " %*.*f", inf, ind, sqrt(varhnb));


    if (flag)
      fprintf(fout->f, " *");
    else if ((long)floor(power * h + 0.5) != (long)floor(power * ha + 0.5)) {
      printf(" program error in procedure themain h <> ha (%*.*f<>%*.*f)\n",
	     inf, ind, h, inf, ind, ha);
      halt();
    }

    putc('\n', fout->f);
  }

  if (!flags)
    return;

  fprintf(fout->f, "*\n");
  fprintf(fout->f, "* for these, only approximate values were computed\n");
  fprintf(fout->f, "last values in scientific notation:\n");

  /*









*/

  fprintf(fout->f, " %3ld % .*E % .*E % .*E",
	  n, P_max(inf - 7, 1), ehnb, P_max(inf - 7, 1), aehnb,
	  P_max(inf - 7, 1), ehnb - aehnb);

  fprintf(fout->f, " % .*E % .*E % .*E % .*E",
	  P_max(inf - 7, 1), varhnb, P_max(inf - 7, 1), avarhnb,
	  P_max(inf - 7, 1), sqrt(varhnb) - sqrt(avarhnb), P_max(inf - 7, 1),
	  h - ehnb);


  fprintf(fout->f, "*\n");

}

#undef inf
#undef ind
#undef mintell
#undef maxsize


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
