/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "coda.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*

*/



#define version         2.05


/*
*/



/*






















































*/



typedef enum {
  a, c, g, t
} base;

typedef struct parameters {
  long depth;
  /*
*/
  long lower, upper;
  Char mode;
  /*


*/
} parameters;




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



Static _TEXT cmp, data, codap;


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
/* p2c: coda.p: Note: Eliminated unused assignment statement [338] */
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




Static Void init(cmp, data, codap)
_TEXT *cmp, *data, *codap;
{
  printf(" coda %4.2f\n", version);

  if (*cmp->name != '\0') {
    if (cmp->f != NULL)
      cmp->f = freopen(cmp->name, "r", cmp->f);
    else
      cmp->f = fopen(cmp->name, "r");
  } else
    rewind(cmp->f);
  if (cmp->f == NULL)
    _EscIO2(FileNotFound, cmp->name);
  RESETBUF(cmp->f, Char);
  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "w", data->f);
    else
      data->f = fopen(data->name, "w");
  } else {
    if (data->f != NULL)
      rewind(data->f);
    else
      data->f = tmpfile();
  }
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  SETUPBUF(data->f, Char);
  if (*codap->name != '\0') {
    if (codap->f != NULL)
      codap->f = freopen(codap->name, "r", codap->f);
    else
      codap->f = fopen(codap->name, "r");
  } else
    rewind(codap->f);
  if (codap->f == NULL)
    _EscIO2(FileNotFound, codap->name);
  RESETBUF(codap->f, Char);
  unlimitln(data);
}


/* Local variables for copystart: */
struct LOC_copystart {
  _TEXT *cmp, *data;
} ;

Local Void aline(LINK)
struct LOC_copystart *LINK;
{
  if (copylines(LINK->cmp, LINK->data, 1L) != 1) {
    printf(" composition file too short\n");
    halt();
  }
}



Static Void copystart(cmp_, data_)
_TEXT *cmp_, *data_;
{
  struct LOC_copystart V;

  V.cmp = cmp_;
  V.data = data_;
  fprintf(V.data->f, "* coda %4.2f from: \n", version);

  putc('*', V.data->f);
  aline(&V);

  if (!P_eoln(V.cmp->f))
    getc(V.cmp->f);
  aline(&V);
}



Static Void getparam(codap, param)
_TEXT *codap;
parameters *param;
{
  if (BUFEOF(codap->f)) {
    printf(" codap is empty\n");
    halt();
  }
  fscanf(codap->f, "%ld%*[^\n]", &param->depth);
  getc(codap->f);
  if (param->depth < 0) {
    printf(" composition depth cannot be negative");
    halt();
  }

  if (BUFEOF(codap->f)) {
    param->lower = 0;
    param->upper = 100000L;
    param->mode = 'b';
    return;
  }
  fscanf(codap->f, "%ld%*[^\n]", &param->lower);
  getc(codap->f);
  if (param->lower < 0) {
    printf(" lower limit of number of oligos is 0.\n");
    halt();
  }

  if (BUFEOF(codap->f)) {
    printf(" missing upper parameter\n");
    halt();
  }
  fscanf(codap->f, "%ld%*[^\n]", &param->upper);
  getc(codap->f);
  if (param->upper < param->lower) {
    printf(" upper boundary cannot be below lower.\n");
    halt();
  }

  if (BUFEOF(codap->f)) {
    printf(" missing mode parameter\n");
    halt();
  }
  fscanf(codap->f, "%c%*[^\n]", &param->mode);
  getc(codap->f);
  if (param->mode == '\n')
    param->mode = ' ';
  if (param->mode != 'a' && param->mode != 'b' && param->mode != 'n' &&
      param->mode != 's') {
    printf(" mode must be one of abns.\n");
    halt();
  }
}


#define spacer          10


/* Local variables for puttodata: */
struct LOC_puttodata {
  _TEXT *data;
  compnode *root;
  parameters param;
  long count, oligosput;
  path *proot, *pwrite;
  long totalcount;
} ;

