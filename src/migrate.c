/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "migrate.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*


*/
#define updateversion   1.00



/*




































































































































































































































































































































*/



#define worldsize       100



typedef struct world {
  /*
*/
  long population[worldsize + worldsize + 1];
} world;


Static _TEXT migratep;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


/* Local variables for numberdigit: */
struct LOC_numberdigit {
  long number, place, absolute;
  Char acharacter;
} ;

Local Void digit(LINK)
struct LOC_numberdigit *LINK;
{
  long tenplace, z, d;

  tenplace = LINK->place * 10;
  z = LINK->absolute - LINK->absolute / tenplace * tenplace;
  if (LINK->place == 1)
    d = z;
  else
    d = z / LINK->place;
  switch (d) {

  case 0:
    LINK->acharacter = '0';
    break;

  case 1:
    LINK->acharacter = '1';
    break;

  case 2:
    LINK->acharacter = '2';
    break;

  case 3:
    LINK->acharacter = '3';
    break;

  case 4:
    LINK->acharacter = '4';
    break;

  case 5:
    LINK->acharacter = '5';
    break;

  case 6:
    LINK->acharacter = '6';
    break;

  case 7:
    LINK->acharacter = '7';
    break;

  case 8:
    LINK->acharacter = '8';
    break;

  case 9:
    LINK->acharacter = '9';
    break;
  }
}

Local Void sign(LINK)
struct LOC_numberdigit *LINK;
{
  if (LINK->number < 0)
    LINK->acharacter = '-';
  else
    LINK->acharacter = '+';
}





Static Char numberdigit(number_, logplace)
long number_, logplace;
{
  /*



*/
  struct LOC_numberdigit V;
  long count;

  V.number = number_;
  V.place = 1;
  for (count = 1; count <= logplace; count++)
    V.place *= 10;

  if (V.number == 0) {
    if (V.place == 1)
      V.acharacter = '0';
    else
      V.acharacter = ' ';
    return V.acharacter;
  }
  V.absolute = labs(V.number);
  if (V.absolute < V.place / 10) {
    V.acharacter = ' ';
    return V.acharacter;
  }
  if (V.absolute >= V.place)
    digit(&V);
  else
    sign(&V);
  return V.acharacter;
}


#define ln10            2.30259
#define epsilon         0.00001



Static long numbersize(n)
long n;
{
  if (n == 0)
    return 1;
  else {
    return ((long)(log((double)labs(n)) / ln10 + epsilon) + 2);
    /*


*/

  }
}

#undef ln10
#undef epsilon



Static Void numberbar(afile, spaces, firstnumber, lastnumber, linesused)
_TEXT *afile;
long spaces, firstnumber, lastnumber, *linesused;
{
  /*
*/
  long logplace, spacecount, number;

  if (labs(firstnumber) > labs(lastnumber))
    *linesused = numbersize(firstnumber);
  else
    *linesused = numbersize(lastnumber);

  for (logplace = *linesused - 1; logplace >= 0; logplace--) {
    for (spacecount = 1; spacecount <= spaces; spacecount++)
      putc(' ', afile->f);
    for (number = firstnumber; number <= lastnumber; number++)
      fputc(numberdigit(number, logplace), afile->f);
    putc('\n', afile->f);
  }
}




Static Void clearworld(w)
world **w;
{
  long i;

  for (i = -worldsize; i <= worldsize; i++)
    (*w)->population[i + worldsize] = 0;
}


Static Void startworld(w, initpop)
world **w;
long initpop;
{
  *w = (world *)Malloc(sizeof(world));
  clearworld(w);
  (*w)->population[worldsize] = initpop;
}


Static Void switchworlds(a, b)
world **a, **b;
{
  world *h;

  h = *a;
  *a = *b;
  *b = h;
}


Static Void showworld(f, w, worldrange, displaymax, pagejump, generation,
		      touchhalt)
_TEXT *f;
world **w;
long worldrange, displaymax;
Char pagejump;
long generation, touchhalt;
{
  long i;
  long mpd = 0;
  long location, mindex, linesused;
  boolean touched = false;
  world *WITH;

  WITH = *w;
  for (i = -worldsize; i <= worldsize; i++) {
    if (mpd < WITH->population[i + worldsize]) {
      mpd = WITH->population[i + worldsize];
      location = i;
    }
    if (i == touchhalt) {
      if (WITH->population[i + worldsize] > 0)
	touched = true;
    }
  }

  if (mpd == 0) {
    printf("population died out\n");
    halt();
  }

  if (pagejump == 'l')
    fprintf(f->f, "\f\n");
  else
    putc('\n', f->f);
  printf("maximum population density = %ld is at location = %ld\n",
	 mpd, location);
  numberbar(f, 1L, -worldrange, worldrange, &linesused);

  for (mindex = displaymax; mindex >= 0; mindex--) {
    putc('|', f->f);
    for (i = -worldrange; i <= worldrange; i++) {
      if ((double)WITH->population[i + worldsize] / mpd <
	  (double)mindex / displaymax)
	putc('.', f->f);
      else
	putc('*', f->f);
    }
    fprintf(f->f, "| %3ld\n", mindex);
  }
  fprintf(f->f, "generation %ld\n", generation);

  if (touched) {
    fprintf(f->f, "the population touched %ld\n", touchhalt);
    halt();
  }
}


#define pow18           262144L
#define pow19           524288L
#define pow30           1073741824L

#define pow31m          2147483647.0


