/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "calc.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         2.71
/*

















*/



/*





















































































*/



#define defrwidth       9
#define defrdecimal     2
#define zbound          38
/*
*/

#define pmstring1       " \\pm "
#define pmstring2       " +/- "



typedef struct range {
  double estimate, error;
} range;

typedef struct stack {
  range n;
  struct stack *next;
} stack;



Static double base;
Static boolean notation;
Static long rwidth, rdecimal;
Static Char printer;
/*
*/
Static boolean showingerrors, latex;
Static Char pmstring[5];
/*
*/
Static boolean fromto;
/*
*/

Static stack *thestack, *spare;
Static boolean carriagereturn;



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



Static Void setrange(f, t)
range f, *t;
{
  t->estimate = f.estimate;
  t->error = f.error;
}


/* Local variables for readrange: */
struct LOC_readrange {
  boolean showingerrors;
} ;

Local Void getnumber(f, number, ok, LINK)
_TEXT *f;
double *number;
boolean *ok;
struct LOC_readrange *LINK;
{
  boolean negative;

  /*
*/
  if (P_peek(f->f) == '-') {
    getc(f->f);
    negative = true;
  } else
    negative = false;

  if (P_peek(f->f) != '9' && P_peek(f->f) != '8' && P_peek(f->f) != '7' &&
      P_peek(f->f) != '6' && P_peek(f->f) != '5' && P_peek(f->f) != '4' &&
      P_peek(f->f) != '3' && P_peek(f->f) != '2' && P_peek(f->f) != '1' &&
      P_peek(f->f) != '0') {
    printf("Error set to zero\n");
    *number = 0.0;
    if (LINK->showingerrors)
      *ok = true;
    else
      *ok = false;
    return;
  }
  /*

*/
  *ok = true;
  fscanf(f->f, "%lg", number);
  if (negative)
    *number = -*number;
}



Static Void readrange(fin, r, ok, showingerrors_)
_TEXT *fin;
range *r;
boolean *ok, showingerrors_;
{
  /*



*/
  struct LOC_readrange V;
  double hold, first, second;

  V.showingerrors = showingerrors_;
  getnumber(fin, &first, ok, &V);
  if (*ok) {
    if (V.showingerrors) {
      skipblanks(fin);
      while (P_eoln(fin->f)) {
	fscanf(fin->f, "%*[^\n]");
	getc(fin->f);
	skipblanks(fin);
      }

      if (P_peek(fin->f) == '\'') {
	getc(fin->f);
	if (fromto)
	  second = first;
	else
	  second = 0.0;
	if (printer != ' ')
	  printf("error assumed zero\n");
      } else
	getnumber(fin, &second, ok, &V);
    }

    else
      second = 0.0;
  }


  if (!*ok) {
    r->estimate = 0.0;
    r->error = 0.0;
    return;
  }
  /*
*/
  if (!fromto && second < 0.0) {
    second = fabs(second);
    printf("(Note: the error was made to be positive number.)\n");
  }


  if (fromto && V.showingerrors && first > second) {
    hold = first;
    first = second;
    second = hold;
    printf("(Note: order of the pair was reversed.)\n");
  }


  if (fromto && V.showingerrors) {
    r->estimate = (first + second) / 2.0;
    r->error = r->estimate - first;
  } else {
    r->estimate = first;
    r->error = second;
  }
}


/* Local variables for writerange: */
struct LOC_writerange {
  _TEXT *fout;
  range r;
  long width, decimal;
  boolean exponential, zerodecimal;
} ;

Local Void makeplusminus(LINK)
struct LOC_writerange *LINK;
{
  if (LINK->exponential) {
    fprintf(LINK->fout->f, "% .*E%.5s% .*E",
	    P_max((int)LINK->width - 7, 1), LINK->r.estimate, pmstring,
	    P_max((int)LINK->width - 7, 1), LINK->r.error);
    return;
  }
  if (LINK->zerodecimal)
    fprintf(LINK->fout->f, "%*ld%.5s%*ld",
	    (int)LINK->width, (long)floor(LINK->r.estimate + 0.5), pmstring,
	    (int)LINK->width, (long)floor(LINK->r.error + 0.5));
  else {
    fprintf(LINK->fout->f, "%*.*f%.5s%*.*f",
	    (int)LINK->width, (int)LINK->decimal, LINK->r.estimate, pmstring,
	    (int)LINK->width, (int)LINK->decimal, LINK->r.error);
    /*




*/
  }
}

Local Void makefromto(LINK)
struct LOC_writerange *LINK;
{
  putc('[', LINK->fout->f);

