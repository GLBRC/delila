/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "module.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*








*/



#define version         "6.21 of module.p 2019 Apr 08"
/*









*/


/*












































































*/



#define lastcharacter   ' '

#define maxname         50

/*





*/
#define maxdepth        10

/*

*/
#define checkuptimes    2

/*
*/

#define recreate        true

#define debugging       false


typedef struct name {
  Char letter[maxname];
  long length;
} name;

typedef struct trigger {
  name n;
  long state;
  boolean skip, found;
} trigger;

typedef struct modcatitem {
  name amodule;
  long line;
} modcatitem;

typedef struct modcatfile {
  FILE *f;
  FILEBUFNC(f,modcatitem);
  Char name[_FNSIZE];
} modcatfile;


Static _TEXT fin, modlib, fout, list;

Static modcatfile modcat;

Static long finline, modlibline;


Static trigger begintrigger, endtrigger;

Static name finname, vermod;


Static boolean showversion;
Static name vername;

/*




*/
Static boolean donthalt;
/*
*/
Static boolean haltcalled;


/*

*/
Static long detectedmodules, transferredmodules;


Static jmp_buf _JL1;



Static Void halt()
{
  if (donthalt) {
    haltcalled = true;
    return;
  }
  if (*modcat.name != '\0') {
    if (modcat.f != NULL)
      modcat.f = freopen(modcat.name, "wb", modcat.f);
    else
      modcat.f = fopen(modcat.name, "wb");
  } else {
    if (modcat.f != NULL)
      rewind(modcat.f);
    else
      modcat.f = tmpfile();
  }
  if (modcat.f == NULL)
    _EscIO2(FileNotFound, modcat.name);
  SETUPBUF(modcat.f, modcatitem);

  if (*fout.name != '\0') {
    if (fout.f != NULL)
      fout.f = freopen(fout.name, "w", fout.f);
    else
      fout.f = fopen(fout.name, "w");
  } else {
    if (fout.f != NULL)
      rewind(fout.f);
    else
      fout.f = tmpfile();
  }
  if (fout.f == NULL)
    _EscIO2(FileNotFound, fout.name);
  SETUPBUF(fout.f, Char);
  printf(" error in module transfer.  see list\n");
  printf(" program halt.\n");
  fprintf(list.f, " program halt.\n");
  longjmp(_JL1, 1);
  /*
*/
}







Static Void copy_(fin, fout, ch)
_TEXT *fin, *fout;
Char *ch;
{
  if (BUFEOF(fin->f))
    return;
  *ch = getc(fin->f);
  if (*ch == '\n')
    *ch = ' ';
  putc(*ch, fout->f);

}


Static Void finishline(fin, fout, finline)
_TEXT *fin, *fout;
long *finline;
{
  Char ch;

  while (!P_eoln(fin->f))
    copy_(fin, fout, &ch);
  fscanf(fin->f, "%*[^\n]");
  getc(fin->f);
  putc('\n', fout->f);
  (*finline)++;
}


Static Void gettoline(line, f, current)
long line;
_TEXT *f;
long *current;
{
  /*
*/
  if (*current > line) {
    if (*f->name != '\0') {
      if (f->f != NULL)
	f->f = freopen(f->name, "r", f->f);
      else
	f->f = fopen(f->name, "r");
    } else
      rewind(f->f);
    if (f->f == NULL)
      _EscIO2(FileNotFound, f->name);
    RESETBUF(f->f, Char);
    *current = 1;
  }

  while ((*current < line) & (!BUFEOF(f->f))) {
    (*current)++;
    fscanf(f->f, "%*[^\n]");
    getc(f->f);
  }

  if (BUFEOF(f->f)) {
    fprintf(list.f,
      " modcat refers to a line (%ld) that is past the end of modlib.\n",
      line);
    halt();
  }
}



Static Void clearname(n)
name *n;
{
  long l;

  for (l = 0; l < maxname; l++)
    n->letter[l] = ' ';
  n->length = 0;
}


Static Void printname(f, n)
_TEXT *f;
name n;
{
/* p2c: module.p, line 278: Note:
 * Format for packed-array-of-char will work only if width < length [321] */
  fprintf(f->f, "%.*s", (int)n.length, n.letter);
}


