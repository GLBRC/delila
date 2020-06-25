/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "histan.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*



*/



#define version         4.22
/*
*/



/*




























*/



#define maxhistwidth    401


typedef enum {
  a, c, g, t
} base;

typedef long histarray[maxhistwidth];

typedef struct histogram {
  long *zero;
  long *mono[4];
  long *di[4][4];
  long *tri[4][4][4];
} histogram;




typedef struct compnode {
  long count;
  struct compnode *son[4];
  /*
*/
} compnode;


typedef struct spider {
  /*


*/
  long depth;
  compnode *place;
  struct spider *next;
} spider;

/*
*/

typedef struct comptotal {
  long count;
  struct comptotal *next;
} comptotal;


typedef struct path {
  base bas;
  struct path *next;
} path;


Static _TEXT cmp, hst, chisq;


Static compnode *root;
Static long compmax, readmax;
Static comptotal *monocomptotal;
/*
*/


Static histogram histo;
Static long histmin, histmax, numseqs, histbegin, histwidth, i;


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








Static Void skipoligo(thefile)
_TEXT *thefile;
{
  /*


*/
  Char ch;

  do {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != 't' && ch != 'g' && ch != 'c' && ch != 'a');
  do {
    ch = getc(thefile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
}


/*


*/

typedef struct list {
  compnode *item;
  struct list *next;
} list;


/* Local variables for readcomp: */
struct LOC_readcomp {
  list *freeitem;
} ;

/*

*/
Local Void getitem(l, LINK)
list **l;
struct LOC_readcomp *LINK;
{
  if (LINK->freeitem != NULL) {
    *l = LINK->freeitem;
    LINK->freeitem = LINK->freeitem->next;
  } else
    *l = (list *)Malloc(sizeof(list));
  (*l)->next = NULL;
}


Local Void clearitem(l, LINK)
list **l;
struct LOC_readcomp *LINK;
{
  list *lptr;

  if (*l == NULL)
    return;
  lptr = *l;
  *l = (*l)->next;
  lptr->next = LINK->freeitem;
  LINK->freeitem = lptr;
}


Static Void readcomp(comp, compmax, readmax, root, monocomptotal)
_TEXT *comp;
long *compmax, readmax;
compnode **root;
comptotal **monocomptotal;
{
  /*






*/
  struct LOC_readcomp V;
  list *listitem, *first, *last;
  comptotal *comptot, *newcomptot;
  long detcomp, level, number;
  Char ch;
  base ba;

  if (*comp->name != '\0') {
    if (comp->f != NULL)
      comp->f = freopen(comp->name, "r", comp->f);
    else
      comp->f = fopen(comp->name, "r");
  } else
    rewind(comp->f);
  if (comp->f == NULL)
    _EscIO2(FileNotFound, comp->name);
  RESETBUF(comp->f, Char);
  if (BUFEOF(comp->f)) {
    printf(" error: no composition file provided\n");
    halt();
  }
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%ld%*[^\n]", &detcomp);
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");


  getc(comp->f);
  if (readmax < 1) {
    printf("\n warning: 0 or negative oligo length requested\n");
    printf(" composition used is depth %ld\n\n", detcomp);
    *compmax = detcomp;
  } else if (readmax > detcomp) {
    printf("\n warning: requested composition oligo length (%ld)\n", readmax);
    printf(" is larger than the determined composition oligo length (%ld).\n",
	   detcomp);
    printf(" composition used is to depth %ld\n\n", detcomp);
    *compmax = detcomp;
  } else
    *compmax = readmax;

  *root = (compnode *)Malloc(sizeof(compnode));
  *monocomptotal = (comptotal *)Malloc(sizeof(comptotal));
  comptot = (comptotal *)Malloc(sizeof(comptotal));
  first = (list *)Malloc(sizeof(list));
  Malloc(sizeof(list));
/* p2c: histan.p: Note: Eliminated unused assignment statement [338] */
  first->item = *root;
  first->next = NULL;
  last = first;
  V.freeitem = (list *)Malloc(sizeof(list));
  V.freeitem->next = NULL;


  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%*[^\n]");
  getc(comp->f);
  fscanf(comp->f, "%ld%*[^\n]", &number);
  getc(comp->f);
  (*root)->count = number;

  do {
    ch = getc(comp->f);
    if (ch == '\n')
      ch = ' ';
    ch = getc(comp->f);
    if (ch == '\n')
      ch = ' ';
    if (ch == '*') {
      fscanf(comp->f, "%*[^\n]");
      getc(comp->f);
      fscanf(comp->f, "%ld%*[^\n]", &level);
      getc(comp->f);
      if (level == 1)
	*monocomptotal = comptot;
      else {
	newcomptot = (comptotal *)Malloc(sizeof(comptotal));
	comptot->next = newcomptot;
	comptot = newcomptot;
      }
      comptot->count = 0;
      comptot->next = NULL;
    } else {
      for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
	skipoligo(comp);
	fscanf(comp->f, "%ld", &number);
	if (number != 0) {
	  first->item->son[(long)ba] = (compnode *)Malloc(sizeof(compnode));
	  first->item->son[(long)ba]->count = number;
	  comptot->count += number;
	  getitem(&listitem, &V);
	  last->next = listitem;
	  last = listitem;
	  last->next = NULL;
	  last->item = first->item->son[(long)ba];
	} else
	  first->item->son[(long)ba] = NULL;
      }
      clearitem(&first, &V);
      fscanf(comp->f, "%*[^\n]");
      getc(comp->f);
    }
  } while (level != *compmax);
  /*

*/
  do {
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1)) {
      skipoligo(comp);
      fscanf(comp->f, "%ld", &number);
      if (number != 0) {
	first->item->son[(long)ba] = (compnode *)Malloc(sizeof(compnode));
	first->item->son[(long)ba]->count = number;
	comptot->count += number;
      } else
	first->item->son[(long)ba] = NULL;
    }
    clearitem(&first, &V);
    fscanf(comp->f, "%*[^\n]");
    getc(comp->f);
  } while (first != NULL);
}