  if (LINK->exponential)
    fprintf(LINK->fout->f, "% .*E to % .*E",
	    P_max((int)LINK->width - 7, 1), LINK->r.estimate - LINK->r.error,
	    P_max((int)LINK->width - 7, 1), LINK->r.estimate + LINK->r.error);
  else if (LINK->zerodecimal)
    fprintf(LINK->fout->f, "%*ld to %*ld",
	    (int)LINK->width,
	    (long)floor(LINK->r.estimate - LINK->r.error + 0.5),
	    (int)LINK->width,
	    (long)floor(LINK->r.estimate + LINK->r.error + 0.5));
  else
    fprintf(LINK->fout->f, "%*.*f to %*.*f",
	    (int)LINK->width, (int)LINK->decimal,
	    LINK->r.estimate - LINK->r.error, (int)LINK->width,
	    (int)LINK->decimal, LINK->r.estimate + LINK->r.error);

  putc(']', LINK->fout->f);
}



Static Void writerange(fout_, r_, width_, decimal_, exponential_)
_TEXT *fout_;
range r_;
long width_, decimal_;
boolean exponential_;
{
  /*
*/
  struct LOC_writerange V;

  V.fout = fout_;
  V.r = r_;
  V.width = width_;
  V.decimal = decimal_;
  V.exponential = exponential_;
  V.zerodecimal = (V.decimal <= 0);

  if (showingerrors) {
    if (fromto)
      makefromto(&V);
    else
      makeplusminus(&V);
    return;
  }
  if (V.exponential) {
    fprintf(V.fout->f, "% .*E", P_max((int)V.width - 7, 1), V.r.estimate);
    return;
  }
  if (V.zerodecimal)
    fprintf(V.fout->f, "%*ld", (int)V.width, (long)floor(V.r.estimate + 0.5));
  else
    fprintf(V.fout->f, "%*.*f", (int)V.width, (int)V.decimal, V.r.estimate);
}



Static Void addrange(a, b, aplusb)
range a, b, *aplusb;
{
  aplusb->estimate = a.estimate + b.estimate;
  aplusb->error = sqrt(a.error * a.error + b.error * b.error);
}



Static Void minusrange(a, b, aminusb)
range a, b, *aminusb;
{
  aminusb->estimate = a.estimate - b.estimate;
  aminusb->error = sqrt(a.error * a.error + b.error * b.error);
}



Static Void timesrange(a, b, atimesb)
range a, b, *atimesb;
{
  double TEMP, TEMP1;

  atimesb->estimate = a.estimate * b.estimate;
  if (a.estimate != 0 && b.estimate != 0) {
    TEMP = a.error / a.estimate;
    TEMP1 = b.error / b.estimate;
    atimesb->error = fabs(atimesb->estimate) * sqrt(TEMP * TEMP + TEMP1 * TEMP1);
    return;
  }

  if (showingerrors) {
    printf("WARNING: one of the estimates to be multiplied is zero\n");
    printf("so the error will be set to zero\n");
  }
  atimesb->error = 0.0;
}



Static Void dividerange(a, b, aoverb)
range a, b, *aoverb;
{
  double TEMP, TEMP1;

  aoverb->estimate = a.estimate / b.estimate;
  if (a.estimate != 0) {
    TEMP = a.error / a.estimate;
    TEMP1 = b.error / b.estimate;
    aoverb->error = fabs(aoverb->estimate) * sqrt(TEMP * TEMP + TEMP1 * TEMP1);
    return;
  }

  if (showingerrors) {
    printf("WARNING: one of the estimates to be divided is zero\n");
    printf("so the error will be set to zero\n");
  }
  aoverb->error = 0.0;
}



Static Void logrange(a, base, loga)
range a;
double base;
range *loga;
{
  /*























*/
  loga->estimate = log(a.estimate) / log(base);
  /*

*/
  loga->error = a.error / (a.estimate * log(base));
}



Static Void exprange(a, b, atotheb)
range a, b, *atotheb;
{
  /*











*/
  if (a.estimate <= 0.0)
    printf("exponentiation refused: non-positive base\n");
  else {
    /*




*/
    atotheb->estimate = exp(b.estimate * log(a.estimate));
    atotheb->error =
      b.estimate * exp((b.estimate - 1) * log(a.estimate)) * a.error +
      exp(b.estimate * log(a.estimate)) * log(a.estimate) * b.error;
    /*



*/
  }
}





Static Void initialize()
{
  base = exp(1.0);
  notation = false;
  rwidth = defrwidth;
  rdecimal = defrdecimal;
  printer = 's';
  showingerrors = false;
  latex = true;
  memcpy(pmstring, pmstring1, 5L);
  fromto = false;

  thestack = NULL;
  spare = NULL;
  carriagereturn = true;
}



