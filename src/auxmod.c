/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "auxmod.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*







*/



#define version         1.41
/*


*/



/*





























*/



#define maxhistwidth    120



#define maxmatrix       120




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



typedef long matrix[4][maxmatrix];


Static _TEXT hst, cmp, patt;


Static long histmin, histmax, histwidth, histbegin, numseqs;
Static histogram histo;


Static long compmax, readmax;
Static compnode *root;
Static comptotal *monocomptotal;


Static long beginning, pattwidth;
Static matrix wmatrix;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Char basetochar(ba)
base ba;
{
  Char Result;

  switch (ba) {

  case a:
    Result = 'a';
    break;

  case c:
    Result = 'c';
    break;

  case g:
    Result = 'g';
    break;

  case t:
    Result = 't';
    break;
  }
  return Result;
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
/* p2c: auxmod.p: Note: Eliminated unused assignment statement [338] */
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


Local Void die()
{
  printf(" no helix list data\n");
  halt();
}






Static Void findcolon(thefile)
_TEXT *thefile;
{
  boolean found = false;

  while (!found) {
    if (BUFEOF(thefile->f)) {
      die();
      continue;
    }
    if (P_peek(thefile->f) != ':') {
      getc(thefile->f);
      continue;
    }
    getc(thefile->f);
    if (BUFEOF(thefile->f))
      die();
    else if (P_peek(thefile->f) == ' ') {
      getc(thefile->f);
      found = true;
    }
  }
}



Static Void gethelix(hlist, x, y, length, getenergy, energy, done)
_TEXT *hlist;
long *x, *y, *length;
boolean getenergy;
double *energy;
boolean *done;
{
  /*



*/
  if (BUFEOF(hlist->f)) {
    *done = true;
    return;
  }
  getc(hlist->f);
  if (P_peek(hlist->f) != ' ') {
    *done = true;
    return;
  }
  *done = false;
  findcolon(hlist);
  fscanf(hlist->f, "%ld", x);
  findcolon(hlist);
  fscanf(hlist->f, "%ld", y);
  findcolon(hlist);
  fscanf(hlist->f, "%ld", length);
  if (getenergy)
    fscanf(hlist->f, "%lg", energy);
  else
    *energy = 0.0;
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
}



Static boolean complines(a_, b)
_TEXT *a_, *b;
{
  /*







*/
  boolean Result;
  Char chara, charb;
  boolean done = false;

  if (BUFEOF(a_->f) & BUFEOF(b->f))
    return true;
  if (BUFEOF(a_->f) | BUFEOF(b->f))
    return false;
  while (!done) {
    if (P_eoln(a_->f))
      chara = ' ';
    else {
      chara = getc(a_->f);
      if (chara == '\n')
	chara = ' ';
    }

    if (P_eoln(b->f))
      charb = ' ';
    else {
      charb = getc(b->f);
      if (charb == '\n')
	charb = ' ';
    }

    if (P_eoln(a_->f) & P_eoln(b->f)) {
      done = true;
      Result = true;
    } else if (chara != charb) {
      done = true;
      Result = false;
    }
  }
  fscanf(a_->f, "%*[^\n]");
  getc(a_->f);
  fscanf(b->f, "%*[^\n]");
  getc(b->f);
  return Result;
}


Local Void gettitleline(xbook, ybook, hlist)
_TEXT *xbook, *ybook, *hlist;
{
  /*
*/
  if (*xbook->name != '\0') {
    if (xbook->f != NULL)
      xbook->f = freopen(xbook->name, "r", xbook->f);
    else
      xbook->f = fopen(xbook->name, "r");
  } else
    rewind(xbook->f);
  if (xbook->f == NULL)
    _EscIO2(FileNotFound, xbook->name);
  RESETBUF(xbook->f, Char);
  if (*ybook->name != '\0') {
    if (ybook->f != NULL)
      ybook->f = freopen(ybook->name, "r", ybook->f);
    else
      ybook->f = fopen(ybook->name, "r");
  } else
    rewind(ybook->f);
  if (ybook->f == NULL)
    _EscIO2(FileNotFound, ybook->name);
  RESETBUF(ybook->f, Char);
  if (*hlist->name != '\0') {
    if (hlist->f != NULL)
      hlist->f = freopen(hlist->name, "r", hlist->f);
    else
      hlist->f = fopen(hlist->name, "r");
  } else
    rewind(hlist->f);
  if (hlist->f == NULL)
    _EscIO2(FileNotFound, hlist->name);
  RESETBUF(hlist->f, Char);
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
}

Local Void findtitle(hlist)
_TEXT *hlist;
{
  long n;

  for (n = 1; n <= 4; n++)
    getc(hlist->f);
}



Static Void hlistvsbooks(hlist, xbook, ybook)
_TEXT *hlist, *xbook, *ybook;
{
  /*

*/
  boolean xasx, yasy, xasy, yasx;

  gettitleline(xbook, ybook, hlist);
  findtitle(hlist);
  xasx = complines(xbook, hlist);
  findtitle(hlist);
  yasy = complines(ybook, hlist);
  gettitleline(xbook, ybook, hlist);
  findtitle(hlist);
  yasx = complines(ybook, hlist);
  findtitle(hlist);
  xasy = complines(xbook, hlist);
  if (!xasx)
    printf(" xbook input file does not correspond to xbook in hlist.\n");
  if (!yasy)
    printf(" ybook input file does not correspond to ybook in hlist.\n");
  if (xasy && !xasx)
    printf(" xbook input file corresponds to ybook in hlist.\n");
  if (yasx && !yasy)
    printf(" ybook input file corresponds to xbook in hlist.\n");
  if (xasy && yasx && !xasx && !yasy)
    printf(" xbook and ybook input files are reversed. try again.\n");
  if (!(xasx && yasy))
    halt();
}


#define learnmax        16




Static Void findlearnend(pattern)
_TEXT *pattern;
{
  Char learnend[learnmax];
  long state = 1;

  if (*pattern->name != '\0') {
    if (pattern->f != NULL)
      pattern->f = freopen(pattern->name, "r", pattern->f);
    else
      pattern->f = fopen(pattern->name, "r");
  } else
    rewind(pattern->f);
  if (pattern->f == NULL)
    _EscIO2(FileNotFound, pattern->name);
  RESETBUF(pattern->f, Char);
  memcpy(learnend, "end of learning.", (long)learnmax);
  while (!BUFEOF(pattern->f) && state < learnmax) {
    if (learnend[state-1] == P_peek(pattern->f))
      state++;
    else
      state = 1;
    if (P_eoln(pattern->f)) {
      fscanf(pattern->f, "%*[^\n]");
      getc(pattern->f);
    } else
      getc(pattern->f);
  }
  if (!BUFEOF(pattern->f))
    return;
  printf("pattern matrix does not contain \"");
  printf("%.*s\" signal\n", learnmax, learnend);
  halt();
}

#undef learnmax


Static Void getcolon(f)
_TEXT *f;
{
  while (P_peek(f->f) != ':') {
    getc(f->f);
    if (BUFEOF(f->f)) {
      printf("pattern is missing colons\n");
      halt();
    }
  }
  getc(f->f);
}


Static Void readmatrix(thefile, wmatrix, beginning, width)
_TEXT *thefile;
long (*wmatrix)[maxmatrix];
long *beginning, *width;
{
  /*
*/
  long i;
  base ba;
  long FORLIM;

  findlearnend(thefile);


  getcolon(thefile);
  getcolon(thefile);

  getcolon(thefile);
  fscanf(thefile->f, "%ld", beginning);

  getcolon(thefile);
  fscanf(thefile->f, "%ld", width);

  if (*width > maxmatrix) {
    printf("input matrix too large\n");
    halt();
  }

  FORLIM = *width;
  for (i = 0; i < FORLIM; i++) {
    getcolon(thefile);
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
      fscanf(thefile->f, "%ld", &wmatrix[(long)ba][i]);
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
  }

  for (i = *width; i < maxmatrix; i++) {
    for (ba = a; (long)ba <= (long)t; ba = (base)((long)ba + 1))
      wmatrix[(long)ba][i] = 0;
  }
}



Static Void readthresholds(pattern, funcmin, nfuncmax)
_TEXT *pattern;
long *funcmin, *nfuncmax;
{
  if (*pattern->name != '\0') {
    if (pattern->f != NULL)
      pattern->f = freopen(pattern->name, "r", pattern->f);
    else
      pattern->f = fopen(pattern->name, "r");
  } else
    rewind(pattern->f);
  if (pattern->f == NULL)
    _EscIO2(FileNotFound, pattern->name);
  RESETBUF(pattern->f, Char);
  findlearnend(pattern);

  getcolon(pattern);
  fscanf(pattern->f, "%ld%*[^\n]", funcmin);
  getc(pattern->f);
  getcolon(pattern);
  fscanf(pattern->f, "%ld%*[^\n]", nfuncmax);
  getc(pattern->f);
}







Static Void demohistread()
{
  readhist(&hst, &histmin, &histmax, &histwidth, &histbegin, &numseqs, &histo);
  printf(" the histogram has the following features:\n");
  printf("%10ld is the minimum oligo searched for\n", histmin);
  printf("%10ld is the maximum oligo searched for\n", histmax);
  printf("%10ld is the width of the histogram\n", histwidth);
  printf("%10ld is the first position of the histogram\n", histbegin);
  printf("%10ld is the number of sequences analyzed\n", numseqs);
}


Static Void democompread()
{
  comptotal *comptot;

  readmax = 100;
  readcomp(&cmp, &compmax, readmax, &root, &monocomptotal);
  printf(" the composition has the following features:\n");
  printf("%10ld is the maximum length determined\n", compmax);
  printf(" the total number of oligos at each level, beginning with the monos, is:\n");
  Malloc(sizeof(comptotal));
/* p2c: auxmod.p: Note: Eliminated unused assignment statement [338] */
  comptot = monocomptotal;
  while (comptot != NULL) {
    printf("%10ld\n", comptot->count);
    comptot = comptot->next;
  }
}


Static Void demomatrixread()
{
  long funcmin, nfuncmax;

  printf(" the pattern matrix has the following features:\n");
  readmatrix(&patt, wmatrix, &beginning, &pattwidth);
  printf("%10ld is the width of the matrix\n", pattwidth);
  printf("%10ld is the first position of the matrix\n", beginning);
  readthresholds(&patt, &funcmin, &nfuncmax);
  printf("%10ld is the functional minimum\n", funcmin);
  printf("%10ld is the non-functional maximum\n", nfuncmax);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  patt.f = NULL;
  strcpy(patt.name, "patt");
  cmp.f = NULL;
  strcpy(cmp.name, "cmp");
  hst.f = NULL;
  strcpy(hst.name, "hst");
  printf("% .5E\n", version);
  printf("library of auxiliary modules\n");

  if (*hst.name != '\0')
    hst.f = fopen(hst.name, "r");
  else
    rewind(hst.f);
  if (hst.f == NULL)
    _EscIO2(FileNotFound, hst.name);
  RESETBUF(hst.f, Char);
  putchar('\n');
  if (!BUFEOF(hst.f))
    demohistread();
  else
    printf("demonstration of histogram reading requires non-empty hst file.\n");
  if (*cmp.name != '\0') {
    if (cmp.f != NULL)
      cmp.f = freopen(cmp.name, "r", cmp.f);
    else
      cmp.f = fopen(cmp.name, "r");
  } else {

    rewind(cmp.f);
  }
  if (cmp.f == NULL)
    _EscIO2(FileNotFound, cmp.name);
  RESETBUF(cmp.f, Char);
  putchar('\n');
  if (!BUFEOF(cmp.f))
    democompread();
  else
    printf("demonstration of composition reading requires non-empty cmp file.\n");
  if (*patt.name != '\0') {
    if (patt.f != NULL)
      patt.f = freopen(patt.name, "r", patt.f);
    else
      patt.f = fopen(patt.name, "r");
  } else {

    rewind(patt.f);
  }
  if (patt.f == NULL)
    _EscIO2(FileNotFound, patt.name);
  RESETBUF(patt.f, Char);
  putchar('\n');
  if (!BUFEOF(patt.f))
    demomatrixread();
  else
    printf("demonstration of matrix reading requires non-empty patt file. \n");
_L1:
  if (hst.f != NULL)
    fclose(hst.f);
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (patt.f != NULL)
    fclose(patt.f);
  exit(EXIT_SUCCESS);
}



/* End. */