Static Void untrail(n)
name *n;
{
  /*
*/
  n->length = maxname;
  while (n->length > 0 && n->letter[n->length - 1] == ' ')
    n->length--;


  n->length++;

  if (n->length <= maxname) {
    n->letter[n->length - 1] = lastcharacter;
    return;
  }
  n->length = maxname;
  fprintf(list.f, " this name was found: \"");
  printname(&list, *n);
  fprintf(list.f, "\".\n");
  fprintf(list.f,
	  " names must be one character shorter than %ld characters.\n",
	  (long)maxname);
  halt();
}


Static Void getname(source, n)
_TEXT *source;
name *n;
{
  /*
*/
  Char ch = '.';

  clearname(n);
  while (!P_eoln(source->f) && ch != lastcharacter && n->length <= maxname) {
    n->length++;
    ch = getc(source->f);
    if (ch == '\n')
      ch = ' ';
    n->letter[n->length - 1] = ch;
  }
  if (n->letter[n->length - 1] == lastcharacter)
    return;
  fprintf(list.f, " this module name: \n");
  printname(&list, *n);
  if (n->length == maxname) {
    fprintf(list.f, " is too long (>%ld characters)\n", maxname - 1L);

  } else
    fprintf(list.f, " did not end with a \"%c\".\n", lastcharacter);
  halt();
}


Static boolean equalname(a, b)
name a, b;
{
  if (a.length == b.length)
    return (strncmp(a.letter, b.letter, maxname) == 0);
  else
    return false;
}


Static boolean greatername(a, b)
name a, b;
{
  return (strncmp(a.letter, b.letter, maxname) > 0);
}



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
  if (t->n.letter[t->state - 1] == ch) {
    t->skip = false;
    if (t->state == t->n.length)
      t->found = true;
    else
      t->found = false;
    return;
  }
  t->state = 0;
  t->skip = true;
  t->found = false;
}


Static Void findmoduleend(fin, amodule, finline)
_TEXT *fin;
name amodule;
long *finline;
{
  /*
*/
  boolean found = false;
  Char ch;
  name endname;

  while ((!found) & (!BUFEOF(fin->f))) {
    resettrigger(&endtrigger);
    while (!(P_eoln(fin->f) || endtrigger.skip || endtrigger.found)) {
      ch = getc(fin->f);
      if (ch == '\n')
	ch = ' ';
      testfortrigger(ch, &endtrigger);
    }

    if (endtrigger.found) {
      getname(fin, &endname);
      if (equalname(endname, amodule))
	found = true;
    }


    fscanf(fin->f, "%*[^\n]");
    getc(fin->f);
    (*finline)++;
  }

  if (!BUFEOF(fin->f) || found)
    return;
  fprintf(list.f, " no end to module ");
  printname(&list, amodule);
  fprintf(list.f, " whose contents were being skipped.\n");
  halt();
}


Static Char copytobound(fin, fout, line)
_TEXT *fin, *fout;
long *line;
{
  /*




*/
  Char Result;
  boolean found = false;
  Char ch;


  while ((!found) & (!BUFEOF(fin->f))) {
    resettrigger(&begintrigger);
    resettrigger(&endtrigger);
    while (!(P_eoln(fin->f) || ((begintrigger.skip || begintrigger.found) &&
				(endtrigger.skip || endtrigger.found)))) {
      copy_(fin, fout, &ch);
      testfortrigger(ch, &begintrigger);
      testfortrigger(ch, &endtrigger);
    }

    found = (begintrigger.found || endtrigger.found);

    if (!found) {
      if (begintrigger.skip || endtrigger.skip) {
	while (!P_eoln(fin->f))
	  copy_(fin, fout, &ch);
      }
    }

    if (P_eoln(fin->f)) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      putc('\n', fout->f);
      (*line)++;
    }
  }

  if (!found) {
    return 'f';

  }

  if (begintrigger.found)
    Result = 'b';
  if (endtrigger.found)
    return 'e';
  return Result;
}


