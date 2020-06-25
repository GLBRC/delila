/* Output from p2c 2.00.Oct.15, the Pascal-to-C translator */
/* From input file "unsqz.p" */


 #include "/root/src/p2c-2.01/home/src/p2c.h"


/*
*/



#define version         1.07


/*
*/



/*


























*/



Static _TEXT fin, fout;


Static jmp_buf _JL1;


Static Void halt()
{
  printf("program halt\n");
  longjmp(_JL1, 1);
}



Static Void themain(filein, fileout)
_TEXT *filein, *fileout;
{
  Char endofline;

  printf("unsqz %4.2f\n", version);
  if (*fileout->name != '\0') {
    if (fileout->f != NULL)
      fileout->f = freopen(fileout->name, "w", fileout->f);
    else
      fileout->f = fopen(fileout->name, "w");
  } else {
    if (fileout->f != NULL)
      rewind(fileout->f);
    else
      fileout->f = tmpfile();
  }
  if (fileout->f == NULL)
    _EscIO2(FileNotFound, fileout->name);
  SETUPBUF(fileout->f, Char);
  if (*filein->name != '\0') {
    if (filein->f != NULL)
      filein->f = freopen(filein->name, "r", filein->f);
    else
      filein->f = fopen(filein->name, "r");
  } else
    rewind(filein->f);
  if (filein->f == NULL)
    _EscIO2(FileNotFound, filein->name);
  RESETBUF(filein->f, Char);
  fscanf(filein->f, "%c%*[^\n]", &endofline);
  getc(filein->f);

  if (endofline == '\n')
    endofline = ' ';
  while (!BUFEOF(filein->f)) {
    while (!P_eoln(filein->f)) {
      if (P_peek(filein->f) == endofline) {
	fscanf(filein->f, "%*[^\n]");
	getc(filein->f);
      } else {
	putc(P_peek(filein->f), fileout->f);
	getc(filein->f);
      }
    }
    fscanf(filein->f, "%*[^\n]");
    getc(filein->f);
    putc('\n', fileout->f);
  }
}


main(argc, argv)
int argc;
Char *argv[];
{
  PASCAL_MAIN(argc, argv);
  if (setjmp(_JL1))
    goto _L1;
  fout.f = NULL;
  strcpy(fout.name, "fout");
  fin.f = NULL;
  strcpy(fin.name, "fin");
  themain(&fin, &fout);
_L1:
  if (fin.f != NULL)
    fclose(fin.f);
  if (fout.f != NULL)
    fclose(fout.f);
  exit(EXIT_SUCCESS);
}



/* End. */
