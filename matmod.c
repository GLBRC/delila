/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "matmod.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         "matmod 2.08 2016sep20 tds/gds"
/*

*/




/*


































*/



#define debugging       false



#define maxvecpart      64
#define vpagewidth      64


/*
*/


















/*






















*/



/*
*/
/*

*/

typedef struct spacelist {
  long skips;
  struct spacelist *next;
} spacelist;

/*

*/
typedef enum {
  start, stop
} endpoints;

typedef struct parameter {
  long range[2];
  /*
*/
  long window, wshift, coding;
  /*
*/
  spacelist *spaces;
  long cshift;


  long wvlength;
  /*
*/
  long pvlength;
  /*
*/
  /*
*/

  struct parameter *next;
} parameter;




/*







*/

typedef struct vectorpart {
  double numbers[maxvecpart];
  struct vectorpart *next;
} vectorpart;

typedef struct vector {
  long length;
  vectorpart *part;
} vector;



Static _TEXT encseq;


Static jmp_buf _JL1;



Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



/*


*/

Static Void regress(control, x, y, sumx, sumy, sumxsqd, sumysqd, sumxy, ex,
		    ey, varx, vary, covxy, r, m, b, n)
Char control;
double x, y, *sumx, *sumy, *sumxsqd, *sumysqd, *sumxy, *ex, *ey, *varx, *vary,
       *covxy, *r, *m, *b;
long *n;
{




  /*









*/
  if (control != 'r' && control != 'e' && control != 'c') {
    printf(" linear regression control variable value, \"%c\" is not acceptable.\n",
	   control);
    printf(" it must be in [\"c\",\"e\",\"r\"]\n");
    halt();
    return;
  }

  switch (control) {

  case 'c':
    x = 0.0;
    y = 0.0;
    *sumx = 0.0;
    *sumy = 0.0;
    *sumxsqd = 0.0;
    *sumysqd = 0.0;
    *sumxy = 0.0;
    *ex = 0.0;
    *ey = 0.0;
    *varx = 0.0;
    *vary = 0.0;
    *covxy = 0.0;
    *r = 2.0;
    /*
*/

    *m = 0.0;
    *b = 0.0;
    *n = 0;
    break;

  case 'e':
    *sumx += x;
    *sumy += y;
    *sumxsqd += x * x;
    *sumysqd += y * y;
    *sumxy += x * y;
    (*n)++;
    break;

  case 'r':
    if (*n == 0) {
      printf("regress: n is 0; no samples recorded\n");
      halt();
    }
    if (*n == 1) {
      printf("regress: n is 1; regression impossible\n");
      halt();
    }

    *ex = *sumx / *n;
    *ey = *sumy / *n;

    *varx = *sumxsqd / *n - *ex * *ex;
    *vary = *sumysqd / *n - *ey * *ey;
    if (*varx == 0.0) {
      printf("regress: variance of x is zero; regression impossible\n");
      halt();
    }
    if (*vary == 0.0) {
      printf("regress: variance of y is zero; regression impossible\n");
      halt();
    }

    *covxy = *sumxy / *n - *ex * *ey;

    *r = *covxy / sqrt(*varx * *vary);

    *m = *covxy / *varx;
    *b = *ey - *m * *ex;
    break;

  }
}


#define pi              3.141592653589



Static Void randomDelila(seed)
double *seed;
{
  /*

























*/
  double intermediate;

  intermediate = *seed + pi;
  intermediate = exp(5.0 * log(intermediate));
  *seed = intermediate - (long)intermediate;
}

#undef pi


#define n               100
#define tests           20

#define seed            4.0



Static Void randomtest(table)
_TEXT *table;
{
  /*


*/
  long test, call;
  double x = seed;
  double sumx, sumxsqd, mean, std;

  putc('\n', table->f);

  fprintf(table->f, " test of the random number generator:\n\n");
  fprintf(table->f, "     mean     std dev\n");
  fprintf(table->f, "    (0.5000)  (0.2887)  (expected values)\n");
  fprintf(table->f, " ===================== %ld calls per test\n", (long)n);

  for (test = 1; test <= tests; test++) {
    sumx = 0.0;
    sumxsqd = 0.0;

    for (call = 1; call <= n; call++) {
      randomDelila(&x);
      sumx += x;
      sumxsqd += x * x;
    }

    mean = sumx / n;
    std = sqrt(n / (n - 1.0) * (sumxsqd / n - mean * mean));
    fprintf(table->f, " %10.4f%10.4f\n", mean, std);
  }

}