Static Void push(a)
range a;
{
  stack *hold;

  if (spare == NULL) {
    spare = (stack *)Malloc(sizeof(stack));

    spare->n.estimate = 0.0;
    spare->n.error = 0.0;
    spare->next = NULL;
  }


  hold = spare;
  if (spare->next != NULL)
    spare = spare->next;
  else
    spare = NULL;


  hold->next = thestack;
  thestack = hold;


  thestack->n = a;
}



Static Void pop()
{
  stack *hold;

  if (thestack == NULL) {
    printf("you cannot remove numbers from an empty stack\n");
    return;
  }

  hold = thestack;
  thestack = thestack->next;


  hold->next = spare;
  spare = hold;
}



Static Void rolldown()
{
  stack *hold, *here;

  if (thestack == NULL)
    return;

  if (thestack->next == NULL)
    return;

  hold = thestack;
  thestack = thestack->next;
  hold->next = NULL;


  here = thestack;
  while (here->next != NULL)
    here = here->next;

  here->next = hold;
}



Static boolean getone(first)
range *first;
{
  /*
*/
  if (thestack == NULL) {
    /*

*/
    return false;
  } else {
    *first = thestack->n;
    return true;
  }
}



Static boolean gettwo(first, second)
range *first, *second;
{
  /*
*/
  if (thestack == NULL) {
    printf("stack is empty\n");
    return false;
  } else if (thestack->next == NULL) {
    printf("there is only one number on the stack\n");
    return false;
  } else {
    *first = thestack->n;
    *second = thestack->next->n;
    return true;
  }
}





Static Void toggleprint()
{
  switch (printer) {

  case ' ':
    printer = 'p';
    break;

  case 'p':
    printer = 's';
    break;

  case 's':
    printer = ' ';
    break;
  }
  switch (printer) {

  case ' ':
    /* blank case */
    break;

  /*



*/
  case 'p':
    printf("printing top of stack all the time\n");
    break;

  case 's':
    printf("printing whole stack all the time\n");
    break;
  }
}



Static Void clear()
{
  while (thestack != NULL)
    pop();
}



Static Void decimal()
{
  long n;
  range top;

  if (!getone(&top))
    return;
  n = (long)floor(top.estimate + 0.5);
  if (n < 0) {
    printf("negative number of decimal places is not allowed\n");
    return;
  }
  rdecimal = n;
  pop();
  if (printer != ' ')
    printf("%ld decimal places\n", rdecimal);
}



Static Void duplicate()
{
  range top;

  if (getone(&top))
    push(top);
}



Static Void width()
{
  long n;
  range top;

  if (!getone(&top))
    return;
  n = (long)floor(top.estimate + 0.5);
  pop();
  if (n < 1) {
    if (printer != ' ')
      printf("non-positive width is not allowed\n");
    return;
  }
  rwidth = n;
  if (printer != ' ')
    printf("%ld width characters\n", rwidth);
}



Static Void dobase()
{
  double n;
  range top;

  if (!getone(&top))
    return;
  n = top.estimate;
  if (n <= 1.0) {
    printf("bases less than or equal to 1 are not allowed\n");
    return;
  }
  base = n;
  pop();
  if (printer == ' ')
    return;
  printf(" base for logarithm and exponentiation is ");
  if (rdecimal > 0)
    printf("%*.*f\n", (int)rwidth, (int)rdecimal, base);
  else
    printf("%*ld\n", (int)rwidth, (long)floor(base + 0.5));
}



Static Void donotation()
{
  notation = !notation;
  if (printer == ' ')
    return;
  if (notation)
    printf("scientific notation\n");
  else
    printf("normal numeric notation\n");
}



Static Void absolute()
{
  range a;

  if (!getone(&a))
    return;
  pop();
  a.estimate = fabs(a.estimate);
  push(a);
}



Static Void truncate()
{
  range a;

  if (!getone(&a))
    return;
  pop();
  a.estimate = (long)a.estimate;
  push(a);
}



Static Void add()
{
  /*
*/
  range a, b, aplusb;

  if (!gettwo(&a, &b))
    return;
  addrange(a, b, &aplusb);
  pop();
  pop();
  push(aplusb);
}



Static Void subtract()
{
  /*
*/
  range a, b, bminusa;

  if (!gettwo(&a, &b))
    return;
  minusrange(b, a, &bminusa);
  pop();
  pop();
  push(bminusa);
}



Static Void squareroot()
{
  /*
*/
  range a, roota, half;

  if (!getone(&a))
    return;
  half.estimate = 0.5;
  half.error = 0.0;
  if (a.estimate < 0.0) {
    printf("cannot take square root of negative real number\n");
    return;
  }
  if (a.estimate > 0.0)
    exprange(a, half, &roota);
  else {
    roota.estimate = 0.0;
    roota.error = 0.0;
  }

  pop();
  push(roota);
}



