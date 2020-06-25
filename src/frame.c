/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "frame.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*









*/



#define version         1.18
/*




*/



/*

































*/



#define maxvecpart      64
#define vpagewidth      64


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


Static _TEXT norm, test, result;
Static parameter *normparam, *testparam;
Static vector normvector, testvector;
Static long r, e;


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



Static long copylines(fin, fout, n)
_TEXT *fin, *fout;
long n;
{
  /*
*/
  long index = 0;

  while (!BUFEOF(fin->f) && index < n) {
    copyaline(fin, fout);
    index++;
  }

  return index;
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




Static Void replacevector(v, n)
vector *v;
long n;
{
  /*










*/
  double sum;
  long i;

  for (i = 1; i <= n; i++) {
    sum = vget(*v, i) + vget(*v, i + n) + vget(*v, i + n * 2);
    vput(v, i, vget(*v, i) - sum / 3);
    vput(v, i + n, vget(*v, i + n) - sum / 3);
    vput(v, i + n * 2, vget(*v, i + n * 2) - sum / 3);
  }
  normalize(v);
}


Static Void rearrange(v, n)
vector *v;
long n;
{
  /*


*/
  long i;
  double r;

  for (i = 1; i <= n; i++) {
    r = vget(*v, i);
    vput(v, i, vget(*v, i + n));
    vput(v, i + n, vget(*v, i + n * 2));
    vput(v, i + n * 2, r);
  }
}


Static Void evaluate()
{
  /*
*/
  long i;

  fprintf(result.f, "%4c%7.4f", ' ', dotproduct(normvector, testvector));
  for (i = 1; i <= 2; i++) {
    rearrange(&testvector, testparam->wvlength);
    fprintf(result.f, "%4c%7.4f", ' ', dotproduct(normvector, testvector));
  }
  putc('\n', result.f);
}


Static Void header()
{
  fprintf(result.f, " frame %4.2f; reading frame evaluation of\n", version);
  fscanf(test.f, "%*[^\n]");
  getc(test.f);
  copyaline(&test, &result);
  fprintf(result.f, " by the standard of\n");
  fscanf(norm.f, "%*[^\n]");
  getc(norm.f);
  copyaline(&norm, &result);
  putc('\n', result.f);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  result.f = NULL;
  strcpy(result.name, "result");
  test.f = NULL;
  strcpy(test.name, "test");
  norm.f = NULL;
  strcpy(norm.name, "norm");
  printf(" frame %4.2f\n", version);
  if (*norm.name != '\0') {
    if (norm.f != NULL)
      norm.f = freopen(norm.name, "r", norm.f);
    else
      norm.f = fopen(norm.name, "r");
  } else
    rewind(norm.f);
  if (norm.f == NULL)
    _EscIO2(FileNotFound, norm.name);
  RESETBUF(norm.f, Char);
  if (*test.name != '\0') {
    if (test.f != NULL)
      test.f = freopen(test.name, "r", test.f);
    else
      test.f = fopen(test.name, "r");
  } else
    rewind(test.f);
  if (test.f == NULL)
    _EscIO2(FileNotFound, test.name);
  RESETBUF(test.f, Char);
  if (*result.name != '\0')
    result.f = fopen(result.name, "w");
  else
    result.f = tmpfile();
  if (result.f == NULL)
    _EscIO2(FileNotFound, result.name);
  SETUPBUF(result.f, Char);
  header();
  normparam = (parameter *)Malloc(sizeof(parameter));
  testparam = (parameter *)Malloc(sizeof(parameter));

  readencpar(&norm, &normparam, &r, &e);
  readencpar(&test, &testparam, &r, &e);
  if (vectorsize(normparam) == vectorsize(testparam)) {
    makevector(&normvector, vectorsize(normparam));
    makevector(&testvector, vectorsize(testparam));
  } else {
    printf(" input vectors of unequal size\n");
    halt();
  }
  readvector(&norm, &normvector);
  replacevector(&normvector, normparam->wvlength);

  fprintf(result.f, " %ld is the oligo length used for comparison\n\n",
	  normparam->coding);
  fprintf(result.f, "%4cframe 0%4cframe 1%4cframe 2\n\n", ' ', ' ', ' ');
  fscanf(test.f, "%*[^\n]");
  getc(test.f);
  fscanf(test.f, "%*[^\n]");
  getc(test.f);
  while (!BUFEOF(test.f)) {
    readvector(&test, &testvector);
    replacevector(&testvector, testparam->wvlength);
    evaluate();
  }
_L1:
  if (norm.f != NULL)
    fclose(norm.f);
  if (test.f != NULL)
    fclose(test.f);
  if (result.f != NULL)
    fclose(result.f);
  exit(EXIT_SUCCESS);
}



/* End. */
