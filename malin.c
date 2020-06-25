/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "malin.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.14
/*








*/
#define updateversion   1.07



/*



































































*/



#define maxstring       1500


#define fillermax       10


typedef struct stringDelila {
  Char letters[maxstring];
  long length, current;
} stringDelila;



/*


*/
typedef Char filler[fillermax];



typedef struct trigger {
  stringDelila seek;
  long state;
  boolean skip, found;
} trigger;


Static _TEXT optinst, optalign, inst, malinp, cinst, distribution;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void clearstring(ribbon)
stringDelila *ribbon;
{
  long index;

  for (index = 0; index < maxstring; index++)
    ribbon->letters[index] = ' ';
  ribbon->length = 0;
  ribbon->current = 0;
}



Static Void getstring(afile, buffer, gotten)
_TEXT *afile;
stringDelila *buffer;
boolean *gotten;
{
  /*
*/
  long index = 0;

  clearstring(buffer);
  if (BUFEOF(afile->f)) {
    *gotten = false;
    return;
  }
  while (!P_eoln(afile->f) && index < maxstring) {
    index++;
    buffer->letters[index-1] = getc(afile->f);
    if (buffer->letters[index-1] == '\n')
      buffer->letters[index-1] = ' ';
  }

  if (!P_eoln(afile->f)) {
    printf(" getstring: a line exceeds maximum string size (%ld)\n",
	   (long)maxstring);
    halt();
  }

  buffer->length = index;
  buffer->current = 1;
  fscanf(afile->f, "%*[^\n]");
  getc(afile->f);
  *gotten = true;
}



Static Void writestring(tofile, s)
_TEXT *tofile;
stringDelila *s;
{
  long i, FORLIM;

  FORLIM = s->length;
  for (i = 0; i < FORLIM; i++)
    putc(s->letters[i], tofile->f);
}



Static Void fillstring(s, a)
stringDelila *s;
Char *a;
{
  /*
*/


  /*


*/
  long length = fillermax;
  long index;

  clearstring(s);
  while (length > 1 && a[length-1] == ' ')
    length--;
  if (length == 1 && a[length-1] == ' ') {
    printf("fillstring: the string is empty\n");
    halt();
  }

  for (index = 0; index < length; index++)
    s->letters[index] = a[index];
  s->length = length;
  s->current = 1;
}



Static Void filltrigger(t, a)
trigger *t;
Char *a;
{
  fillstring(&t->seek, a);
}



/*




*/

Static Void resettrigger(t)
trigger *t;
{
  t->state = 0;
  t->skip = false;
  t->found = false;
}