Static Void multiply()
{
  /*
*/
  range a, b, atimesb;

  if (!gettwo(&a, &b))
    return;
  timesrange(a, b, &atimesb);
  pop();
  pop();
  push(atimesb);
}



Static Void divide()
{
  /*
*/
  range a, b, bovera;

  if (!gettwo(&a, &b))
    return;
  dividerange(b, a, &bovera);
  pop();
  pop();
  push(bovera);
}



Static Void dolog()
{
  range a, top;

  if (!getone(&top))
    return;
  if (top.estimate > 0.0) {
    logrange(top, base, &a);
    setrange(a, &thestack->n);
  } else
    printf("top number on the stack is not positive, cannot take logarithm\n");
}



Static Void dolatex()
{
  latex = !latex;
  if (latex)
    memcpy(pmstring, pmstring1, 5L);
  else {
    memcpy(pmstring, pmstring2, 5L);

  }
}



Static Void doexp()
{
  /*





*/
  range a, top;

  if (!getone(&top))
    return;
  pop();
  a.estimate = exp(top.estimate);
  a.error = a.estimate * top.error;
  push(a);
}



Static Void douncertainty()
{
  /*
*/
  range a, logp, top;

  if (!getone(&top))
    return;
  if (top.estimate < 0.0) {
    printf("u command: cannot take log because p is less than zero\n");
    return;
  }
  if (top.estimate > 1.0)
    printf("u command: WARNING: p was greater than or 1.0\n");
  pop();
  if (top.estimate > 0.0) {
    logrange(top, 2.0, &logp);
    timesrange(top, logp, &a);
    a.estimate = fabs(a.estimate);
  } else {
    a.estimate = 0.0;
    a.error = 0.0;
  }
  push(a);
}



Static Void simpson(upper, answer, tol)
double upper, *answer, *tol;
{
  /*













*/
  long i;

  double x, x1, x2;
  /*
*/
  double pi = 4.0 * atan(1.0);
  double val;
  /*

*/
  double deltax;
  double evensum = 0.0;
  double oddsum, endsum, endcor, sum1;
  /*

*/
  long pieces = 2;
  double lower = 0.0;
  double sum;

  val = 1 / sqrt(2 * pi);

  upper = fabs(upper);
  /*

*/
  *tol = 1.0e-6;
  deltax = (upper - lower) / pieces;
  x = lower + deltax;
  oddsum = val * exp(-0.5 * x * x);
  x1 = lower;
  x2 = upper;
  endsum = val * exp(-0.5 * x1 * x1) + val * exp(-0.5 * x2 * x2);
  endcor = x2 * val * exp(-0.5 * x2 * x2) - x1 * val * exp(-0.5 * x1 * x1);
  sum = (endsum + 4.0 * oddsum) * deltax / 3.0;

  do {
    pieces *= 2;
    sum1 = sum;
    deltax = (upper - lower) / pieces;
    evensum += oddsum;
    oddsum = 0.0;
    for (i = 1; i <= pieces / 2; i++) {
      x = lower + deltax * (2.0 * i - 1.0);
      oddsum += val * exp(-0.5 * x * x);
    }
    sum = (7.0 * endsum + 14.0 * evensum + 16.0 * oddsum + endcor * deltax) *
	  deltax / 15.0;
  } while (fabs(sum - sum1) > fabs(*tol * sum));
  *answer = 0.5 - sum;
  if (*answer < 0.0)
    *answer = 0.0;
  /*

*/
}



Static Void dogaussian()
{
  /*
*/
  range a, Z;

  if (!getone(&Z))
    return;
  pop();
  if (fabs(Z.estimate) > zbound) {
    a.estimate = 0.0;
    a.error = 1.0 / LONG_MAX;
    printf("warning: z > %ld; returning zero for Gaussian\n", (long)zbound);
  } else
    simpson(Z.estimate, &a.estimate, &a.error);
  push(a);
}



Static Void doz()
{
  /*

*/
  range first, second;

  if (!gettwo(&first, &second))
    return;
  pop();
  pop();
  if (second.error <= 0)
    printf("error of second number on stack must be positive to do Z function\n");
  else {
    first.estimate = (first.estimate - second.estimate) / second.error;
    first.error = 0.0;
  }
  push(second);
  push(first);
}



Static Void doraise()
{
  range a, first, second;

  if (!gettwo(&first, &second))
    return;
  pop();
  pop();
  exprange(second, first, &a);
  push(a);
}



Static Void swap_()
{
  range first, second;

  if (!gettwo(&first, &second))
    return;
  pop();
  pop();
  push(first);
  push(second);
}