Static long getcount(root, start)
compnode *root;
path *start;
{
  /*



*/
  compnode *place = root;
  path *point = start;

  while (place != NULL && point != NULL) {
    place = place->son[(long)point->bas];
    point = point->next;
  }
  if (place == NULL)
    return 0;
  else
    return (place->count);
}




Static Void readhist(hist, histmin, histmax, histwidth, histbegin, numseqs,
		     histo)
_TEXT *hist;
long *histmin, *histmax, *histwidth, *histbegin, *numseqs;
histogram *histo;
{
  /*







*/
  base b1, b2, b3;
  long i, FORLIM;

  if (*hist->name != '\0') {
    if (hist->f != NULL)
      hist->f = freopen(hist->name, "r", hist->f);
    else
      hist->f = fopen(hist->name, "r");
  } else
    rewind(hist->f);
  if (hist->f == NULL)
    _EscIO2(FileNotFound, hist->name);
  RESETBUF(hist->f, Char);
  fscanf(hist->f, "%*[^\n]");
  getc(hist->f);
  fscanf(hist->f, "%*[^\n]");
  getc(hist->f);
  fscanf(hist->f, "%ld%*[^\n]", histmin);
  getc(hist->f);
  fscanf(hist->f, "%ld%*[^\n]", histmax);
  getc(hist->f);
  fscanf(hist->f, "%ld%*[^\n]", numseqs);
  getc(hist->f);
  fscanf(hist->f, "%ld%*[^\n]", histwidth);
  getc(hist->f);
  fscanf(hist->f, "%*[^\n]");
  getc(hist->f);
  fscanf(hist->f, "%ld%*[^\n]", histbegin);
  getc(hist->f);
  fscanf(hist->f, "%*[^\n]");


  getc(hist->f);
  printf(" saw parameters: \n");
  printf("%ld\n", *histmin);
  printf("%ld\n", *histmax);
  printf("%ld\n", *numseqs);
  printf("%ld\n", *histwidth);
  printf("%ld\n", *histbegin);


  if (*histmin == 0) {
    histo->zero = (long *)Malloc(sizeof(histarray));
    FORLIM = *histwidth;
    for (i = 0; i < FORLIM; i++)
      fscanf(hist->f, "%ld", &histo->zero[i]);
  } else
    histo->zero = NULL;

  if (*histmin <= 1 && *histmax >= 1) {
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      skipoligo(hist);
      histo->mono[(long)b1] = (long *)Malloc(sizeof(histarray));
      FORLIM = *histwidth;
      for (i = 0; i < FORLIM; i++)
	fscanf(hist->f, "%ld", &histo->mono[(long)b1][i]);
    }
  } else {
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1))
      histo->mono[(long)b1] = NULL;
  }

  if (*histmin <= 2 && *histmax >= 2) {
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
	skipoligo(hist);
	histo->di[(long)b1][(long)b2] = (long *)Malloc(sizeof(histarray));
	FORLIM = *histwidth;
	for (i = 0; i < FORLIM; i++)
	  fscanf(hist->f, "%ld", &histo->di[(long)b1][(long)b2][i]);
      }
    }
  } else {
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1))
	histo->di[(long)b1][(long)b2] = NULL;
    }
  }

  if (*histmax >= 3) {
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
	for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	  skipoligo(hist);
	  histo->tri[(long)b1][(long)b2]
	    [(long)b3] = (long *)Malloc(sizeof(histarray));
	  FORLIM = *histwidth;
	  for (i = 0; i < FORLIM; i++)
	    fscanf(hist->f, "%ld", &histo->tri[(long)b1][(long)b2][(long)b3]
		   [i]);
	}
      }
    }
    return;
  }
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1))
	histo->tri[(long)b1][(long)b2][(long)b3] = NULL;
    }
  }
}



