/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "sepa.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         2.18
/*








*/



/*



















































*/


/*












































*/



#define verbose         false





#define dnamax          3000
#define namelength      20
#define linelength      80




typedef Char alpha[namelength];




typedef struct orgspec {
  alpha nam;
  struct chrspec *chr;
  struct orgspec *next;
} orgspec;

typedef struct chrspec {
  alpha nam;
  struct piespec *pie;
  struct chrspec *next;
} chrspec;

typedef struct piespec {
  alpha nam;
  boolean isinstname;
  alpha instname;
  struct getreq *get;
  struct piespec *next;
} piespec;

typedef struct getreq {
  long fr, po, tr;
  boolean dp, givedirection;
  /*
*/
  struct getreq *next;
} getreq;


Static _TEXT presites, mixture, sites, nonsites;


Static orgspec *sitetree, *nonsitetree, *none;

Static long totalgets;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}





Static Void startname(nam)
Char *nam;
{
  long index;

  for (index = 0; index < namelength; index++)
    nam[index] = ' ';
}


Static Void startorg(org)
orgspec **org;
{
  orgspec *WITH;

  *org = (orgspec *)Malloc(sizeof(orgspec));
  WITH = *org;
  startname(WITH->nam);
  WITH->chr = NULL;
  WITH->next = NULL;
}


Static Void startchr(chr)
chrspec **chr;
{
  chrspec *WITH;

  *chr = (chrspec *)Malloc(sizeof(chrspec));
  WITH = *chr;
  startname(WITH->nam);
  WITH->pie = NULL;
  WITH->next = NULL;
}


Static Void startpie(pie)
piespec **pie;
{
  piespec *WITH;

  *pie = (piespec *)Malloc(sizeof(piespec));
  WITH = *pie;
  startname(WITH->nam);
  WITH->get = NULL;
  WITH->next = NULL;
}


Static Void startget(get)
getreq **get;
{
  getreq *WITH;

  *get = (getreq *)Malloc(sizeof(getreq));
  WITH = *get;
  WITH->fr = 0;
  WITH->po = 0;
  WITH->tr = 0;
  WITH->dp = true;
  WITH->givedirection = false;
  WITH->next = NULL;
}


Static Void startset(pre, post, org)
_TEXT *pre, *post;
orgspec **org;
{
  if (*pre->name != '\0') {
    if (pre->f != NULL)
      pre->f = freopen(pre->name, "r", pre->f);
    else
      pre->f = fopen(pre->name, "r");
  } else
    rewind(pre->f);
  if (pre->f == NULL)
    _EscIO2(FileNotFound, pre->name);
  RESETBUF(pre->f, Char);
  if (*post->name != '\0') {
    if (post->f != NULL)
      post->f = freopen(post->name, "w", post->f);
    else
      post->f = fopen(post->name, "w");
  } else {
    if (post->f != NULL)
      rewind(post->f);
    else
      post->f = tmpfile();
  }
  if (post->f == NULL)
    _EscIO2(FileNotFound, post->name);
  SETUPBUF(post->f, Char);
  *org = NULL;
}






Static Void findblank(afile)
_TEXT *afile;
{
  Char ch;

  do {
    ch = getc(afile->f);
    if (ch == '\n')
      ch = ' ';
  } while (ch != ' ');
}



Static Void findnonblank(afile, ch)
_TEXT *afile;
Char *ch;
{
  *ch = ' ';
  while (!BUFEOF(afile->f) && *ch == ' ') {
    *ch = getc(afile->f);
    if (*ch == '\n')
      *ch = ' ';
    if (P_eoln(afile->f)) {
      fscanf(afile->f, "%*[^\n]");
      getc(afile->f);
    }
  }
}



Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) == ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((P_peek(thefile->f) != ' ') & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}


Static Void readname(afile, nam)
_TEXT *afile;
Char *nam;
{
  long index = 0;
  Char ch = ' ';

  startname(nam);
  while (ch != ';') {
    ch = getc(afile->f);

    if (ch == '\n')
      ch = ' ';
    if (ch == ';')
      break;
    index++;
    nam[index-1] = ch;
  }

}


