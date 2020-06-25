/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "rean.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.02
/*



*/
#define updateversion   1.00



/*

























































*/


Static _TEXT data, oinst, reanp, dataout, strongestinst;
Static long annotatedstart, annotatedstop, betterstart, bettersd, orientation;
Static double annotatedrbsstrength, betterrbsstrength;
Static Char c, k;
Static boolean inframe;
Static double strongestrbs;
Static long strongeststart, distance, annotatedlength, newlength;
Static long storestrength[50];
Static long storesdistance[50];
Static long count;
Static double rank, bestrank;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
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




Static Void findnewinfo(arbss, thedatafile, ori, betterrbss, betters,
			bettersd)
double *arbss;
_TEXT *thedatafile;
long *ori;
double *betterrbss;
long *betters, *bettersd;
{

  /*

*/
  skipcolumn(thedatafile);
  skipcolumn(thedatafile);
  skipcolumn(thedatafile);
  skipblanks(thedatafile);
  fscanf(thedatafile->f, "%ld", betters);
  skipcolumn(thedatafile);
  skipblanks(thedatafile);
  fscanf(thedatafile->f, "%ld", bettersd);
  skipcolumn(thedatafile);
  skipblanks(thedatafile);
  fscanf(thedatafile->f, "%ld", ori);
  skipcolumn(thedatafile);
  skipcolumn(thedatafile);
  skipcolumn(thedatafile);
  skipcolumn(thedatafile);
  fscanf(data.f, "%lg", betterrbss);
}



Static Void findoldinfo(start, stop, theinstfile)
long *start, *stop;
_TEXT *theinstfile;
{
  skipcolumn(theinstfile);
  skipcolumn(theinstfile);
  skipblanks(theinstfile);
  fscanf(theinstfile->f, "%ld", start);
  skipcolumn(theinstfile);
  skipcolumn(theinstfile);
  skipblanks(theinstfile);
  fscanf(theinstfile->f, "%ld", stop);
}



Static Void inframeornot(astart, bstart, frame)
long *astart, *bstart;
boolean *frame;
{
  long num;

  num = labs(*astart - *bstart);
  while (num > 2)
    num -= 3;
  if (num == 0)
    *frame = true;
  else
    *frame = false;
}