Static Void copytoend(fin, fout, amodule, finline)
_TEXT *fin, *fout;
name amodule;
long *finline;
{
  /*
*/
  boolean done = false;
  name endname;

  while (!done) {
    switch (copytobound(fin, fout, finline)) {

    case 'b':
      /* blank case */
      break;

    case 'e':
      getname(fin, &endname);
      if (equalname(endname, amodule))
	done = true;
      printname(fout, endname);
      finishline(fin, fout, finline);
      break;

    case 'f':
      fprintf(list.f, " the end of module ");
      printname(&list, amodule);
      fprintf(list.f, " was not found during copying\n");
      halt();
      break;
    }
  }
}


Static Void skiptoend(fin, fout, finline)
_TEXT *fin, *fout;
long *finline;
{
  /*

*/
  name amodule;


  getname(fin, &amodule);
  printname(fout, amodule);
  finishline(fin, fout, finline);

  if (equalname(amodule, vermod)) {
    copytoend(fin, fout, amodule, finline);
    return;
  }

  findmoduleend(fin, amodule, finline);

  /*
*/
  printname(fout, endtrigger.n);
  printname(fout, amodule);



  if (lastcharacter != ' ')
    putc(' ', fout->f);
  fprintf(fout->f, "*)\n");


}



Static Void grab(f, item)
modcatfile *f;
modcatitem *item;
{
  fread(item, sizeof(modcatitem), 1, f->f);
}


Static Void drop(t, item)
modcatfile *t;
modcatitem *item;
{
  fwrite(item, sizeof(modcatitem), 1, t->f);
}


Static Void show(o, c)
_TEXT *o;
modcatfile *c;
{
  modcatitem item;

  if (*c->name != '\0') {
    if (c->f != NULL)
      c->f = freopen(c->name, "rb", c->f);
    else
      c->f = fopen(c->name, "rb");
  } else
    rewind(c->f);
  if (c->f == NULL)
    _EscIO2(FileNotFound, c->name);
  RESETBUF(c->f, modcatitem);
  fprintf(o->f, "\n   line module name\n");
  while (!BUFEOF(c->f)) {
    grab(c, &item);
    fprintf(o->f, " %6ld ", item.line);
    printname(o, item.amodule);
    putc('\n', o->f);
  }
  putc('\n', o->f);
}


Static Void build(modlib, modcat)
_TEXT *modlib;
modcatfile *modcat;
{
  long li = 1;
  Char ch;
  name na;
  modcatitem item;
  long number = 0;

  if (*modlib->name != '\0') {
    if (modlib->f != NULL)
      modlib->f = freopen(modlib->name, "r", modlib->f);
    else
      modlib->f = fopen(modlib->name, "r");
  } else
    rewind(modlib->f);
  if (modlib->f == NULL)
    _EscIO2(FileNotFound, modlib->name);
  RESETBUF(modlib->f, Char);
  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "wb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "wb");
  } else {
    if (modcat->f != NULL)
      rewind(modcat->f);
    else
      modcat->f = tmpfile();
  }
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  SETUPBUF(modcat->f, modcatitem);

  while (!BUFEOF(modlib->f)) {
    resettrigger(&begintrigger);
    resettrigger(&endtrigger);
    while (!(P_eoln(modlib->f) ||
	     ((begintrigger.skip || begintrigger.found) &&
	      (endtrigger.skip || endtrigger.found)))) {
      ch = getc(modlib->f);
      if (ch == '\n')
	ch = ' ';
      testfortrigger(ch, &begintrigger);
      testfortrigger(ch, &endtrigger);
    }

    if (begintrigger.found) {
      getname(modlib, &item.amodule);
      item.line = li;
      drop(modcat, &item);
      number++;
      findmoduleend(modlib, item.amodule, &li);
      continue;
    }
    if (!endtrigger.found) {
      fscanf(modlib->f, "%*[^\n]");
      getc(modlib->f);
      li++;
      continue;
    }
    fprintf(list.f, " unexpected module end: ");
    getname(modlib, &na);
    printname(&list, na);
    fprintf(list.f, " at line %ld in modlib.\n", li);
    halt();
  }

  if (number == 0) {
    fprintf(list.f, " no modules in modlib.\n");
    halt();
    return;
  }
  fprintf(list.f, " %ld module", number);
  if (number != 1)
    putc('s', list.f);
  fprintf(list.f, " in modlib.\n");
  if (*modlib->name != '\0') {
    if (modlib->f != NULL)
      modlib->f = freopen(modlib->name, "r", modlib->f);
    else
      modlib->f = fopen(modlib->name, "r");
  } else
    rewind(modlib->f);
  if (modlib->f == NULL)
    _EscIO2(FileNotFound, modlib->name);
  RESETBUF(modlib->f, Char);
}


