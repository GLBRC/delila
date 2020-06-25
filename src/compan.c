/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "compan.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         3.26
/*



*/



/*







































*/


#define defcompmax      4


typedef enum {
  a, c, g, t
} base;


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


Static _TEXT cmp, anal, companp;
Static compnode *root;
Static comptotal *monocomptotal;
Static long compmax, readmax;


Static jmp_buf _JL1;




Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void unlimitln(afile)
_TEXT *afile;
{
  /*


*/
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
/* p2c: compan.p: Note: Eliminated unused assignment statement [338] */
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



Static Void header()
{
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
  fprintf(anal.f,
	  " compan version %4.2f; chi-squared analysis of composition\n",
	  version);
  if (copylines(&cmp, &anal, 2L) != 2) {
    printf(" composition file is too short\n");
    halt();
  }

  fprintf(anal.f, "\n %ld-long oligos are the longest analyzed\n", compmax);
  fprintf(anal.f, " (+) means observed > expected;");
  fprintf(anal.f, " (-) means observed < expected\n\n");
}


Static Void chisq(observed, expected, chi, sign)
double observed, expected, *chi;
Char *sign;
{
  /*
*/
  double diff;

  diff = observed - expected;
  if (diff >= 0)
    *sign = '+';
  else
    *sign = '-';
  if (expected > 0)
    *chi = diff * diff / expected;
  else
    *chi = 0.0;
}


Static Void calc0()
{
  base b1, b2, b3, b4;
  long comptot, observed;
  double expected, chi;
  double totchi = 0.0;
  double frequency;
  double inf = 0.0;
  /*
*/
  path *start, *point;
  Char sign;

  fprintf(anal.f, " **** chi squareds assuming equal frequencies ****\n\n");


  comptot = monocomptotal->count;
  expected = comptot / 4.0;
  fprintf(anal.f, " partials\n");
  start = (path *)Malloc(sizeof(path));
  start->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    start->bas = b1;
    observed = getcount(root, start);
    chisq((double)observed, expected, &chi, &sign);
    fprintf(anal.f, "%5c%c%8.2f (%c);", ' ', basetochar(b1), chi, sign);
    totchi += chi;
    if (observed > 0) {
      frequency = (double)observed / comptot;
      inf -= frequency * log(frequency) / log(2.0);
    }
  }
  fprintf(anal.f, "\n * total chisquared = %8.2f\n", totchi);
  fprintf(anal.f, " * Uncertainty H = %10.8f bits per base\n", inf);
  fprintf(anal.f, " * Normalized Uncertainty H = %10.8f\n\n", inf / 2);


  if (compmax >= 2) {
    totchi = 0.0;
    inf = 0.0;
    comptot = monocomptotal->next->count;
    expected = comptot / 16.0;
    fprintf(anal.f, " partials\n");
    point = (path *)Malloc(sizeof(path));
    point->next = NULL;
    start->next = point;
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      start->bas = b1;
      for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
	point->bas = b2;
	observed = getcount(root, start);
	chisq((double)observed, expected, &chi, &sign);
	fprintf(anal.f, "%4c%c%c%8.2f (%c);",
		' ', basetochar(b1), basetochar(b2), chi, sign);
	totchi += chi;
	if (observed > 0) {
	  frequency = (double)observed / comptot;
	  inf -= frequency * log(frequency) / log(2.0);
	}
      }
      putc('\n', anal.f);
    }
    fprintf(anal.f, " * total chisquared = %8.2f\n", totchi);
    fprintf(anal.f, " * information = %5.3f bits per base\n\n", inf / 2);
  }


  if (compmax >= 3) {
    totchi = 0.0;
    inf = 0.0;
    comptot = monocomptotal->next->next->count;
    expected = comptot / 64.0;
    fprintf(anal.f, " partials\n");
    point = (path *)Malloc(sizeof(path));
    point->next = NULL;
    start->next->next = point;
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      start->bas = b1;
      for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
	start->next->bas = b2;
	for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	  point->bas = b3;
	  observed = getcount(root, start);
	  chisq((double)observed, expected, &chi, &sign);
	  fprintf(anal.f, "%3c%c%c%c%8.2f (%c);",
		  ' ', basetochar(b1), basetochar(b2), basetochar(b3), chi,
		  sign);
	  totchi += chi;
	  if (observed > 0) {
	    frequency = (double)observed / comptot;
	    inf -= frequency * log(frequency) / log(2.0);
	  }
	}
	putc('\n', anal.f);
      }
      putc('\n', anal.f);
    }
    fprintf(anal.f, " * total chisquared = %8.2f\n", totchi);
    fprintf(anal.f, " * information = %5.3f bits per base\n\n", inf / 3);
  }


  if (compmax < 4)
    return;
  totchi = 0.0;
  inf = 0.0;
  comptot = monocomptotal->next->next->next->count;
  expected = comptot / 256.0;
  fprintf(anal.f, " partials\n");
  point = (path *)Malloc(sizeof(path));
  point->next = NULL;
  start->next->next->next = point;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    start->bas = b1;
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      start->next->bas = b2;
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	start->next->next->bas = b3;
	for (b4 = a; (long)b4 <= (long)t; b4 = (base)((long)b4 + 1)) {
	  point->bas = b4;
	  observed = getcount(root, start);
	  chisq((double)observed, expected, &chi, &sign);
	  fprintf(anal.f, "%2c%c%c%c%c%8.2f (%c);",
		  ' ', basetochar(b1), basetochar(b2), basetochar(b3),
		  basetochar(b4), chi, sign);
	  totchi += chi;
	  if (observed > 0) {
	    frequency = (double)observed / comptot;
	    inf -= frequency * log(frequency) / log(2.0);
	  }
	}
	putc('\n', anal.f);
      }
      putc('\n', anal.f);
    }
    putc('\n', anal.f);
  }
  fprintf(anal.f, " * total chisquared = %8.2f\n", totchi);
  fprintf(anal.f, " * information = %5.3f bits per base\n\n", inf / 4);
}


