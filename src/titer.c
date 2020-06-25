/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "titer.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         2.29
/*
*/



/*








































*/



#define namelength      10
#define rows            8
#define columns         12
#define platesize       96

#define percent         '%'


typedef double wellarray[platesize];

typedef Char nametype[namelength];

typedef struct data {
  double ave, sd;
} data;

typedef struct measurement {
  double time;
  wellarray od414;
  struct measurement *next;
} measurement;

typedef struct position {
  Char row;
  long column;
} position;

typedef struct well {
  position pos;
  struct well *next;
} well;

typedef struct sample {
  nametype name, condition;
  well *wells;
  data activity;
  struct sample *next;
} sample;

typedef struct plate {
  long number;
  nametype name;
  wellarray volume, od620;
  measurement *assays;
  wellarray slope, cc;
  sample *samples;
  boolean gotod620;
  struct plate *next;
} plate;


Static _TEXT plates, verbose, result;
Static plate *p;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



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




Static Void getname(fin, name)
_TEXT *fin;
Char *name;
{
  /*
*/
  long i = 0;
  long j;
  boolean done = false;
  Char ch;

  do {
    ch = getc(fin->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch == ' ');

  while (ch != ' ' && !done) {
    i++;
    if (i <= namelength)
      name[i-1] = ch;
    if (P_eoln(fin->f)) {
      done = true;
      break;
    }
    ch = getc(fin->f);
    if (ch == '\n')
      ch = ' ';
  }
  for (j = i; j < namelength; j++)
    name[j] = ' ';
}





Static Void writename(fout, name)
_TEXT *fout;
Char *name;
{
  long i;

  for (i = 0; i < namelength; i++)
    putc(name[i], fout->f);
}



Static Void header(fin, result, verbose)
_TEXT *fin, *result, *verbose;
{
  /*
*/
  fprintf(result->f, " titer %4.2f  results of analysis of:\n", version);
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
  copyaline(fin, result);
  copyaline(fin, result);
  copyaline(fin, result);
  fprintf(result->f, "\nisolate    ");
  fprintf(result->f, "condition  ");
  fprintf(result->f, "plate      ");
  fprintf(result->f, "   activity");
  fprintf(result->f, "    st.dev.");
  fprintf(result->f, "     %c dev.\n\n", percent);

  fprintf(verbose->f, " titer %4.2f verbose analysis of:\n", version);
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
  copyaline(fin, verbose);
  copyaline(fin, verbose);
  copyaline(fin, verbose);
  putc('\n', verbose->f);
}


#define debugging       false


typedef struct welldata {
  nametype name, condition;
  Char row;
  long column;
} welldata;

typedef welldata namearray[platesize];


/* Local variables for getids: */
struct LOC_getids {
  _TEXT *fin;
} ;

/* Local variables for getcond: */
struct LOC_getcond {
  struct LOC_getids *LINK;
  Char ch;
} ;

Local Void skiptodata(LINK)
struct LOC_getcond *LINK;
{
  fscanf(LINK->LINK->fin->f, "%*[^\n]");
  getc(LINK->LINK->fin->f);
  fscanf(LINK->LINK->fin->f, "%*[^\n]");

  getc(LINK->LINK->fin->f);
  getc(LINK->LINK->fin->f);
  getc(LINK->LINK->fin->f);
  fscanf(LINK->LINK->fin->f, "%c%*[^\n]", &LINK->ch);
  getc(LINK->LINK->fin->f);

  if (LINK->ch == '\n')
    LINK->ch = ' ';
  fscanf(LINK->LINK->fin->f, "%*[^\n]");
  getc(LINK->LINK->fin->f);
  if (debugging)
    printf("%c\n", LINK->ch);
}

Local Void getcond(w, LINK)
welldata *w;
struct LOC_getids *LINK;
{
  /*



*/
  struct LOC_getcond V;
  long i, j, num, k;

  V.LINK = LINK;
  skiptodata(&V);
  if (V.ch == 'c') {
    for (i = 0; i < columns; i++) {
      fscanf(LINK->fin->f, "%ld", &num);
      for (j = 1; j <= rows; j++) {
	k = j + rows * i;
	getname(LINK->fin, w[k-1].condition);
      }
      fscanf(LINK->fin->f, "%*[^\n]");
      getc(LINK->fin->f);
    }
    fscanf(LINK->fin->f, "%*[^\n]");
    getc(LINK->fin->f);
    skiptodata(&V);
  } else {
    for (i = 0; i < platesize; i++) {
      w[i].condition[0] = 's';
      w[i].condition[1] = 't';
      w[i].condition[2] = 'd';
      for (j = 3; j < namelength; j++)
	w[i].condition[j] = ' ';
    }
  }
  if (V.ch != 'v') {
    printf("you are missing a volume plate\n");
    halt();
  }
}

Local Void addwells(w, sawell, LINK)
welldata w;
well **sawell;
struct LOC_getids *LINK;
{
  *sawell = (well *)Malloc(sizeof(well));
  (*sawell)->pos.row = w.row;
  (*sawell)->pos.column = w.column;
  (*sawell)->next = NULL;
}

Local Void makelist(well_, s, LINK)
welldata *well_;
sample **s;
struct LOC_getids *LINK;
{
  /*

*/
  long i, j;
  well *lastwell, *w;
  sample *lastsa, *sa;
  Char ch;
  boolean newsamp;
  _TEXT TEMP;

  sa = (sample *)Malloc(sizeof(sample));
  sa->next = NULL;
  lastsa = sa;
  *s = sa;
  for (i = 0; i < namelength; i++)
    sa->name[i] = well_[0].name[i];
  if (debugging) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, sa->name);
  }
  for (i = 0; i < namelength; i++)
    sa->condition[i] = well_[0].condition[i];
  if (debugging) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, sa->condition);
  }
  addwells(well_[0], &w, LINK);
  sa->wells = w;
  lastwell = w;
  /*
*/
  for (j = 1; j < platesize; j++) {
    newsamp = false;

    i = 0;
    do {
      i++;
      ch = well_[j].name[i-1];
    } while (ch == sa->name[i-1] && i != namelength);
    if (ch != sa->name[i-1])
      newsamp = true;
    else {
      i = 0;
      do {
	i++;
	ch = well_[j].condition[i-1];
      } while (ch == sa->condition[i-1] && i != namelength);
      if (ch != sa->condition[i-1])
	newsamp = true;
    }

    if (newsamp) {
      sa = (sample *)Malloc(sizeof(sample));
      sa->next = NULL;
      lastsa->next = sa;
      lastsa = sa;
      for (i = 0; i < namelength; i++)
	sa->name[i] = well_[j].name[i];
      if (debugging) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writename(&TEMP, sa->name);
      }
      for (i = 0; i < namelength; i++)
	sa->condition[i] = well_[j].condition[i];
      if (debugging) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writename(&TEMP, sa->condition);
      }
      addwells(well_[j], &w, LINK);
      sa->wells = w;
      lastwell = w;
    } else {
      addwells(well_[j], &w, LINK);
      lastwell->next = w;
      lastwell = w;
    }
  }
}




