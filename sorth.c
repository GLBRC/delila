/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sorth.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*



*/



#define version         2.42
/*

*/


/*

















































*/



#define prime           '"'

#define lowpriority     3
/*
*/
#define maxhelix        1000


/*
*/



typedef struct operations {
  /*

*/
  Char priority[lowpriority];
  long low;
  boolean getenergy;
  long top;
  double minlenmaxene;
  /*
*/
  boolean doinglength;
} operations;

typedef short position;

typedef struct helix {
  long x, y, length;
  double energy;
} helix;



Static _TEXT hlist, shlist, list, sorthp;


Static position ordering[maxhelix];
Static helix helixes[maxhelix];
/*
*/
Static Char sorton;
/*
*/
Static boolean both;


Static jmp_buf _JL1;


/*

*/





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


/* Local variables for lessthan: */
struct LOC_lessthan {
  position a, b;
  double ea, eb;
  /*
*/
  long la, lb;
} ;

Local boolean byenergy(LINK)
struct LOC_lessthan *LINK;
{
  LINK->ea = fabs(helixes[ordering[LINK->a-1] - 1].energy);
  LINK->eb = fabs(helixes[ordering[LINK->b-1] - 1].energy);
  return (LINK->ea < LINK->eb);
}

Local boolean bylength(LINK)
struct LOC_lessthan *LINK;
{
  LINK->la = helixes[ordering[LINK->a-1] - 1].length;
  LINK->lb = helixes[ordering[LINK->b-1] - 1].length;
  return (LINK->la < LINK->lb);
}



Static boolean lessthan(a_, b_)
position a_, b_;
{
  struct LOC_lessthan V;
  boolean Result;

  V.a = a_;
  V.b = b_;
  switch (sorton) {

  case 'e':
    if (byenergy(&V))
      Result = true;
    else if (both) {
      if (V.ea == V.eb)
	Result = bylength(&V);
      else
	Result = false;
    } else
      Result = false;
    break;

  case 'l':
    if (bylength(&V))
      Result = true;
    else if (both) {
      if (V.la == V.lb)
	Result = byenergy(&V);
      else
	Result = false;
    } else
      Result = false;
    break;
  }
  return Result;
}


