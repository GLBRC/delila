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

1) run dbbk, The first step is to create a Delila book containing the genomic or artificial sequence you want to manipulate. 

src/dbbk -f rhodo_genome.gbff -c rhodo_changes.txt  -o rhodo_dbbk.txt

arguments:

-f takes a genebank file

-c  is just a name which will create a text file to write any changes the 

    program had to make to the sequence

-o is the output file name, this is also the l1 file used with catal


2) run catal,  you need to create the Delila library

  copy the book from step one to a file named l1  ( cp rhodo_dbbk.txt l1 )
  then create empty files named l2, l3, catalp    ( touch l2 l3 catalp )

run:  src/catal -f sample.csv
 
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

3) run delila

delila -b lib1 -i NC_007490.2_delila_instructions.inst -l outlisting.tx

-b is the lib1 output from catal

-i delila instructions file

-l listing output file