Static Void calc1()
{
  base b1, b2, b3, b4;
  double observed, expected;
  /*

*/
  double ratio, n1, n2, n3, n4, chi;
  double totchi = 0.0;
  Char sign;
  path *point1, *point2, *point3, *point4;

  fprintf(anal.f, "\f");
  fprintf(anal.f, " **** chi squareds using mono frequencies ****\n\n");


  ratio = log((double)monocomptotal->next->count) -
	  2 * log((double)monocomptotal->count);
  fprintf(anal.f, " partials\n");
  point1 = (path *)Malloc(sizeof(path));
  point2 = (path *)Malloc(sizeof(path));
  point2->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    point1->bas = b1;
    point1->next = NULL;
    n1 = getcount(root, point1);
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      point2->bas = b2;
      n2 = getcount(root, point2);
      if (n1 > 0 && n2 > 0)
	expected = exp(log(n1) + log(n2) + ratio);
      else
	expected = 0.0;
      point1->next = point2;
      observed = getcount(root, point1);
      chisq(observed, expected, &chi, &sign);
      fprintf(anal.f, "%4c%c%c%8.2f (%c);",
	      ' ', basetochar(b1), basetochar(b2), chi, sign);
      totchi += chi;
    }
    putc('\n', anal.f);
  }
  fprintf(anal.f, " * total chisquared = %8.2f\n\n", totchi);


  if (compmax >= 3) {
    totchi = 0.0;
    ratio = log((double)monocomptotal->next->next->count) -
	    3 * log((double)monocomptotal->count);
    fprintf(anal.f, " partials\n");
    point3 = (path *)Malloc(sizeof(path));
    point3->next = NULL;
    for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
      point1->bas = b1;
      point1->next = NULL;
      n1 = getcount(root, point1);
      for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
	point2->bas = b2;
	point2->next = NULL;
	n2 = getcount(root, point2);
	point1->next = point2;
	for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	  point3->bas = b3;
	  n3 = getcount(root, point3);
	  if (n1 > 0 && n2 > 0 && n3 > 0)
	    expected = exp(log(n1) + log(n2) + log(n3) + ratio);
	  else
	    expected = 0.0;
	  point2->next = point3;
	  observed = getcount(root, point1);
	  chisq(observed, expected, &chi, &sign);
	  fprintf(anal.f, "%3c%c%c%c%8.2f (%c);",
		  ' ', basetochar(b1), basetochar(b2), basetochar(b3), chi,
		  sign);
	  totchi += chi;
	}
	putc('\n', anal.f);
      }
      putc('\n', anal.f);
    }
    fprintf(anal.f, " * total chisquared = %8.2f\n\n", totchi);
  }


  if (compmax < 4)
    return;
  totchi = 0.0;
  ratio = log((double)monocomptotal->next->next->next->count) -
	  4 * log((double)monocomptotal->count);
  fprintf(anal.f, " partials\n");
  point4 = (path *)Malloc(sizeof(path));
  point4->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    point1->bas = b1;
    point1->next = NULL;
    n1 = getcount(root, point1);
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      point2->bas = b2;
      point2->next = NULL;
      n2 = getcount(root, point2);
      point1->next = point2;
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	point3->bas = b3;
	point3->next = NULL;
	n3 = getcount(root, point3);
	point2->next = point3;
	for (b4 = a; (long)b4 <= (long)t; b4 = (base)((long)b4 + 1)) {
	  point4->bas = b4;
	  n4 = getcount(root, point4);
	  if (n1 > 0 && n2 > 0 && n3 > 0 && n4 > 0)
	    expected = exp(log(n1) + log(n2) + log(n3) + log(n4) + ratio);
	  else
	    expected = 0.0;
	  point3->next = point4;
	  observed = getcount(root, point1);
	  chisq(observed, expected, &chi, &sign);
	  fprintf(anal.f, "%3c%c%c%c%c%8.2f (%c);",
		  ' ', basetochar(b1), basetochar(b2), basetochar(b3),
		  basetochar(b4), chi, sign);
	  totchi += chi;
	}
	putc('\n', anal.f);
      }
      putc('\n', anal.f);
    }
    putc('\n', anal.f);
  }
  fprintf(anal.f, " * total chisquared = %8.2f\n\n", totchi);
}