Static Void join()
{
  /*

*/
  range first, second;
  double average, sigma;

  if (!gettwo(&first, &second))
    return;
  pop();
  pop();

  if (fromto) {
    average = (first.estimate + second.estimate) / 2;
    sigma = fabs(average - first.estimate);
    first.estimate = average;
    first.error = sigma;
    push(first);
    if (printer != ' ')
      printf("joined as from-to\n");
    return;
  }
  second.error = fabs(first.estimate);

  if (first.estimate < 0)
    printf("(Note: the error was made to be positive number.)\n");

  push(second);
  if (printer != ' ')
    printf("joined as estimate-error\n");
}



Static Void helpalways(f)
_TEXT *f;
{
  fprintf(f->f, "\nFor more help type one of these commands:\n");
  fprintf(f->f, "1h gives general help for calc\n");
  fprintf(f->f, "2h gives help for display of numbers\n");
  fprintf(f->f, "3h gives help for functions\n");
  fprintf(f->f, "4h gives help for commands\n\n");
  fprintf(f->f,
	  "NOTE: use the form 4'h (for example) when in \":\" mode.\n\n");
}



Static Void helpcommand(f)
_TEXT *f;
{
  fprintf(f->f, "Help for Commands\n");
  fprintf(f->f, "-----------------\n");

  fprintf(f->f, "c clear the stack\n");
  fprintf(f->f,
	  "b top number of stack is used as the base for taking logarithms\n");
  if (rdecimal > 0)
    fprintf(f->f, "  (currently %*.*f)\n", (int)rwidth, (int)rdecimal, base);
  else
    fprintf(f->f, "  (currently %*ld)\n",
	    (int)rwidth, (long)floor(base + 0.5));
  if (base == exp(1.0))
    fprintf(f->f, "  (this is e, which gives the natural logarithm)\n");
  else
    fprintf(f->f,
      "  (Note: to get back to natural logs use the commands \"1eb\")\n");
  fprintf(f->f,
    "d top number of stack is used as the number of decimal places to print\n");
  fprintf(f->f, "  (currently %ld)\n", rdecimal);
  fprintf(f->f,
    "j join: first number on stack becomes error of the second number\n");
  fprintf(f->f, "  In from-to mode, the estimates of the top two numbers\n");
  fprintf(f->f, "  become the range.\n");
  fprintf(f->f,
	  "L toggle between LaTeX and regular number notation: \\pm vs +/-\n");
  fprintf(f->f, "n toggle between scientific and regular number notation\n");
  fprintf(f->f, "  (currently ");
  if (notation)
    fprintf(f->f, "scientific");
  else
    fprintf(f->f, "regular");
  fprintf(f->f, " notation)\n");
  fprintf(f->f, "p print top number on the stack\n");
  fprintf(f->f, "s print the entire stack\n");

  fprintf(f->f,
	  "t toggles the way the stack is shown between three states:\n");
  fprintf(f->f, "  silent, print top number only, print whole stack.");
  fprintf(f->f, "  (currently ");
  switch (printer) {

  case ' ':
    fprintf(f->f, "SILENT)\n");
    break;

  case 'p':
    fprintf(f->f, "PRINT TOP)\n");
    break;

  case 's':
    fprintf(f->f, "PRINT STACK)\n");
    break;
  }

  fprintf(f->f, "q quit the program\n");
  fprintf(f->f,
	  "w top number of stack is used as the width of numbers to print\n");
  fprintf(f->f, "  (currently %ld)\n", rwidth);
  fprintf(f->f, "x throw away the top number on the stack\n");
  fprintf(f->f, "y rolldown: top number on stack goes to bottom of stack\n");
  fprintf(f->f, "~ swap the top two numbers on the stack\n");
  fprintf(f->f, ", duplicate the top number on the stack\n");
  fprintf(f->f, ". do not show error ranges and do not read new ones,\n");
  fprintf(f->f,
	  "  (but keep calculating them if they had already been read)\n");
  fprintf(f->f, ": do show and do error ranges\n");
  fprintf(f->f, "# make a comment: calc will ignore the rest of the line\n");
  fprintf(f->f, "m make a message: like #, but appears in silent mode.\n");
  fprintf(f->f,
	  "  A \"\\\" at the end of the line suppresses carriage returns\n");
  fprintf(f->f,
	  "\\ as a command toggles carriage returns on and off in p and s\n");

}



