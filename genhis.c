/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "genhis.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*









*/



#define version         1.99
/*



























*/



/*

































































































*/



#define e               2.71828459045
#define pi              3.14159265358974



#define maxslots        20000
#define defslots        100
#define pageheight      53
/*


*/

#define dch             '+'
#define sch             ':'
#define bch             '*'

#define defaultwid      12
#define defaultdec      8
#define defaultwidcount  defaultwid



typedef enum {
  none, gaussian, poisson
} plots;
/*
*/
typedef enum {
  start, stop
} endpoints;

typedef struct histarray {
  long numbers[maxslots];
  double rnumbers[maxslots];
  double range[2];
  double interval;
  long slots;
} histarray;



Static _TEXT genhisp, data, histog, curve;
Static long wid, dec, widfrequency, decfrequency, widcount;
Static boolean dofrequency, documulativefrequency;



Static histarray dataarray, stanarray;
Static long hlines, column, entries;
Static double min, max, sum, ave, scaling, vari, uncertainty;
Static plots standplot;
Static boolean splot, curvedata;
Static double maxgausscurve;


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









Static boolean isblankDelila(c)
Char c;
{
  return (c == ' ' || c == '\t');
}


Static Void skipblanks(thefile)
_TEXT *thefile;
{
  while (isblankDelila(P_peek(thefile->f)) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


Static Void skipnonblanks(thefile)
_TEXT *thefile;
{
  while ((!isblankDelila(P_peek(thefile->f))) & (!P_eoln(thefile->f)))
    getc(thefile->f);
}


/*











*/

Static Void skipcolumn(thefile)
_TEXT *thefile;
{
  skipblanks(thefile);
  skipnonblanks(thefile);
}



Static Void setparam()
{
  /*





















*/
  Char ch;
  long c, s, i;

  wid = defaultwid;
  dec = defaultdec;
  widfrequency = defaultwid;
  decfrequency = defaultdec;
  widcount = defaultwidcount;
  dofrequency = false;
  documulativefrequency = false;
  if (*data.name != '\0') {
    if (data.f != NULL)
      data.f = freopen(data.name, "r", data.f);
    else
      data.f = fopen(data.name, "r");
  } else
    rewind(data.f);
  if (data.f == NULL)
    _EscIO2(FileNotFound, data.name);
  RESETBUF(data.f, Char);
  if (BUFEOF(data.f)) {
    printf(" data file empty\n");
    halt();
  }
  if (*histog.name != '\0') {
    if (histog.f != NULL)
      histog.f = freopen(histog.name, "w", histog.f);
    else
      histog.f = fopen(histog.name, "w");
  } else {
    if (histog.f != NULL)
      rewind(histog.f);
    else
      histog.f = tmpfile();
  }
  if (histog.f == NULL)
    _EscIO2(FileNotFound, histog.name);
  SETUPBUF(histog.f, Char);
  fprintf(histog.f, "* genhis %4.2f: generalized histogram data from\n",
	  version);


  hlines = 0;
  column = 1;
  standplot = none;
  dataarray.slots = defslots;
  /*
*/
  dataarray.range[(long)start] = 1.0;
  dataarray.range[(long)stop] = 0.0;

  if (*genhisp.name != '\0') {
    if (genhisp.f != NULL)
      genhisp.f = freopen(genhisp.name, "r", genhisp.f);
    else
      genhisp.f = fopen(genhisp.name, "r");
  } else
    rewind(genhisp.f);
  if (genhisp.f == NULL)
    _EscIO2(FileNotFound, genhisp.name);
  RESETBUF(genhisp.f, Char);
  while (!BUFEOF(genhisp.f)) {
    ch = getc(genhisp.f);
    if (ch == '\n')
      ch = ' ';
    if (ch != 'F' && ch != 'f' && ch != 'n' && ch != 'w' && ch != 'd' &&
	ch != 'r' && ch != 'c' && ch != 'x' && ch != 'p' && ch != 'h') {
      printf("genhisp commands must begin with one of: hpxcrdwfF, not \"%c\".\n",
	     ch);

      halt();
      continue;
    }

    switch (ch) {

    case 'h':
      fscanf(genhisp.f, "%ld%ld%*[^\n]", &c, &s);
      getc(genhisp.f);
      if (c < 0) {
	c = 0;
	while (P_peek(data.f) == '*') {
	  fprintf(histog.f, "* ");
	  copyaline(&data, &histog);
	  c++;
	}
      }

      else {
	i = copylines(&data, &histog, c);
	if (i < c) {
	  printf(" parameter error:  header too short\n");
	  halt();
	}
      }

      for (i = 1; i <= s; i++) {
	fscanf(data.f, "%*[^\n]");
	getc(data.f);
      }
      /*
*/
      hlines += c + s;
      break;

    case 'p':
      skipblanks(&genhisp);
      fscanf(genhisp.f, "%c%*[^\n]", &ch);
      getc(genhisp.f);
      if (ch == '\n')
	ch = ' ';
      if (ch != 'P' && ch != 'G' && ch != 'n' && ch != 'p' && ch != 'g') {
	printf("genhisp plot command can accept only g G p P or n commands\n");
	halt();
      }
      switch (ch) {

      case 'g':
	standplot = gaussian;
	break;

      case 'G':
	standplot = gaussian;
	break;

      case 'p':
	standplot = poisson;
	break;

      case 'P':
	standplot = poisson;
	break;

      case 'n':
	standplot = none;
	break;
      }
      if (ch == 'P' || ch == 'G')
	curvedata = true;
      else
	curvedata = false;
      break;

    case 'x':
      skipblanks(&genhisp);
      ch = getc(genhisp.f);
      if (ch == '\n')
	ch = ' ';
      if (ch == 's') {
	fscanf(genhisp.f, "%lg%*[^\n]", &dataarray.interval);
	getc(genhisp.f);
	if (dataarray.interval <= 0.0) {
	  printf(" error in parameter file:  interval size must be positive\n");
	  halt();
	}
	dataarray.slots = 0;
      } else if (ch == 'n') {
	fscanf(genhisp.f, "%ld%*[^\n]", &dataarray.slots);
	getc(genhisp.f);
	if (dataarray.slots < 1) {
	  printf(" error in parameter file: number of slots must be positive\n");
	  halt();
	} else if (dataarray.slots > maxslots) {
	  printf(" error in parameter file: number of slots must be less than %ld\n",
		 (long)maxslots);
	  halt();
	}
      } else {
	printf(" x parameter must be followed by \"s\" or \"n\".\n");
	halt();
      }
      break;

    case 'c':
      fscanf(genhisp.f, "%ld%*[^\n]", &column);
      getc(genhisp.f);
      break;

    case 'r':
      fscanf(genhisp.f, "%lg%lg%*[^\n]", dataarray.range,
	     &dataarray.range[(long)stop]);
      getc(genhisp.f);
      break;

    case 'd':
      fscanf(genhisp.f, "%ld%*[^\n]", &dec);
      getc(genhisp.f);
      if (dec < 0) {
	printf("decimal places must not be negative\n");
	halt();
      }
      break;

    case 'w':
      fscanf(genhisp.f, "%ld%*[^\n]", &wid);
      getc(genhisp.f);
      if (wid < 1) {
	printf("wid places must be positive\n");
	halt();
      }
      break;

    case 'n':
      fscanf(genhisp.f, "%ld%*[^\n]", &widcount);
      getc(genhisp.f);
      if (widcount < 1) {
	printf("widcount places must be positive\n");
	halt();
      }
      break;

    case 'f':
    case 'F':
      skipblanks(&genhisp);
      if (!P_eoln(genhisp.f)) {
	fscanf(genhisp.f, "%ld", &widfrequency);
	skipblanks(&genhisp);
	if (!P_eoln(genhisp.f))
	  fscanf(genhisp.f, "%ld", &decfrequency);
      }
      fscanf(genhisp.f, "%*[^\n]");
      getc(genhisp.f);
      dofrequency = true;
      if (ch == 'F')
	documulativefrequency = true;
      break;


    }
  }
  /*
*/
  if (hlines == 0) {
    while (P_peek(data.f) == '*') {
      copyaline(&data, &histog);
      hlines++;
    }
  }
  fprintf(histog.f, "*\n");
}



/*
*/














Static Void datahist(thefile, hlines, column, entries, min, max, sum, ave,
		     vari, dataarray)
_TEXT *thefile;
long hlines, column, *entries;
double *min, *max, *sum, *ave, *vari;
histarray *dataarray;
{

  /*





*/
  double num;
  long int_, i, j;
  double frequency;
  long bubbaline = 1;
  long FORLIM;

  if (*thefile->name != '\0') {
    if (thefile->f != NULL)
      thefile->f = freopen(thefile->name, "r", thefile->f);
    else
      thefile->f = fopen(thefile->name, "r");
  } else
    rewind(thefile->f);
  if (thefile->f == NULL)
    _EscIO2(FileNotFound, thefile->name);
  RESETBUF(thefile->f, Char);
  if (BUFEOF(thefile->f)) {
    printf(" data file is empty\n");
    halt();
  }
  for (i = 1; i <= hlines; i++) {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
    bubbaline++;
  }
  *entries = 0;
  *sum = 0.0;


  while (!BUFEOF(thefile->f)) {
    if (P_peek(thefile->f) == '*') {
      fscanf(thefile->f, "%*[^\n]");
      /*


*/


      getc(thefile->f);
      bubbaline++;
      continue;
    }

    for (i = 2; i <= column; i++) {
      skipblanks(thefile);
      skipnonblanks(thefile);
    }
    skipblanks(thefile);
    /*






*/

    if (P_eoln(thefile->f)) {
      fscanf(thefile->f, "%*[^\n]");
      getc(thefile->f);
      bubbaline++;
      /*

*/
      continue;
    }
    /*

*/
    if (P_peek(thefile->f) != '9' && P_peek(thefile->f) != '8' &&
	P_peek(thefile->f) != '7' && P_peek(thefile->f) != '6' &&
	P_peek(thefile->f) != '5' && P_peek(thefile->f) != '4' &&
	P_peek(thefile->f) != '3' && P_peek(thefile->f) != '2' &&
	P_peek(thefile->f) != '1' && P_peek(thefile->f) != '0' &&
	P_peek(thefile->f) != '+' && P_peek(thefile->f) != '-') {
      printf("BUBBA found a BAD NUMBER on line %ld column %ld\n",
	     bubbaline, column);
      printf("The \"number\" begins with the character \"%c\".\n",
	     P_peek(thefile->f));
      printf("BUBBA MY OL' BUDDY!!  Saves the day again!\n");
      halt();
    }
    fscanf(thefile->f, "%lg%*[^\n]", &num);
    getc(thefile->f);
    bubbaline++;
    /*

*/
    (*entries)++;
    /*
*/
    if (*entries == 1) {
      *min = num;
      *max = num;
    } else {
      if (num < *min)
	*min = num;
      if (num > *max)
	*max = num;
    }
    *sum += num;
    /*

*/
  }


  if (*entries <= 0)
    return;
  *ave = *sum / *entries;


  /*
*/
  if (*thefile->name != '\0') {
    if (thefile->f != NULL)
      thefile->f = freopen(thefile->name, "r", thefile->f);
    else
      thefile->f = fopen(thefile->name, "r");
  } else
    rewind(thefile->f);
  if (thefile->f == NULL)
    _EscIO2(FileNotFound, thefile->name);
  RESETBUF(thefile->f, Char);
  for (i = 1; i <= hlines; i++) {
    fscanf(thefile->f, "%*[^\n]");
    getc(thefile->f);
  }
  *vari = 0.0;
  /*
*/
  if (dataarray->range[(long)start] > dataarray->range[(long)stop]) {
    dataarray->range[(long)start] = *min;
    dataarray->range[(long)stop] = *max;
  }
  /*

*/
  if (dataarray->slots > 0)
    dataarray->interval = (dataarray->range[(long)stop] -
			   dataarray->range[(long)start]) / dataarray->slots;

  if (dataarray->interval == 0)
    dataarray->interval = 1.0;
  dataarray->slots =
    (long)((dataarray->range[(long)stop] - dataarray->range[(long)start]) /
	   dataarray->interval) + 1;

  if (dataarray->slots > maxslots) {
    printf(" the calculated number of slots (%ld) will not fit\n",
	   dataarray->slots);
    printf(" into the maximum number of slots (constant maxslots = %ld)\n",
	   (long)maxslots);
    halt();
  }


  FORLIM = dataarray->slots;
  for (i = 0; i < FORLIM; i++)
    dataarray->numbers[i] = 0;
  FORLIM = dataarray->slots;
  for (i = 0; i < FORLIM; i++)
    dataarray->rnumbers[i] = 0.0;


  /*



*/
  while (!BUFEOF(thefile->f)) {
    if (P_peek(thefile->f) == '*') {
      fscanf(thefile->f, "%*[^\n]");
      getc(thefile->f);
      continue;
    }

    for (j = 2; j <= column; j++) {
      skipblanks(thefile);
      skipnonblanks(thefile);
    }

    if (P_eoln(thefile->f)) {
      fscanf(thefile->f, "%*[^\n]");
      getc(thefile->f);
      continue;
    }
    fscanf(thefile->f, "%lg%*[^\n]", &num);
    getc(thefile->f);
    /*
*/
    if (num >= dataarray->range[(long)start] &&
	num <= dataarray->range[(long)stop]) {
      int_ = (long)((num - dataarray->range[(long)start]) / dataarray->interval) + 1;
      dataarray->numbers[int_-1]++;
    }
    *vari += (*ave - num) * (*ave - num);
  }

  *vari /= *entries - 1;


  uncertainty = 0.0;
  FORLIM = dataarray->slots;
  for (i = 0; i < FORLIM; i++) {
    if (dataarray->numbers[i] > 0.0) {
      frequency = (double)dataarray->numbers[i] / *entries;
      uncertainty -= frequency * log(frequency);
    }
  }
  uncertainty /= log(2.0);
}


#define pi_             3.14159265358974

#define tolerance       (-700)



Static Void gaushist(entries, ave, vari, stanarray, maxgausscurve)
long entries;
double ave, vari;
histarray *stanarray;
double *maxgausscurve;
{
  /*









*/
  /*


*/
  double sd, d1, d2, x, ex, exinner;
  long i, FORLIM;

  if (vari == 0.0) {
    printf("WARNING: There is no variance to the data\n");
    printf("so the Gaussian distribution cannot be plotted.\n");
    printf("No standard plot will be made.\n");
    fprintf(histog.f, "* no standard plot\n");
    splot = false;
    return;
  }
  sd = sqrt(vari);
  d1 = 2 * vari;
  d2 = sd * sqrt(2 * pi_);
  *maxgausscurve = entries / d2 * stanarray->interval;
  FORLIM = stanarray->slots;
  /*





*/
  /*





*/
  for (i = 0; i < FORLIM; i++) {
    x = stanarray->range[(long)start] + (i + 0.5) * stanarray->interval;
    /*





















*/

    exinner = (ave - x) * (x - ave) / d1;
    if (exinner < tolerance)
      ex = 0.0;
    else
      ex = exp(exinner) / d2 * stanarray->interval;
    /*


*/
    stanarray->rnumbers[i] = ex * entries;
    stanarray->numbers[i] = (long)floor(stanarray->rnumbers[i] + 0.5);
    /*


*/
  }
}

#undef pi_
#undef tolerance



Static Void stanplot(fout, stanarray)
_TEXT *fout;
histarray stanarray;
{
  double x;
  long i;

  if (*fout->name != '\0') {
    if (fout->f != NULL)
      fout->f = freopen(fout->name, "w", fout->f);
    else
      fout->f = fopen(fout->name, "w");
  } else {
    if (fout->f != NULL)
      rewind(fout->f);
    else
      fout->f = tmpfile();
  }
  if (fout->f == NULL)
    _EscIO2(FileNotFound, fout->name);
  SETUPBUF(fout->f, Char);
  fprintf(fout->f, "* position value\n");
  for (i = 0; i < stanarray.slots; i++) {
    x = stanarray.range[(long)start] + (i + 0.5) * stanarray.interval;
    /*


*/
    fprintf(fout->f, "% .*E %*.*f\n",
	    P_max((int)wid - 7, 1), x, (int)wid, (int)dec,
	    stanarray.rnumbers[i]);
    /*





*/
  }
}


#define pi_             3.14159265358974



Static Void poishist(entries, ave, stanarray)
long entries;
double ave;
histarray *stanarray;
{
  /*
















*/
  double x, lnex, ex;
  long i, FORLIM;

  if (ave <= 0) {
    printf(
      " warning: poisson not defined for ave <= 0, procedure poishist called but not used\n");
    return;
  }
  FORLIM = stanarray->slots;
  for (i = 0; i < FORLIM; i++) {
    x = stanarray->range[(long)start] + (i + 0.5) * stanarray->interval;

    if (x < 0) {
      ex = 0.0;

    } else if (x <= 1)
      ex = exp(-ave);
    else {
      lnex = x - ave + x * log(ave / x) - log(2 * pi_ * x) / 2;
      ex = exp(lnex) * stanarray->interval;
    }
    /*


*/
    stanarray->rnumbers[i] = ex * entries;
    stanarray->numbers[i] = (long)floor(stanarray->rnumbers[i] + 0.5);
  }

}

#undef pi_



Static Void hplot(thefile, scaling, splot, dataarray, stanarray, dch_, sch_,
		  bch_)
_TEXT *thefile;
double scaling;
boolean splot;
histarray dataarray, stanarray;
Char dch_, sch_, bch_;
{
  /*


*/
  long dval, sval, i, j;
  double frequency;
  double cumulativefrequency = 0.0;

  fprintf(thefile->f, "* interval  | histogram");

  if (dofrequency) {
    if (documulativefrequency)
      fprintf(thefile->f, " | cumulative");
    else
      fprintf(thefile->f, " | histogram");
  }

  if (curvedata)
    fprintf(thefile->f, " | curve");
  fprintf(thefile->f, "\n* beginning | number   ");

  if (dofrequency)
    fprintf(thefile->f, " | frequency");
  if (curvedata)
    fprintf(thefile->f, " | data");
  putc('\n', thefile->f);

  fprintf(thefile->f, "*\n");
  for (i = 0; i < dataarray.slots; i++) {
    dval = (long)floor(dataarray.numbers[i] * scaling + 0.5);


    if (dec == 0)
      fprintf(thefile->f, " %*ld %*ld ",
	(int)wid,
	(long)floor(dataarray.range[(long)start] + i * dataarray.interval + 0.5),
	(int)widcount, dataarray.numbers[i]);
    else
      fprintf(thefile->f, " %*.*f %*ld ",
	      (int)wid, (int)dec,
	      dataarray.range[(long)start] + i * dataarray.interval,
	      (int)widcount, dataarray.numbers[i]);
    if (dofrequency) {
      /*





*/
      frequency = (double)dataarray.numbers[i] / entries;
      if (documulativefrequency) {
	cumulativefrequency += frequency;
	fprintf(thefile->f, " %*.*f",
		(int)widfrequency, (int)decfrequency, cumulativefrequency);
      } else
	fprintf(thefile->f, " %*.*f",
		(int)widfrequency, (int)decfrequency, frequency);
    }


    if (curvedata) {
      if (dec == 0)
	fprintf(thefile->f, " %*ld   ",
		(int)wid, (long)floor(stanarray.rnumbers[i] + 0.5));
      else
	fprintf(thefile->f, " %*.*f   ",
		(int)wid, (int)dec, stanarray.rnumbers[i]);
    }

    if (splot) {
      putc(' ', thefile->f);
      sval = (long)floor(stanarray.numbers[i] * scaling + 0.5);
      if (dval < sval) {
	for (j = 1; j <= dval; j++)
	  putc(dch_, thefile->f);
	for (j = dval + 1; j < sval; j++)
	  putc(' ', thefile->f);
	putc(sch_, thefile->f);
      } else if (dval == sval && dval > 0) {
	for (j = 1; j < dval; j++)
	  putc(dch_, thefile->f);
	putc(bch_, thefile->f);
      } else {
	for (j = 1; j < sval; j++)
	  putc(dch_, thefile->f);
	if (sval > 0)
	  putc(bch_, thefile->f);
	for (j = sval + 1; j <= dval; j++)
	  putc(dch_, thefile->f);
      }
    } else {
      putc(' ', thefile->f);
      for (j = 1; j <= dval; j++)
	putc(dch_, thefile->f);
    }
    putc('\n', thefile->f);
  }
}



Static Void writeit()
{
  long maxval = 0;
  long i;
  double sd;
  boolean dzero;
  long FORLIM;

  /*
*/

  fprintf(histog.f, "* parameters:\n");

  fprintf(histog.f, "* %*ld  is the data column used\n", (int)wid, column);
  fprintf(histog.f, "* %*ld  numbers are in the file\n", (int)wid, entries);

  sd = sqrt(vari);

  if (dec == 0) {
    fprintf(histog.f, "* %*ld  is the minimum number\n",
	    (int)wid, (long)floor(min + 0.5));
    fprintf(histog.f, "* %*ld  is the maximum number\n",
	    (int)wid, (long)floor(max + 0.5));
    fprintf(histog.f, "* %*ld  is the MEAN\n",
	    (int)wid, (long)floor(ave + 0.5));
    fprintf(histog.f, "* %*ld  is the STANDARD DEVIATION\n",
	    (int)wid, (long)floor(sd + 0.5));
    fprintf(histog.f, "* %*ld  is the STANDARD ERROR OF THE MEAN (SEM)\n",
	    (int)wid, (long)floor(sd / sqrt(entries - 1.0) + 0.5));
    fprintf(histog.f, "* %*ld  is the variance\n",
	    (int)wid, (long)floor(vari + 0.5));
    fprintf(histog.f, "* %*ld  is the uncertainty in bits\n",
	    (int)wid, (long)floor(uncertainty + 0.5));
  } else {
    fprintf(histog.f, "* %*.*f  is the minimum number\n",
	    (int)wid, (int)dec, min);
    fprintf(histog.f, "* %*.*f  is the maximum number\n",
	    (int)wid, (int)dec, max);
    fprintf(histog.f, "* %*.*f  is the MEAN\n", (int)wid, (int)dec, ave);
    fprintf(histog.f, "* %*.*f  is the STANDARD DEVIATION\n",
	    (int)wid, (int)dec, sd);
    fprintf(histog.f, "* %*.*f  is the STANDARD ERROR OF THE MEAN (SEM)\n",
	    (int)wid, (int)dec, sd / sqrt(entries - 1.0));
    fprintf(histog.f, "* %*.*f  is the variance\n", (int)wid, (int)dec, vari);
    fprintf(histog.f, "* %*.*f  is the uncertainty in bits\n",
	    (int)wid, (int)dec, uncertainty);
  }


  if (dec == 0) {
    dec = 1;
    dzero = true;
  } else
    dzero = false;

  if (dzero)
    dec = 1;

  if (sd > 0)
    fprintf(histog.f,
	    "* %*.*f  is the computed uncertainty in bits (Shannon p.57)\n",
	    (int)wid, (int)dec, log(sqrt(2 * pi * e) * sd) / log(2.0));
  else
    fprintf(histog.f, "* 0 [sd <= 0, can't compute uncertainty from sd]\n");
  fprintf(histog.f, "*\n");
  fprintf(histog.f, "* %*.*f  to  %*.*f is the range of data plotted\n",
	  (int)wid, (int)dec, dataarray.range[(long)start], (int)wid,
	  (int)dec, dataarray.range[(long)stop]);
  fprintf(histog.f, "* %*.*f  is the x-axis interval\n",
	  (int)wid, (int)dec, dataarray.interval);
  fprintf(histog.f, "* %6ld     is the number of intervals\n",
	  dataarray.slots);

  FORLIM = dataarray.slots;
  for (i = 0; i < FORLIM; i++) {
    if (maxval < dataarray.numbers[i])
      maxval = dataarray.numbers[i];
  }


  if (maxval == 0)
    maxval = pageheight;
  scaling = (double)pageheight / maxval;
  if (scaling > 1)
    scaling = 1.0;
  fprintf(histog.f, "* %*ld is the maximum y-axis value\n", (int)wid, maxval);
  fprintf(histog.f, "* %*.*f is the y-axis scale\n",
	  (int)wid, (int)dec, scaling);


  fprintf(histog.f, "*\n");
  if (standplot == none) {
    fprintf(histog.f, "* no standard plot\n");
    splot = false;
  } else {
    splot = true;

    stanarray.range[(long)start] = dataarray.range[(long)start];
    stanarray.range[(long)stop] = dataarray.range[(long)stop];
    stanarray.slots = dataarray.slots;
    stanarray.interval = dataarray.interval;

    if (standplot == gaussian) {
      gaushist(entries, ave, vari, &stanarray, &maxgausscurve);
      stanplot(&curve, stanarray);
      if (splot == true)
	fprintf(histog.f, "* gaussian standard plotted\n");
      fprintf(histog.f, "* %*.*f is the maximum Gaussian curve value \n",
	      (int)wid, (int)dec, maxgausscurve);
    } else if (standplot == poisson) {

      if (ave > 0) {
	fprintf(histog.f, "* poisson standard plotted.  ");
	if (stanarray.range[(long)start] < 0) {
	  fprintf(histog.f, "* warning: poisson not defined for x < 0");
	  printf(" warning: poisson not defined for x < 0\n");
	}
	fprintf(histog.f, "*\n");
	poishist(entries, ave, &stanarray);
      } else {
	printf(
	  "warning: poisson not defined for ave <= 0, no standard will be plotted\n");
	splot = false;
      }
    }
  }
  fprintf(histog.f, "*\n");
  if (entries > 0) {
    /*

*/
    if (dzero)
      dec = 0;
    hplot(&histog, scaling, splot, dataarray, stanarray, dch, sch, bch);
  } else
    fprintf(histog.f,
	    "* (no numbers are in the file, so no histogram is made)\n");

  if (dzero)
    dec = 0;

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  curve.f = NULL;
  strcpy(curve.name, "curve");
  histog.f = NULL;
  strcpy(histog.name, "histog");
  data.f = NULL;
  strcpy(data.name, "data");
  genhisp.f = NULL;
  strcpy(genhisp.name, "genhisp");
  printf(" genhis %4.2f\n", version);
  setparam();
  datahist(&data, hlines, column, &entries, &min, &max, &sum, &ave, &vari,
	   &dataarray);
  writeit();

_L1:
  if (genhisp.f != NULL)
    fclose(genhisp.f);
  if (data.f != NULL)
    fclose(data.f);
  if (histog.f != NULL)
    fclose(histog.f);
  if (curve.f != NULL)
    fclose(curve.f);
  exit(EXIT_SUCCESS);
}



/* End. */
