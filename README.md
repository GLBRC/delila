delila_pipeline.py

Quick Start
-----------

Prior to running delila set scriptDir = the directory where the delila code is located
in the delila_pipeline.py

    scriptDir = '/home/<userName>/bin/delila/'

    For each c file the p2c.h #include will have to be changed,
    currently it is: #include </home/mplace/bin/p2c/src/p2c.h>
    this should be changed to reflex the location of p2c.h

To Create a logo you need to provide the following:

    1) genbank file for your genome of interest 
    2) sites file which is tab delimited with columns: 
        
        chromosome name : must match the chromosome names in genbank file
        name : unique identifier for site
        strand :  forward or reverse 
        position :  base pair position of site

    example:
    NC_007493.2     RSP_4324_1682   forward 1682

    where NC_007493.2 is a R.sphaeroides chromosome name
    RSP_4324_1682 is the name
    forward is the strand 
    1682 is the base position of the site

To run delila and create a logo:

    delila_pipeline.py -g rhodo_genome.gbff -l -45 -r -25 -t rhodo_sites_for_delila.txt
    

DESCRIPTION
    Create a sequence logo for user defined sites using the Delila package.
    Supports single or multiple chromosomes.
    
    Notes
    ----- 
    
    The programs used in this pipeline have been converted from Pascal to C, by
    Oliver Giramma.  They were then modified to use command line arguments.
    
    (From the Delila Documentation, http://users.fred.net/tds/lab/software.html)
    
    DELILA stands for DEoxyribnucleaic-acid LIbrary LAnguage.
    
    It is a language for extracting DNA fragments from a large collection of sequences,
    invented around 1980 (T. D. Schneider, G. D. Stormo, J. S. Haemer, and L. Gold", A
    design for computer nucleic-acid sequence storage, retrieval and manipulation, 
    Nucl. Acids Res., 10: 3013-3024, 1982). The idea is that there is a large database
    containing all the sequences one would like, which we call a `library'. 
    
    One would like a particular subset of these sequences, so one writes up some 
    instructions and gives them to the librarian, Delila, which returns a `book' 
    containing just the sequences one wants for a particular analysis. So `Delila' 
    also stands for the program that does the extraction (delila.p). Since it is 
    easier to manipulate Delila instructions than to edit DNA sequences, one makes
    fewer mistakes in generating one's data set for analysis, and they are trivial
    to correct. Also, a number of programs create instructions, which provides a 
    powerful means of sequence manipulation. One of Delila's strengths is that it
    can handle any continuous coordinate system. The `Delila system' refers to a 
    set of programs that use these sequence subsets for molecular information 
    theory analysis of binding sites and proteins. In the spring of 1999 Delila 
    became capable of making sequence mutations, which can be displayed graphically
    along with sequence walkers on a lister map. A complete definition for the
    language is available https://alum.mit.edu/www/toms/libdef.html, although not
    all of it is implemented. There are also tutorials on building Delila libraries
    and using Delila instructions. 
    
    Understanding Left and Right boundaries:
    Delila positioning, bounds will be set by LEFT and RIGHT.
    
    scenario 1:
    Left = -10 
    Right = +10 
    site example:
    Site position 100, then the bounds are Left = position 90, Right = position 110
    cmd: delila_pipeline.py -g genome.gbff -l -20 -r +10 -t sites_for_delila.txt
    
    scenario 2, looking for the -10 position upstream from a transcription start site position.
    Left = -20
    Right = 0
    site example:
    TSS at position 100,  Left = position 80, Right = position 100
    cmd: delila_pipeline.py -g enome.gbff -l -20 -r 0 -t sites_for_delila.txt
    
    
    scenario 3, looking for the -35 position upstream from a transcription start site
    Left = +5
    Right = +25
    site example:
    TSS at position 100, Left = position 105, Right = position 125
    
    Delila is a modular set of programs which produce results that feed into the next
    program.  It is highly configurable using parameter files.  The delila programs
    used are listed below with general descripbtions.
    
    Delila programs used to make logo (descriptions from the Delila documentation)
    ------------------------------------------------------------------------------
    
    dbbk: 
    Convert GenBank flat file format to Delila format, which is called a `book'
    (db = database, bk = book). This produces an 'l1' file which contains the book.
    (l1 is a lower case L [standing for 'Library'] followed by the symbol 'one').
    
    catal: 
    Catalogue the contents of a book, usually l1. If you are not using them
    (which is generally the case) make empty l2, l3 and catalp files, then run catal.
    This will produce six files: lib1, lib2, lib3 and cat1, cat2, cat3. These make 
    up the library used by Delila in the next step.
    
    delila:
    Extract fragments of sequences from a library of sequences and create 
    a subset, a book. This is the core of the Delila system. You give Delila 
    instructions (inst file) and those are used to create the subset desired. 
    For a logo, generally one makes instructions that look like this:
    
     get from 5600 -200 to same +200 direction +;
    
    This means to get 200 bases before position 5600 to 200 bases afterwards, 
    for a total of 401 bases. I recommend that you use a wide range like this to
    be able to see the background noise around the binding site. By the way, the 
    number 5600 becomes the zero coordinate of the binding site. I try to pick a
    position that is strongly conserved (high information content). 
 
    malign: 
    Given a book of aligned sequences, this program searches for the alignment
    of the sequences that has the lowest uncertainty, i.e. the highest value of
    Rsequence. The user specifies the "window" of bases within which
    uncertainty is calculated, and the maximum number of bases that each
    sequence is allowed to shift from the original alignment.  The program
    considers each sequence in turn, shifting it to an alignment with minimum
    uncertainty while holding the other sequences fixed.  A "pass" is complete
    when all sequences have been considered.  A "run" is complete when no
    alignments have changed in the preceding pass, and the alignment is then
    considered "optimal".  The first run starts with the original alignment;
    every run after that starts with a "shuffled" alignment obtained by shifting
    each sequence independently by a random amount between the allowed limits.
    The program maintains a list of all of the unique optimal alignments
    achieved from these starting alignments, and it outputs them in order of
    increasing uncertainty.
    
    malin:
    This program allows one to select one of the alignments created by malign
    and to make the corresponding Delila instructions.  Because it copies the
    inst file it keeps the organism and chromosome information (along with all
    comments) so it is better than the "bestinst" file created by malign!
    
    alist:
    Make an aligned listing of sequences using the Delila instructions 
    and the book created by Delila. The pair of the book and inst file is a set
    called an 'aligned book'. I create a listing of the sequences to be sure 
    that they are aligned correctly. If this works, making the logo is fast
    and a piece of cake.  Even if you have a large range defined in the delila
    instructions, you can use a smaller one for the aligned listing. Note that
    running this program does not affect the inst or book files, so can't
    affect later steps. The program is controled by a file `alistp', which 
    stands for alist-parameters. 

    encode:
    Convert the book/inst into 0 and 1's. This is historically the way
    we did it, but it is fast so we still do it this way. The output is encseq,
    which is a bit large. To save space, you can delete the encseq file after
    running the next program. Note that the range can be reduced in the
    parameter file, so I normally set this to -200 to 200.

    comp:
    Comp counts the number of each oligonucleotide (from length 1 to
    compmax) in the book and prints that to file "cmp".  The output is
    printed in order of increasing length of oligonucleotide (i.e., first
    the monos, then the dis, ...).  If there are no occurences of an
    oligonucleotide, but its one-shorter parent did occur, it will be given
    a zero.  None of its descendants will be printed in the composition
    file.

    rseq:
    Compute Rsequence for each position in the aligned book, make an rsdata file.

    dalvec:
    Convert the rsdata file into a symvec file, which the next program
    can use to make the logo.

    ri:
    The program determines the individual informations of the sites in the book
    as aligned by the instructions, according to the frequency table given in
    the rsdata file.  The program calculates the Ri(b,l) table:

        Ri(b,l) := 2 - (- log2( f(b,l)))

    and sums this up for each sequence.  Ri is defined so that the average of
    the Ri's for a set of sequences is Rsequence.

    makelogo:
    Finally! Make the sequence logo!

    Steps
    ------
    
    1) dbbk    -- convert genbank file into a delila book
    2) catal   -- create catalogue and library files
    3) delila  -- extract a subset of sequences to examine, store in a book
    4) malign  -- alignment of the sequences that has the lowest uncertainty
    5) malin   -- select malign's best alignment
    6) delila  -- create a new book from best alignment (from malign)
    7) alist   -- make an aligned listing of sequences
    8) encode  -- convert the book/inst into 0 and 1's
    9) comp    -- counts the number of each oligonucleotide
    10) rseq   -- compute Rsequence
    11) dalvec -- convert the rsdata file into a symvec file
    12) ri     -- calculate site information
    13) make initial logo -- for comparison to the final logo
    13) filter -- remove sites that ri identified as <= 0 information
    14) repeat steps 3 through 11 with filtered sites
    15) make final logo 
    
    Parameters
    ----------
    
    g : str
        A genbank file for a genome.
    
    t : str
        Transcription start site information file.
    
        NC_007488.2     RSP_4039_1700   forward 1700
        NC_007488.2     RSP_4037_3543   forward 3543
        NC_007488.2     RSP_4025_19218  reverse 19218
    
    l : int
        Left boundary relative to center of the site, defaults to -10
    
    r : int
        Right boundary relative to the center of the site, defaults to +10
    
    Example
    -------
        usage:
    
       delila_pipeline.py -g rhodo_genome.gbff -t TSS.txt -l -8  -r +5

   References
    ----------
    
    http://users.fred.net/tds/lab/delila.html
    
    A design for computer nucleic-acid-sequence storage, retrieval, and manipulation
    Thomas D. Schneider, Gary D. Stormo, Jeffrey S. Haemer, Larry Gold
    Nucleic Acids Research, Volume 10, Issue 9, 11 May 1982, Pages 3013–3024, 
    https://doi.org/10.1093/nar/10.9.3013
    
    Delila system tools, Thomas D. Schneider, Gary D. Stormo, M.A. Yarus,
    Larry Gold, Nucleic Acids Research, Volume 12, 11 Jan 1984, Pages 129-140,
    doi: 10.1093/nar/12.1part1.129

    GenBank Flat File format information :  https://www.ncbi.nlm.nih.gov/Sitemap/samplerecord.html
    
    
