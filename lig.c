/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "lig.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*




*/


#define version         1.28
/*



*/



/*










































































*/




#define width           10
#define decimal         5


#define kblambda        48.514


#define n0              6.022e+23


main(argc, argv)
int argc;
Char *argv[];
{
  _TEXT list;

  double kbplasmid, kblinker, pmolplasmid, pmollinker, vol, mw, dnaconc,
	 micrograms, aconc, pmol, i, j;

  Char kind;

  PASCAL_MAIN(argc, argv);
  list.f = NULL;
  strcpy(list.name, "list");
  printf("lig %4.2f\n", version);
  if (*list.name != '\0')
    list.f = fopen(list.name, "w");
  else
    list.f = tmpfile();
  if (list.f == NULL)
    _EscIO2(FileNotFound, list.name);
  SETUPBUF(list.f, Char);
  fprintf(list.f, "lig %4.2f\n", version);

  printf("plasmid size in kb: ");
  fprintf(list.f, "plasmid size in kb: ");
  scanf("%lg%*[^\n]", &kbplasmid);
  getchar();
  fprintf(list.f, "%*.*f\n", width, decimal, kbplasmid);

  printf("for the input plasmid, do you want to give ug or pMOL (u or p):");
  fprintf(list.f,
	  "for the input plasmid, do you want to give ug or pMOL (u or p):");
  scanf("%c%*[^\n]", &kind);
  getchar();
  if (kind == '\n')
    kind = ' ';
  fprintf(list.f, "%c\n", kind);
  if (kind == 'u') {
    printf("ug input plasmid: ");
    fprintf(list.f, "ug input plasmid: ");
    scanf("%lg%*[^\n]", &micrograms);
    getchar();
    fprintf(list.f, "%*.*f\n", width, decimal, micrograms);
    pmolplasmid = 1.5 * micrograms / kbplasmid;
    printf("that is %*.*f pMol\n", width, decimal, pmolplasmid);
    fprintf(list.f, "that is %*.*f pMol\n", width, decimal, pmolplasmid);
  } else {
    printf("pMOL input plasmid: ");
    fprintf(list.f, "pMOL input plasmid: ");
    scanf("%lg%*[^\n]", &pmolplasmid);
    getchar();
    fprintf(list.f, "%*.*f\n", width, decimal, pmolplasmid);
  }

  putchar('\n');
  putc('\n', list.f);

  printf("for the input linker, do you want to give ug or pMOL (u or p):");
  fprintf(list.f,
	  "for the input linker, do you want to give ug or pMOL (u or p):");
  scanf("%c%*[^\n]", &kind);
  getchar();
  if (kind == '\n')
    kind = ' ';
  fprintf(list.f, "%c\n", kind);
  if (kind == 'u') {
    printf("ug linker: ");
    fprintf(list.f, "ug linker: ");
    scanf("%lg%*[^\n]", &micrograms);
    getchar();
    fprintf(list.f, "%*.*f\n", width, decimal, micrograms);

    printf("linker size in kb: ");
    fprintf(list.f, "linker size in kb: ");
    scanf("%lg%*[^\n]", &kblinker);
    getchar();
    fprintf(list.f, "%*.*f\n", width, decimal, kblinker);
    pmollinker = 1.5 * micrograms / kblinker;
    printf("that is %*.*f pMol\n", width, decimal, pmollinker);
    fprintf(list.f, "that is %*.*f pMol\n", width, decimal, pmollinker);
  } else {
    printf("pMOL input linker: ");
    fprintf(list.f, "pMOL input linker: ");
    scanf("%lg%*[^\n]", &pmollinker);
    getchar();
    fprintf(list.f, "%*.*f\n", width, decimal, pmollinker);
  }

  printf("volume in microliters: ");
  fprintf(list.f, "volume in microliters: ");
  scanf("%lg%*[^\n]", &vol);
  getchar();
  fprintf(list.f, "%*ld\n", width, (long)floor(vol + 0.5));

  mw = kbplasmid * 625000L;

  dnaconc = 51.1 / sqrt(mw);
  putchar('\n');
  putc('\n', list.f);
  printf("for the plasmid alone\n");
  fprintf(list.f, "for the plasmid alone\n");
  printf("critical [dna] = %6.4f (g/l = micrograms/microliter)\n", dnaconc);
  fprintf(list.f, "critical [dna] = %6.4f (g/l = micrograms/microliter)\n",
	  dnaconc);
  printf("above this concentration: linears below: circles of the plasmid\n");
  fprintf(list.f,
	  "above this concentration: linears below: circles of the plasmid\n");
  micrograms = pmolplasmid * kbplasmid / 1.5;
  aconc = micrograms / vol;
  printf("actual concentration of %5.3f micrograms of dna: %6.4f (g/l)\n",
	 micrograms, aconc);
  fprintf(list.f,
	  "actual concentration of %5.3f micrograms of dna: %6.4f (g/l)\n",
	  micrograms, aconc);
  if (aconc > dnaconc)
    printf("LINEAR");
  if (aconc > dnaconc)
    fprintf(list.f, "LINEAR");
  if (aconc < dnaconc)
    printf("CIRCULAR");
  if (aconc < dnaconc)
    fprintf(list.f, "CIRCULAR");
  printf(" MOLECULES FAVORED");
  fprintf(list.f, " MOLECULES FAVORED");
  printf("  (j/i = % .3E)\n", 51.1 / (aconc * sqrt(mw)));
  fprintf(list.f, "  (j/i = % .3E)\n", 51.1 / (aconc * sqrt(mw)));


  j = 3.6e+11 * exp(3.0 / 2 * log(kblambda / kbplasmid));




  pmol = pmollinker + 2 * pmolplasmid;
  i = n0 * pmol * (1 / 1e+09) * (1 / vol) * 1000;
  putchar('\n');
  putc('\n', list.f);
  printf("for the plasmid mixed with linkers:\n");
  fprintf(list.f, "for the plasmid mixed with linkers:\n");
  printf("ratio of linker molecules to plasmid: % .3E\n",
	 pmollinker / pmolplasmid);
  fprintf(list.f, "ratio of linker molecules to plasmid: % .3E\n",
	  pmollinker / pmolplasmid);
  printf("j = % .3E (ends / mil)  effective concentration of plasmid ends\n",
	 j);
  fprintf(list.f,
	  "j = % .3E (ends / mil)  effective concentration of plasmid ends\n",
	  j);
  printf("i = % .3E (ends / mil)  total ends\n", i);
  fprintf(list.f, "i = % .3E (ends / mil)  total ends\n", i);
  if (i > j)
    printf("LINEAR");
  if (i > j)
    fprintf(list.f, "LINEAR");
  if (i < j)
    printf("CIRCULAR");
  if (i < j)
    fprintf(list.f, "CIRCULAR");
  if (i == j)
    printf("LINEARS AND CIRCULAR");
  if (i == j)
    fprintf(list.f, "LINEARS AND CIRCULAR");
  printf(" MOLECULES FAVORED");
  fprintf(list.f, " MOLECULES FAVORED");
  printf("  (j/i = % .3E)\n", j / i);
  fprintf(list.f, "  (j/i = % .3E)\n", j / i);
  fclose(list.f);
  exit(EXIT_SUCCESS);
}



/* End. */
