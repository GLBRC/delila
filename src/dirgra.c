/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "dirgra.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*





*/



#define version         1.08
/*










*/



/*





















































































*/



#define maxnodes        50
#define maxlinks        50


Static _TEXT dirgrap, list;


Static jmp_buf _JL1;


Static Void halt()
{
  /*





*/
  printf(" program halt.\n");
  longjmp(_JL1, 1);
}


#define pi              3.141592653589





Static Void randomDelila(seed)
double *seed;
{
  /*






























*/
  double intermediate;

  intermediate = *seed + pi;
  intermediate = exp(5.0 * log(intermediate));
  *seed = intermediate - (long)intermediate;
}

#undef pi



Static Void themain(dirgrap, list)
_TEXT *dirgrap, *list;
{
  long firstnode, i, iterations, lastnode, n, l;
  long net[maxnodes + 1][maxlinks];
  long count[maxnodes + 1][maxlinks];
  long links[maxnodes + 1];
  long node1, node2, nextnode, r;
  long steps = 0;
  double seed = 0.0;
  long FORLIM;

  printf("dirgra %4.2f\n", version);
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
  fprintf(list->f, "dirgra %4.2f\n", version);



  for (n = 0; n <= maxnodes; n++) {
    for (l = 0; l < maxlinks; l++) {
      net[n][l] = 0;
      count[n][l] = 0;
    }
    links[n] = 0;
  }


  if (*dirgrap->name != '\0') {
    if (dirgrap->f != NULL)
      dirgrap->f = freopen(dirgrap->name, "r", dirgrap->f);
    else
      dirgrap->f = fopen(dirgrap->name, "r");
  } else
    rewind(dirgrap->f);
  if (dirgrap->f == NULL)
    _EscIO2(FileNotFound, dirgrap->name);
  RESETBUF(dirgrap->f, Char);
  fscanf(dirgrap->f, "%ld%*[^\n]", &iterations);
  getc(dirgrap->f);
  fscanf(dirgrap->f, "%ld%ld%*[^\n]", &firstnode, &lastnode);
  getc(dirgrap->f);
  if (firstnode < 0 || lastnode < 0) {
    printf("first node and last node must be positive integers\n");
    halt();
  }

  while (!BUFEOF(dirgrap->f)) {
    fscanf(dirgrap->f, "%ld%ld%*[^\n]", &node1, &node2);
    getc(dirgrap->f);
    links[node1]++;
    net[node1][links[node1] - 1] = node2;
  }


  fprintf(list->f, "* first node: %ld\n", firstnode);
  fprintf(list->f, "*  last node: %ld\n", lastnode);
  fprintf(list->f, "* Nodes:\n");
  n = 0;
  while (links[n] > 0) {
    fprintf(list->f, "* %ld to: ", n);
    FORLIM = links[n];
    for (l = 1; l <= FORLIM; l++) {
      if (l > 1)
	putc(',', list->f);
      fprintf(list->f, " %ld", net[n][l-1]);
    }
    putc('\n', list->f);
    n++;
  }

  for (i = 1; i <= iterations; i++) {
    printf("------ Iteration %ld ------\n", i);
    n = firstnode;
    do {

      randomDelila(&seed);
      r = (long)(links[n] * seed) + 1;
      nextnode = net[n][r-1];

      printf("at %ld with %ld links; ", n, links[n]);
      printf("chose number %ld so ", r);
      printf("next node is %ld\n", nextnode);

      count[n][r-1]++;
      steps++;
      n = nextnode;
    } while (n != lastnode);
  }


  fprintf(list->f,
	  "\n* Nodes with traversal counts and percentages: for %ld steps\n",
	  steps);
  n = 0;
  while (links[n] > 0) {
    fprintf(list->f, "* %ld to: ", n);
    FORLIM = links[n];
    for (l = 1; l <= FORLIM; l++) {
      if (l > 1)
	putc(',', list->f);
      fprintf(list->f, " %ld", net[n][l-1]);
      fprintf(list->f, " (%ld", count[n][l-1]);
      fprintf(list->f, ", %ld%%)",
	      (long)floor(100 * ((double)count[n][l-1] / steps) + 0.5));
    }
    putc('\n', list->f);
    n++;
  }

}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  list.f = NULL;
  strcpy(list.name, "list");
  dirgrap.f = NULL;
  strcpy(dirgrap.name, "dirgrap");
  themain(&dirgrap, &list);
_L1:
  if (dirgrap.f != NULL)
    fclose(dirgrap.f);
  if (list.f != NULL)
    fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