#undef n
#undef tests
#undef seed






Static double f(x)
double x;
{
  return log(x);
}


Static double fp(x)
double x;
{
  return (1 / x);
}



Static double newton(initialx, tolerance, tries, passes)
double initialx, tolerance;
long tries, *passes;
{
  /*






*/
  double deltax;
  boolean searching = true;
  double x = initialx;

  *passes = 0;
  while (searching && *passes < tries) {
    deltax = f(x) / fp(x);
    if (fabs(deltax) > tolerance) {
      x -= deltax;
      (*passes)++;
    } else
      searching = false;
  }

  return x;
}


#define initialx        0.00000001

#define maxtries        100

#define tolerance       0.00000001



Static Void newtontest(fout)
_TEXT *fout;
{
  double answer;
  long passes = 0, tries = 0;

  fprintf(fout->f, "\n test of newton's method.\n");
  fprintf(fout->f, " the function is the natural logarithm, for which the\n");
  fprintf(fout->f, " root should be 1.00.\n");
  fprintf(fout->f, " the tolerance in the answer is:  %10.8f\n", tolerance);
  fprintf(fout->f, " the initial guess is:%12c%10.8f\n", ' ', initialx);

  while (tries <= passes && tries < maxtries) {
    tries++;
    answer = newton(initialx, tolerance, tries, &passes);
    fprintf(fout->f, " tries: %4ld passes: %4ld answer: %10.8f\n",
	    tries, passes, answer);
  }
}

#undef initialx
#undef maxtries
#undef tolerance





































Static double vget(v, pos)
vector v;
long pos;
{
  long i;

  if (pos > v.length || pos < 1) {
    printf(
      " error in call to function vget: position %ld is beyond the end of the vector\n",
      pos);
    halt();
  }


  for (i = 1; i <= (pos - 1) / maxvecpart; i++)
    v.part = v.part->next;


  return (v.part->numbers[(pos - 1) & (maxvecpart - 1)]);
}


Static Void vput(v, pos, number)
vector *v;
long pos;
double number;
{
  long i;
  vectorpart *firstpart;

  if (pos > v->length || pos < 1) {
    printf(
      " error in call to function vput: position %ld is beyond the end of the vector\n",
      pos);
    halt();
  }


  firstpart = v->part;
  for (i = 1; i <= (pos - 1) / maxvecpart; i++)
    v->part = v->part->next;


  v->part->numbers[(pos - 1) & (maxvecpart - 1)] = number;
  v->part = firstpart;
}


Static Void makevector(v, l)
vector *v;
long l;
{
  /*
*/
  long numparts, i;
  vectorpart *firstpart, *newpart;

  if (l < 1) {
    printf(" makevector: positive length required\n");
    halt();
  }

  v->length = l;
  v->part = (vectorpart *)Malloc(sizeof(vectorpart));
  firstpart = v->part;
  numparts = (v->length - 1) / maxvecpart + 1;
  for (i = 1; i < numparts; i++) {
    newpart = (vectorpart *)Malloc(sizeof(vectorpart));
    v->part->next = newpart;
    v->part = newpart;
  }
  v->part->next = NULL;
  v->part = firstpart;
}



Static Void readvector(thefile, v)
_TEXT *thefile;
vector *v;
{
  /*




*/
  long i, j, numparts, lastpart;
  vectorpart *firstpart;

  firstpart = v->part;
  numparts = (v->length - 1) / maxvecpart + 1;
  lastpart = ((v->length - 1) & (maxvecpart - 1)) + 1;
  for (i = 1; i < numparts; i++) {
    for (j = 0; j < maxvecpart; j++)
      fscanf(thefile->f, "%lg", &v->part->numbers[j]);
    v->part = v->part->next;
  }
  for (j = 0; j < lastpart; j++)
    fscanf(thefile->f, "%lg", &v->part->numbers[j]);
  fscanf(thefile->f, "%*[^\n]");
  getc(thefile->f);
  v->part = firstpart;
}