Translating and Compiling Delila Programs with P2C
--------------------------------------------------
The original delila Pascal code is available here : users.fred.net/tds/lab/delila.html

The Delila programs can be automatically translated from Pascal to C using
David Gillespie's p2c translater. The resulting c code can be compiled by the gcc compiler.
This was done for the following programs: alist, catal, comp, dalvec, dbbk, delila, encode,
makelogo, malign, malin, mkdb, ri, rseq

See  http://users.fred.net/tds/lab/pascalp2c.html for original help,  
instructions below are adapted from those instructions.

To translate (Pascal to C)
--------------------------

Obtain the current version of p2c from  
(https://github.com/FranklinChen/p2c) or 
(https://www.gsp.com/cgi-bin/man.cgi?section=1&topic=p2c)

You will need three files:

include file: p2c.h
pascal library: p2clib.c
control file: .p2crc In your home directory (Linux operating system) you will need to have a
              control file for p2c (it's called .p2crc) with following lines:
  
    LiteralFiles 2
    NestedComments 2
    StructFiles 1

You may get a warning about "SYSTEM" which you can ignore.

To Compile C code
-----------------

1) You will need to get the libraries from the p2c package and compile them on your machine.
2) You will need the p2c.h file. This can be stored in /usr/local/include in a directory 
   called 'p2c', which is the location given in the -I flag for gcc, below.
3) You will need several /usr/local/lib files. Put the files into /usr/local/lib/p2c.
   These are in the files picked up by -L/usr/local/lib flag for gcc, below.

If you put p2c /home/<userName>/bin/p2c/src/   and make sure to include
the following headers in the c code:

#include <getopt.h>  /* getopt API */
#include <stdio.h> /* printf */
#include <stdlib.h>
#include </home/<userName/bin/p2c/src/p2c.h>

Then dbbk.c will compile with the following command using gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0

gcc  dbbk.c -o dbbk  -I/home/<userName>/bin/p2c/src -L /home/<userName>/bin/p2c/src -lm -lp2c