Static Void testfortrigger(ch, t)
Char ch;
trigger *t;
{
  /*




*/
  t->state++;
  /*



*/
  if (t->seek.letters[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->seek.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
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


Local Char sign(i)
long i;
{
  if (i >= 0)
    return '+';
  else
    return '-';
}



Static Void themain(optinst, optalign, inst, malinp, cinst, distribution)
_TEXT *optinst, *optalign, *inst, *malinp, *cinst, *distribution;
{
  /*
*/
  long a = 1;
  long alignments, analignment;
  Char c;
  long class_;
  boolean debugging = false;
  long fromvalue, fromrange;
  /*
*/
  double H;
  long occurences;
  double parameterversion;
  long s, sequences, shift;
  long state = 0;
  /*





























*/

  boolean shutup = false;

  trigger t0a, t0b, t1a, t2a, t3a, t0c, t0d, t0e, t5a;
  long tovalue, torange;
  /*
*/
  long theclass, zerobase;

  printf("malin%5.2f\n", version);

  filltrigger(&t0a, "(*        ");
  filltrigger(&t0b, "get       ");
  filltrigger(&t0c, "{         ");
  filltrigger(&t0d, "\"         ");
  filltrigger(&t0e, "'         ");
  filltrigger(&t1a, "*)        ");
  filltrigger(&t2a, "from      ");
  filltrigger(&t3a, "to        ");
  filltrigger(&t5a, "}         ");


  if (*malinp->name != '\0') {
    if (malinp->f != NULL)
      malinp->f = freopen(malinp->name, "r", malinp->f);
    else
      malinp->f = fopen(malinp->name, "r");
  } else
    rewind(malinp->f);
  if (malinp->f == NULL)
    _EscIO2(FileNotFound, malinp->name);
  RESETBUF(malinp->f, Char);
  fscanf(malinp->f, "%lg%*[^\n]", &parameterversion);
  getc(malinp->f);
  if (parameterversion < updateversion) {
    printf("You have an old parameter file!\n");
    halt();
  }
  fscanf(malinp->f, "%ld%*[^\n]", &analignment);
  getc(malinp->f);
  fscanf(malinp->f, "%ld%*[^\n]", &zerobase);
  getc(malinp->f);

  if (*optinst->name != '\0') {
    if (optinst->f != NULL)
      optinst->f = freopen(optinst->name, "r", optinst->f);
    else
      optinst->f = fopen(optinst->name, "r");
  } else
    rewind(optinst->f);
  if (optinst->f == NULL)
    _EscIO2(FileNotFound, optinst->name);
  RESETBUF(optinst->f, Char);
  fscanf(optinst->f, "%ld%ld%*[^\n]", &sequences, &alignments);
  getc(optinst->f);

  while (a < analignment) {
    if (BUFEOF(optinst->f)) {
      printf("alignment %ld does not exist\n", analignment);
      halt();
    }
    a++;
    fscanf(optinst->f, "%ld%lg%*[^\n]", &occurences, &H);
    getc(optinst->f);
    for (s = 1; s <= sequences; s++)
      fscanf(optinst->f, "%ld", &fromvalue);
    fscanf(optinst->f, "%*[^\n]");
    getc(optinst->f);
  }
  fscanf(optinst->f, "%ld%lg%*[^\n]", &occurences, &H);
  getc(optinst->f);

  if (*inst->name != '\0') {
    if (inst->f != NULL)
      inst->f = freopen(inst->name, "r", inst->f);
    else
      inst->f = fopen(inst->name, "r");
  } else
    rewind(inst->f);
  if (inst->f == NULL)
    _EscIO2(FileNotFound, inst->name);
  RESETBUF(inst->f, Char);
  if (*cinst->name != '\0') {
    if (cinst->f != NULL)
      cinst->f = freopen(cinst->name, "w", cinst->f);
    else
      cinst->f = fopen(cinst->name, "w");
  } else {
    if (cinst->f != NULL)
      rewind(cinst->f);
    else
      cinst->f = tmpfile();
  }
  if (cinst->f == NULL)
    _EscIO2(FileNotFound, cinst->name);
  SETUPBUF(cinst->f, Char);

  if (*distribution->name != '\0') {
    if (distribution->f != NULL)
      distribution->f = freopen(distribution->name, "w", distribution->f);
    else
      distribution->f = fopen(distribution->name, "w");
  } else {
    if (distribution->f != NULL)
      rewind(distribution->f);
    else
      distribution->f = tmpfile();
  }
  if (distribution->f == NULL)
    _EscIO2(FileNotFound, distribution->name);
  SETUPBUF(distribution->f, Char);
  fprintf(distribution->f, "* malin %5.2f alignment distribution\n", version);
  fprintf(distribution->f, "* alignment class: %ld\n", analignment);

  if (*optalign->name != '\0') {
    if (optalign->f != NULL)
      optalign->f = freopen(optalign->name, "r", optalign->f);
    else
      optalign->f = fopen(optalign->name, "r");
  } else
    rewind(optalign->f);
  if (optalign->f == NULL)
    _EscIO2(FileNotFound, optalign->name);
  RESETBUF(optalign->f, Char);

  while (P_peek(optalign->f) == '*') {
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
  }
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  fscanf(optalign->f, "%*[^\n]");
  getc(optalign->f);
  fscanf(optalign->f, "%*[^\n]");

  getc(optalign->f);
  for (class_ = 1; class_ <= analignment; class_++) {
    if (BUFEOF(optalign->f)) {
      printf("premature end of optalign\n");
      halt();
    }
    fscanf(optalign->f, "%*[^\n]");
    getc(optalign->f);
    fscanf(optalign->f, "%ld%*[^\n]", &theclass);
    getc(optalign->f);
    if (theclass != class_) {
      printf("classes not being read correctly\n");
      halt();
    }
    for (s = 1; s <= sequences; s++) {
      fscanf(optalign->f, "%ld", &shift);
      if (class_ == analignment) {
	if (zerobase >= 0)
	  fprintf(distribution->f, "%ld\n", shift + zerobase);
	else {
	  fprintf(distribution->f, "%ld\n", -(shift + zerobase));

	}
      }
    }
  }

  /*













*/



  while (!BUFEOF(inst->f)) {
    if (!P_eoln(inst->f)) {
      resettrigger(&t0a);
      resettrigger(&t0b);
      resettrigger(&t0c);
      resettrigger(&t0d);
      resettrigger(&t0e);
      resettrigger(&t1a);
      resettrigger(&t2a);
      resettrigger(&t3a);
      resettrigger(&t5a);

      while (!P_eoln(inst->f)) {
	c = getc(inst->f);
	if (c == '\n')
	  c = ' ';
	if (!shutup)
	  putc(c, cinst->f);

	if (debugging)
	  printf("%c[%ld]", c, state);

	testfortrigger(c, &t0a);
	testfortrigger(c, &t0b);
	testfortrigger(c, &t0c);
	testfortrigger(c, &t0d);
	testfortrigger(c, &t0e);
	testfortrigger(c, &t1a);
	testfortrigger(c, &t2a);
	testfortrigger(c, &t3a);
	testfortrigger(c, &t5a);

	switch (state) {

	case 0:
	  if (t0a.found)
	    state = 1;

	  else if (t0b.found)
	    state = 2;

	  else if (t0c.found)
	    state = 5;

	  else if (t0d.found)
	    state = 6;

	  else if (t0e.found) {

	    state = 7;
	  }
	  break;

	case 1:
	  if (t1a.found)
	    state = 0;
	  break;

	case 2:
	  if (t2a.found) {
	    fscanf(inst->f, "%ld%ld", &fromvalue, &fromrange);


	    fscanf(optinst->f, "%ld", &fromvalue);
	    /*

*/


	    fromvalue += zerobase;

	    fprintf(cinst->f, " %ld %c%ld",
		    fromvalue, sign(fromrange), labs(fromrange));

	    state = 3;
	  }
	  break;

	case 3:
	  if (t3a.found)
	    state = 4;
	  break;

	case 4:
	  skipblanks(inst);
	  if (P_peek(inst->f) == 's') {
	    tovalue = fromvalue;
	    skipnonblanks(inst);
	    fscanf(inst->f, "%ld", &torange);

	    fprintf(cinst->f, "same %c%ld", sign(torange), labs(torange));
	  }

	  else if (P_peek(inst->f) == 'p') {

	    tovalue = fromvalue;
	    skipnonblanks(inst);
	    /*

*/
	    torange = 100;
	    shutup = true;

	    fprintf(cinst->f, "same %c%ld", sign(torange), labs(torange));
	  }

	  else {
	    fscanf(inst->f, "%ld%ld", &tovalue, &torange);
	    fprintf(cinst->f, "%ld %c%ld",
		    fromvalue, sign(torange), labs(torange));
	  }
	  state = 0;
	  break;


	case 5:
	  if (t5a.found)
	    state = 0;
	  break;


	case 6:
	  if (t0d.found)
	    state = 0;
	  break;

	case 7:
	  if (t0e.found)
	    state = 0;
	  break;

	}

      }
      continue;
    }

    fscanf(inst->f, "%*[^\n]");
    getc(inst->f);
    if (debugging)
      putchar('\n');
    if (shutup) {
      fprintf(cinst->f, ";\n");
      shutup = false;
    } else
      putc('\n', cinst->f);
  }

  fprintf(cinst->f, "\n(* malin%5.2f *)\n", version);
  fprintf(cinst->f, "(* alignment: %ld, occurences: %ld, H: %10.5f bits *)\n",
	  analignment, occurences, H);
  printf("alignment: %ld, occurences: %ld, H: %10.5f bits\n",
	 analignment, occurences, H);

}



main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  distribution.f = NULL;
  strcpy(distribution.name, "distribution");
  cinst.f = NULL;
  strcpy(cinst.name, "cinst");
  malinp.f = NULL;
  strcpy(malinp.name, "malinp");
  inst.f = NULL;
  strcpy(inst.name, "inst");
  optalign.f = NULL;
  strcpy(optalign.name, "optalign");
  optinst.f = NULL;
  strcpy(optinst.name, "optinst");
  themain(&optinst, &optalign, &inst, &malinp, &cinst, &distribution);
_L1:
  if (optinst.f != NULL)
    fclose(optinst.f);
  if (optalign.f != NULL)
    fclose(optalign.f);
  if (inst.f != NULL)
    fclose(inst.f);
  if (malinp.f != NULL)
    fclose(malinp.f);
  if (cinst.f != NULL)
    fclose(cinst.f);
  if (distribution.f != NULL)
    fclose(distribution.f);
  exit(EXIT_SUCCESS);
}



/* End. */