Static Void readorg(afile, org)
_TEXT *afile;
orgspec **org;
{
  orgspec *WITH;

  if (*org == NULL)
    startorg(org);

  findblank(afile);
  WITH = *org;
  readname(afile, WITH->nam);
  WITH->chr = NULL;
  WITH->next = NULL;
}


Static Void readchr(afile, chr)
_TEXT *afile;
chrspec **chr;
{
  chrspec *WITH;

  if (*chr == NULL)
    startchr(chr);

  findblank(afile);
  WITH = *chr;
  readname(afile, WITH->nam);
  WITH->pie = NULL;
  WITH->next = NULL;
}


Static Void readpie(afile, pie)
_TEXT *afile;
piespec **pie;
{
  piespec *WITH;

  if (*pie == NULL)
    startpie(pie);

  findblank(afile);
  WITH = *pie;
  readname(afile, WITH->nam);
  WITH->get = NULL;
  WITH->next = NULL;
}


Static Void readget(afile, get)
_TEXT *afile;
getreq **get;
{
  Char ch;
  getreq *WITH;

  if (*get == NULL)
    startget(get);

  findblank(afile);
  findnonblank(afile, &ch);
  findblank(afile);

  WITH = *get;
  fscanf(afile->f, "%ld", &WITH->po);
  fscanf(afile->f, "%ld", &WITH->fr);

  findnonblank(afile, &ch);
  findblank(afile);

  skipblanks(afile);
  if (P_peek(afile->f) == 's')
    skipnonblanks(afile);
  else
    fscanf(afile->f, "%ld", &WITH->po);

  fscanf(afile->f, "%ld", &WITH->tr);

  findnonblank(afile, &ch);
  if (ch == 'd') {
    WITH->givedirection = true;
    findblank(afile);
    findnonblank(afile, &ch);
    if (ch == '-')
      WITH->dp = false;
    else
      WITH->dp = true;
  } else
    WITH->givedirection = false;
  while (ch != ';') {
    ch = getc(afile->f);
    if (ch == '\n')
      ch = ' ';
  }
}


/*
*/

Static Void moveorg(org, ref, cur)
orgspec **org, **ref, **cur;
{
  /*
*/
  orgspec *index;
  boolean found = false;

  index = *ref;
  while (!found && index != NULL) {
    found = (strncmp((*org)->nam, index->nam, sizeof(alpha)) == 0);
    if (!found)
      index = index->next;
  }

  if (found)
    *cur = index;
  else
    *cur = NULL;
}


Static Void movechr(chr, ref, cur)
chrspec **chr, **ref, **cur;
{
  /*
*/
  chrspec *index;
  boolean found = false;

  index = *ref;
  while (!found && index != NULL) {
    found = (strncmp((*chr)->nam, index->nam, sizeof(alpha)) == 0);
    if (!found)
      index = index->next;
  }

  if (found)
    *cur = index;
  else
    *cur = NULL;
}


Static Void movepie(pie, ref, cur)
piespec **pie, **ref, **cur;
{
  /*
*/
  piespec *index;
  boolean found = false;

  index = *ref;
  while (!found && index != NULL) {
    found = (strncmp((*pie)->nam, index->nam, sizeof(alpha)) == 0);
    if (!found)
      index = index->next;
  }

  if (found)
    *cur = index;
  else
    *cur = NULL;
}


Static boolean findget(get, ref)
getreq **get, **ref;
{
  /*
*/
  getreq *index;
  boolean found = false;

  index = *ref;

  while (!found && index != NULL) {
    found = ((*get)->po == index->po &&
	     (((*get)->dp == index->dp && (*get)->givedirection &&
	       index->givedirection) ||
	      !(*get)->givedirection && !index->givedirection));
    /*
*/

    if (!found)
      index = index->next;
  }

  return found;
}