Static Void randomDelila(randDelila, iseed)
double *randDelila;
long *iseed;
{
  /*











*/
  long i, nrep;

  nrep = ((*iseed) & 7) + 4;
  for (i = 1; i <= nrep; i++) {
    if (((*iseed) & 1) == (((*iseed) & (pow19 - 1)) >= pow18))
      *iseed /= 2;
    else
      *iseed = *iseed / 2 + pow30;
  }

  *randDelila = *iseed / pow31m;
}

#undef pow18
#undef pow19
#undef pow30
#undef pow31m


Static long chose(survival, iseed)
double survival;
long *iseed;
{
  /*

*/
  double randDelila;

  randomDelila(&randDelila, iseed);
  if (randDelila < survival)
    return 1;
  else
    return 0;
}


Static Void stepselect(a, b, nsurvival, psurvival, iseed)
world **a, **b;
double nsurvival, psurvival;
long *iseed;
{
  /*


*/
  long i, c, FORLIM1;

  clearworld(b);
  for (i = 1 - worldsize; i < worldsize; i++) {
    FORLIM1 = (*a)->population[i + worldsize];
    for (c = 1; c <= FORLIM1; c++) {
      (*b)->population[i + worldsize - 1] += chose(nsurvival, iseed);
      (*b)->population[i + worldsize + 1] += chose(psurvival, iseed);
    }
  }
}


Static Void sleep(delaycount)
long delaycount;
{
  long j, k;

  for (j = 1; j <= delaycount; j++)
    k++;
}


/* Local variables for themain: */
struct LOC_themain {
  _TEXT *migratep;
  long delaycount;
  /*
*/
  long displaymax, initpop, iseed;
  double parameterversion;
  long maxgens;
  Char pagejump;
  double psurvival, nsurvival;
  /*
*/
  long touchhalt, worldrange;
} ;

Local Void readparameters(LINK)
struct LOC_themain *LINK;
{
  if (*LINK->migratep->name != '\0') {
    if (LINK->migratep->f != NULL)
      LINK->migratep->f = freopen(LINK->migratep->name, "r", LINK->migratep->f);
    else
      LINK->migratep->f = fopen(LINK->migratep->name, "r");
  } else
    rewind(LINK->migratep->f);
  if (LINK->migratep->f == NULL)
    _EscIO2(FileNotFound, LINK->migratep->name);
  RESETBUF(LINK->migratep->f, Char);
  fscanf(LINK->migratep->f, "%lg%*[^\n]", &LINK->parameterversion);
  getc(LINK->migratep->f);
  if (LINK->parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }

  fscanf(LINK->migratep->f, "%ld%*[^\n]", &LINK->worldrange);
  getc(LINK->migratep->f);
  fscanf(LINK->migratep->f, "%ld%*[^\n]", &LINK->displaymax);
  getc(LINK->migratep->f);
  fscanf(LINK->migratep->f, "%c%*[^\n]", &LINK->pagejump);
  getc(LINK->migratep->f);
  if (LINK->pagejump == '\n')
    LINK->pagejump = ' ';
  fscanf(LINK->migratep->f, "%ld%*[^\n]", &LINK->maxgens);
  getc(LINK->migratep->f);
  fscanf(LINK->migratep->f, "%ld%*[^\n]", &LINK->initpop);
  getc(LINK->migratep->f);
  fscanf(LINK->migratep->f, "%lg%lg%*[^\n]", &LINK->nsurvival,
	 &LINK->psurvival);
  getc(LINK->migratep->f);
  fscanf(LINK->migratep->f, "%ld%*[^\n]", &LINK->iseed);
  getc(LINK->migratep->f);
  fscanf(LINK->migratep->f, "%ld%*[^\n]", &LINK->delaycount);
  getc(LINK->migratep->f);
  fscanf(LINK->migratep->f, "%ld%*[^\n]", &LINK->touchhalt);
  getc(LINK->migratep->f);

  if (LINK->worldrange <= 0) {
    printf("worldrange must be positive\n");
    halt();
  }

  if (LINK->worldrange >= worldsize) {
    printf("worldrange must be less than worldsize =%ld\n", (long)worldsize);
    halt();
  }

  if (LINK->displaymax <= 0) {
    printf("displaymax must be positive\n");
    halt();
  }

  if (LINK->maxgens <= 0) {
    printf("maxgens must be positive\n");
    halt();
  }

  if (LINK->initpop <= 0) {
    printf("initpop must be positive\n");
    halt();
  }

  if ((unsigned)LINK->nsurvival > 1.0 || (unsigned)LINK->psurvival > 1.0) {
    printf("selection must be between 0 and 1\n");
    halt();
  }

  if (LINK->iseed < 1) {
    printf("iseed must be positive\n");
    halt();
  }



}


Static Void themain(migratep_)
_TEXT *migratep_;
{
  struct LOC_themain V;
  world *c, *p;
  long generation, FORLIM;
  _TEXT TEMP;

  V.migratep = migratep_;
  printf("migrate %4.2f\n", version);
  readparameters(&V);
  startworld(&p, 0L);
  startworld(&c, V.initpop);

  FORLIM = V.maxgens;
  for (generation = 1; generation <= FORLIM; generation++) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    showworld(&TEMP, &c, V.worldrange, V.displaymax, V.pagejump, generation,
	      V.touchhalt);
    switchworlds(&c, &p);
    stepselect(&p, &c, V.nsurvival, V.psurvival, &V.iseed);
    sleep(V.delaycount);
  }

  printf("done\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  migratep.f = NULL;
  strcpy(migratep.name, "migratep");
  themain(&migratep);
_L1:
  if (migratep.f != NULL)
    fclose(migratep.f);
  exit(EXIT_SUCCESS);
}



/* End. */