Static Void writevector(thefile, v, y, z)
_TEXT *thefile;
vector v;
long y, z;
{
  /*


*/
  long pos = 0;
  long i, j, numparts, lastpart;
  vectorpart *firstpart;
  long x;

  x = (long)(vpagewidth / (y + 1.0));
  firstpart = v.part;
  numparts = (v.length - 1) / maxvecpart + 1;
  lastpart = ((v.length - 1) & (maxvecpart - 1)) + 1;
  if (z == 0) {
    for (i = 1; i < numparts; i++) {
      for (j = 0; j < maxvecpart; j++) {
	fprintf(thefile->f, " %*ld",
		(int)y, (long)floor(v.part->numbers[j] + 0.5));
	pos++;
	if (pos % x == 0)
	  putc('\n', thefile->f);
      }
      v.part = v.part->next;
    }
    for (j = 0; j < lastpart; j++) {
      fprintf(thefile->f, " %*ld",
	      (int)y, (long)floor(v.part->numbers[j] + 0.5));
      pos++;
      if (pos < v.length && pos % x == 0)
	putc('\n', thefile->f);
    }
  } else {
    for (i = 1; i < numparts; i++) {
      for (j = 0; j < maxvecpart; j++) {
	fprintf(thefile->f, " %*.*f", (int)y, (int)z, v.part->numbers[j]);
	pos++;
	if (pos % x == 0)
	  putc('\n', thefile->f);
      }
      v.part = v.part->next;
    }
    for (j = 0; j < lastpart; j++) {
      fprintf(thefile->f, " %*.*f", (int)y, (int)z, v.part->numbers[j]);
      pos++;
      if (pos < v.length && pos % x == 0)
	putc('\n', thefile->f);
    }
  }
  putc('\n', thefile->f);
  v.part = firstpart;
}



Static double dotproduct(vectora, vectorb)
vector vectora, vectorb;
{
  long i;
  double j = 0.0;

  if (vectora.length != vectorb.length) {
    printf(" function dotproduct: vector lengths must be equal\n");
    halt();
  }
  for (i = 1; i <= vectora.length; i++)
    j += vget(vectora, i) * vget(vectorb, i);
  return j;
}



Static double magnitude(v)
vector *v;
{
  return sqrt(dotproduct(*v, *v));
}



Static Void normalize(v)
vector *v;
{
  /*
*/
  long i;
  double length;
  long FORLIM;

  length = magnitude(v);
  FORLIM = v->length;
  for (i = 1; i <= FORLIM; i++)
    vput(v, i, vget(*v, i) / length);
}



Static Void vset(thevalue, v)
double thevalue;
vector *v;
{
  long i, FORLIM;

  FORLIM = v->length;
  for (i = 1; i <= FORLIM; i++)
    vput(v, i, thevalue);
}



Static Void vadd(a, b)
vector a, *b;
{
  long i;

  if (a.length != b->length) {
    printf(" function vadd: vector lengths must be equal\n");
    halt();
  }

  for (i = 1; i <= a.length; i++)
    vput(b, i, vget(*b, i) + vget(a, i));
}



Static Void vsub(a, b)
vector a, *b;
{
  long i;

  if (a.length != b->length) {
    printf(" function vsub: vector lengths must be equal\n");
    halt();
  }

  for (i = 1; i <= a.length; i++)
    vput(b, i, vget(*b, i) - vget(a, i));
}



Static Void vmult(k, v)
double k;
vector *v;
{
  long i, FORLIM;

  FORLIM = v->length;
  for (i = 1; i <= FORLIM; i++)
    vput(v, i, k * vget(*v, i));
}



Static Void extract(a, astart, astop, b, bstart)
vector a;
long astart, astop;
vector *b;
long bstart;
{
  /*



*/
  long i, precalc;

  if (astart <= astop) {
    precalc = bstart - astart;
    for (i = astart; i <= astop; i++)
      vput(b, i + precalc, vget(a, i));
    return;
  }
  precalc = bstart + astart;
  for (i = astart; i >= astop; i--)
    vput(b, precalc - i, vget(a, i));
}