Static Void helpfunction(f)
_TEXT *f;
{
  fprintf(f->f, "Help for Functions\n");
  fprintf(f->f, "------------------\n");

  fprintf(f->f,
	  "a absolute value of top number on stack, put on top of stack\n");
  fprintf(f->f, "T truncate top number on stack, put on top of stack\n");
  fprintf(f->f, "+ ADD top two numbers on the stack, put on top of stack\n");
  fprintf(f->f,
    "_ (UNDERSCORE) SUBTRACT top two numbers on the stack, put on top of stack\n");
  fprintf(f->f,
	  "* MULTIPLY top two numbers on the stack, put on top of stack\n");
  fprintf(f->f,
    "/ DIVIDE second number by first number on the stack put on top of stack\n");
  fprintf(f->f,
	  "^ raise second number to first number on the stack, put on top\n");
  fprintf(f->f, "e raise e to the first number on the stack, put on top\n");
  fprintf(f->f,
	  "g convert the Gaussian Z value to a probability, put on top\n");
  fprintf(f->f, "l replace top number on stack by its LOG to the base \n");
  if (rdecimal > 0)
    fprintf(f->f, "  (currently %*.*f)\n", (int)rwidth, (int)rdecimal, base);
  else
    fprintf(f->f, "  (currently %*ld)\n",
	    (int)rwidth, (long)floor(base + 0.5));
  if (base == exp(1.0))
    fprintf(f->f, "  (this is e, which gives the natural logarithm)\n");
  fprintf(f->f, "r replace top number on stack by its SQUARE ROOT\n");
  fprintf(f->f,
    "u uncertainty: if p is the top of the stack, replace it with -p*log2(p)\n");
  fprintf(f->f,
    "z replace top number on stack by its Z distance from second number\n");
  fprintf(f->f,
    "%% poisson: error of top number on stack is square root of estimate\n");
  fprintf(f->f,
	  "@ toggle range display between \"8 +/- 3\" and \"[5 to 11]\".\n");
  fprintf(f->f,
	  "  Not only will numbers show up differently, but they will\n");
  fprintf(f->f, "  be READ IN DIFFERENTLY when the : is in operation.\n");
  fprintf(f->f,
	  "  eg: after starting calc, \"0d1w :@ 5 11\" gives \"[5 to 11]\"\n");
  fprintf(f->f, "! extra special cowculation\n");

}



Static Void helpgeneral(f)
_TEXT *f;
{
  fprintf(f->f, "General Help for Calc version %4.2f\n", version);
  fprintf(f->f, "----------------------------------\n\n");

  fprintf(f->f, "The calc program is a reverse polish calculator that\n");
  fprintf(f->f,
	  "propagates errors.  The program is based on the idea of the\n");
  fprintf(f->f,
	  "dc program under UNIX.  That program takes input as reverse\n");
  fprintf(f->f,
	  "polish and calculates values.  This program does the same,\n");
  fprintf(f->f, "but values have estimates so one may calculate and\n");
  fprintf(f->f, "propagate errors.  Tokens (commands and numbers) are\n");
  fprintf(f->f, "usually separated by spaces or carriage returns.  Tokens\n");
  fprintf(f->f, "that begin with a digit or a dash (-) are numbers.\n\n");

  fprintf(f->f,
	  "Normally the program does not show you the errors, and you\n");
  fprintf(f->f,
	  "run it like a regular calculator.  For example, to average\n");
  fprintf(f->f,
	  "three numbers, one enters them, adds them together, and then\n");
  fprintf(f->f,
	  "divides by three:  \"3 4 5 + + 3 /\".  Try this with carriage\n");
  fprintf(f->f,
	  "returns between the commands to see how the stack changes.\n\n");

  fprintf(f->f, "The methods of calculation are given in:\n");
  fprintf(f->f, "An Introduction to Error Analysis,  John R. Taylor\n");
  fprintf(f->f, "University Science Books, Mill Valley, CA. 1982.\n\n");
  fprintf(f->f, "Tom Schneider\n");
  fprintf(f->f, "toms@alum.mit.edu\n");

}



Static Void helpdisplay(f)
_TEXT *f;
{
  fprintf(f->f, "Help for displaying numbers in Calc\n");
  fprintf(f->f, "-----------------------------------\n");

  fprintf(f->f,
	  "To enter errors and display them, type \":\".  The prompt of\n");
  fprintf(f->f,
	  "the program will change from \".\" to \":\" to indicate this.\n");
  fprintf(f->f,
	  "Now every pair of numbers you type represents a number and\n");
  fprintf(f->f,
	  "its error range.  Thus \"5 2\" means \"5 +/- 2\" and it will be\n");
  fprintf(f->f,
	  "displayed that way.  A single quote, as in \"5'\" means to\n");
  fprintf(f->f, "set the error to zero.\n\n");
  fprintf(f->f,
    "You can also enter numbers as a range by flipping the toggle \"@\".\n");
  fprintf(f->f, "The prompt will become \"@:\" to indicate this.\n");
  fprintf(f->f, "Now every pair of numbers you type represents a range.\n");
  fprintf(f->f, "Thus \"1 5\" means the range [1 to 5] or \"3 +/- 2\".\n");
  fprintf(f->f, "The numbers shown on the stack will reflect this.\n\n");
  fprintf(f->f,
    "You can see things in scientific notation with the toggle \"n\".\n");
  fprintf(f->f, "This has no effect on how you enter numbers.\n\n");

  fprintf(f->f,
	  "Calculation is not affected by the display.  So you can enter\n");
  fprintf(f->f,
	  "numbers as errors or ranges, switch to \"period\" mode, do your\n");
  fprintf(f->f,
	  "calculations without the clutter, then switch back to \"colon\"\n");
  fprintf(f->f, "to see them.\n");

}