Local Void down(p, LINK)
path *p;
struct LOC_puttodata *LINK;
{
  /*

*/
  base b;
  long dwrite, FORLIM1;

  for (b = a; (long)b <= (long)t; b = (base)((long)b + 1)) {
    p->bas = b;
    if (p->next != NULL)
      down(p->next, LINK);
    else {
      LINK->count = getcount(LINK->root, LINK->proot);

      if (LINK->param.lower <= LINK->count) {
	if (LINK->count <= LINK->param.upper) {
	  if (LINK->param.mode == 'b')
	    fprintf(LINK->data->f, "%*ld", spacer, LINK->count);
	  putc(' ', LINK->data->f);
	  LINK->pwrite = LINK->proot;
	  FORLIM1 = LINK->param.depth;
	  for (dwrite = 1; dwrite <= FORLIM1; dwrite++) {
	    switch (LINK->pwrite->bas) {

	    case a:
	      putc('a', LINK->data->f);
	      break;

	    case c:
	      putc('c', LINK->data->f);
	      break;

	    case g:
	      putc('g', LINK->data->f);
	      break;

	    case t:
	      putc('t', LINK->data->f);
	      break;
	    }
	    LINK->pwrite = LINK->pwrite->next;
	  }
	  if (LINK->param.mode == 'a')
	    fprintf(LINK->data->f, "%*ld", spacer, LINK->count);
	  putc('\n', LINK->data->f);
	  LINK->oligosput++;
	  LINK->totalcount += LINK->count;
	}

      }
    }
  }
}



Static Void puttodata(data_, root_, param_)
_TEXT *data_;
compnode *root_;
parameters param_;
{
  /*


*/
  struct LOC_puttodata V;
  long d;
  path *p;
  long FORLIM;

  V.data = data_;
  V.root = root_;
  V.param = param_;
  fprintf(V.data->f, "* %ld long oligos with occurences from %ld to %ld\n",
	  V.param.depth, V.param.lower, V.param.upper);
  if (V.param.mode == 's') {
    fprintf(V.data->f, "* coda search mode\n");
    fprintf(V.data->f, "view nothing view position\n");
  }


  V.proot = (path *)Malloc(sizeof(path));
  p = V.proot;
  FORLIM = V.param.depth;
  for (d = 1; d < FORLIM; d++) {
    p->next = (path *)Malloc(sizeof(path));
    p = p->next;
  }
  p->next = NULL;


  V.oligosput = 0;
  V.totalcount = 0;

  if (V.param.depth > 0)
    down(V.proot, &V);
  else
    fprintf(V.data->f, "%*ld\n", spacer, getcount(V.root, NULL));

  if (V.param.mode == 's')
    fprintf(V.data->f, "quit\n");

  printf(" %ld oligos put to data file\n", V.oligosput);
  printf(" representing %ld positions in the original sequence(s)\n",
	 V.totalcount);
}

#undef spacer



Static Void themain(cmp, data, codap)
_TEXT *cmp, *data, *codap;
{
  long compmax;
  comptotal *monoct;
  parameters param;
  compnode *root;

  init(cmp, data, codap);
  copystart(cmp, data);
  getparam(codap, &param);
  readcomp(cmp, &compmax, param.depth, &root, &monoct);
  if (compmax < param.depth) {
    printf(" requested depth does not exist.\n");
    halt();
  }

  puttodata(data, root, param);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  codap.f = NULL;
  strcpy(codap.name, "codap");
  data.f = NULL;
  strcpy(data.name, "data");
  cmp.f = NULL;
  strcpy(cmp.name, "cmp");
  themain(&cmp, &data, &codap);
_L1:
  if (cmp.f != NULL)
    fclose(cmp.f);
  if (data.f != NULL)
    fclose(data.f);
  if (codap.f != NULL)
    fclose(codap.f);
  exit(EXIT_SUCCESS);
}



/* End. */
