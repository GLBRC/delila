/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "encsum.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         1.21
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

typedef struct vectorpart {
  double numbers[maxvecpart];
  struct vectorpart *next;
} vectorpart;

typedef struct vector {
  long length;
  vectorpart *part;
} vector;



Static _TEXT encseq, sum;


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



Static Void themain(encseq, sum)
_TEXT *encseq, *sum;
{
  vector sumvector, newvector;
  long seqnum = 0;
  long length;

  printf(" encsum %4.2f\n", version);
  if (*sum->name != '\0') {
    if (sum->f != NULL)
      sum->f = freopen(sum->name, "w", sum->f);
    else
      sum->f = fopen(sum->name, "w");
  } else {
    if (sum->f != NULL)
      rewind(sum->f);
    else
      sum->f = tmpfile();
  }
  if (sum->f == NULL)
    _EscIO2(FileNotFound, sum->name);
  SETUPBUF(sum->f, Char);
  fprintf(sum->f, " encsum version %4.2f; sum of vectors in\n", version);
  enchead(encseq, sum, &length);

  makevector(&sumvector, length);
  makevector(&newvector, length);
  vset(0.0, &sumvector);
  while (!BUFEOF(encseq->f)) {
    readvector(encseq, &newvector);
    seqnum++;
    vadd(newvector, &sumvector);
  }
  fprintf(sum->f, ";   %ld sequences in this sum\n\n", seqnum);
  writevector(sum, sumvector, 3L, 0L);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  sum.f = NULL;
  strcpy(sum.name, "sum");
  encseq.f = NULL;
  strcpy(encseq.name, "encseq");
  themain(&encseq, &sum);
_L1:
  if (encseq.f != NULL)
    fclose(encseq.f);
  if (sum.f != NULL)
    fclose(sum.f);
  exit(EXIT_SUCCESS);
}



/* End. */