Static Void help(f)
_TEXT *f;
{
  range top;
  long t;

  putc('\n', f->f);
  /*
*/
  if (thestack == NULL)
    helpgeneral(f);
  else {
    if (getone(&top)) {
      t = (long)floor(top.estimate + 0.5);
      pop();
      /*
*/
      if (t < 1 || t > 4)
	helpgeneral(f);
      else {
	switch (t) {

	case 1:
	  helpgeneral(f);
	  break;

	case 2:
	  helpdisplay(f);
	  break;

	case 3:
	  helpfunction(f);
	  break;

	case 4:
	  helpcommand(f);
	  break;
	}
      }
    } else
      printf("program error in procedure help\n");
  }

  helpalways(f);
}



Static Void print()
{
  range top;
  _TEXT TEMP;

  if (!getone(&top))
    return;
  TEMP.f = stdout;
  *TEMP.name = '\0';
  writerange(&TEMP, top, rwidth, rdecimal, notation);
  if (carriagereturn) {
    putchar('\n');
    /*


*/
  }
}



Static Void printstack()
{
  range top;
  stack *s;
  _TEXT TEMP;

  if (!getone(&top))
    return;
  printf("stack:\n");
  s = thestack;
  while (s != NULL) {
    TEMP.f = stdout;
    *TEMP.name = '\0';
    writerange(&TEMP, s->n, rwidth, rdecimal, notation);
    if (carriagereturn)
      putchar('\n');
    else
      putchar(' ');
    s = s->next;
  }
}



Static Void prompt(fout)
_TEXT *fout;
{
  if (printer == ' ')
    return;
  if (notation)
    putc('n', fout->f);
  if (!showingerrors) {
    fprintf(fout->f, ". ");
    return;
  }
  if (fromto)
    putc('@', fout->f);
  fprintf(fout->f, ": ");
}



Static Void poisson()
{
  /*
*/
  duplicate();
  squareroot();
  join();
}



Static Void dofromto()
{
  fromto = !fromto;
}



Static Void docomment(fin, fout)
_TEXT *fin, *fout;
{
  if (printer != ' ')
    fprintf(fout->f, "# ");

  while (!P_eoln(fin->f)) {
    CPUTFBUF(fout->f, P_peek(fin->f));
    if (printer != ' ')
      CPUT(fout->f);
    getc(fin->f);
  }

  if (printer != ' ')
    putc('\n', fout->f);
}



Static Void docow()
{
  /*


*/
  printf("\n                         (__)\n");
  printf("                         (oo)\n");
  printf("          / ------------- \\/\n");
  printf("         / (   |7 8 9 +|  )\n");
  printf("        *  (   |4 5 6 -|  )\n");
  printf("            (  |1 2 3 X|  )\n");
  printf("             ( |C 0 = /| )\n");
  printf("              ^---------^\n");
  printf("              ||       ||\n");
  printf("              ||       ||\n");
  printf("              ^^       ^^\n");
  printf("              \"COWCULATOR\"\n\n");

  printf("Dudley's Criteria\n\n");
  printf("The proof may seem to be unsatisfying: each step is correct, and hence\n");
  printf("the conclusion is true, but it is not clear why the steps are there\n");
  printf("and where they came from. That is because there are at least eight\n");
  printf("levels of mathematical understanding, and it is hard for someone on a\n");
  printf("lower level to appreciate what goes on at a higher level. The levels\n");
  printf("are, I think:\n\n");
  printf("1. Being able to do arithmetic.\n\n");
  printf("2. Being able to substitute numbers in formulas.\n\n");
  printf("3. Given formulas, being able to get other formulas.\n\n");
  printf("4. Being able to understand the hypotheses and conclusions of\n");
  printf("theorems.\n\n");
  printf("5. Being able to understand the proofs of theorems, step by step.\n\n");
  printf("6. Being able to _really_ understand the proofs of theorems: that is,\n");
  printf("seeing why the proof is as it is, and comprehending the inwardness of\n");
  printf("the theorem and its relation to other theorems.\n\n");
  printf("7. Being able to generalize and extend theorems.\n\n");
  printf("8. Being able to see new relationships and discover and prove entirely\n");
  printf("new theorems.\n\n");
  printf("Those of us stuck on level 5 can no more understand the workings of a\n");
  printf("level 8 mind than a cow could understand calculus.\n\n");
  printf("Elementary Number Theory, 2nd ed, page 103-104, by Underwood Dudley.\n");
  printf("W. H. Freeman & co. 1969, second edition 1978 DUDLEY@DEPAUW.EDU\n");

}