Static double arccos_(x)
double x;
{
  /*













*/
  double pi;

  if (x < -1.0 || x > 1.0) {
    printf(" function arccos(x): x was not -1<=x<=+1\n");
    halt();
  }

  if (x == 0.0)
    return (2.0 * atan(1.0));
  else if (x > 0.0)
    return atan(sqrt(1.0 / (x * x) - 1.0));
  else {
    pi = 4.0 * atan(1.0);
    return (pi - atan(sqrt(1.0 / (x * x) - 1.0)));
  }
}



Static double angle(a, b)
vector *a, *b;
{
  /*


*/
  return (arccos_(dotproduct(*a, *b) / (magnitude(a) * magnitude(b))));
}



Static Void vectortest(fout)
_TEXT *fout;
{
  vector a, b, c, d, e, f;
  long i;
  double pi;

  makevector(&a, 2L);
  makevector(&b, 2L);
  makevector(&c, 2L);
  makevector(&d, 5L);
  makevector(&e, 3L);
  makevector(&f, 2L);

  vput(&a, 1L, 1.0);
  vput(&a, 2L, 0.0);
  vset(1.0, &b);
  for (i = 1; i <= 5; i++)
    vput(&d, i, (double)i);
  vput(&f, 1L, -1.0);
  vput(&f, 2L, 1.0);

  fprintf(fout->f, "\n test of vector math\n");
  fprintf(fout->f, " vector a: ");
  writevector(fout, a, 1L, 0L);
  fprintf(fout->f, " vector b: ");
  writevector(fout, b, 1L, 0L);
  fprintf(fout->f, " vector d: ");
  writevector(fout, d, 2L, 1L);
  fprintf(fout->f, " vector f: ");
  writevector(fout, f, 2L, 1L);

  fprintf(fout->f, " extract parts of d: \n");
  extract(d, 2L, 3L, &c, 1L);
  fprintf(fout->f, " from 2 to 3: ");
  writevector(fout, c, 1L, 0L);
  extract(d, 5L, 3L, &e, 1L);
  fprintf(fout->f, " from 5 to 3: ");
  writevector(fout, e, 1L, 0L);
  extract(d, 3L, 5L, &e, 1L);
  fprintf(fout->f, " from 3 to 5: ");
  writevector(fout, e, 1L, 0L);
  extract(d, 4L, 2L, &e, 1L);
  fprintf(fout->f, " from 4 to 2: ");
  writevector(fout, e, 1L, 0L);

  fprintf(fout->f, " a + b = ");
  vset(0.0, &c);
  vadd(a, &c);
  vadd(b, &c);
  writevector(fout, c, 1L, 0L);

  fprintf(fout->f, " a - b = ");
  vset(0.0, &c);
  vadd(a, &c);
  vsub(b, &c);
  writevector(fout, c, 1L, 0L);

  fprintf(fout->f, " a is multiplied by 5, a now is ");
  vmult(5.0, &a);
  writevector(fout, a, 1L, 0L);

  fprintf(fout->f, " magnitude(a) = %5.3f\n", magnitude(&a));
  fprintf(fout->f, " magnitude(b) = %5.3f\n\n", magnitude(&b));

  fprintf(fout->f, " test arccos function\n");
  pi = 2 * arccos_(0.0);
  fprintf(fout->f, " pi = %6.4f\n", pi);
  fprintf(fout->f, "cos(arccos(0.00 * pi/4.0)) * 4/pi = %6.4f\n",
	  cos(arccos_(0.00 * pi / 4.0)) * 4 / pi);
  fprintf(fout->f, "cos(arccos(0.12 * pi/4.0)) * 4/pi = %6.4f\n",
	  cos(arccos_(0.12 * pi / 4.0)) * 4 / pi);
  fprintf(fout->f, "cos(arccos(0.25 * pi/4.0)) * 4/pi = %6.4f\n",
	  cos(arccos_(0.25 * pi / 4.0)) * 4 / pi);
  fprintf(fout->f, "cos(arccos(0.37 * pi/4.0)) * 4/pi = %6.4f\n",
	  cos(arccos_(0.37 * pi / 4.0)) * 4 / pi);
  fprintf(fout->f, "cos(arccos(0.50 * pi/4.0)) * 4/pi = %6.4f\n",
	  cos(arccos_(0.50 * pi / 4.0)) * 4 / pi);
  fprintf(fout->f, "cos(arccos(0.75 * pi/4.0)) * 4/pi = %6.4f\n",
	  cos(arccos_(0.75 * pi / 4.0)) * 4 / pi);
  fprintf(fout->f, "cos(arccos(1.00 * pi/4.0)) * 4/pi = %6.4f\n",
	  cos(arccos_(1.00 * pi / 4.0)) * 4 / pi);

  fprintf(fout->f, " angle(a,b) = pi/%6.4f\n", pi / angle(&a, &b));
  fprintf(fout->f, " angle(b,f) = pi/%6.4f\n", pi / angle(&b, &f));
  fprintf(fout->f, " angle(a,f) = pi*%6.4f\n", angle(&a, &f) / pi);
}