Static Void themain(oinst, data, reanp, dataout, strongestinst)
_TEXT *oinst, *data, *reanp, *dataout, *strongestinst;
{
  double parameterversion;

  printf("rean %4.2f\n", version);
  if (*reanp->name != '\0') {
    if (reanp->f != NULL)
      reanp->f = freopen(reanp->name, "r", reanp->f);
    else
      reanp->f = fopen(reanp->name, "r");
  } else
    rewind(reanp->f);
  if (reanp->f == NULL)
    _EscIO2(FileNotFound, reanp->name);
  RESETBUF(reanp->f, Char);
  fscanf(reanp->f, "%lg%*[^\n]", &parameterversion);
  getc(reanp->f);
  if ((long)floor(100 * parameterversion + 0.5) < (long)floor(100.0 + 0.5)) {
    printf("You have an old parameter file!\n");
    halt();
  }
  if (*oinst->name != '\0') {
    if (oinst->f != NULL)
      oinst->f = freopen(oinst->name, "r", oinst->f);
    else
      oinst->f = fopen(oinst->name, "r");
  } else
    rewind(oinst->f);
  if (oinst->f == NULL)
    _EscIO2(FileNotFound, oinst->name);
  RESETBUF(oinst->f, Char);
  if (*data->name != '\0') {
    if (data->f != NULL)
      data->f = freopen(data->name, "r", data->f);
    else
      data->f = fopen(data->name, "r");
  } else
    rewind(data->f);
  if (data->f == NULL)
    _EscIO2(FileNotFound, data->name);
  RESETBUF(data->f, Char);

  if (*strongestinst->name != '\0') {
    if (strongestinst->f != NULL)
      strongestinst->f = freopen(strongestinst->name, "w", strongestinst->f);
    else
      strongestinst->f = fopen(strongestinst->name, "w");
  } else {
    if (strongestinst->f != NULL)
      rewind(strongestinst->f);
    else
      strongestinst->f = tmpfile();
  }
  if (strongestinst->f == NULL)
    _EscIO2(FileNotFound, strongestinst->name);
  SETUPBUF(strongestinst->f, Char);
  fprintf(strongestinst->f, "(* rean %4.2f *)\n", version);

  if (*dataout->name != '\0') {
    if (dataout->f != NULL)
      dataout->f = freopen(dataout->name, "w", dataout->f);
    else
      dataout->f = fopen(dataout->name, "w");
  } else {
    if (dataout->f != NULL)
      rewind(dataout->f);
    else
      dataout->f = tmpfile();
  }
  if (dataout->f == NULL)
    _EscIO2(FileNotFound, dataout->name);
  SETUPBUF(dataout->f, Char);
  fprintf(dataout->f, "* rean %4.2f\n", version);

  fprintf(dataout->f, "* Columns:\n");
  fprintf(dataout->f, "* 1.  Annotated Start\n");
  fprintf(dataout->f, "* 2.  Annotated RBS strength (bits)\n");
  fprintf(dataout->f,
    "* 3.  Better Start Inframe (between upstream and downstream stop codons inframe)\n");
  fprintf(dataout->f, "* 4.  Better RBS strength (bits)\n");
  fprintf(dataout->f, "* 5.  Orientation\n");
  fprintf(dataout->f,
	  "* 6.  number of bases between better start and annotated start\n");
  fprintf(dataout->f, "* 7.  length of anntoated gene in bases\n");
  fprintf(dataout->f, "* 8.  length of new gene in bases\n");
  fprintf(dataout->f,
	  "* 9.  strongest site is marked by an \"!\", otherwise \"-\"\n");

  while (!BUFEOF(oinst->f)) {
    k = getc(oinst->f);
    if (k == '\n')
      k = ' ';
    while (k != 'g') {
      fscanf(oinst->f, "%*[^\n]");
      getc(oinst->f);
      k = getc(oinst->f);
      if (k == '\n')
	k = ' ';
    }

    findoldinfo(&annotatedstart, &annotatedstop, oinst);
    c = getc(data->f);
    if (c == '\n')
      c = ' ';
    while (c != ' ') {
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      c = getc(data->f);
      if (c == '\n')
	c = ' ';
    }

    strongestrbs = -100.0;
    count = 1;
    bestrank = 0.0;
    while (c == ' ') {
      findnewinfo(&annotatedrbsstrength, data, &orientation,
		  &betterrbsstrength, &betterstart, &bettersd);
      /*

*/
      inframeornot(&annotatedstart, &betterstart, &inframe);
      if (betterrbsstrength > 0) {
	if (inframe == true) {
	  if (betterrbsstrength > annotatedrbsstrength) {
	    if (betterrbsstrength > strongestrbs) {
	      strongestrbs = betterrbsstrength;
	      strongeststart = betterstart;
	    }

	    fprintf(dataout->f, "%12ld", annotatedstart);
	    fprintf(dataout->f, "%15.6f", annotatedrbsstrength);
	    fprintf(dataout->f, "%12ld", betterstart);
	    fprintf(dataout->f, "%15.6f", betterrbsstrength);
	    fprintf(dataout->f, "%12ld", orientation);
	    distance = betterstart - annotatedstart;
	    if (distance < 0 && orientation < 0)
	      distance *= orientation;
	    fprintf(dataout->f, "%12ld", distance);
	    annotatedlength = labs(annotatedstart - annotatedstop);
	    newlength = labs(betterstart - annotatedstop);
	    rank = (double)newlength / annotatedlength;

	    fprintf(dataout->f, "%12ld", annotatedlength);
	    fprintf(dataout->f, "%12ld", newlength);
	    fprintf(dataout->f, "%15.6f\n", rank);
	    if (rank > bestrank)
	      bestrank = rank;
	  }
	}
      }
      fscanf(data->f, "%*[^\n]");
      getc(data->f);
      c = getc(data->f);
      if (c == '\n')
	c = ' ';
      if (betterstart == annotatedstart)
	annotatedrbsstrength = betterrbsstrength;
    }

    /*














*/

    /*








*/

    if (strongestrbs == -100)
      fprintf(dataout->f, "%12ld%15.6f   THERE IS NO BETTER SITE\n",
	      annotatedstart, annotatedrbsstrength);

    fscanf(data->f, "%*[^\n]");
    getc(data->f);
    fscanf(data->f, "%*[^\n]");

    getc(data->f);
    if (strongestrbs > 0) {
      if (orientation < 0)
	fprintf(strongestinst->f,
	  "get from %12ld +30 to %12ld -20 direction -; (* %5.2f bits *)\n",
	  strongeststart, strongeststart, strongestrbs);
      else
	fprintf(strongestinst->f,
	  "get from %12ld -30 to %12ld +20 direction +; (* %5.2f bits *)\n",
	  strongeststart, strongeststart, strongestrbs);
    }
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  strongestinst.f = NULL;
  strcpy(strongestinst.name, "strongestinst");
  dataout.f = NULL;
  strcpy(dataout.name, "dataout");
  reanp.f = NULL;
  strcpy(reanp.name, "reanp");
  oinst.f = NULL;
  strcpy(oinst.name, "oinst");
  data.f = NULL;
  strcpy(data.name, "data");
  themain(&oinst, &data, &reanp, &dataout, &strongestinst);
_L1:
  if (data.f != NULL)
    fclose(data.f);
  if (oinst.f != NULL)
    fclose(oinst.f);
  if (reanp.f != NULL)
    fclose(reanp.f);
  if (dataout.f != NULL)
    fclose(dataout.f);
  if (strongestinst.f != NULL)
    fclose(strongestinst.f);
  exit(EXIT_SUCCESS);
}



/* End. */