Static Void putorg(org, ref, cur)
orgspec **org, **ref, **cur;
{
  /*

*/
  orgspec *preindex = NULL;
  orgspec *index;
  boolean found = false;

  if (*ref == NULL) {
    *ref = *org;
    *cur = *org;
    *org = NULL;
    return;
  }
  index = *ref;
  while (!found && index != NULL) {
    found = (strncmp((*org)->nam, index->nam, sizeof(alpha)) == 0);
    preindex = index;
    index = index->next;
  }
  if (found) {
    *cur = preindex;
    return;
  }
  preindex->next = *org;
  *cur = *org;
  *org = NULL;
}


Static Void putchr(chr, ref, cur)
chrspec **chr, **ref, **cur;
{
  /*

*/
  chrspec *preindex = NULL;
  chrspec *index;
  boolean found = false;

  if (*ref == NULL) {
    *ref = *chr;
    *cur = *chr;
    *chr = NULL;
    return;
  }
  index = *ref;
  while (!found && index != NULL) {
    found = (strncmp((*chr)->nam, index->nam, sizeof(alpha)) == 0);
    preindex = index;
    index = index->next;
  }
  if (found) {
    *cur = preindex;
    return;
  }
  preindex->next = *chr;
  *cur = *chr;
  *chr = NULL;
}


Static Void putpie(pie, ref, cur)
piespec **pie, **ref, **cur;
{
  /*

*/
  piespec *preindex = NULL;
  piespec *index;
  boolean found = false;

  if (*ref == NULL) {
    *ref = *pie;
    *cur = *pie;
    *pie = NULL;
    return;
  }
  index = *ref;
  while (!found && index != NULL) {
    found = (strncmp((*pie)->nam, index->nam, sizeof(alpha)) == 0);
    preindex = index;
    index = index->next;
  }
  if (found) {
    *cur = preindex;
    return;
  }
  preindex->next = *pie;
  *cur = *pie;
  *pie = NULL;
}


Static Void putget(get, gbui)
getreq **get, **gbui;
{
  getreq *preindex = NULL;
  getreq *index;
  boolean equal = false, after = false;

  index = *gbui;

  while (index != NULL && !equal && !after) {
    equal = (index->po == (*get)->po);
    after = (index->po > (*get)->po);
    if (after)
      break;
    preindex = index;
    index = index->next;
  }

  if (equal)
    return;
  if (index == NULL) {
    if (preindex == NULL)
      *gbui = *get;
    else
      preindex->next = *get;
    *get = NULL;
    return;
  }
  if (!after)
    return;
  if (preindex == NULL) {
    (*get)->next = index;
    *gbui = *get;
  } else {
    preindex->next = *get;
    (*get)->next = index;
  }
  *get = NULL;
}




Static Void iwritename(thefile, thename)
_TEXT *thefile;
Char *thename;
{
  long index = 1;

  while (thename[index-1] != ' ' && index <= namelength) {
    putc(thename[index-1], thefile->f);
    index++;
  }
}


Static Void iwriteorg(afile, org)
_TEXT *afile;
orgspec *org;
{
  fprintf(afile->f, "organism ");
  iwritename(afile, org->nam);
  fprintf(afile->f, ";\n");
}


Static Void iwritechr(afile, chr)
_TEXT *afile;
chrspec *chr;
{
  fprintf(afile->f, "chromosome ");
  iwritename(afile, chr->nam);
  fprintf(afile->f, ";\n");
}


Static Void iwritepie(afile, pie)
_TEXT *afile;
piespec *pie;
{
  fprintf(afile->f, "piece ");
  iwritename(afile, pie->nam);
  fprintf(afile->f, ";\n");
}


/* Local variables for iwriteget: */
struct LOC_iwriteget {
  _TEXT *afile;
  getreq *g;
} ;

Local Void iwriteplace(relative, LINK)
long relative;
struct LOC_iwriteget *LINK;
{
  fprintf(LINK->afile->f, " %ld", LINK->g->po);
  if (relative >= 0)
    fprintf(LINK->afile->f, " +%ld", relative);
  else if (relative < 0)
    fprintf(LINK->afile->f, " %ld", relative);
}


