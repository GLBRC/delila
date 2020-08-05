Delila programs were designed to manipulate sets of sequence fragments. 
Some of the programs are used to create sequence logos and sequence walkers 
while others are tools that I find useful such as rembla and the TheThe program.

website:  users.fred.net/tds/lab/delila.html


REFERENCES for the Delila System

   @article{Schneider1982,
   author = "T. D. Schneider
   and G. D. Stormo
   and J. S. Haemer
   and L. Gold",
   title = "A design for computer nucleic-acid sequence storage,
   retrieval and manipulation",
   journal = "Nucl. Acids Res.",
   volume = "10",
   pages = "3013-3024",
   year = "1982"}


   @article{Schneider1984,
   author = "T. D. Schneider
   and G. D. Stormo
   and M. A. Yarus
   and L. Gold",
   title = "Delila system tools",
   journal = "Nucl. Acids Res.",
   volume = "12",
   pages = "129-140",
   year = "1984"}


 STEPS to run malign:

1) run dbbk, The first step is to create a Delila book containing the genomic
   or artificial sequence you want to manipulate. 

src/dbbk -f rhodo_genome.gbff -c rhodo_changes.txt  -o rhodo_dbbk.txt

arguments:

-f takes a genebank file

-c output file name, will create a text file to write any changes the program
   had to make to the sequence, due to base ambiguity.

-o is the output file name, this is also the l1 file used with catal



The pascal version of dbbk looks like this:

/opt/bifxapps/delila/dbbk
 dbbk 3.50
Input file `db': rhodo_genome.gbff
Output file `l1': l1
organism * R.sphaeroides-2.4.1
NC_007493.2
NC_007494.2
Output file `changes': changes.txt
NC_009007.1
NC_007488.2
NC_007489.1
NC_007490.2
NC_009008.1
WARNING: some sequences have been altered,see the changes file.

the OUTPUT is a file called l1 (extracted chromosome sequences) and a file called changes.txt

The primary output is a book, which is the genome sequence in a special format.

2) run catal, to create the Delila library

  copy the book from step one to a file named l1  ( cp rhodo_dbbk.txt l1 )

  then create empty files named l2, l3, catalp    ( touch l2 l3 catalp )


pascal version  , l1 is the book from dbbk

catal 9.63 2020/08/05 14:34:34
Input file `l1': l1
Input file `l2': l2
Input file `l3': l3
Input file `catalp': catalp
 library dates advanced
Output file `cat1': cat1
Output file `cat2': cat2
Output file `cat3': cat3
Output file `humcat': humcat
Output file `catin': catin
Output file `lib1': lib1

 library 1 date will be: 2020/08/05 14:34:34
 NC_007493.2 3188524 bp
 NC_007494.2 943018 bp
 NC_009007.1 114045 bp
 NC_007488.2 114179 bp
 NC_007489.1 105284 bp
 NC_007490.2 100827 bp
 NC_009008.1  37100 bp
Output file `lib2': lib2
Output file `lib3': lib3


run:  /catal -f catal_parameters.txt 
you should see output something like this:

 catal 9.64 2020/07/10 10:20:57

 library dates advanced
 library 1 date will be: 2020/07/10 10:20:57

 NC_007493.2 3188524 bp
 NC_007494.2 943018 bp
 NC_009007.1 114045 bp
 NC_007488.2 114179 bp
 NC_007489.1 105284 bp
 NC_007490.2 100827 bp
 NC_009008.1  37100 bp


This will create some new files  lib1, lib2, lib3, cat1, cat2, cat3, humcat, catin

3) Parse the Transcription Start site input file and create delila instructions
   files by chromosomes.

   delila_instructions.py -f TSS_file.txt -o output Prefix 

4) run delila

delila -b lib1 -i NC_007490.2_delila_instructions.inst -l outlisting.txt

-b is the lib1 output from catal

-i delila instructions file

-l listing output file

pascal version

/opt/bifxapps/delila/delila 
delila 5.04
sequence segment size (dnamax) is 1024
Input file `lib1': lib1
Input file `lib2': lib2
Input file `lib3': lib3
Input file `cat1': cat1
Input file `cat2': cat2
Input file `cat3': cat3
Input file `inst': NC_007489.1_delila_instructions.inst
Output file `listing': results
Output file `book': book
1 library
Pass 1
No syntax errors found.

Pass 2
Request for: NC_007489.1 - reading in the piece
No extraction errors found.

30 pieces extracted
420 bases extracted
 
5)