Local Void bubblepass(f, t, changes)
modcatfile *f, *t;
boolean *changes;
{
  /*

*/
  modcatitem a, b;

  *changes = false;
  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "rb", f->f);
    else
      f->f = fopen(f->name, "rb");
  } else
    rewind(f->f);
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  RESETBUF(f->f, modcatitem);
  if (*t->name != '\0') {
    if (t->f != NULL)
      t->f = freopen(t->name, "wb", t->f);
    else
      t->f = fopen(t->name, "wb");
  } else {
    if (t->f != NULL)
      rewind(t->f);
    else
      t->f = tmpfile();
  }
  if (t->f == NULL)
    _EscIO2(FileNotFound, t->name);
  SETUPBUF(t->f, modcatitem);
  grab(f, &a);

  while (!BUFEOF(f->f)) {
    grab(f, &b);


    if (greatername(b.amodule, a.amodule) | equalname(b.amodule, a.amodule)) {
      drop(t, &a);
      a = b;
    }

    else {
      *changes = true;

      drop(t, &b);
    }
  }


  drop(t, &a);
}


Static Void sort(f)
modcatfile *f;
{
  /*

*/
  modcatfile i;
  boolean changes = true;

  i.f = NULL;
  *i.name = '\0';

  while (changes) {
    bubblepass(f, &i, &changes);

    if (changes)
      bubblepass(&i, f, &changes);
  }


  if (i.f != NULL)
    fclose(i.f);
}


Static Void checkduplicate(f)
modcatfile *f;
{
  /*
*/
  modcatitem a, b;
  boolean ok = true;

  if (*f->name != '\0') {
    if (f->f != NULL)
      f->f = freopen(f->name, "rb", f->f);
    else
      f->f = fopen(f->name, "rb");
  } else
    rewind(f->f);
  if (f->f == NULL)
    _EscIO2(FileNotFound, f->name);
  RESETBUF(f->f, modcatitem);
  grab(f, &a);
  while (!BUFEOF(f->f)) {
    grab(f, &b);
    if (equalname(a.amodule, b.amodule)) {
      ok = false;
      fprintf(list.f, " duplicate module name: ");
      printname(&list, a.amodule);
      fprintf(list.f, "\n           found at lines %ld and %ld of modlib.\n",
	      a.line, b.line);
    }
    a = b;
  }
  if (ok)
    return;
  if (*modcat.name != '\0') {
    if (modcat.f != NULL)
      modcat.f = freopen(modcat.name, "wb", modcat.f);
    else
      modcat.f = fopen(modcat.name, "wb");
  } else {
    if (modcat.f != NULL)
      rewind(modcat.f);
    else
      modcat.f = tmpfile();
  }
  if (modcat.f == NULL)
    _EscIO2(FileNotFound, modcat.name);
  SETUPBUF(modcat.f, modcatitem);
  halt();
}


Static Void createmodcat(modlib, modcat)
_TEXT *modlib;
modcatfile *modcat;
{
  fprintf(list.f, " creating module catalogue (modcat)\n");
  build(modlib, modcat);
  sort(modcat);
  checkduplicate(modcat);
  show(&list, modcat);
  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "rb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "rb");
  } else
    rewind(modcat->f);
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  RESETBUF(modcat->f, modcatitem);
}