Static Void togglereturn()
{
  carriagereturn = !carriagereturn;
}



Static Void domessage(fin, fout)
_TEXT *fin, *fout;
{
  /*



*/
  Char lastcharacter;
  boolean suppress = false;

  /*

*/
  while (!P_eoln(fin->f)) {
    lastcharacter = getc(fin->f);
    if (lastcharacter == '\n')
      lastcharacter = ' ';
    suppress = (lastcharacter == '\\') & P_eoln(fin->f);
    if (!suppress)
      putc(lastcharacter, fout->f);
  }

  if (!suppress)
    putc('\n', fout->f);
}



Static Void themain(fin, fout)
_TEXT *fin, *fout;
{
  range a;
  Char achar;
  boolean done = false;
  boolean ok;

  fprintf(fout->f, "calc %4.2f\n", version);
  initialize();
  prompt(fout);




  do {
    skipblanks(fin);

    if (P_eoln(fin->f)) {
      fscanf(fin->f, "%*[^\n]");
      getc(fin->f);
      prompt(fout);
    } else {
      if (P_peek(fin->f) == '9' || P_peek(fin->f) == '8' ||
	  P_peek(fin->f) == '7' || P_peek(fin->f) == '6' ||
	  P_peek(fin->f) == '5' || P_peek(fin->f) == '4' ||
	  P_peek(fin->f) == '3' || P_peek(fin->f) == '2' ||
	  P_peek(fin->f) == '1' || P_peek(fin->f) == '0' ||
	  P_peek(fin->f) == '-') {
	readrange(fin, &a, &ok, showingerrors);
	if (ok)
	  push(a);
      }

      else {
	achar = getc(fin->f);
	if (achar == '\n')
	  achar = ' ';
	if (achar == 'a')
	  absolute();
	else if (achar == 'c')
	  clear();
	else if (achar == 'b')
	  dobase();
	else if (achar == 'd')
	  decimal();
	else if (achar == ',')
	  duplicate();
	else if (achar == '^')
	  doraise();
	else if (achar == 'e')
	  doexp();
	else if (achar == 'g')
	  dogaussian();
	else if (achar == 'h')
	  help(fout);
	else if (achar == '?')
	  help(fout);
	else if (achar == 'j')
	  join();
	else if (achar == 'l')
	  dolog();
	else if (achar == 'L')
	  dolatex();
	else if (achar == 'm')
	  domessage(fin, fout);
	else if (achar == 'n')
	  donotation();
	else if (achar == 'p')
	  print();
	else if (achar == 's')
	  printstack();
	else if (achar == 't')
	  toggleprint();
	else if (achar == 'u')
	  douncertainty();
	else if (achar == 'q')
	  done = true;
	else if (achar == 'w')
	  width();
	else if (achar == 'x')
	  pop();
	else if (achar == 'y')
	  rolldown();
	else if (achar == 'z')
	  doz();
	else if (achar == 'T')
	  truncate();
	else if (achar == '+')
	  add();
	else if (achar == '_')
	  subtract();
	else if (achar == 'r')
	  squareroot();
	else if (achar == '*')
	  multiply();
	else if (achar == '/')
	  divide();
	else if (achar == '~')
	  swap_();
	else if (achar == '.')
	  showingerrors = false;
	else if (achar == ':')
	  showingerrors = true;
	else if (achar == '%')
	  poisson();
	else if (achar == '@')
	  dofromto();
	else if (achar == '#')
	  docomment(fin, fout);
	else if (achar == '!')
	  docow();
	else if (achar == '\\')
	  togglereturn();
	else
	  fprintf(fout->f, "unrecognized command: %c\n", achar);
      }

      if (achar != 'q' && achar != 's' && achar != 'p') {
	switch (printer) {

	case ' ':
	  /* blank case */
	  break;

	case 'p':
	  print();
	  break;

	case 's':
	  printstack();
	  break;
	}
      }
    }
  } while (!done);
}


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT TEMP, TEMP1;

  PASCAL_MAIN(argc, argv);
  TEMP.f = stdin;
  *TEMP.name = '\0';
  TEMP1.f = stdout;
  *TEMP1.name = '\0';
  themain(&TEMP, &TEMP1);
  exit(EXIT_SUCCESS);
}



/* End. */