Static Void getids(fin_, s)
_TEXT *fin_;
sample **s;
{
  /*
*/
  struct LOC_getids V;
  long i, j, num, k;
  namearray well_;


  V.fin = fin_;
  for (i = 1; i <= columns; i++) {
    fscanf(V.fin->f, "%ld", &num);
    for (j = 1; j <= rows; j++) {
      /*



*/
      k = j + rows * (i - 1);
      if (debugging)
	printf("row=%ldcolumn=%ldk=%ld\n", i, j, k);
      getname(V.fin, well_[k-1].name);
      well_[k-1].column = i;

      well_[k-1].row = (Char)(j - 1 + 'a');
    }
    fscanf(V.fin->f, "%*[^\n]");
    getc(V.fin->f);
  }
  fscanf(V.fin->f, "%*[^\n]");
  getc(V.fin->f);
  getcond(well_, &V);
  makelist(well_, s, &V);
}

#undef debugging




Static Void getnumbers(fin, w)
_TEXT *fin;
double *w;
{
  long i, j, n;

  for (i = 0; i < columns; i++) {
    fscanf(fin->f, "%ld", &n);
    for (j = 0; j < rows; j++)
      fscanf(fin->f, "%lg", &w[j + rows * i]);
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
}




Static Void getvol(fin, p)
_TEXT *fin;
plate **p;
{
  getnumbers(fin, (*p)->volume);
}




Static Void avedata(p)
plate **p;
{
  /*


*/
  long r, c, i, wpos;
  double act, sumact, sumsqdact;
  well *w;
  sample *sa;

  sa = (*p)->samples;
  while (sa != NULL) {
    sumact = 0.0;
    sumsqdact = 0.0;
    i = 0;
    w = sa->wells;
    while (w != NULL) {
      c = w->pos.column;
      r = w->pos.row + 1 - 'a';
      i++;
      wpos = r + (c - 1) * rows;

      if ((*p)->od620[wpos-1] * (*p)->volume[wpos-1] > 0.0001)
	act = (*p)->slope[wpos-1] * 60000L /
	      ((*p)->od620[wpos-1] * (*p)->volume[wpos-1]);
      else
	act = 0.0;

      sumact += act;
      sumsqdact += act * act;
      w = w->next;
    }
    sa->activity.ave = sumact / i;

    sa->activity.sd = sqrt(sumsqdact / i - sumact / i * (sumact / i));
    sa = sa->next;
  }
}






Static Void getod620(fin, p)
_TEXT *fin;
plate **p;
{
  long i;


  for (i = 1; i <= 3; i++) {
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }
  getnumbers(fin, (*p)->od620);
  (*p)->gotod620 = true;
}


#define debugging       false





Static Void getdata(fin, p)
_TEXT *fin;
plate **p;
{
  measurement *lastassay, *assay;


  getc(fin->f);
  assay = (measurement *)Malloc(sizeof(measurement));
  if ((*p)->assays == NULL)
    (*p)->assays = assay;
  else {
    lastassay = (*p)->assays;
    while (lastassay->next != NULL)
      lastassay = lastassay->next;
    lastassay->next = assay;
  }
  assay->next = NULL;
  fscanf(fin->f, "%lg%*[^\n]", &assay->time);
  getc(fin->f);
  if (debugging)
    printf("time %6.2f\n", assay->time);
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  getnumbers(fin, assay->od414);
}

#undef debugging




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
    *ex = *sumx / *n;
    *ey = *sumy / *n;

    *varx = *sumxsqd / *n - *ex * *ex;
    *vary = *sumysqd / *n - *ey * *ey;

    *covxy = *sumxy / *n - *ex * *ey;

    if (*varx * *vary > 0.0001)
      *r = *covxy / sqrt(*varx * *vary);
    else
      *r = 0.0;



    if (*varx > 0.0001)
      *m = *covxy / *varx;
    else
      *m = 0.0;
    *b = *ey - *m * *ex;
    break;
  }
}