Static long vectorsize(param)
parameter *param;
{
  /*
*/
  long size = 0;

  while (param != NULL) {
    size += param->pvlength;
    param = param->next;
  }
  return size;
}


Static long paramsize(param)
parameter *param;
{
  long rangesize, numwindows;

  /*
*/
  rangesize = param->range[(long)stop] - param->range[(long)start] + 1;
  numwindows = (long)((rangesize - 1.0) / param->wshift + 1);
  return (numwindows * param->wvlength);
}


Static Void readencpar(thefile, param, regions, vectorlength)
_TEXT *thefile;
parameter **param;
long *regions, *vectorlength;
{
  /*


*/
  parameter *aparam, *newparam;
  spacelist *firstspaces, *newspaces;
  long i, j;
  Char ch;
  long FORLIM, FORLIM1;

  if (*thefile->name != '\0') {
    if (thefile->f != NULL)
      thefile->f = freopen(thefile->name, "r", thefile->f);
    else
      thefile->f = fopen(thefile->name, "r");
  } else
    rewind(thefile->f);
  if (thefile->f == NULL)
    _EscIO2(FileNotFound, thefile->name);
  RESETBUF(thefile->f, Char);
  if (BUFEOF(thefile->f)) {
    printf(" encoded sequence file is empty\n");
    halt();
  }
  aparam = (parameter *)Malloc(sizeof(parameter));
  if (*param == NULL)
    *param = (parameter *)Malloc(sizeof(parameter));
  aparam = *param;

  for (i = 1; i <= 2; i++) {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
  }


  fscanf(thefile->f, "%*[^\n]");


  getc(thefile->f);
  fscanf(thefile->f, "%ld%*[^\n]", regions);
  getc(thefile->f);

  FORLIM = *regions;
  for (i = 1; i <= FORLIM; i++) {



    fscanf(thefile->f, "%ld", aparam->range);
    do {
      ch = getc(thefile->f);
      if (ch == '\n')
	ch = ' ';
    } while (ch != 'o');
    fscanf(thefile->f, "%ld%*[^\n]", &aparam->range[(long)stop]);
    getc(thefile->f);

    fscanf(thefile->f, "%ld%*[^\n]", &aparam->window);
    getc(thefile->f);
    fscanf(thefile->f, "%ld%*[^\n]", &aparam->wshift);
    getc(thefile->f);

    fscanf(thefile->f, "%ld", &aparam->coding);
    aparam->spaces = (spacelist *)Malloc(sizeof(spacelist));
    if (aparam->coding > 1) {
      do {
	ch = getc(thefile->f);
	if (ch == '\n')
	  ch = ' ';
      } while (ch != ':');
      firstspaces = (spacelist *)Malloc(sizeof(spacelist));
      aparam->spaces = firstspaces;
      fscanf(thefile->f, "%ld", &aparam->spaces->skips);
      FORLIM1 = aparam->coding;
      for (j = 2; j < FORLIM1; j++) {
	newspaces = (spacelist *)Malloc(sizeof(spacelist));
	aparam->spaces->next = newspaces;
	aparam->spaces = newspaces;
	fscanf(thefile->f, "%ld", &aparam->spaces->skips);
      }
      aparam->spaces->next = NULL;
      aparam->spaces = firstspaces;
    } else
      aparam->spaces = NULL;
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    fscanf(thefile->f, "%ld%*[^\n]", &aparam->cshift);
    getc(thefile->f);


    aparam->wvlength = (long)floor(exp(aparam->coding * log(4.0)) + 0.5);
    aparam->pvlength = paramsize(aparam);

    if (i < *regions) {
      newparam = (parameter *)Malloc(sizeof(parameter));
      aparam->next = newparam;
      aparam = newparam;
    }
  }
  aparam->next = NULL;


  fscanf(thefile->f, "%ld%*[^\n]", vectorlength);
  getc(thefile->f);
  if (*vectorlength == vectorsize(*param))
    return;
  printf(" vector length in the encoded file\n");
  printf(" does not correspond to the parameters\n");
  halt();
}