Static boolean inmodcat(amodule, line)
name amodule;
long *line;
{
  /*
*/
  boolean Result;
  modcatitem n;
  boolean found;

  if (BUFEOF(modcat.f)) {
    if (*modcat.name != '\0') {
      if (modcat.f != NULL)
	modcat.f = freopen(modcat.name, "rb", modcat.f);
      else
	modcat.f = fopen(modcat.name, "rb");
    } else
      rewind(modcat.f);
    if (modcat.f == NULL)
      _EscIO2(FileNotFound, modcat.name);
    RESETBUF(modcat.f, modcatitem);
    found = false;
  } else {
    grab(&modcat, &n);
    if (greatername(n.amodule, amodule)) {
      if (*modcat.name != '\0') {
	if (modcat.f != NULL)
	  modcat.f = freopen(modcat.name, "rb", modcat.f);
	else
	  modcat.f = fopen(modcat.name, "rb");
      } else
	rewind(modcat.f);
      if (modcat.f == NULL)
	_EscIO2(FileNotFound, modcat.name);
      RESETBUF(modcat.f, modcatitem);
      found = false;
    }

    else if (equalname(n.amodule, amodule))
      found = true;
    else
      found = false;
  }


  while ((!found) & (!BUFEOF(modcat.f))) {
    grab(&modcat, &n);
    if (equalname(n.amodule, amodule))
      found = true;
  }

  if (found) {
    Result = true;
    *line = n.line;
  } else {
    Result = false;
    *line = -1;
  }
  return Result;
}


Static Void checkup(modlib, modcat)
_TEXT *modlib;
modcatfile *modcat;
{
  /*

*/
  long times = 0;
  boolean fail = false;
  modcatitem cat;
  Char ch;
  name libname;
  long modlibline = 1;
  long modcatline;

  fprintf(list.f, " check modlib-modcat correspondence:\n");
  if (*modlib->name != '\0') {
    if (modlib->f != NULL)
      modlib->f = freopen(modlib->name, "r", modlib->f);
    else
      modlib->f = fopen(modlib->name, "r");
  } else
    rewind(modlib->f);
  if (modlib->f == NULL)
    _EscIO2(FileNotFound, modlib->name);
  RESETBUF(modlib->f, Char);
  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "rb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "rb");
  } else
    rewind(modcat->f);
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  RESETBUF(modcat->f, modcatitem);

  /*
*/
  donthalt = true;
  do {

    grab(modcat, &cat);


    gettoline(cat.line, modlib, &modlibline);

    if (haltcalled) {
      fail = true;
      haltcalled = false;
    }

    else {
      resettrigger(&begintrigger);

      while (!(P_eoln(modlib->f) || begintrigger.found || begintrigger.skip)) {
	ch = getc(modlib->f);
	if (ch == '\n')
	  ch = ' ';
	testfortrigger(ch, &begintrigger);
      }

      if (begintrigger.skip | P_eoln(modlib->f))
	fail = true;
      else {
	getname(modlib, &libname);
	if (!equalname(libname, cat.amodule))
	  fail = true;
      }

      times++;
    }
  } while (!(((times >= checkuptimes) | BUFEOF(modcat->f)) || fail));

  donthalt = false;

  /*
*/
  if (!fail) {
    if (*modlib->name != '\0') {
      if (modlib->f != NULL)
	modlib->f = freopen(modlib->name, "r", modlib->f);
      else
	modlib->f = fopen(modlib->name, "r");
    } else
      rewind(modlib->f);
    if (modlib->f == NULL)
      _EscIO2(FileNotFound, modlib->name);
    RESETBUF(modlib->f, Char);
    if (*modcat->name != '\0') {
      if (modcat->f != NULL)
	modcat->f = freopen(modcat->name, "rb", modcat->f);
      else
	modcat->f = fopen(modcat->name, "rb");
    } else
      rewind(modcat->f);
    if (modcat->f == NULL)
      _EscIO2(FileNotFound, modcat->name);
    RESETBUF(modcat->f, modcatitem);
    modlibline = 1;
    times = 0;

    do {
      resettrigger(&begintrigger);
      while (!(P_eoln(modlib->f) || begintrigger.skip || begintrigger.found)) {
	ch = getc(modlib->f);
	if (ch == '\n')
	  ch = ' ';
	testfortrigger(ch, &begintrigger);
      }

      if (begintrigger.found) {
	times++;
	getname(modlib, &libname);
	if (!inmodcat(libname, &modcatline)) {
	  fail = true;

	} else if (modcatline != modlibline)
	  fail = true;
	if (!fail)
	  findmoduleend(modlib, libname, &modlibline);
      } else {
	fscanf(modlib->f, "%*[^\n]");
	getc(modlib->f);
	modlibline++;
      }
    } while (!(((times > checkuptimes) | BUFEOF(modlib->f)) || fail));
  }


  if (fail) {
    fprintf(list.f, " failed: ");
    if (recreate)
      createmodcat(modlib, modcat);
    else
      halt();
    return;
  }
  fprintf(list.f, " passed.\n");
  modlibline = 1;
  if (*modlib->name != '\0') {
    if (modlib->f != NULL)
      modlib->f = freopen(modlib->name, "r", modlib->f);
    else
      modlib->f = fopen(modlib->name, "r");
  } else
    rewind(modlib->f);
  if (modlib->f == NULL)
    _EscIO2(FileNotFound, modlib->name);
  RESETBUF(modlib->f, Char);
  if (*modcat->name != '\0') {
    if (modcat->f != NULL)
      modcat->f = freopen(modcat->name, "rb", modcat->f);
    else
      modcat->f = fopen(modcat->name, "rb");
  } else
    rewind(modcat->f);
  if (modcat->f == NULL)
    _EscIO2(FileNotFound, modcat->name);
  RESETBUF(modcat->f, modcatitem);
}