#define debugging       false




Static Void writedata(p, result, verbose)
plate *p;
_TEXT *result, *verbose;
{
  Char row;
  long column, i;
  double act;
  sample *s;
  data *WITH;

  while (p != NULL) {
    s = p->samples;
    while (s != NULL) {
      /*
*/
      writename(result, s->name);
      putc(' ', result->f);
      writename(result, s->condition);
      putc(' ', result->f);
      writename(result, p->name);
      putc(' ', result->f);
      WITH = &s->activity;

      /*
*/
      fprintf(result->f, "%10.2f", WITH->ave);
      putc(' ', result->f);
      fprintf(result->f, "%10.2f", WITH->sd);
      putc(' ', result->f);
      if (WITH->ave > 0.0)
	fprintf(result->f, "%10ld\n",
		(long)floor(100 * WITH->sd / WITH->ave + 0.5));
      else
	fprintf(result->f, "%10c\n", ' ');
      putc('\n', verbose->f);
      writename(verbose, s->name);
      putc(' ', verbose->f);
      writename(verbose, s->condition);
      putc(' ', verbose->f);
      writename(verbose, p->name);
      fprintf(verbose->f, " is averaged from wells:\n");
      fprintf(verbose->f, " well  activity  correlation coefficient\n");

      while (s->wells != NULL) {
	column = s->wells->pos.column;
	row = s->wells->pos.row;
	fprintf(verbose->f, "   %c%ld", row, column);

	i = (column - 1) * rows + row + 1 - 'a';
	if (debugging)
	  fprintf(verbose->f, "  array pos = %ld", i);
	if (!p->gotod620) {
	  printf("an od620 plate is missing\n");
	  halt();
	}
	if (p->od620[i-1] * p->volume[i-1] > 0.0001)
	  act = p->slope[i-1] * 60000L / (p->od620[i-1] * p->volume[i-1]);
	else
	  act = 0.0;
	fprintf(verbose->f, "  %8.2f", act);
	fprintf(verbose->f, "  %8.2f\n", p->cc[i-1]);
	s->wells = s->wells->next;
      }
      fprintf(verbose->f, "actvity =%8.2f", s->activity.ave);
      fprintf(verbose->f, "   st. dev. = %6.3f\n", s->activity.sd);
      s = s->next;
    }
    p = p->next;
  }
}