/*
*/

Static Void copyaline(a, b)
_TEXT *a, *b;
{
}


Static long copylines(a, b, i)
_TEXT *a, *b;
long i;
{
  return 0;
}



Static Void enchead(encseq, fout, length)
_TEXT *encseq, *fout;
long *length;
{
  /*








*/
  long regions, i;

  if (*encseq->name != '\0') {
    if (encseq->f != NULL)
      encseq->f = freopen(encseq->name, "r", encseq->f);
    else
      encseq->f = fopen(encseq->name, "r");
  } else
    rewind(encseq->f);
  if (encseq->f == NULL)
    _EscIO2(FileNotFound, encseq->name);
  RESETBUF(encseq->f, Char);
  fscanf(encseq->f, "%*[^\n]");
  getc(encseq->f);
  copyaline(encseq, fout);
  copyaline(encseq, fout);

  fscanf(encseq->f, "%ld", &regions);
  fprintf(fout->f, " %ld", regions);
  i = regions * 6 + 2;
  /*
*/

  if (copylines(encseq, fout, i) != i) {
    printf(" encseq is missing parameters\n");
    halt();
  }


  fscanf(encseq->f, "%ld%*[^\n]", length);
  getc(encseq->f);
  fprintf(fout->f, " %ld is the vector length", *length);
}



/*



*/

Static boolean beginpv(aparam, element)
parameter *aparam;
long element;
{
  if (aparam != NULL)
    return (element == 0);
  else
    return true;
}


Static boolean endpv(aparam, element)
parameter *aparam;
long element;
{
  /*
*/
  if (aparam != NULL)
    return (element >= aparam->pvlength);
  else
    return true;
}


Static boolean beginwv(aparam, element)
parameter *aparam;
long element;
{
  /*
*/
  if (aparam != NULL)
    return (element % aparam->wvlength == 0);
  else
    return false;
}


Static boolean endwv(aparam, element)
parameter *aparam;
long element;
{
  /*
*/
  if (aparam != NULL)
    return ((element + 1) % aparam->wvlength == 0);
  else {
    return true;

  }
}


Static long encposition(aparam, element)
parameter *aparam;
long element;
{
  /*

*/
  return (aparam->range[(long)start] + element / aparam->wvlength * aparam->wshift);
}





Static Void evln(afile, element, curparam)
_TEXT *afile;
long element;
parameter *curparam;
{
  /*
*/
  if (beginwv(curparam, element))
    putc('\n', afile->f);
  else if (curparam == NULL)
    putc('\n', afile->f);


  if (beginpv(curparam, element))
    putc('\n', afile->f);
}


#define codingmax       4


/* Local variables for evheader: */
struct LOC_evheader {
  _TEXT *afile;
  Char bases[codingmax];
} ;

Local Void space(i, LINK)
long i;
struct LOC_evheader *LINK;
{
  long index;

  for (index = 1; index <= i; index++)
    putc(' ', LINK->afile->f);
}