Static Void swap_(a, b)
position a, b;
{
  position hold;

  hold = ordering[a-1];
  ordering[a-1] = ordering[b-1];
  ordering[b-1] = hold;
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




Static Void hcopy(hlist, shlist, list)
_TEXT *hlist, *shlist, *list;
{
  long dummy;

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
  if (*shlist->name != '\0') {
    if (shlist->f != NULL)
      shlist->f = freopen(shlist->name, "w", shlist->f);
    else
      shlist->f = fopen(shlist->name, "w");
  } else {
    if (shlist->f != NULL)
      rewind(shlist->f);
    else
      shlist->f = tmpfile();
  }
  if (shlist->f == NULL)
    _EscIO2(FileNotFound, shlist->name);
  SETUPBUF(shlist->f, Char);
  fprintf(shlist->f, " sorth %4.2f sorted helixes from ", version);

  if (copylines(hlist, shlist, 8L) != 8) {
    printf("hlist does not have a header\n");
    halt();
  }

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
  if (*list->name != '\0') {
    if (list->f != NULL)
      list->f = freopen(list->name, "w", list->f);
    else
      list->f = fopen(list->name, "w");
  } else {
    if (list->f != NULL)
      rewind(list->f);
    else
      list->f = tmpfile();
  }
  if (list->f == NULL)
    _EscIO2(FileNotFound, list->name);
  SETUPBUF(list->f, Char);
  fprintf(list->f, " sorth %4.2f sorted helixes from ", version);
  dummy = copylines(hlist, list, 7L);
  fscanf(hlist->f, "%*[^\n]");
  getc(hlist->f);
}



Static Void getparams(hlist, sorthp, order, list)
_TEXT *hlist, *sorthp;
operations *order;
_TEXT *list;
{
  /*

*/
  long i;
  long doe = 0, dol = 0;
  long FORLIM;

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
  for (i = 1; i <= 5; i++) {
    fscanf(hlist->f, "%*[^\n]");
    getc(hlist->f);
  }
  getc(hlist->f);

  if (P_peek(hlist->f) == 'e')
    order->getenergy = true;
  else
    order->getenergy = false;

  for (i = 1; i <= 3; i++) {
    fscanf(hlist->f, "%*[^\n]");
    getc(hlist->f);
  }


  if (*sorthp->name != '\0') {
    if (sorthp->f != NULL)
      sorthp->f = freopen(sorthp->name, "r", sorthp->f);
    else
      sorthp->f = fopen(sorthp->name, "r");
  } else
    rewind(sorthp->f);
  if (sorthp->f == NULL)
    _EscIO2(FileNotFound, sorthp->name);
  RESETBUF(sorthp->f, Char);
  if (BUFEOF(sorthp->f)) {
    printf("sorthp parameter file is empty\n");
    halt();
  }
  order->low = 0;
  while (!P_eoln(sorthp->f) && order->low < lowpriority) {
    if (P_peek(sorthp->f) == ' ') {
      getc(sorthp->f);
      continue;
    }
    order->low++;
    order->priority[order->low - 1] = getc(sorthp->f);
    if (order->priority[order->low - 1] == '\n')
      order->priority[order->low - 1] = ' ';
    if (order->priority[order->low - 1] == 'e') {
      doe++;
      continue;
    }
    if (order->priority[order->low - 1] == 'l')
      dol++;
    else if (order->priority[order->low - 1] != 'a') {
      printf("priority \"%c\" is not allowed\n",
	     order->priority[order->low - 1]);
      halt();
    }
  }
  if (doe > 1 || dol > 1) {
    printf("eea and lla are not allowed parameters\n");
    halt();
  }
  if (order->low < 2) {
    printf("priority list is too short in file sorthp\n");
    halt();
  }
  if (order->priority[order->low - 1] != 'a') {
    printf("priority list in file sorthp must end with an \"a\"\n");

    halt();
  }
  /*
*/
  if (!order->getenergy) {
    i = 1;
    while (i < order->low && order->priority[i-1] != 'e')
      i++;
    if (order->priority[i-1] == 'e') {
      printf("sorthp requests sorting by energy\n");
      printf("but there are no energies in hlist\n");
      halt();
    }
  }
  fscanf(sorthp->f, "%*[^\n]");


  getc(sorthp->f);
  if (BUFEOF(sorthp->f)) {
    order->top = 1;
    order->minlenmaxene = 0.0;
  } else {
    fscanf(sorthp->f, "%ld%*[^\n]", &order->top);
    getc(sorthp->f);
    if (order->top < 1) {
      printf(" \"top\" parameter must be > 1\n");
      halt();
    }
    if (BUFEOF(sorthp->f))
      order->minlenmaxene = 0.0;
    else {
      fscanf(sorthp->f, "%lg%*[^\n]", &order->minlenmaxene);
      getc(sorthp->f);
    }
  }
  order->doinglength = (order->minlenmaxene > 0.0 || !order->getenergy);


  putc(' ', list->f);
  FORLIM = order->low;
  for (i = 0; i < FORLIM; i++)
    putc(order->priority[i], list->f);
  fprintf(list->f, " priority order of sorting:");
  FORLIM = order->low - 2;
  for (i = 0; i <= FORLIM; i++) {
    switch (order->priority[i]) {

    case 'e':
      fprintf(list->f, " energy");
      break;

    case 'l':
      fprintf(list->f, " length");
      break;
    }
    fprintf(list->f, " then");
  }
  fprintf(list->f, " ambiguous.\n");
  fprintf(list->f, " %4ld helixes or fewer are written to shlist.  ",
	  order->top);
  if (order->top > maxhelix)
    fprintf(list->f, "all helixes guaranteed to be written.\n");
  else
    fprintf(list->f, "some helixes may be removed.\n");

  if (order->doinglength)
    fprintf(list->f, " %ld is the minimum length helix recorded\n",
	    (long)order->minlenmaxene);
  else
    fprintf(list->f, " %7.2f is the maximum energy helix recorded\n",
	    order->minlenmaxene);


  if (order->low == 3)
    both = true;
  else
    both = false;
}



Static Void hrecord(hlist, x, y, length, doenergy, energy)
_TEXT *hlist;
long x, y, length;
boolean doenergy;
double energy;
{
  /*
*/
  fprintf(hlist->f, "    x5%c: %6ld    y5%c: %6ld    length: %3ld",
	  prime, x, prime, y, length);
  if (doenergy)
    fprintf(hlist->f, "  %7.2f kcal", energy);
  putc('\n', hlist->f);
}



Static Void doset(hlist, shlist, order, n, u, e, l, a)
_TEXT *hlist, *shlist;
operations order;
long *n, *u, *e, *l, *a;
{
  /*






*/
  boolean done;
  long h = 0;
  /*
*/
  long s;
  helix *WITH;

  do {

    h++;
    if (h > maxhelix) {
      printf(" there are more helixes than can be handled.\n");
      printf(" (there are at least %ld of them.)  increase constant maxhelix.\n",
	     (long)maxhelix);
      halt();
    }
    WITH = &helixes[h-1];

    gethelix(hlist, &WITH->x, &WITH->y, &WITH->length, order.getenergy,
	     &WITH->energy, &done);
    if (done)
      h--;
    else {
      ordering[h-1] = h;


      switch (order.doinglength) {

      case true:
	if (helixes[h-1].length < order.minlenmaxene)
	  h--;
	break;

      case false:
	if (helixes[h-1].energy > order.minlenmaxene)
	  h--;
	break;
      }
    }
  } while (!done);
  if (h == 0)
    sorton = 'n';
  else if (h == 1) {
    WITH = &helixes[ordering[h-1] - 1];
    hrecord(shlist, WITH->x, WITH->y, WITH->length, order.getenergy,
	    WITH->energy);
    sorton = 'u';
  } else {
    sorton = order.priority[0];
    quicksort(1, (int)h);
    if (!lessthan((int)(h - 1), (int)h) && order.top == 1)
      sorton = 'a';
    else {
      s = h;
      while (s > 0 && s > h - order.top) {
	WITH = &helixes[ordering[s-1] - 1];
	hrecord(shlist, WITH->x, WITH->y, WITH->length, order.getenergy,
		WITH->energy);
	s--;
      }
    }
  }

  /*
*/
  fprintf(list.f, " helixes: %3ld", h);
  if (sorton == 'a' || sorton == 'n')
    fprintf(list.f, "      %3c", ' ');
  else
    fprintf(list.f, " top: %3d", ordering[h-1]);
  fprintf(list.f, " by: %c", sorton);

  switch (sorton) {

  case 'n':
    (*n)++;
    break;

  case 'u':
    (*u)++;
    break;

  case 'e':
    (*e)++;
    break;

  case 'l':
    (*l)++;
    break;

  case 'a':
    (*a)++;
    break;
  }


  if (!BUFEOF(hlist->f)) {
    putc(' ', shlist->f);
    copyaline(hlist, shlist);
  }
}



Static Void themain(hlist, shlist, list, sorthp)
_TEXT *hlist, *shlist, *list, *sorthp;
{
  /*
*/
  operations order;
  /*
*/
  long n = 0, u = 0, e = 0, l = 0, a = 0, sets = 0;
  boolean endofline;

  printf(" sorth %4.2f\n", version);

  hcopy(hlist, shlist, list);
  getparams(hlist, sorthp, &order, list);


  while (!BUFEOF(hlist->f)) {
    sets++;
    endofline = ((sets & 1) == 1);
    if (endofline)
      putc('\n', list->f);
    fprintf(list->f, " set: %3ld", sets);

    doset(hlist, shlist, order, &n, &u, &e, &l, &a);

    if (endofline)
      putc('.', list->f);
  }

  fprintf(list->f,
	  "\n\n classification of sets in hlist by number of helixes:\n");
  fprintf(list->f, " none:      %4ld no helixes\n", n);
  fprintf(list->f, " unique:    %4ld one helix\n", u);
  fprintf(list->f, " energy:    %4ld sorted by energy first\n", e);
  fprintf(list->f, " length:    %4ld sorted by length first\n", l);
  fprintf(list->f,
    " ambiguous: %4ld sorted, with no single unambiguous strongest helix\n",
    a);
  fprintf(list->f, " total:     %4ld\n", n + u + e + l + a);

  printf(" %ld sets of helixes sorted\n", sets);

  if (n + u + e + l + a != sets) {
    printf("sorth.themain, program error\n");
    halt();
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  sorthp.f = NULL;
  strcpy(sorthp.name, "sorthp");
  list.f = NULL;
  strcpy(list.name, "list");
  shlist.f = NULL;
  strcpy(shlist.name, "shlist");
  hlist.f = NULL;
  strcpy(hlist.name, "hlist");
  themain(&hlist, &shlist, &list, &sorthp);
_L1:
  if (hlist.f != NULL)
    fclose(hlist.f);
  if (shlist.f != NULL)
    fclose(shlist.f);
  if (list.f != NULL)
    fclose(list.f);
  if (sorthp.f != NULL)
    fclose(sorthp.f);
  exit(EXIT_SUCCESS);
}



/* End. */