Static Void header()
{
  fprintf(chisq.f,
    "* histan version %4.2f; chi-squared histogram analysis using composition\n",
    version);
  if (!BUFEOF(hst.f)) {
    if (copylines(&hst, &chisq, 2L) != 2) {
      printf(" hst file too short\n");
      halt();
    }
  } else {
    printf("empty histogram file\n");
    halt();
  }
  if (!BUFEOF(cmp.f)) {
    if (copylines(&cmp, &chisq, 2L) != 2) {
      printf(" cmp file too short\n");
      halt();
    }
  } else {
    fprintf(chisq.f, "* no composition provided\n");
    fprintf(chisq.f, "* all oligos assumed equally frequent;\n");
  }

  fprintf(chisq.f, "*\n");
  fprintf(chisq.f, "* position");
  fprintf(chisq.f, "     mono");
  fprintf(chisq.f, "        di");
  fprintf(chisq.f, "       tri\n");
  fprintf(chisq.f, "*\n");
}


Static double expectedmono(ba, pos)
base ba;
long pos;
{
  base b1;
  long columnsum;
  double fractionexpected;
  path *start;

  if (histmin == 0)
    columnsum = histo.zero[pos-1];
  else {
    columnsum = 0;
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1))
      columnsum += histo.mono[(long)b1][pos-1];
  }
  start = (path *)Malloc(sizeof(path));
  start->bas = ba;
  start->next = NULL;
  if (monocomptotal == NULL)
    fractionexpected = 1.0 / 4;
  else
    fractionexpected = (double)getcount(root, start) / monocomptotal->count;
  return (columnsum * fractionexpected);
}


Static double expecteddi(ba1, ba2, pos)
base ba1, ba2;
long pos;
{
  base b1, b2;
  long columnsum = 0;
  double fractionexpected;
  path *point, *start;

  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1))
      columnsum += histo.di[(long)b1][(long)b2][pos-1];
  }
  start = (path *)Malloc(sizeof(path));
  start->bas = ba1;
  point = (path *)Malloc(sizeof(path));
  point->bas = ba2;
  start->next = point;
  point->next = NULL;
  if (monocomptotal == NULL)
    fractionexpected = 1.0 / 16;
  else
    fractionexpected = (double)getcount(root, start) / monocomptotal->next->count;
  return (columnsum * fractionexpected);
}