Local Void advance(level, LINK)
long level;
struct LOC_evheader *LINK;
{
  switch (LINK->bases[level-1]) {

  case 'a':
    LINK->bases[level-1] = 'c';
    break;

  case 'c':
    LINK->bases[level-1] = 'g';
    break;

  case 'g':
    LINK->bases[level-1] = 't';
    break;

  case 't':
    LINK->bases[level-1] = 'a';
    advance(level - 1, LINK);
    break;
  }
}


Static Void evheader(afile_, posfield, valfield, firstparam, curparam)
_TEXT *afile_;
long posfield, valfield;
parameter *firstparam, **curparam;
{
  /*

*/
  struct LOC_evheader V;
  long i, j, n, size;
  /*
*/
  spacelist *s;
  parameter *WITH;
  long FORLIM, FORLIM1, FORLIM2;

  V.afile = afile_;
  if (*curparam == NULL)
    *curparam = firstparam;

  WITH = *curparam;
  if (WITH->coding > codingmax) {
    printf(" evheader: coding levels must not be higher than %ld\n",
	   (long)codingmax);
    halt();
  }


  size = WITH->coding;
  s = (*curparam)->spaces;
  while (s != NULL) {
    size += s->skips;
    s = s->next;
  }


  if (labs(valfield) < size) {
    printf(
      " evheader: valfield (%ld) cannot be less than coding level and spaces (%ld)\n",
      valfield, size);
    halt();
  }


  FORLIM = WITH->coding;
  for (i = 0; i < FORLIM; i++)
    V.bases[i] = 'a';



  space(posfield * 2 + 5, &V);
  FORLIM = WITH->wvlength;
  for (i = 1; i <= FORLIM; i++) {
    space(labs(valfield) - size + 1, &V);
    s = WITH->spaces;
    FORLIM1 = WITH->coding;
    for (n = 0; n < FORLIM1; n++) {
      putc(V.bases[n], V.afile->f);
      if (s != NULL) {
	FORLIM2 = s->skips;
	for (j = 1; j <= FORLIM2; j++)
	  putc('x', V.afile->f);
	s = s->next;
      }
    }


    if (i != WITH->wvlength)
      advance(WITH->coding, &V);
  }
}

#undef codingmax


/* Local variables for evprint: */
struct LOC_evprint {
  _TEXT *afile;
  long posfield;
  double val;
  long valfield, valdecimal;
  boolean ln;
  parameter **curparam;
  long *element;
} ;

Local Void sider(LINK)
struct LOC_evprint *LINK;
{
  long position;
  parameter *WITH;

  /*
*/
  WITH = *LINK->curparam;
  position = encposition(*LINK->curparam, *LINK->element);
  fprintf(LINK->afile->f, " %*ld to %*ld",
	  (int)LINK->posfield, position, (int)LINK->posfield,
	  position + WITH->window - 1);
}

Local Void middler(LINK)
struct LOC_evprint *LINK;
{
  putc(' ', LINK->afile->f);
  if (LINK->valfield > 0) {
    if (LINK->valdecimal <= 0)
      fprintf(LINK->afile->f, "%*ld",
	      (int)LINK->valfield, (long)floor(LINK->val + 0.5));
    else
      fprintf(LINK->afile->f, "%*.*f",
	      (int)LINK->valfield, (int)LINK->valdecimal, LINK->val);
    return;
  }
  if (LINK->ln) {
    printf(" evprint: if valfield is negative, then ln must be false\n");
    halt();
  }
}

Local Void step(LINK)
struct LOC_evprint *LINK;
{
  (*LINK->element)++;

  if (endpv(*LINK->curparam, *LINK->element)) {
    *LINK->curparam = (*LINK->curparam)->next;
    *LINK->element = 0;
  }
}


Static Void evprint(afile_, posfield_, val_, valfield_, valdecimal_, ln_,
		    firstparam, curparam_, element_)