Static Void initialize()
{
  printf(" %s\n", version);
  if (*fin.name != '\0') {
    if (fin.f != NULL)
      fin.f = freopen(fin.name, "r", fin.f);
    else
      fin.f = fopen(fin.name, "r");
  } else
    rewind(fin.f);
  if (fin.f == NULL)
    _EscIO2(FileNotFound, fin.name);
  RESETBUF(fin.f, Char);
  if (*modlib.name != '\0') {
    if (modlib.f != NULL)
      modlib.f = freopen(modlib.name, "r", modlib.f);
    else
      modlib.f = fopen(modlib.name, "r");
  } else
    rewind(modlib.f);
  if (modlib.f == NULL)
    _EscIO2(FileNotFound, modlib.name);
  RESETBUF(modlib.f, Char);
  if (*fout.name != '\0') {
    if (fout.f != NULL)
      fout.f = freopen(fout.name, "w", fout.f);
    else
      fout.f = fopen(fout.name, "w");
  } else {
    if (fout.f != NULL)
      rewind(fout.f);
    else
      fout.f = tmpfile();
  }
  if (fout.f == NULL)
    _EscIO2(FileNotFound, fout.name);
  SETUPBUF(fout.f, Char);
  if (*modcat.name != '\0') {
    if (modcat.f != NULL)
      modcat.f = freopen(modcat.name, "rb", modcat.f);
    else
      modcat.f = fopen(modcat.name, "rb");
  } else
    rewind(modcat.f);
  if (modcat.f == NULL)
    _EscIO2(FileNotFound, modcat.name);
  RESETBUF(modcat.f, modcatitem);
  if (*list.name != '\0') {
    if (list.f != NULL)
      list.f = freopen(list.name, "w", list.f);
    else
      list.f = fopen(list.name, "w");
  } else {
    if (list.f != NULL)
      rewind(list.f);
    else
      list.f = tmpfile();
  }
  if (list.f == NULL)
    _EscIO2(FileNotFound, list.name);
  SETUPBUF(list.f, Char);

  fprintf(list.f, " %s\n", version);

  finline = 1;
  modlibline = 1;




  memcpy(begintrigger.n.letter,
	 "(* begin module                                   ", (long)maxname);
  untrail(&begintrigger.n);



  memcpy(endtrigger.n.letter,
	 "(* end module                                     ", (long)maxname);
  untrail(&endtrigger.n);

  /*
*/


  memcpy(finname.letter, "(source input)                                    ",
	 (long)maxname);
  untrail(&finname);




  memcpy(vermod.letter, "version                                           ",
	 (long)maxname);
  untrail(&vermod);


  donthalt = false;
  haltcalled = false;


  detectedmodules = 0;
  transferredmodules = 0;
}