Static Void calc2()
{
  base b1, b2, b3, b4;
  double observed, expected, ratio, ratio2, ratio3, n1, n2, n3, chi;
  double totchi = 0.0;
  Char sign;
  path *point1, *point2, *point3, *point4;

  fprintf(anal.f, "\f");
  fprintf(anal.f, " **** chi squareds using di frequencies ****\n\n");


  /*

*/

  ratio = log((double)monocomptotal->count) +
	  log((double)monocomptotal->next->next->count) -
	  2 * log((double)monocomptotal->next->count);
  fprintf(anal.f, " partials\n");

  point1 = (path *)Malloc(sizeof(path));
  point2 = (path *)Malloc(sizeof(path));
  point3 = (path *)Malloc(sizeof(path));
  point3->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    point1->bas = b1;
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      point2->bas = b2;
      point1->next = point2;
      point2->next = NULL;
      n1 = getcount(root, point1);
      n2 = getcount(root, point2);
      if (n1 > 0) {
	ratio2 = ratio + log(n1) - log(n2);

      } else
	ratio2 = 1.0;
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	point3->bas = b3;
	point2->next = point3;
	n2 = getcount(root, point2);
	if (n2 > 0 && ratio2 < 1)
	  expected = exp(log(n2) + ratio2);
	else
	  expected = 0.0;
	observed = getcount(root, point1);
	chisq(observed, expected, &chi, &sign);
	fprintf(anal.f, "%3c%c%c%c%8.2f (%c);",
		' ', basetochar(b1), basetochar(b2), basetochar(b3), chi,
		sign);
	totchi += chi;
      }
      putc('\n', anal.f);
    }
    putc('\n', anal.f);
  }
  fprintf(anal.f, " * total chisquared = %8.2f\n\n", totchi);


  /*

*/

  if (compmax < 4)
    return;
  totchi = 0.0;

  ratio = 2 * log((double)monocomptotal->count) +
	  log((double)monocomptotal->next->next->next->count) -
	  3 * log((double)monocomptotal->next->count);
  fprintf(anal.f, " partials\n");

  point4 = (path *)Malloc(sizeof(path));
  point4->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    point1->bas = b1;
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      point2->bas = b2;
      point1->next = point2;
      point2->next = NULL;
      n1 = getcount(root, point1);
      n2 = getcount(root, point2);
      if (n1 > 0) {
	ratio2 = ratio + log(n1) - log(n2);

      } else
	ratio2 = 1.0;
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	point3->bas = b3;
	point2->next = point3;
	point3->next = NULL;
	n2 = getcount(root, point2);
	n3 = getcount(root, point3);
	if (n2 > 0 && ratio2 < 1) {
	  ratio3 = ratio2 + log(n2) - log(n3);

	} else
	  ratio3 = 1.0;
	for (b4 = a; (long)b4 <= (long)t; b4 = (base)((long)b4 + 1)) {
	  point4->bas = b4;
	  point3->next = point4;
	  n3 = getcount(root, point3);
	  if (n3 > 0 && ratio3 < 1)
	    expected = exp(ratio3 + log(n3));
	  else
	    expected = 0.0;
	  observed = getcount(root, point1);
	  chisq(observed, expected, &chi, &sign);
	  fprintf(anal.f, "%3c%c%c%c%c%8.2f (%c);",
		  ' ', basetochar(b1), basetochar(b2), basetochar(b3),
		  basetochar(b4), chi, sign);
	  totchi += chi;
	}
	putc('\n', anal.f);
      }
      putc('\n', anal.f);
    }
    putc('\n', anal.f);
  }
  fprintf(anal.f, " * total chisquared = %8.2f\n", totchi);
}