#undef debugging




Static Void getplatenumber(fin, n)
_TEXT *fin;
long *n;
{
  Char ch;

  do {
    ch = getc(fin->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
  do {
    ch = getc(fin->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
  fscanf(fin->f, "%ld", n);
}



Static Void gettype(fin, t)
_TEXT *fin;
Char *t;
{
  getc(fin->f);
  getc(fin->f);
  fscanf(fin->f, "%c%*[^\n]", t);
  getc(fin->f);
  if (*t == '\n')
    *t = ' ';
}


#define debugging       false



Static Void skipplate(fin)
_TEXT *fin;
{
  long i;

  if (debugging)
    printf("skipping plate\n");
  for (i = 1; i <= columns + 3; i++) {
    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
  }
}

#undef debugging


#define debugging       false



Static Void readplates(fin, p)
_TEXT *fin;
plate **p;
{
  long i, n;
  long lastn = 0, currentn = 0;
  Char t;
  plate *lastp, *pa;
  measurement *assay;
  _TEXT TEMP;

  while (!BUFEOF(fin->f)) {
    getplatenumber(fin, &n);
    if (debugging)
      printf("platenumber %ld\n", n);
    if (n > lastn) {
      lastn = n;
      currentn = n;
      pa = (plate *)Malloc(sizeof(plate));
      if (n == 1)
	*p = pa;
      else {
	lastp = *p;
	while (lastp->next != NULL)
	  lastp = lastp->next;
	lastp->next = pa;
      }
      pa->number = n;
      pa->next = NULL;
      pa->assays = NULL;
      pa->samples = NULL;
      pa->gotod620 = false;
      getname(fin, pa->name);
      if (debugging) {
	TEMP.f = stdout;
	*TEMP.name = '\0';
	writename(&TEMP, pa->name);
      }
      for (i = 1; i <= 4; i++) {
	fscanf(fin->f, "%*[^\n]");
	getc(fin->f);
      }

      getids(fin, &pa->samples);

      getvol(fin, &pa);
      continue;
    }
    fscanf(fin->f, "%*[^\n]");

    getc(fin->f);
    if (currentn != n) {
      pa = *p;
      while (pa->number < n)
	pa = pa->next;
      currentn = n;
    }

    gettype(fin, &t);
    if (debugging)
      printf("platetype %c\n", t);
    if (t == 'd') {
      getod620(fin, &pa);
      continue;
    }
    if (t == 't') {
      getdata(fin, &pa);
      continue;
    }
    printf("invalid platetype %c\n", t);
    printf("REMAINDER OF THE INPUT FILE:\n");
    while (!BUFEOF(fin->f)) {
      TEMP.f = stdout;
      *TEMP.name = '\0';
      copyaline(fin, &TEMP);
    }
    halt();
  }
  if (!debugging)
    return;
  pa = *p;
  while (pa != NULL) {
    printf("%12ld  ", pa->number);
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writename(&TEMP, pa->name);
    putchar('\n');
    assay = pa->assays;
    while (assay != NULL) {
      printf("% .1E", assay->time);
      assay = assay->next;
    }
    putchar('\n');
    pa = pa->next;
  }
}

#undef debugging


#define debugging       false

#define stopdiff        0.3




Static Void analyze(p)
plate **p;
{
  /*
*/
  long i;
  measurement *a;
  plate *pa;

  boolean done;
  double x, y, lastx, lasty, sx, sy, sxs, sys, sxy, ex, ey, vx, vy, covxy, cc,
	 m, b;
  long n, column;
  Char row;

  pa = *p;
  if (debugging)
    printf(" beginning regressions \n");
  while (pa != NULL) {
    for (i = 1; i <= platesize; i++) {
      lasty = 0.0;
      done = false;

      regress('c', x, y, &sx, &sy, &sxs, &sys, &sxy, &ex, &ey, &vx, &vy,
	      &covxy, &cc, &m, &b, &n);
      a = pa->assays;
      do {
	x = a->time;
	y = a->od414[i-1];
	if (lasty - y > stopdiff) {
	  done = true;
	  fprintf(verbose.f, " plate ");
	  writename(&verbose, pa->name);

	  column = (long)((i - 1.0) / rows) + 1;
	  row = (Char)(((i - 1) & (rows - 1)) + 'a');
	  fprintf(verbose.f, "  well %c%ld", row, column);
	  fprintf(verbose.f, " analysis stopped after time %4ld seconds\n",
		  (long)floor(lastx + 0.5));
	}

	else {
	  lastx = x;
	  lasty = y;
	  regress('e', x, y, &sx, &sy, &sxs, &sys, &sxy, &ex, &ey, &vx, &vy,
		  &covxy, &cc, &m, &b, &n);
	  a = a->next;
	  if (a == NULL)
	    done = true;
	}
      } while (!done);
      if (debugging)
	printf("time points =%2ld\n", n);
      regress('r', x, y, &sx, &sy, &sxs, &sys, &sxy, &ex, &ey, &vx, &vy,
	      &covxy, &cc, &m, &b, &n);
      pa->slope[i-1] = m;
      pa->cc[i-1] = cc;
    }
    if (debugging)
      printf("regression done for plate %2ld\n", pa->number);
    /*
*/
    avedata(&pa);
    pa = pa->next;
  }
}

#undef debugging
#undef stopdiff


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  result.f = NULL;
  strcpy(result.name, "result");
  verbose.f = NULL;
  strcpy(verbose.name, "verbose");
  plates.f = NULL;
  strcpy(plates.name, "plates");
  if (*result.name != '\0') {
    if (result.f != NULL)
      result.f = freopen(result.name, "w", result.f);
    else
      result.f = fopen(result.name, "w");
  } else {
    if (result.f != NULL)
      rewind(result.f);
    else
      result.f = tmpfile();
  }
  if (result.f == NULL)
    _EscIO2(FileNotFound, result.name);
  SETUPBUF(result.f, Char);
  if (*verbose.name != '\0') {
    if (verbose.f != NULL)
      verbose.f = freopen(verbose.name, "w", verbose.f);
    else
      verbose.f = fopen(verbose.name, "w");
  } else {
    if (verbose.f != NULL)
      rewind(verbose.f);
    else
      verbose.f = tmpfile();
  }
  if (verbose.f == NULL)
    _EscIO2(FileNotFound, verbose.name);
  SETUPBUF(verbose.f, Char);
  printf("titer %4.2f\n", version);
  header(&plates, &result, &verbose);
  readplates(&plates, &p);
  analyze(&p);
  writedata(p, &result, &verbose);

_L1:
  if (plates.f != NULL)
    fclose(plates.f);
  if (verbose.f != NULL)
    fclose(verbose.f);
  if (result.f != NULL)
    fclose(result.f);
  exit(EXIT_SUCCESS);
}



/* End. */