Static Void getversion()
{
  /*
*/
  long line;
  Char ch;
  boolean endofcomment = false;

  /*


*/
  if (!inmodcat(vermod, &line)) {
    showversion = false;
    return;
  }
  gettoline(line, &modlib, &modlibline);


  fscanf(modlib.f, "%*[^\n]");
  getc(modlib.f);
  modlibline++;


  clearname(&vername);
  while ((!P_eoln(modlib.f)) & (P_peek(modlib.f) == ' '))
    getc(modlib.f);
  while (!P_eoln(modlib.f) && vername.length <= maxname && !endofcomment) {
    vername.length++;
    ch = getc(modlib.f);
    if (ch == '\n')
      ch = ' ';
    vername.letter[vername.length - 1] = ch;



    if (vername.letter[vername.length - 1] == '*') {
      if (vername.length > 1) {
	if (vername.letter[vername.length - 2] == '(')
	  vername.letter[vername.length - 1] = '@';
      }
    }


    if (vername.letter[vername.length - 1] != ')')
      continue;
    if (vername.length > 1) {
      if (vername.letter[vername.length - 2] == '*') {
	endofcomment = true;


	vername.letter[vername.length - 1] = ' ';
	vername.letter[vername.length - 2] = ' ';
	/*
*/
      }
    }
  }
  fscanf(modlib.f, "%*[^\n]");
  getc(modlib.f);
  modlibline++;

  untrail(&vername);
  showversion = true;
}


Static Void strip(fin, fout)
_TEXT *fin, *fout;
{
  boolean done = false;
  long finline = 1;
  name error;

  fprintf(list.f, " no module library (modlib): stripping fin to fout.\n");
  printf(" no module library (modlib): stripping fin to fout.\n");

  while (!done) {
    switch (copytobound(fin, fout, &finline)) {

    case 'b':
      skiptoend(fin, fout, &finline);
      detectedmodules++;
      break;

    case 'e':
      fprintf(list.f, " extra module end named ");
      getname(fin, &error);
      printname(&list, error);
      fprintf(list.f, " detected at line %ld of fin.\n", finline);
      halt();
      break;

    case 'f':
      done = true;
      break;
    }
  }
}


Static boolean transfer PP((name amodule, _TEXT *fin, _TEXT *fout,
			    _TEXT *modlib, long *finline, long *modlibline,
			    long depth));

/* Local variables for transfer: */
struct LOC_transfer {
  _TEXT *fin, *fout, *modlib;
  long *finline, *modlibline, depth;
} ;

Local Void report(f, depth, what, amodule, LINK)
_TEXT *f;
long depth;
Char what;
name amodule;
struct LOC_transfer *LINK;
{
  /*





*/
  putc(' ', f->f);
  switch (what) {

  case 't':
    putc(' ', f->f);
    break;

  case 'n':
    putc('*', f->f);
    break;

  case 'i':
    putc('?', f->f);
    break;

  case 'v':
    putc('v', f->f);
    break;
  }
  fprintf(f->f, " %3ld    ", depth);
  switch (what) {

  case 't':
    fprintf(f->f, "transferred ");
    break;

  case 'n':
    fprintf(f->f, "not found   ");
    break;

  case 'i':
    fprintf(f->f, "infinite??  ");
    break;

  case 'v':
    fprintf(f->f, "no transfer ");
    break;
  }
  printname(f, amodule);
  putc('\n', f->f);
}

Local Void recurse(LINK)
struct LOC_transfer *LINK;
{
  name inner;
  long line, remember;

  /*
*/
  getname(LINK->fin, &inner);
  printname(LINK->fout, inner);
  finishline(LINK->fin, LINK->fout, LINK->finline);
  remember = *LINK->finline;
  if (LINK->depth == 0)
    detectedmodules++;

  if (!inmodcat(inner, &line)) {
    report(&list, LINK->depth, 'n', inner, LINK);
    copytoend(LINK->fin, LINK->fout, inner, LINK->finline);
    return;
  }
  if (LINK->depth >= maxdepth) {
    /*
*/
    report(&list, LINK->depth, 'i', inner, LINK);
    fprintf(LINK->fout->f,
	    "(* the modules are nested to a depth of %ld at this point.\n",
	    LINK->depth + 1);
    fprintf(LINK->fout->f,
	    "   perhaps the modlib has an infinite module nesting.\n");
    fprintf(LINK->fout->f, "   further recursive transfers are aborted. *)\n");
    printf(" a possible infinitely recursive nesting of");
    printf(" modules was detected.  see list.\n");
    copytoend(LINK->fin, LINK->fout, inner, LINK->finline);
    return;
  }
  if (equalname(vermod, inner)) {
    report(&list, LINK->depth, 'v', inner, LINK);
    copytoend(LINK->fin, LINK->fout, inner, LINK->finline);
    return;
  }