Static Void calc3()
{
  base b1, b2, b3, b4;
  double observed, expected;
  /*

*/
  double ratio, ratio2, n1, n2, chi;
  double totchi = 0.0;
  Char sign;
  path *point1, *point2, *point3, *point4;

  fprintf(anal.f, "\f");
  fprintf(anal.f, " **** chi squareds using tri frequenies ****\n\n");


  ratio = log((double)monocomptotal->next->count) +
	  log((double)monocomptotal->next->next->next->count) -
	  2 * log((double)monocomptotal->next->next->count);
  fprintf(anal.f, " partials\n");

  point1 = (path *)Malloc(sizeof(path));
  point2 = (path *)Malloc(sizeof(path));
  point1->next = point2;
  point3 = (path *)Malloc(sizeof(path));
  point2->next = point3;
  point4 = (path *)Malloc(sizeof(path));
  point4->next = NULL;
  for (b1 = a; (long)b1 <= (long)t; b1 = (base)((long)b1 + 1)) {
    point1->bas = b1;
    for (b2 = a; (long)b2 <= (long)t; b2 = (base)((long)b2 + 1)) {
      point2->bas = b2;
      for (b3 = a; (long)b3 <= (long)t; b3 = (base)((long)b3 + 1)) {
	point3->bas = b3;
	point3->next = NULL;
	n1 = getcount(root, point1);
	n2 = getcount(root, point2);
	if (n1 > 0) {
	  ratio2 = ratio + log(n1) - log(n2);

	} else
	  ratio2 = 1.0;
	for (b4 = a; (long)b4 <= (long)t; b4 = (base)((long)b4 + 1)) {
	  point4->bas = b4;
	  point3->next = point4;
	  n2 = getcount(root, point2);
	  if (n2 > 0 && ratio2 < 1)
	    expected = exp(ratio2 + log(n2));
	  else
	    expected = 0.0;
	  observed = getcount(root, point1);
	  chisq(observed, expected, &chi, &sign);
	  fprintf(anal.f, "%3c%c%c%c%c%8.2f (%c);",
		  ' ', basetochar(b1), basetochar(b2), basetochar(b3),
		  basetochar(b4), chi, sign);
	  totchi += chi;
	}
	putc('\n', anal.f);
      }
      putc('\n', anal.f);
    }
    putc('\n', anal.f);
  }
  fprintf(anal.f, " * total chisquared = %8.2f\n", totchi);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  companp.f = NULL;
  strcpy(companp.name, "companp");
  anal.f = NULL;
  strcpy(anal.name, "anal");
  cmp.f = NULL;
  strcpy(cmp.name, "cmp");
  printf("compan %4.2f\n", version);
  if (*anal.name != '\0')
    anal.f = fopen(anal.name, "w");
  else
    anal.f = tmpfile();
  if (anal.f == NULL)
    _EscIO2(FileNotFound, anal.name);
  SETUPBUF(anal.f, Char);
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
  if (BUFEOF(cmp.f)) {
    printf(" no composition file provided\n");
    halt();
  }
  if (*companp.name != '\0') {
    if (companp.f != NULL)
      companp.f = freopen(companp.name, "r", companp.f);
    else
      companp.f = fopen(companp.name, "r");
  } else
    rewind(companp.f);
  if (companp.f == NULL)
    _EscIO2(FileNotFound, companp.name);
  RESETBUF(companp.f, Char);
  if (BUFEOF(companp.f))
    readmax = defcompmax;
  else {
    fscanf(companp.f, "%ld%*[^\n]", &readmax);
    getc(companp.f);
  }
  if (readmax > defcompmax)
    readmax = defcompmax;
  readcomp(&cmp, &compmax, readmax, &root, &monocomptotal);
  header();
  calc0();
  if (compmax >= 2)
    calc1();
  if (compmax >= 3)
    calc2();
  if (compmax >= 4)
    calc3();
_L1:
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (anal.f != NULL)
    fclose(anal.f);
  if (companp.f != NULL)
    fclose(companp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
