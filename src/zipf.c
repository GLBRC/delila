/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "zipf.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.33
/*



*/



/*

































































































































*/



#define maxsymbols      25



#define maxvecpart      64
#define vpagewidth      64


/*
*/


typedef char position;


typedef double maptype[maxsymbols];


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


Static _TEXT zipfp, data, xyin;
Static maptype map;
Static double seed;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


Static boolean lessthan(a, b)
position a, b;
{
  return (map[a-1] > map[b-1]);
}


Static Void swap_(a, b)
position a, b;
{
  double hold;

  hold = map[a-1];
  map[a-1] = map[b-1];
  map[b-1] = hold;

}



Static Void quicksort(left, right)
position left, right;
{
  /*












*/
  position lower = left;
  position upper, center;

  center = (left + right) / 2;
  upper = right;

  do {
    while (lessthan(lower, center))
      lower++;
    while (lessthan(center, upper))
      upper--;

    if (lower <= upper) {
      if (lower == center)
	center = upper;
      else if (upper == center)
	center = lower;
      swap_(lower, upper);
      lower++;
      upper--;
    }

  } while (lower <= upper);

  if (left < upper)
    quicksort(left, upper);
  if (lower < right)
    quicksort(lower, right);
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



Static Void vset(thevalue, v)
double thevalue;
vector *v;
{
  long i, FORLIM;

  FORLIM = v->length;
  for (i = 1; i <= FORLIM; i++)
    vput(v, i, thevalue);
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



Static Void polrec(r, theta, x, y)
double r, theta, *x, *y;
{
  /*
*/
  *x = r * cos(theta);
  *y = r * sin(theta);
}


#define debug           false


#define pi              3.141592653589


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *data, *xyin;

  double x, y;

  /*

*/
  vector acvector;
  long actualsymbols;
  vector cvector;
  /*
*/
  boolean graphing;
  /*
*/
  vector pcvector;
  double radius, theta;
  boolean showlow;
} ;

Local Void show(LINK)
struct LOC_themain *LINK;
{
  long max;
  double sum = 0.0;
  long index;

  if (LINK->showlow)
    max = LINK->actualsymbols - 1;
  else
    max = LINK->actualsymbols;
  for (index = 1; index <= max; index++) {
    fprintf(LINK->data->f, "%10.5f", map[index-1]);
    if (index > 1)
      fprintf(LINK->data->f, " +");
    putc('\n', LINK->data->f);
    sum += map[index-1];
  }
  fprintf(LINK->data->f, "sum is: %15.13f\n", sum);
  fprintf(LINK->data->f, " \n");
}




Local Void randomDelila(seed, LINK)
double *seed;
struct LOC_themain *LINK;
{
  /*






























*/
  double intermediate;

  intermediate = *seed + pi;
  intermediate = exp(5.0 * log(intermediate));
  *seed = intermediate - (long)intermediate;
}

#undef pi


Local Void graph(LINK)
struct LOC_themain *LINK;
{
  long rank, FORLIM;

  if (!LINK->graphing)
    return;

  FORLIM = LINK->actualsymbols;
  for (rank = 1; rank <= FORLIM; rank++)
    vput(&LINK->pcvector, rank, map[rank-1]);

  /*



*/


  vsub(LINK->cvector, &LINK->pcvector);

  /*


*/


  LINK->theta = angle(&LINK->pcvector, &LINK->acvector);

  /*


*/
  LINK->radius = magnitude(&LINK->pcvector);

  /*


*/


  polrec(LINK->radius, LINK->theta, &LINK->x, &LINK->y);
  fprintf(LINK->xyin->f, "%15.12f %15.12f\n", LINK->x, LINK->y);

  /*



*/

}



Static Void themain(zipfp, data_, xyin_)
_TEXT *zipfp, *data_, *xyin_;
{
  struct LOC_themain V;
  double sumx, sumy, sumxsqd, sumysqd, sumxy, ex, ey, varx, vary, covxy, r, m,
	 b;
  long n;
  double delta;
  long count = 0;
  long number;
  double previous;
  long rank;
  /*
*/
  boolean tomsway;
  double sum;
  _TEXT TEMP;
  long FORLIM;

  V.data = data_;
  V.xyin = xyin_;
  printf("zipf %4.2f\n", version);
  seed = 0.0;

  if (*zipfp->name != '\0') {
    if (zipfp->f != NULL)
      zipfp->f = freopen(zipfp->name, "r", zipfp->f);
    else
      zipfp->f = fopen(zipfp->name, "r");
  } else
    rewind(zipfp->f);
  if (zipfp->f == NULL)
    _EscIO2(FileNotFound, zipfp->name);
  RESETBUF(zipfp->f, Char);
  fscanf(zipfp->f, "%ld%*[^\n]", &number);
  getc(zipfp->f);
  if (number < 3) {
    printf("at least 3 correlations are required in zipfp\n");
    halt();
  }
  fscanf(zipfp->f, "%ld%*[^\n]", &V.actualsymbols);
  getc(zipfp->f);
  if (V.actualsymbols > maxsymbols) {
    printf("use fewer than %ld symbols in zipfp\n", (long)maxsymbols);
    halt();
  }
  if (P_peek(zipfp->f) == 't')
    tomsway = true;
  else
    tomsway = false;
  fscanf(zipfp->f, "%*[^\n]");
  getc(zipfp->f);
  if (P_peek(zipfp->f) == 'g')
    V.graphing = true;
  else
    V.graphing = false;
  if (*V.data->name != '\0') {
    if (V.data->f != NULL)
      V.data->f = freopen(V.data->name, "w", V.data->f);
    else
      V.data->f = fopen(V.data->name, "w");
  } else {
    if (V.data->f != NULL) {

      rewind(V.data->f);
    } else
      V.data->f = tmpfile();
  }
  if (V.data->f == NULL)
    _EscIO2(FileNotFound, V.data->name);
  SETUPBUF(V.data->f, Char);
  fprintf(V.data->f, "* zipf %4.2f\n", version);
  fprintf(V.data->f, "* %ld number of correlation coefficients made\n",
	  number);
  fprintf(V.data->f, "* %ld number of symbols\n", V.actualsymbols);
  if (tomsway)
    fprintf(V.data->f, "* Tom's way\n");
  else
    fprintf(V.data->f, "* Peter's way\n");
  if (V.graphing)
    fprintf(V.data->f, "* graphing simplex\n");
  else
    fprintf(V.data->f, "* not graphing simplex\n");

  if (V.graphing) {
    if (*V.xyin->name != '\0') {
      if (V.xyin->f != NULL)
	V.xyin->f = freopen(V.xyin->name, "w", V.xyin->f);
      else
	V.xyin->f = fopen(V.xyin->name, "w");
    } else {
      if (V.xyin->f != NULL)
	rewind(V.xyin->f);
      else
	V.xyin->f = tmpfile();
    }
    if (V.xyin->f == NULL)
      _EscIO2(FileNotFound, V.xyin->name);
    SETUPBUF(V.xyin->f, Char);

    makevector(&V.cvector, V.actualsymbols);
    vset(1.0 / V.actualsymbols, &V.cvector);
    printf("c vector: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writevector(&TEMP, V.cvector, 6L, 3L);

    /*
*/

    makevector(&V.acvector, V.actualsymbols);


    vset(0.0, &V.acvector);

    vput(&V.acvector, 1L, 1.0);


    vsub(V.cvector, &V.acvector);
    printf("ac vector: ");
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writevector(&TEMP, V.acvector, 6L, 3L);

    printf("dot ac c: %15.12f\n", dotproduct(V.acvector, V.cvector));


    makevector(&V.pcvector, V.actualsymbols);

  }

  V.showlow = false;
  do {


    regress('c', 0.0, 0.0, &sumx, &sumy, &sumxsqd, &sumysqd, &sumxy, &ex, &ey,
	    &varx, &vary, &covxy, &r, &m, &b, &n);


    FORLIM = V.actualsymbols;
    for (rank = 0; rank < FORLIM; rank++) {
      randomDelila(&seed, &V);
      map[rank] = seed;
    }

    if (debug) {
      if (!tomsway)
	V.showlow = true;
      fprintf(V.data->f, "\n*****************\n");
      fprintf(V.data->f, "* random numbers:\n");
      show(&V);
    }

    if (tomsway) {
      sum = 0.0;
      FORLIM = V.actualsymbols;
      for (rank = 0; rank < FORLIM; rank++)
	sum += map[rank];

      FORLIM = V.actualsymbols;
      /*
*/
      for (rank = 0; rank < FORLIM; rank++)
	map[rank] /= sum;

      if (debug) {
	fprintf(V.data->f, "* normalized:\n");
	show(&V);
      }

      graph(&V);


      quicksort(1, (int)V.actualsymbols);

      if (debug) {
	fprintf(V.data->f, "* sorted :\n");
	show(&V);
      }
    }

    else {
      quicksort(1, (int)(V.actualsymbols - 1));

      if (debug) {
	V.showlow = true;
	fprintf(V.data->f, "* sorted :\n");
	show(&V);
      }


      previous = 1.0;
      FORLIM = V.actualsymbols - 2;
      for (rank = 0; rank <= FORLIM; rank++) {
	delta = previous - map[rank];
	previous = map[rank];
	map[rank] = delta;
      }
      map[V.actualsymbols-1] = previous;

      if (debug) {
	V.showlow = false;
	fprintf(V.data->f, "* differences:\n");
	show(&V);
      }

      graph(&V);


      quicksort(1, (int)V.actualsymbols);

      if (debug) {
	fprintf(V.data->f, "* RE-sorted :\n");
	show(&V);
      }
    }
    FORLIM = V.actualsymbols;




    for (rank = 1; rank <= FORLIM; rank++) {
      if (debug)
	fprintf(V.data->f, "%ld %10.5f rank and ln(Pi)\n",
		rank, log(map[rank-1]));



      regress('e', (double)rank, log(map[rank-1]), &sumx, &sumy, &sumxsqd,
	      &sumysqd, &sumxy, &ex, &ey, &varx, &vary, &covxy, &r, &m, &b,
	      &n);
    }



    regress('r', 0.0, 0.0, &sumx, &sumy, &sumxsqd, &sumysqd, &sumxy, &ex, &ey,
	    &varx, &vary, &covxy, &r, &m, &b, &n);

    if (debug)
      fprintf(V.data->f, "correlation coefficient: ");
    fprintf(V.data->f, "%15.12f\n", r);


    count++;
  } while (count < number);

}

#undef debug


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  xyin.f = NULL;
  strcpy(xyin.name, "xyin");
  data.f = NULL;
  strcpy(data.name, "data");
  zipfp.f = NULL;
  strcpy(zipfp.name, "zipfp");
  themain(&zipfp, &data, &xyin);
_L1:
  if (zipfp.f != NULL)
    fclose(zipfp.f);
  if (data.f != NULL)
    fclose(data.f);
  if (xyin.f != NULL)
    fclose(xyin.f);
  exit(EXIT_SUCCESS);
}



/* End. */