  /*
*/
  gettoline(line + 1, LINK->modlib, LINK->modlibline);
  if (!transfer(inner, LINK->modlib, LINK->fout, LINK->modlib,
		LINK->modlibline, LINK->modlibline, LINK->depth + 1)) {
    fprintf(list.f, " missing end of module ");
    printname(&list, inner);
    fprintf(list.f, " in modlib.\n");
    halt();
    return;
  }
  /*
*/
  gettoline(remember, LINK->fin, LINK->finline);


  findmoduleend(LINK->fin, inner, LINK->finline);


  if (LINK->depth == 0)
    transferredmodules++;
}


Static boolean transfer(amodule, fin_, fout_, modlib_, finline_, modlibline_,
			depth_)
name amodule;
_TEXT *fin_, *fout_, *modlib_;
long *finline_, *modlibline_, depth_;
{
  /*




*/
  struct LOC_transfer V;
  boolean Result;
  boolean done = false;
  name endname;

  V.fin = fin_;
  V.fout = fout_;
  V.modlib = modlib_;
  V.finline = finline_;
  V.modlibline = modlibline_;
  V.depth = depth_;
  while (!done) {
    switch (copytobound(V.fin, V.fout, V.finline)) {

    case 'b':
      recurse(&V);
      break;

    case 'e':
      getname(V.fin, &endname);
      printname(V.fout, endname);


      if (showversion) {
	printname(V.fout, vername);
	fprintf(V.fout->f, "*)\n");
	fscanf(V.fin->f, "%*[^\n]");
	getc(V.fin->f);
	(*V.finline)++;
      } else
	finishline(V.fin, V.fout, V.finline);

      if (equalname(endname, amodule)) {
	done = true;
	Result = true;
      } else {
	if (V.depth == 0) {
	  fprintf(list.f, " fin module ");
	  printname(&list, endname);
	  fprintf(list.f, "ended at line %ld.\n", *V.finline - 1);
	  fprintf(list.f, " the begin is missing or incorrect.\n");
	} else {
	  fprintf(list.f, " module began with the name ");
	  printname(&list, amodule);
	  fprintf(list.f, ",\n");
	  fprintf(list.f, " but ended with ");
	  printname(&list, endname);
	  fprintf(list.f, " at line %ld in modlib.", *V.finline - 1);
	}
	halt();
      }
      break;

    case 'f':
      done = true;
      Result = false;
      break;
    }
  }
  report(&list, V.depth, 't', amodule, &V);
  return Result;
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  modcat.f = NULL;
  strcpy(modcat.name, "modcat");
  list.f = NULL;
  strcpy(list.name, "list");
  fout.f = NULL;
  strcpy(fout.name, "fout");
  modlib.f = NULL;
  strcpy(modlib.name, "modlib");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  initialize();

  if (BUFEOF(fin.f)) {
    fprintf(list.f, " no source (fin) file.\n");
    halt();
  } else if (BUFEOF(modlib.f))
    strip(&fin, &fout);
  else {
    if (BUFEOF(modcat.f))
      createmodcat(&modlib, &modcat);
    else
      checkup(&modlib, &modcat);


    getversion();
    if (showversion) {
      fprintf(list.f, " module ");
      printname(&list, vername);
      putc('\n', list.f);
    } else
      fprintf(list.f, " no version for modlib.\n");

    fprintf(list.f, "\n nesting              module\n");
    fprintf(list.f, "  depth   action      name\n");


    if (transfer(finname, &fin, &fout, &modlib, &finline, &modlibline, 0L)) {
      fprintf(list.f, " zero depth module name ");
      printname(&list, finname);
      fprintf(list.f, " detected as a module - program error\n");
      halt();
    }
  }
  printf(" %ld modules detected in fin, %ld modules transferred\n",
	 detectedmodules, transferredmodules);
  fprintf(list.f,
	  "\n %ld modules detected in fin, %ld modules transferred\n\n",
	  detectedmodules, transferredmodules);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (modlib.f != NULL)
    fclose(modlib.f);
  if (fout.f != NULL)
    fclose(fout.f);
  if (list.f != NULL)
    fclose(list.f);
  if (modcat.f != NULL)
    fclose(modcat.f);
  exit(EXIT_SUCCESS);
}



/* End. */