_TEXT *afile_;
long posfield_;
double val_;
long valfield_, valdecimal_;
boolean ln_;
parameter *firstparam, **curparam_;
long *element_;
{
  /*
*/
  /*
*/
  /*


*/
  /*




































*/
  struct LOC_evprint V;

  V.afile = afile_;
  V.posfield = posfield_;
  V.val = val_;
  V.valfield = valfield_;
  V.valdecimal = valdecimal_;
  V.ln = ln_;
  V.curparam = curparam_;
  V.element = element_;
  if (V.ln) {
    if (beginpv(*V.curparam, *V.element)) {
      evheader(V.afile, V.posfield, V.valfield, firstparam, V.curparam);
      putc('\n', V.afile->f);
    }
  }

  if (beginwv(*V.curparam, *V.element))
    sider(&V);

  middler(&V);

  step(&V);

  if (V.ln)
    evln(V.afile, *V.element, *V.curparam);
}




Static Void enctest(fin, fout)
_TEXT *fin, *fout;
{
  /*





*/
  parameter *firstparam, *aparam;
  long regions, vindex, vectorlength;
  vector v;
  long element = 0;

  putc('\n', fout->f);
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
    fprintf(fout->f, " if file encseq contains the output from the encode\n");
    fprintf(fout->f, " program, then parameter reading will be tested.\n");
    return;
  }
  firstparam = (parameter *)Malloc(sizeof(parameter));
  readencpar(fin, &firstparam, &regions, &vectorlength);
  fprintf(fout->f,
    " the encode file contains vectors with the following %ld sized components:\n",
    regions);
  aparam = firstparam;
  while (aparam != NULL) {
    fprintf(fout->f, " %5ld", paramsize(aparam));
    aparam = aparam->next;
  }
  fprintf(fout->f, "\n the total size of each vector is %ld\n\n",
	  vectorlength);

  fprintf(fout->f, " the first vector is:\n");
  makevector(&v, vectorlength);
  readvector(fin, &v);
  aparam = NULL;
  for (vindex = 1; vindex <= vectorlength; vindex++)
    evprint(fout, 4L, vget(v, vindex), 5L, 0L, true, firstparam, &aparam,
	    &element);
}



Static Void simpson(upper, answer, tol)
double upper, *answer, *tol;
{
  /*













*/
  long i;

  double x, x1, x2;
  /*
*/
  double pi = 4.0 * atan(1.0);
  double val;
  /*

*/
  double deltax;
  double evensum = 0.0;
  double oddsum, endsum, endcor, sum1;
  /*

*/
  long pieces = 2;
  double lower = 0.0;
  double sum;

  val = 1 / sqrt(2 * pi);

  upper = fabs(upper);
  /*

*/
  *tol = 1.0e-6;
  deltax = (upper - lower) / pieces;
  x = lower + deltax;
  oddsum = val * exp(-0.5 * x * x);
  x1 = lower;
  x2 = upper;
  endsum = val * exp(-0.5 * x1 * x1) + val * exp(-0.5 * x2 * x2);
  endcor = x2 * val * exp(-0.5 * x2 * x2) - x1 * val * exp(-0.5 * x1 * x1);
  sum = (endsum + 4.0 * oddsum) * deltax / 3.0;

  do {
    pieces *= 2;
    sum1 = sum;
    deltax = (upper - lower) / pieces;
    evensum += oddsum;
    oddsum = 0.0;
    for (i = 1; i <= pieces / 2; i++) {
      x = lower + deltax * (2.0 * i - 1.0);
      oddsum += val * exp(-0.5 * x * x);
    }
    sum = (7.0 * endsum + 14.0 * evensum + 16.0 * oddsum + endcor * deltax) *
	  deltax / 15.0;
  } while (fabs(sum - sum1) > fabs(*tol * sum));
  *answer = 0.5 - sum;
  if (*answer < 0.0)
    *answer = 0.0;
  /*

*/
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  encseq.f = NULL;
  strcpy(encseq.name, "encseq");
  printf(" mathematics modules %s\n", version);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  vectortest(&TEMP);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  newtontest(&TEMP);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  enctest(&encseq, &TEMP);

  TEMP.f = stdout;
  *TEMP.name = '\0';
  randomtest(&TEMP);

_L1:
  if (encseq.f != NULL)
    fclose(encseq.f);
  exit(EXIT_SUCCESS);
}



/* End. */