Static Void iwriteget(afile_, g_)
_TEXT *afile_;
getreq *g_;
{
  struct LOC_iwriteget V;

  V.afile = afile_;
  V.g = g_;
  fprintf(V.afile->f, "get from");
  iwriteplace(V.g->fr, &V);
  fprintf(V.afile->f, " to");
  iwriteplace(V.g->tr, &V);
  if (V.g->givedirection) {
    fprintf(V.afile->f, " direction ");
    if (V.g->dp)
      putc('+', V.afile->f);
    else
      putc('-', V.afile->f);
  }
  fprintf(V.afile->f, ";\n");

  totalgets++;
}


/*




*/

Static Void printpie(afile, p)
_TEXT *afile;
piespec *p;
{
  getreq *index;

  index = p->get;
  if (index == NULL)
    return;
  iwritepie(afile, p);
  while (index != NULL) {
    if (p->isinstname) {
      fprintf(afile->f, "name ");
      iwritename(afile, p->instname);
      fprintf(afile->f, "; ");
    }
    iwriteget(afile, index);
    index = index->next;
  }
}


Static Void printchr(afile, c)
_TEXT *afile;
chrspec *c;
{
  piespec *index;

  iwritechr(afile, c);
  index = c->pie;
  while (index != NULL) {
    printpie(afile, index);
    index = index->next;
  }
}


Static Void printorg(afile, o)
_TEXT *afile;
orgspec *o;
{
  chrspec *index;

  iwriteorg(afile, o);
  index = o->chr;
  while (index != NULL) {
    printchr(afile, index);
    index = index->next;
  }
}


Static Void printinst(afile, root)
_TEXT *afile;
orgspec *root;
{
  orgspec *index = root;

  while (index != NULL) {
    printorg(afile, index);
    index = index->next;
  }

}