Static double expectedtri(ba1, ba2, ba3, pos)
base ba1, ba2, ba3;
long pos;
{
  base b1, b2, b3;
  long columnsum = 0;
  double fractionexpected;
  path *point, *start;

  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1))
	columnsum += histo.tri[(long)b1][(long)b2][(long)b3][pos-1];
    }
  }

  start = (path *)Malloc(sizeof(path));
  start->bas = ba1;
  point = (path *)Malloc(sizeof(path));
  point->bas = ba2;
  start->next = point;
  point = (path *)Malloc(sizeof(path));
  point->bas = ba3;
  point->next = NULL;
  start->next->next = point;
  if (monocomptotal == NULL)
    fractionexpected = 1.0 / 64;
  else
    fractionexpected = (double)getcount(root, start) /
		       monocomptotal->next->next->count;
  return (columnsum * fractionexpected);
}


Static Void chisquared(pos)
long pos;
{
  /*
*/
  base b1, b2, b3;
  long obs;
  double exp, chi;

  fprintf(chisq.f, "%8ld", pos + histbegin - 1);
  /*
*/
  if (histmin <= 1) {
    chi = 0.0;
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      obs = histo.mono[(long)b1][pos-1];
      exp = expectedmono(b1, pos);
      if (exp > 0)
	chi += (obs - exp) * (obs - exp) / exp;
    }
    fprintf(chisq.f, "%10.2f", chi);
  } else
    fprintf(chisq.f, "          ");

  if (histmin <= 2 && histmax >= 2 && compmax >= 2) {
    chi = 0.0;
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
	obs = histo.di[(long)b1][(long)b2][pos-1];
	exp = expecteddi(b1, b2, pos);
	if (exp > 0)
	  chi += (obs - exp) * (obs - exp) / exp;
      }
    }
    fprintf(chisq.f, "%10.2f", chi);
  } else
    fprintf(chisq.f, "          ");

  if (histmax < 3 || compmax < 3) {
    putc('\n', chisq.f);
    return;
  }
  chi = 0.0;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	obs = histo.tri[(long)b1][(long)b2][(long)b3][pos-1];
	exp = expectedtri(b1, b2, b3, pos);
	if (exp > 0)
	  chi += (obs - exp) * (obs - exp) / exp;
      }
    }
  }
  fprintf(chisq.f, "%10.2f\n", chi);
}


main(argc, argv)
int argc;
Char *argv[];
{
  long FORLIM;

  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  chisq.f = NULL;
  strcpy(chisq.name, "chisq");
  hst.f = NULL;
  strcpy(hst.name, "hst");
  cmp.f = NULL;
  strcpy(cmp.name, "cmp");
  printf("histan %4.2f\n", version);
  if (*chisq.name != '\0')
    chisq.f = fopen(chisq.name, "w");
  else
    chisq.f = tmpfile();
  if (chisq.f == NULL)
    _EscIO2(FileNotFound, chisq.name);
  SETUPBUF(chisq.f, Char);
  if (*cmp.name != '\0') {
    if (cmp.f != NULL)
      cmp.f = freopen(cmp.name, "r", cmp.f);
    else
      cmp.f = fopen(cmp.name, "r");
  } else
    rewind(cmp.f);
  if (cmp.f == NULL)
    _EscIO2(FileNotFound, cmp.name);
  RESETBUF(cmp.f, Char);
  if (*hst.name != '\0') {
    if (hst.f != NULL)
      hst.f = freopen(hst.name, "r", hst.f);
    else
      hst.f = fopen(hst.name, "r");
  } else
    rewind(hst.f);
  if (hst.f == NULL)
    _EscIO2(FileNotFound, hst.name);
  RESETBUF(hst.f, Char);
  header();

  readhist(&hst, &histmin, &histmax, &histwidth, &histbegin, &numseqs, &histo);

  if (histmax > 3)
    readmax = 3;
  else
    readmax = histmax;
  if (BUFEOF(cmp.f))
    monocomptotal = NULL;
  else
    readcomp(&cmp, &compmax, readmax, &root, &monocomptotal);
  FORLIM = histwidth;
  for (i = 1; i <= FORLIM; i++)
    chisquared(i);
_L1:
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (hst.f != NULL)
    fclose(hst.f);
  if (chisq.f != NULL)
    fclose(chisq.f);
  exit(EXIT_SUCCESS);
}



/* End. */
