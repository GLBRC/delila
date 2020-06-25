/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "frese.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*






*/



#define version         1.01
/*
*/



/*

































*/



#define maxarray        100


Static _TEXT fresep, sequ;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}



Static Void themain(fresep, sequ)
_TEXT *fresep, *sequ;
{
  long a, c, g, t, l, n, newtotal;
  long oldtotal = 0, p = 0;
  long q;
  boolean spat;
  long store[maxarray][5];

  printf("frese %4.2f\n", version);

  if (*fresep->name != '\0') {
    if (fresep->f != NULL)
      fresep->f = freopen(fresep->name, "r", fresep->f);
    else
      fresep->f = fopen(fresep->name, "r");
  } else
    rewind(fresep->f);
  if (fresep->f == NULL)
    _EscIO2(FileNotFound, fresep->name);
  RESETBUF(fresep->f, Char);
  while (!BUFEOF(fresep->f)) {
    fscanf(fresep->f, "%ld%ld%ld%ld%ld%*[^\n]", &l, &a, &c, &g, &t);
    getc(fresep->f);
    p++;
    if (p > maxarray) {
      printf("the table is too wide, increase constant maxarry\n");
      halt();
    }
    store[p-1][0] = l;
    store[p-1][1] = a;
    store[p-1][2] = c;
    store[p-1][3] = g;
    store[p-1][4] = t;
    newtotal = a + c + g + t;
    if (newtotal != oldtotal) {
      if (oldtotal != 0) {
	printf("at position %ld the %ld position in the table, the total is %ld\n",
	       l, p, newtotal);
	printf("at the previous position the total was %ld\n", oldtotal);
	printf(
	  "This program only works for constant numbers at each position.  sorry... \n");
	halt();
      }
    }
    oldtotal = newtotal;
  }

  printf("total number of sequences to make: %ld\n", newtotal);

  if (*sequ->name != '\0') {
    if (sequ->f != NULL)
      sequ->f = freopen(sequ->name, "w", sequ->f);
    else
      sequ->f = fopen(sequ->name, "w");
  } else {
    if (sequ->f != NULL)
      rewind(sequ->f);
    else
      sequ->f = tmpfile();
  }
  if (sequ->f == NULL)
    _EscIO2(FileNotFound, sequ->name);
  SETUPBUF(sequ->f, Char);

  /*





*/

  for (n = 1; n <= newtotal; n++) {
    /*

*/
    for (l = 0; l < p; l++) {
      q = 1;
      spat = false;
      while (!spat) {
	if (store[l][q] <= 0) {
	  q++;
	  if (q > 4)
	    q = 1;
	  continue;
	}
	switch (q) {

	case 1:
	  putc('a', sequ->f);
	  break;

	case 2:
	  putc('c', sequ->f);
	  break;

	case 3:
	  putc('g', sequ->f);
	  break;

	case 4:
	  putc('t', sequ->f);
	  break;
	}
	store[l][q]--;
	spat = true;
      }
    }

    fprintf(sequ->f, ".\n");
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  sequ.f = NULL;
  strcpy(sequ.name, "sequ");
  fresep.f = NULL;
  strcpy(fresep.name, "fresep");
  themain(&fresep, &sequ);
_L1:
  if (fresep.f != NULL)
    fclose(fresep.f);
  if (sequ.f != NULL)
    fclose(sequ.f);
  exit(EXIT_SUCCESS);
}


































/* End. */