Static Void readinst(pre, post, ref, bui)
_TEXT *pre, *post;
orgspec **ref, **bui;
{
  /*

*/
  /*


*/

  orgspec *oref = NULL, *o = NULL, *obui = NULL;
  chrspec *cref = NULL, *c = NULL, *cbui = NULL;
  piespec *pref = NULL, *p = NULL, *pbui = NULL;
  getreq *g = NULL;

  Char ch;

  boolean isname = false;
  alpha n;

  /*






*/
  Char previous;
  boolean done;

  while (!BUFEOF(pre->f)) {
    if (P_eoln(pre->f)) {
      fscanf(pre->f, "%*[^\n]");

      getc(pre->f);
      if (verbose)
	putchar('\n');
      continue;
    }
    findnonblank(pre, &ch);

    if (P_eoln(pre->f))
      continue;
    if (ch == 'g' || ch == 'p' || ch == 'c' || ch == 'o' || ch == 'n') {
      switch (ch) {

      case 'o':
	if (verbose)
	  printf("[org found]\n");
	readorg(pre, &o);
	moveorg(&o, ref, &oref);
	putorg(&o, bui, &obui);
	break;

      case 'c':
	if (verbose)
	  printf("[chr found]\n");
	readchr(pre, &c);


	if (oref != NULL)
	  movechr(&c, &oref->chr, &cref);

	if (obui == NULL) {
	  printf("chr instruction found but there is no org instruction!\n");
	  halt();
	}

	putchr(&c, &obui->chr, &cbui);
	break;

      case 'n':
	if (verbose)
	  printf("[name found]\n");
	isname = true;

	findblank(pre);
	findnonblank(pre, &ch);
	readname(pre, n);
	break;

      case 'p':
	if (verbose)
	  printf("[piece found]\n");
	readpie(pre, &p);


	if (isname) {
	  memcpy(p->instname, n, sizeof(alpha));
	  p->isinstname = true;
	  isname = false;
	}

	else
	  p->isinstname = false;

	if (cref != NULL)
	  movepie(&p, &cref->pie, &pref);
	putpie(&p, &cbui->pie, &pbui);
	break;

      case 'g':
	if (verbose)
	  printf("[get found]\n");
	readget(pre, &g);
	if (pref == NULL)
	  putget(&g, &pbui->get);
	else if (!findget(&g, &pref->get))
	  putget(&g, &pbui->get);
	break;
      }
      continue;
    }
    if (ch == '{') {
      putc(ch, post->f);
      done = false;
      while (!done) {
	if (P_eoln(pre->f)) {
	  fscanf(pre->f, "%*[^\n]");
	  getc(pre->f);
	  putc('\n', post->f);
	  continue;
	}
	ch = getc(pre->f);
	if (ch == '\n')
	  ch = ' ';
	putc(ch, post->f);
	if (ch == '}')
	  done = true;
      }

      if (P_eoln(pre->f)) {
	fscanf(pre->f, "%*[^\n]");
	getc(pre->f);
	putc('\n', post->f);
      }
    }


    else if (ch == '(') {

      putc(ch, post->f);
      done = false;
      while (!done) {
	if (P_eoln(pre->f)) {
	  fscanf(pre->f, "%*[^\n]");
	  getc(pre->f);
	  putc('\n', post->f);
	  continue;
	}
	previous = ch;
	ch = getc(pre->f);
	if (ch == '\n')
	  ch = ' ';
	putc(ch, post->f);
	if (previous == '*' && ch == ')')
	  done = true;
      }


      if (P_eoln(pre->f)) {
	fscanf(pre->f, "%*[^\n]");
	getc(pre->f);
	putc('\n', post->f);
      }
    }

    else {
      putc(ch, post->f);
      done = false;
      while (!done) {
	if (P_eoln(pre->f)) {
	  fscanf(pre->f, "%*[^\n]");
	  getc(pre->f);
	  putc('\n', post->f);
	  continue;
	}
	ch = getc(pre->f);
	if (ch == '\n')
	  ch = ' ';
	putc(ch, post->f);
	if (ch == ';')
	  done = true;
      }
    }



    if (ch == ';') {
      putc('\n', post->f);

    }
    /*






*/
  }
}


Static Void writeversion(afile)
_TEXT *afile;
{
  fprintf(afile->f, "(* sepa %4.2f *)\n", version);
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  nonsites.f = NULL;
  strcpy(nonsites.name, "nonsites");
  sites.f = NULL;
  strcpy(sites.name, "sites");
  mixture.f = NULL;
  strcpy(mixture.name, "mixture");
  presites.f = NULL;
  strcpy(presites.name, "presites");
  printf("sepa %4.2f\n", version);
  totalgets = 0;
  none = NULL;

  startset(&presites, &sites, &sitetree);
  readinst(&presites, &sites, &none, &sitetree);
  if (verbose)
    printf("done first readinst\n");
  fprintf(sites.f, "(* sites *)\n");
  writeversion(&sites);
  printinst(&sites, sitetree);

  printf(" gets written in sites: %ld\n", totalgets);
  fprintf(sites.f, "\n(* gets written in sites: %ld *)\n", totalgets);
  totalgets = 0;

  startset(&mixture, &nonsites, &nonsitetree);
  readinst(&mixture, &nonsites, &sitetree, &nonsitetree);
  if (verbose)
    printf("done second readinst\n");
  fprintf(nonsites.f, "(* nonsites *)\n");
  writeversion(&nonsites);
  printinst(&nonsites, nonsitetree);

  fprintf(nonsites.f, "\n(* gets written in nonsites: %ld *)\n", totalgets);
  printf(" gets written in nonsites: %ld\n", totalgets);
_L1:
  if (presites.f != NULL)
    fclose(presites.f);
  if (mixture.f != NULL)
    fclose(mixture.f);
  if (sites.f != NULL)
    fclose(sites.f);
  if (nonsites.f != NULL)
    fclose(nonsites.f);
  exit(EXIT_SUCCESS);
}



/* End. */
