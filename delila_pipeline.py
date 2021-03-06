#!/usr/bin/env python
"""delila_pipeline.py

Create a sequence logo for user defined sites using the Delila package.
Supports single or multiple chromosomes.

Notes
----- 

The programs used in this pipeline have been converted from Pascal to C, by
Oliver Giramma.  They were then modified to use C command line arguments.

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
Delila positioning, bounds will be set by -l (Left) and -r (Right) of the defined site
in the sites file.

Left = -10 
Right = +10 
Site position  = 100, then the bounds are Left = position 90, Right = position 110
cmd: delila_pipeline.py -g genome.gbff -l -20 -r +10 -s sites_for_delila.txt

Delila is a modular set of programs which produce results that feed into the next
program.  It is highly configurable using parameter files.  The delila programs
used are listed below with general descripbtions.

Delila programs used to make logo (descriptions from the Delila documentation)
------------------------------------------------------------------------------

dbbk: Convert GenBank flat file format to Delila format, which is called a `book'
(db = database, bk = book). This produces an 'l1' file which contains the book.
(l1 is a lower case L [standing for 'Library'] followed by the symbol 'one').

catal: Catalogue the contents of a book, usually l1. If you are not using them
(which is generally the case) make empty l2, l3 and catalp files, then run catal.
This will produce six files: lib1, lib2, lib3 and cat1, cat2, cat3. These make 
up the library used by Delila in the next step.

delila: Extract fragments of sequences from a library of sequences and create 
a subset, a book. This is the core of the Delila system. You give Delila 
instructions (inst file) and those are used to create the subset desired. 
For a logo, generally one makes instructions that look like this:

 get from 5600 -200 to same +200 direction +;

This means to get 200 bases before position 5600 to 200 bases afterwards, 
for a total of 401 bases. I recommend that you use a wide range like this to
be able to see the background noise around the binding site. By the way, the 
number 5600 becomes the zero coordinate of the binding site. I try to pick a
position that is strongly conserved (high information content). 

malign: Given a book of aligned sequences, this program searches for the alignment
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

malin: This program allows one to select one of the alignments created by malign
and to make the corresponding Delila instructions.  Because it copies the
inst file it keeps the organism and chromosome information (along with all
comments) so it is better than the "bestinst" file created by malign!

alist: Make an aligned listing of sequences using the Delila instructions 
and the book created by Delila. The pair of the book and inst file is a set
called an 'aligned book'. I create a listing of the sequences to be sure 
that they are aligned correctly. If this works, making the logo is fast
and a piece of cake.  Even if you have a large range defined in the delila
instructions, you can use a smaller one for the aligned listing. Note that
running this program does not affect the inst or book files, so can't
affect later steps. The program is controled by a file `alistp', which 
stands for alist-parameters. 

encode: Convert the book/inst into 0 and 1's. This is historically the way 
we did it, but it is fast so we still do it this way. The output is encseq,
which is a bit large. To save space, you can delete the encseq file after 
running the next program. Note that the range can be reduced in the 
parameter file, so I normally set this to -200 to 200.

comp:Comp counts the number of each oligonucleotide (from length 1 to
compmax) in the book and prints that to file "cmp".  The output is
printed in order of increasing length of oligonucleotide (i.e., first
the monos, then the dis, ...).  If there are no occurences of an
oligonucleotide, but its one-shorter parent did occur, it will be given
a zero.  None of its descendants will be printed in the composition
file.

rseq: Compute Rsequence for each position in the aligned book, make an rsdata file.

dalvec: Convert the rsdata file into a symvec file, which the next program
can use to make the logo. 

ri: The program determines the individual informations of the sites in the book
    as aligned by the instructions, according to the frequency table given in
    the rsdata file.  The program calculates the Ri(b,l) table:

       Ri(b,l) := 2 - (- log2( f(b,l)))

    and sums this up for each sequence.  Ri is defined so that the average of
    the Ri's for a set of sequences is Rsequence. 

makelogo: Finally! Make the sequence logo! 
    
Method
------

1) dbbk    -- convert genbank file into a delila book
2) catal   -- create catalogue and library files
3) delila  -- extract a subset of sequences to examine, store in a book
4) malign  -- alignment of the sequences that has the lowest uncertainty
5) malin   -- select malign's best alignment
6) delila  -- create a new book from best alignment
7) alist   -- make an aligned listing of sequences
8) encode  -- convert the book/inst into 0 and 1's
9) comp    -- counts the number of each oligonucleotide
10) rseq   -- compute Rsequence
11) dalvec -- convert the rsdata file into a symvec file
12) ri     -- calculate site information
13) make initial logo -- for comparison to the final logo
13) filter -- remove sites that ri identified as <= 0 information
14) repeat steps 3 through 11
15) make final logo 

Parameters
----------

g : str
    A genbank file for a genome.

t : str
    site information file.

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

   delila_pipeline.py -g rhodo_genome.gbff -s sites.txt -l -8  -r +5
        
References
----------

http://users.fred.net/tds/lab/delila.html

A design for computer nucleic-acid-sequence storage, retrieval, and manipulation
Thomas D. Schneider, Gary D. Stormo, Jeffrey S. Haemer, Larry Gold
Nucleic Acids Research, Volume 10, Issue 9, 11 May 1982, Pages 3013–3024, 
https://doi.org/10.1093/nar/10.9.3013


https://www.ncbi.nlm.nih.gov/Sitemap/samplerecord.html

General bacterial promoter positioning

  <-- upstream                                            downstream -->
5'-XXXXXXXPPPPPPXXXXXXPPPPPPXXXXGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGXXXX-3'
           -35       -10       Gene to be transcribed


"""
import argparse        # command line args
import glob
import logging         
import os
import re              # regex 
import subprocess      # call external programs, delila's programs in this case 
import sys

# external python scripts    
import merge_books     
import merge_instructions 
import rename_lib1 
import removeRI_books
import removeRI_instructions

# program home directory
scriptDir = '/home/mplace/scripts/delila/'
pdir = scriptDir + 'src/'

# set up log file configuration
logging.basicConfig(filename='delila_pipeline.log', format='%(asctime)s %(message)s',
                        filemode='w')
# Create logging object
logger=logging.getLogger()                           
# Set logger threshold level
logger.setLevel(logging.INFO)

class delilaPipe( object ):
    """
    Methods and data structures for delila pipeline
    """
    def __init__(self, genbank, prefix, sites, left, right, title):
        """
        Set up delilaPipe object

        Parameters
        ----------
        genbank : str
            Genbank file
        prefix : str
            Prefix string used to name output files
        sites : str
            Site file name
        left : int
            Integer defining the left bound relative to the central base of the site.
        right : int
            Integer defining the right bound relative to the central base of the site.
 
        Returns
        -------

        delilaPipe object which can be used to access the following:

        gnbk            delila book, result of running makeDBBK
        catalp          parameter file for catal
        dbbkChanges     changes recored from makeDBBK
        l1              l1 is output of makeDBBK
        l2,l3           empty files required by Delila
        lib1,lib2,lib3  delila library file names
        cat1,cat2,cat3  catalog file names 
        instructions    list of instruction files, one for each chromosome
        delilaBook      list of books from the Delila program output
        sites           sites input file
        title           title for logo
        left,right      input left and right window around site
        """
        self.cat1        = 'cat1'              # cat1, cat2, cat3 required by delila, only cat1 has info
        self.cat2        = 'cat2'
        self.cat3        = 'cat3'
        self.catalParams = 'catalparameters'
        self.dbbkChanges = prefix + '_' + 'dbbk_changes.txt'  # record seq changes from dbbk
        self.delilaBOOK  = []                  # list of books, Output of Delila program  
        self.gnbk        = genbank             # genbank file, the primary input file
        self.instructions = []                 # list of instruction files, one per chromosome
        self.l1          = 'l1'                # l1,l2,l3 required by delila, only l1 contains info
        self.l2          = 'l2'
        self.l3          = 'l3'
        self.lib1        = 'lib1'              # lib1, lib2, lib3 required by delila, only lib1 contains info
        self.lib2        = 'lib2'
        self.lib3        = 'lib3'
        self.prefix      = prefix
        self.sites       = sites                 # Transcription Start Site information file name
        self.title       = title
        self.left        = left                # left bound number, +/-
        self.right       = right               # right bound number, +/-
        # Set shift parameter which is used in malignp, 
        # set the shiftmin, shiftmax and winleft, winright to the smallest boundary value
        tmpLeft          = int(self.left)
        tmpRight         = int(self.right)
        self.shift       = abs(tmpLeft) if abs(tmpLeft) < abs(tmpRight) else abs(tmpRight)

    def __repr__(self):
        '''
        Print delilaPipe object information for debugging.
        '''
        rep = 'delilaPipe object\n'
        rep += 'gnbk  {}\n'.format(self.gnbk)
        rep += 'prefix {}\n'.format(self.prefix)
        rep += 'dbkkChanges {}\n'.format(self.dbbkChanges)
        rep += 'l1\n'
        rep += 'l2\n'
        rep += 'l3\n'
        rep += 'lib1\n'
        rep += 'lib2\n'
        rep += 'lib3\n'
        rep += 'cat1\n'
        rep += 'cat2\n'
        rep += 'cat3\n'   
        rep += 'Site File {}\n'.format(self.sites) 
        rep += 'left and right bounds {}  {} \n'.format(self.left, self.right)
        rep += 'instruction files: {} \n'.format('\t'.join(self.instructions))
        return rep 
       
    def makeDBBK(self):
        '''
        Call to Delila dbbk.

        This program converts GenBank and EMBL data base entries into a
        book of delila entries.  The organism name is fused together
        with a period and is used for both organsim and chromosome
        names.  Organism and chromosome only change if the name changes
        in db
        
        dbbk: missing -o option
        usage: dbbk -f genome.gnbk -c changes.txt -o output.txt

        -f is a genome genbank file
        -c output file, reports base changes due to base ambiguity 
           i.e. bases Not in the set {G,A,T,C}
        -o output file, delila book 

        '''
        program = pdir + 'dbbk'       # location of delila 
        # set up dbbk command parameters
        cmd = [program, '-f', self.gnbk, '-c', self.dbbkChanges, '-o', self.prefix + '_' + 'book.txt']
        # log function call
        logger.info("Running makeDBBK ")
        logger.info(' '.join(cmd))
        # run dbbk
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)
        # create a symbolic link to l1 for running catal
        os.symlink(self.prefix + '_' + 'book.txt', 'l1')

    def catalParameters(self):
        '''
        Write catal parameters file
        '''
        with open('catalparameters', 'w') as out:
            out.write('l1=l1\n')
            out.write('l2=l2\n')
            out.write('l3=l3\n')
            out.write('cat1=cat1\n')
            out.write('cat2=cat2\n')
            out.write('cat3=cat3\n')
            out.write('humcat=humcat\n')
            out.write('catin=catin\n')
            out.write('lib1=lib1\n')
            out.write('lib2=lib2\n')
            out.write('lib3=lib3\n')
            out.write('catalp=catalp\n')
        out.close()

    def runCATAL(self):
        '''
        Call to Delila catal
        
        The catalogue program checks all the input libraries for correct
        structure.  Duplicated names are removed and a new set of library
        files is created, along with their catalogues for delila.
        
        catal expects a file named: l1 this is a symbolic link to the dbbk
        produced earlier in the pipeline.

        Need to create 3 empty files for delila catal : l2, l3, catalp

        catal -f parameters.txt

        '''
        # create l2
        with open('l2', 'w') as l2:
            pass
        # create l3
        with open('l3', 'w') as l3:
            pass
        # create catal parameters file
        if not os.path.exists(self.catalParams):
            self.catalParameters()
        
        with open('catalp','w') as catalout:
            pass
        catalout.close()

        program = pdir + 'catal'                         # path to catal
        cmd = [program , '-f', self.catalParams ]        # set up command
        logger.info("Running catal ")
        logger.info(' '.join(cmd))
        # run catal
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def splitSites(self, sites):
        '''
        Break up the input transcription start site input file into delila 
        instruction files by chromosome. 

        File is provided by the user.  Formatted as follows:

        NC_007488.2	RSP_4039_1700	forward	1700
        NC_007488.2	RSP_4038_2627	forward	2627

        creates instruction files for running delila by chromosome.

        Parameters
        ----------
        sites : str
            Site location file

        '''
        program = scriptDir + 'delila_instructions.py'
        cmd = [ program, '-f', sites, '-o', self.prefix, '-l', self.left, '-r', self.right ]
        logger.info('Running splitSites ')
        logger.info(' '.join(cmd))
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def getInstructions(self):
        '''
        Open instruction.list file and load the instructions files.
        For use with runDELILA function.
        '''
        with open('instructions.list', 'r') as f:
            for line in f:
                inst = line.rstrip()
                self.instructions.append(inst)
        f.close()

    def runDELILA(self, inst):
        '''
        Extract fragments of sequences from a library of sequences and create a
        subset, a book. This is the core of the Delila system.
        
        Delila is a data base manager for nucleic acid sequences.  It takes a set
        of instructions, written in the language delila (DEoxyribonucleic acid
        LIbrary LAnguage) and a large set of sequences called a library.  The
        output is a listing of the actions taken (or errors) corresponding to the
        instructions, and a "book" containing the sequences desired.

        instructions.list  -- holds the names of delila instruction files for 
        the chromosomes in project genome.

        delila -b book.txt -i instructions.txt -l listing.txt

        -b book: the set of sequences pulled out of the library.

        -i inst: instructions written in the language delila that tell the
                 program delila what sequences to pull out of the library.

        -l listing: the instructions are listed along with errors found or
                    actions taken.

        delila also expects the following files to be present:
        
        lib1: the first library from which to obtain sequences
        cat1: the first catalogue, corresponding to lib1
        lib2: the second library, EMPTY
        cat2: the second catalogue, corresponding to lib2, EMPTY
        lib3: the third library, EMPTY
        cat3: the third catalogue, corresponding to lib3, EMPTY

        Parameters
        ----------
        inst : str
            Delila instruction file

        '''
        book    = 'BOOK.tmp'
        listing = 'LISTING.tmp' 

        program = pdir + 'delila'
        cmd = [program , '-b', book, '-i', inst, '-l' , listing ]
        logger.info("Running delila ")
        logger.info(' '.join(cmd))
        # run delila
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

        # rename temporary output files 
        chrom = re.sub('_sites.inst', '', inst)
        os.rename(book, self.prefix + '_' + chrom + '_' + 'book.txt' )
        book = self.prefix + '_' + chrom + '_' + 'book.txt'
        os.rename(listing, self.prefix + '_' + chrom + '_' + 'listing.txt')
        self.delilaBOOK.append(self.prefix + '_' + chrom + '_' + 'book.txt')
        return book

    def createMalignp(self):
        '''
        Create the malign parameter file.
        Output a text file called malignp, used as input to malign.
        '''
        with open('malignp', 'w') as out:
            out.write('-{} +{}\n'.format(self.shift, self.shift))   # winleft, winright: left and right ends of window
            out.write('-{} +{}\n'.format(self.shift, self.shift))   # shiftmin, shiftmax: minimum and maximum shift of aligned base
            out.write('54321\n')   # iseed: integer random seed
            out.write('0\n')       # nranseq: number of random sequences, or 0 to use sequences in book
            out.write('2000\n')    # nshuffle: number of times to redo alignment after random shuffle
            out.write('0\n')       # ifpaired: 1 for pairs of complementary sequences for symmetric site
            out.write('-1\n')      # standout: -1 no, 0 limited, 1 full standard output
            out.write('-1\n')      # npassout: output H and alignment every npassout passes
            out.write('-1\n')      # nshiftout: output H(L) every nshiftout shifts, every pass if 0, -1 no
            out.write('0.0001\n')  # tolerance: tolerance in change of H
            out.write('50')        # ntolpass: maximum number of passes with change less than tolerance

        out.close()

    def runMALIGN(self, book, inst):
        '''
        From Delila documentation:

        Given a book of aligned sequences, this program searches for the alignment
        of the sequences that has the lowest uncertainty, i.e. the highest value of
        Rsequence.  The user specifies the "window" of bases within which
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

        malign -b NC_007493.2_book -i NC_007493.2_delila_instructions.inst -m malignp

        Parameters
        ----------
        book : str
            Name of delila book
        inst : str
            Delila instruction file

        '''
        # check if malignp parameter file exists
        if not os.path.exists('malignp'):
            logger.info('\nCreating malignp ')
            self.createMalignp()

        # set up malign
        program = pdir + 'malign'
        cmd = [ program , '-b', book, '-i', inst, '-m', 'malignp' ]
        logger.info("Running malign ")
        logger.info(' '.join(cmd))
        # run malign
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def createMalinp(self):
        '''
        Create the malin parameter file. (NOT FOR MALIGN)
        Output a text file called malinp, used as input to malin.
        '''
        with open('malinp', 'w') as out:
            out.write('1.14\n') # Program version number, warn user if too old
            out.write('1\n')    # integer, defines which alignment to use to create cinst
            out.write('0\n')    # integer, defines how much to add to move the location of the zero base in the new instructions
        out.close()        

    def runMALIN(self, inst):
        '''
        From Delila documentation:

        This program allows one to select one of the alignments created by malign
        and to make the corresponding Delila instructions.  Because it copies the
        inst file it keeps the organism and chromosome information (along with all
        comments) so it is better than the "bestinst" file created by malign!

        malin -a optalign -i NC_007493.2_sites.inst -o optinst -p malinp 

        Parameters
        ----------
        inst : str
            Delila instruction file
        '''
        # create the malin parameter file
        self.createMalinp()

        # set up malin
        program = pdir + 'malin'
        cmd = [ program , '-a', 'optalign', '-i', inst, '-o', 'optinst' ,'-p','malinp' ]
        logger.info("Running malin ")
        logger.info(' '.join(cmd))
        # run malin
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def makeALISTp(self):
        '''
        Create the alist parameter file
        '''
        with open('alistp', 'w') as out:
            out.write("6.64        version of alistp that this parameter file is designed for.\n")
            out.write("-10 +10     From and To, allows a wide alignment, but only look at a portion\n")  
            out.write("pl          display control p: piece&coordinate of zero base; l: long name\n")
            out.write("p           p: paging, n: no paging\n")
            out.write("i           f: first base, i: inst, b: book alignment\n")
            out.write("6  4  1     avalues: column, output width, output decimals\n")
            out.write("n 0 0 0 0   edgecontrol (p=page), edgeleft, edgeright, edgelow, edgehigh in cm\n")
            out.write("C 15        mapcontrol: C=do map, R= rotate, char height (72 points/inch)\n")
            out.write("1.0 -1.0 1.0 amount to move image in x and y (cm) and scale factor\n")
            out.write("h           headercontrol: h(eader); 0: no header, no numbar; else numbar\n")

    def runALIST(self, book, cinst):
        '''
        from Delila documentation:    
      
        Alist creates an aligned listing of a sets of sequences.  The pieces in
        the book are aligned according to the instructions in file inst, and
        listed in the list file.  Each piece is identified, and a bar of numbers
        (called a 'numbar') that are read vertically defines the locations of
        bases around the aligning point.
        
        Requires the creation of empty files for program to run.
        files created are avalues, colors, namebook, namelist

        alist  -b book.txt -i cinst -p alistp

        Output:
        list: the aligned listing
        clist:the aligned listing, in PostScript color.  Paging is ALWAYS done
              to this file, using the page parameter. 
        output: messages to user

        Parameters
        ----------
        book : str
            delila book file
        cinst : str
            updated instructions from malin

        '''
        # create the required empty files   
        with open('avalues', 'w') as f:
            f.write('* avalues')
        f.close()

        # create the colors parameter file
        self.makeColors()

        # create the alistp parameter file
        self.makeALISTp()
        # create empty file namebook
        with open("namebook", 'w') as f:
            pass
        f.close()
        # create empty file namelist
        with open("namelist", 'w') as f:
            pass
        f.close()

        # set up alist
        program = pdir + 'alist'
        cmd = [ program , '-b', book, '-i', cinst, '-p', 'alistp' ]
        logger.info("Running alist ")
        logger.info(' '.join(cmd))
        # run alist
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def makeENCODEp(self):
        '''
        Create a parameter file for encode program.
        '''
        with open('encodep', 'w') as out:
            out.write("i\n")
            out.write("-100 100\n")
            out.write("1\n")
            out.write("1\n")
            out.write("1\n")
            out.write("1\n")

    def runENCODE(self, book, cinst, encodep):
        '''
        from Delila documentation: 
        
        This program is used to encode a book of sequences into a string of
        integers.  Each sequence in the book is encoded into a single string of
        integers (ended by an 'end of sequence' symbol) according to the user
        specified parameters, which are in the file 'encodep'.

        Parameters
        ----------
        book : str
            Delila book
        cinst : str
            cinst file from malin
        encodep : str
            encode parameter file

        Output:
        encseq: the encoded sequences 

        '''
        self.makeENCODEp()
        # set up encode
        program = pdir + 'encode'
        cmd = [ program , '-b', book, '-i', cinst, '-p', encodep ]
        logger.info("Running encode ")
        logger.info(' '.join(cmd))
        # run encode
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def runCOMP(self, book, compp):
        '''
        from Delila documentation:

        Comp counts the number of each oligonucleotide (from length 1 to
        compmax) in the book and prints that to file "cmp".  The output is
        printed in order of increasing length of oligonucleotide (i.e., first
        the monos, then the dis, ...).  If there are no occurences of an
        oligonucleotide, but its one-shorter parent did occur, it will be given
        a zero.

        Parameters
        ----------
        book : str
            Delila book
        compp : str
            comp parameter file

        Output:
        cmp: the composition, determined for mononucleotides up to 
             oligonucleotides of length "compmax"

        '''
        with open("compp", 'w') as f:
            pass
        f.close()

        # set up comp
        program = pdir + 'comp'
        cmd = [ program , '-b', book, '-p', compp ]
        logger.info("Running compp ")
        logger.info(' '.join(cmd))
        # run comp
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def runRSEQ(self, cmp, encseq):
        '''
        from Delila documentation:

        Encoded sequences from encseq are converted to a table of frequencies
        for each base (b) at each aligned position (l).  rsequence(l)
        and the variance var(hnb) are calculated and shown along with
        their running sums.  rsequence and the variance due to sampling
        error are shown for the whole site, but the running sums let one
        find rsequence and the variance for any subrange desired.
            n, the number of example sequences may vary with position, so
        both n and e(hnb) are shown.

        rseq -c cmp -e encseq 

        Parameters
        ----------
        cmp : str
            Composition from the comp program.
        encseq : str
            Output of the encode program.
        
        Output:
        rsdata: a display of the information content of each position  
                of the sequences, with the sampling error variance.

        ''' 
        # set up rseq
        program = pdir + 'rseq'
        cmd = [ program , '-c', cmp, '-e', encseq ]
        logger.info("Running rseq ")
        logger.info(' '.join(cmd))
        # run rseq
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)       

    def runDALVEC(self, rsdata):
        '''
        from Delila documentation:

        Convert the rsdata file from rseq into a format that the makelogo program
        can use.  The format is a 'symbol vector'.
        
        dalvec -r rsdata -p dalvecp

        Parameters
        ----------
        rsdata : str
            Data file from rseq program
        
        '''
        # create empty parameter file
        if not os.path.exists('dalvcep'):
            with open('dalvecp', 'w') as f:
                pass
            f.close()

        # set up dalvec
        program = pdir + 'dalvec'
        cmd = [ program , '-r', rsdata, '-p', 'dalvecp' ]
        logger.info("Running dalvec ")
        logger.info(' '.join(cmd))
        # run dalvec
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)  

    def makeColors(self):
        '''
        Create the colors parameter file.
        The color parameters were provided by Tom Schneider
        '''
        with open('colors', 'w') as cout:
            cout.write("* Color order is red-green-blue and these are the values\n")
            cout.write("* that PostScript uses to generate the figures.\n\n")
            cout.write("* green:\n")
            cout.write("a 0.182082 1 0.181899\n")
            cout.write("* blue:\n")
            cout.write("c 0 0.9372 1\n")
            cout.write("* organge\n")
            cout.write("g 1 0.7 0\n")
            cout.write("* red:\n")
            cout.write("t 1 0 0\n")
            cout.write("u 1 0 0\n")
            cout.write("\n")            
        cout.close()

    def makeWaveFile(self):
        '''
        Make a wave input file, empty file means no sine wave
        '''
        with open("wave", 'w') as f:
            pass
        f.close()
        
        #with open('wave', 'w') as out:
        #    out.write("h\n")
        #    out.write("0\n")
        #    out.write("2.0\n")
        #    out.write("0.5\n")
        #    out.write("10.6\n")
        #    out.write("0.2\n")
        #    out.write("0\n")  

    def makeLOGOp(self):
        '''
        Create a default parameter file to make a logo
        '''
        with open('makelogop', 'w') as out:
            out.write(str(self.left)  + str(self.right) + "\n") # sets the logo width
            out.write("100\n")
            out.write("5.0 10.0\n")      # change the coordinate position
            out.write("0\n")
            out.write("0.36\n")          # set character width
            out.write("5 0.05\n")        # set side bar height and width, .05 is good for width
            out.write("2\n")             # sets the height of the bar in bits, 2 works well
            out.write("1.0\n")           
            out.write("bt 2 2\n")        # display the side bar on both sides of the logo
            out.write("no show\n")
            out.write("no outline\n")
            out.write("caps\n")          # show capital letters
            out.write("14\n")
            out.write("1\n")
            out.write("1.0\n")           # line separation relative to the barheight
            out.write("n\n")
            out.write("1\n")
            out.write("1\n")
            out.write("-1000 5.2 1\n")
            out.write("{}\n".format(self.title))  # Logo label      
    
    def runMAKELOGO(self, symvec, output ):
        '''
        from Delila documentation:
        
        The makelogo program generates a `sequence logo' for a set of aligned
        sequences.  A full description is in the documentation paper.  The input
        is an `symvec', or symbol-vector that contains the information at each
        position and the numbers of each symbol.  The output is in the graphics
        language PostScript.

        The program now indicates the small sample error in the logo by a small
        'I-beam' overlayed on the top of the logo.  Although the user may turn
        this off to make pretty logos, I strongly recommend use of it to avoid
        being fooled by small amounts of data.

        Parameters
        ----------
        symvec : str
             A "symbol vector" file from dalvec
        output : str
            Logo output name
        '''
        # check if the user has provided a make logo parameter file if not make one
        if not os.path.exists('makelogop'):
            self.makeLOGOp()

        # check if the user has provided a wave file if not make one
        if not os.path.exists('wave'):
            self.makeWaveFile()

        # check if the user has provided a wave file if not make one
        if not os.path.exists('colors'):
            self.makeColors()
        # create empty marks file
        with open("marks", 'w') as f:
            pass
        f.close()            

        # set up makeLOGO
        program = pdir + 'makelogo'
        cmd = [ program , '-s', symvec, '-o', output ]
        logger.info("Running makelogo ")
        logger.info(' '.join(cmd))
        # run dalvec
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)     

    def retrievePWM(self):
        '''
        Extract the position weight matrix from the rsdata file (output from rseq).
        Write to new file.
        '''
        switch = 0
        # open rsdata file
        with open('rsdata', 'r') as f, open(self.prefix + '_PWM.txt', 'w') as out:
            for line in f:
                # pwm starts here
                if line.startswith('*   l'):
                    switch = 1
                # last line in file, don't print
                if line.startswith('* rsequence'):
                    break
                # write the PWM to file    
                if switch == 1:
                    out.write(line)

    def makeRIP(self):
        '''
        Write RI parameters file
        '''
        with open('rip', 'w') as out:
            out.write('2.54\n')          # parameterversion
            out.write('"weight matrix"\n') # name of the weight matrix, here imaginatively named       
            out.write('{} {}\n'.format(self.left, self.right))   # The FROM and TO over which to do the Ri calculation
            out.write('1\n')             # defines the column of the values file to use
            out.write('a\n')             #  the lowest and highest Ri evaluation to report
            out.write('a\n')             # the lowest and highest Value evaluation to report
            out.write('n\n')             # determines whether or not to produce any raw sequences in the sequ file
            out.write('p\n')             # determines whether or not to print the sequence of the site being analyzed
            out.write('-\n')             # determines whether or not to print sequences which have a partial site
            out.write('s 2\n')           # determines what to do when f(b,l) = 0, 's' means to use
                                         # an "extended" Rodger Staden's method of         giving f(b,l) = 1/(n+t)
            out.write('i\n')             # defines piece alignment, 'i' sequences are aligned using delila instructions
            out.write('-5000\n')         # the Ri boundary, -500 or lowser to locate all sites.
            out.write('100\n')           # lower bound on standard deviation to find in the book.
                                         # Use 100 to locate all sites.
            out.write('1.0\n')           # upper probability P to find in the book, 1.00 to locate all sites.
        out.close()

    def runRi(self, book, cinst, rsdata, out):
        '''
        Ri determines the individual information of the sites in the book
        as aligned by the instructions, according to the frequency table given in
        the rsdata file.  The program calculates the Ri(b,l) table:

            Ri(b,l) := 2 - (- log2( f(b,l)))

        and sums this up for each sequence.  Ri is defined so that the average of
        the Ri's for a set of sequences is Rsequence.  However, if the sequences are
        incomplete, the average will probably be less than Rsequence.  The rixyin
        output is ready to read into the xyplo program for plotting and linear
        regression.  The ribl matrix is ready to be used to scan sequences with the
        scan program.

        ri -b R.sphaeroides-2.4.1_cinst_book.txt -i cinst -r rsdata -v values -o ri_inst.out -p rip -w wave

        Parameters
        ----------
        book : str
            A book generated by delila
        cinst : str
            delila instructions
        rsdata : str
            data file from rseq program
        out :str
            Output file name, contains messages to the user.
        '''
        # create the required empty values file   
        with open('values', 'w') as f:
            pass
        f.close()

        # check if the user has provided a wave file, if not make one
        if not os.path.exists('wave'):
            self.makeWaveFile()

        # check if the user has provided a rip file, if not make one
        if not os.path.exists('rip'):
            self.makeRIP()

        # set up to run Ri
        program = pdir + 'ri'
        cmd = [ program , '-b', book, '-i', cinst, '-r', rsdata, '-p', 'rip', '-v', 'values', '-w', 'wave', '-o', out ]
        logger.info("Running runRi ")
        logger.info(' '.join(cmd))
        # run ri
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)     
            
    def runParseRI(self, malign_list, rixyin, out ):
        '''
        Script to organize the Delila outputs of Malign_list and Ri Results and
        write them to files.

        Selects for sequences with an RI score greater than zero.

        Parameters
        ----------
        malign_list : str
            Malign list file from Delila pipeline
        rixyin : str
            Rixyin results file from the Ri progam
        out : str
            Output file name
        '''
        # set up for running
        program = scriptDir + 'organizing_ri_delila_results.py'
        cmd = [ program, '-l', malign_list, '-r', rixyin, '-o', out ]
        logger.info('Running  runParseRI ')
        logger.info(' '.join(cmd))
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def cleanUp(self):
        """
        Organize and clean up the files produced by the pipeline.
        """
        cwd = os.getcwd() + '/'

        if not os.path.exists('parameters'):
            os.mkdir('parameters')
        # move parameter file
        [ os.rename( cwd + par, cwd + 'parameters/' +  par) for par in os.listdir(cwd) if par.endswith('p') ]
        os.rename(cwd + 'wave', cwd + 'parameters/' + 'wave' )

        if not os.path.exists('other'):
            os.mkdir('other')
        # move cat1,cat2,cat3
        for cat in os.listdir():
            if re.match('cat\d',cat):
                os.rename(cwd + cat, cwd + 'other/' + cat)
        # move lib1,lib2,lib3
        for lib in os.listdir():
            if re.match('lib\d',lib):
                os.rename(cwd + lib, cwd + 'other/' + lib)
        # move l1,l2,l3
        for l in os.listdir():
            if re.match('l\d',l):
                os.rename(cwd + l, cwd + 'other/' + l)        
        # move books to book dir
        if not os.path.exists('books'):
            os.mkdir('books')
        [ os.rename(cwd + bk, cwd + 'books/' + bk) for bk in os.listdir(cwd) if bk.endswith('_book.txt') ]
        # remove listing files 
        [ os.remove(cwd + listing) for listing in os.listdir(cwd) if listing.endswith('_listing.txt') ]
        # move instruction files
        if not os.path.exists('instructions'):
            os.mkdir('instructions')
        [ os.rename(cwd + inst, cwd + 'instructions/' + inst) for inst in os.listdir(cwd) if inst.endswith('inst') ]
        
        # move instructions files
        if os.path.exists('MERGED_INSTRUCTIONS.txt'):
            os.rename(cwd + 'MERGED_INSTRUCTIONS.txt', cwd + 'instructions/' + 'MERGED_INSTRUCTIONS.txt')
        
        os.rename(cwd + 'NEW_MERGED_INSTRUCTIONS.txt', cwd + 'instructions/' + 'NEW_MERGED_INSTRUCTIONS.txt')

        # move files to the other dir
        if os.path.exists('New_lib1.txt'):
            os.rename(cwd + 'New_lib1.txt',cwd + 'other/' + 'New_lib1.txt' )
        os.rename(cwd + 'optalign',cwd + 'other/' + 'optalign' )
        os.rename(cwd + 'malignxyin',cwd + 'other/' + 'malignxyin' )
        os.rename(cwd + 'encseq',cwd + 'other/' + 'encseq' )
        os.rename(cwd + 'rixyin',cwd + 'other/' + 'rixyin' )
        os.rename(cwd + 'riplog',cwd + 'other/' + 'riplog' )
        os.rename(cwd + 'ribl',cwd + 'other/' + 'ribl' )
        os.rename(cwd + 'humcat', cwd + 'other/' + 'humcat')
        os.rename(cwd + 'symvec', cwd + 'other/' + 'symvec')      
        os.rename(cwd + 'rsdata', cwd + 'other/' + 'rsdata') 
        for chg in glob.glob('*_changes.txt'):
            os.rename(cwd + chg, cwd + 'other/' + chg)
        
        # list files to remove
        removalList = [ 'malign_list_organized.txt', 'ri_results_organized.txt', 'RI_out.txt', 'uncert',
        'values', 'catin', 'namelist', 'namebook', 'list', 'colors', 'clist','avalues', 'marks',
        'instructions.list', 'mybooks.txt','distribution', 'catalparameters' ]
        # remove files
        for f in removalList:
            os.rename(cwd + f, cwd + 'other/' + f)       

    def ps2pdf(self):
        """
        Call ps2pdf to convert the postscript logo files to PDFs.
        """
        # get list of all logo files
        # then use subprocess to call ps2pdf 
        for logo in glob.glob('*.logo'):
            cmd = ['ps2pdf', logo ]
            subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()

def main():
    usage ='%(prog)s -g genome.gnbk -s site_file.txt -l -10 -r +10 '             
    cmdparser = argparse.ArgumentParser(description="Delila pipeline constructs a sequence logo around a site.",
                                        usage=usage, prog='delila_pipeline.py'  )                          
    cmdparser.add_argument('-g', '--genbank', action='store', dest='GENBANK',
                            help='Genbank file', metavar='')
    cmdparser.add_argument('-i', '--info', action='store_true', dest='INFO',
                            help='Print script information to stdout') 
    cmdparser.add_argument('-l', '-left', action='store', dest='LEFT', metavar='',
                            help='Left (upstream) boundary from site, defaults to -10')
    cmdparser.add_argument('-r', '--right', action='store', dest='RIGHT', metavar='',
                            help='Upper (downstream) boundary from site, defaults to +10')
    cmdparser.add_argument('-s', '--sites',  action='store', dest='SITES',  
                            help='Site information text file.', metavar='') 
    cmdparser.add_argument('-t', '--title',  action='store', dest='TITLE',  
                            help='Logo Title in Quotes, defaults to species name.', metavar='')
    cmdResults = vars(cmdparser.parse_args())

    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)
        
    # if user asked for more information
    if cmdResults['INFO']:
        print("")
        print("    Program: delila_pipline.py ")
        print("")
        print("    Create a sequence logo for sequence sites using the Delila package.")
        print("")
        print("    The original delila site: http://users.fred.net/tds/lab/software.html")
        print("")
        print("To Run:\n")
        print("delila_pipeline.py -g genome.gnbk -s sites_info.txt -l -10 -r +5 -t \"My Awesome Logo\" ")
        print("")
        print("    -g genome genbank file ")
        print("    -l left boundary relative to site, defaults to -10")
        print("    -s site information file")
        print("    -r right boundary relative to site, defaults to +10")
        print("    -t title, should be within quotes.")
        print("")
        print("    Site information file provides chromosome, name, strand, position in a tab delimited format.")
        print("")
        print("\t NC_007488.2     RSP_4039_1700   forward 1700")
        print("\t NC_007488.2     RSP_4025_19218  reverse 19218")
        print("")
        print("Output :")
        print("")
        print("    Logo postscript file") 
        print("    Logo pdf ")
        print("    position weight matrix file ")
        print("")
        print("")
        print("For help contact:  bioinformaticshelp@glbrc.wisc.edu\n")
        sys.exit(1)

    prefix = ''
    # check the command line parameters
    # Do we have a genbank file
    chromList = set()                                             # store a list of all chromosomes in genbank file
    if cmdResults['GENBANK'] is not None:
        inFile = cmdResults['GENBANK']                            # genbank file
        # need to capture the organism name from the genbank file
        found = False
        with open(inFile, 'r') as f:
            for ln in f:
                if ln.startswith('SOURCE') and not found:         # provides organism name       
                    species = ln.rstrip().split(' ')[6:]          # remove all white space            
                    abrv = species.pop(0)[0]                      # Get genus name's first letter 
                    prefix = abrv + '.' + '-'.join(species)       
                    found = True
                elif ln.startswith('VERSION'):                    # get the chromosome name(s) from genbank file
                    ch = ln.rstrip().split()[1]
                    chromList.add(ch)
    else:
        print('\n\t-g genbank file missing')
        cmdparser.print_help()
        sys.exit(1)

    # Get the left boundary value or set default
    if cmdResults['LEFT']:
        left = cmdResults['LEFT']
        # check for a sign
        if left != '0' and left[:1] not in ['-','+']:
            print('\n\tLeft boundary requires sign (+/-)\n')
            cmdparser.print_help()
            sys.exit(1)
    else:
        left = '-10'
    
    # Get the right boundary or set default
    if cmdResults['RIGHT']:
        right = cmdResults['RIGHT']
        if right != '0' and right[:1] not in ['-', '+']:
            print('\n\tRight boundary requires sign (+/-)\n')
            cmdparser.print_help()
            sys.exit(1)
    else:
        right = '+10'

    # check if boundary positions make sense, 
    # left should be < the right 
    check = [ int(left), int(right)]
    if not (all(check[i] <= check[i+1] for i in range(len(check)-1))):
        print('\n\tLeft and right boundaries must be in numeric order.')
        print('\te.g. left must be less than right\n')
        cmdparser.print_help()
        sys.exit(1)

    if cmdResults['SITES'] is not None:                  # start site file
        sitesFile = cmdResults['SITES'] 
        # attempt to verify site file format
        with open(sitesFile, 'r') as f:
            row = f.readline().rstrip().split('\t')
            if len(row) != 4 or row[0] not in chromList:
                if len(row) != 4:
                    print('\n\tNot enough columns found, expecting 4, found: {}\n'.format(str(len(row))))
                    print('\tMake sure file has 4 tab delimited columns\n')
                    print('\tColumns should be:')
                    print('\t1) Chromosome name, should equal the name in the VERSION line of the genbank file')
                    print('\t2) Site identifier')
                    print('\t3) Strand, forward or reverse')
                    print('\t4) Base position, 1414\n')
                    cmdparser.print_help()
                    sys.exit(1)
                if row[0] not in chromList:
                    print('\n\tChromosome name does not match genbank file')
                    print('\tChromosome should be the same as in the VERSION line of the genbank file\n')
                    cmdparser.print_help()
                    sys.exit(1)
    else:
        print('\n\tStart Site file missing')
        cmdparser.print_help()
        sys.exit(1) 

    # get user defined LOGO title, if none provided use species in logo
    if cmdResults['TITLE'] is not None:
        title = cmdResults['TITLE']  
    else:
        title = prefix      
    
    # log program start
    logger.info("Running delila_pipeline ")
    logger.info('Working Directory: ' + os.getcwd())
    logger.info('Input file       : {}'.format(inFile))
    logger.info('Organism Name    : {}'.format(prefix))
    logger.info('Start Site file  : {}'.format(sitesFile))
    logger.info('Left Bound       : {}'.format(left))
    logger.info('Right Bound      : {}\n'.format(right))

    # create delila object and get to work
    pipe = delilaPipe(inFile, prefix, sitesFile, left, right, title)
    
    # converts GenBank and EMBL data base entries into a book of delila entries.
    pipe.makeDBBK()          
    
    # The catalogue program checks all the input libraries for correct structure.
    pipe.runCATAL()

    # split input Sites file by chromosome, pass site information
    pipe.splitSites(pipe.sites)

    # Read instructions from file
    pipe.getInstructions()
    
    # run delila for each chromosome's instruction set
    for inst in pipe.instructions:
        pipe.runDELILA( inst)   
    
    # write the list of generate books from runDELILA step to file
    bookCount = 0
    with open('mybooks.txt', 'w') as out:
        for b in pipe.delilaBOOK:
            out.write(b + '\n') 
            bookCount += 1   

    logger.info('\nbookCount : {}\n'.format(str(bookCount)))

    # if there are multiple chromosomes, merge books and instructions
    if bookCount > 1:       
        # If more than one chromosome merge books, input is a text file listing the split books
        merge_books.mergeBook('mybooks.txt')
        # if more than one chromosome merge instruction file
        merge_instructions.mergeInst('instructions.list')
        # Rename the tags in the original lib1 to match the merged book and merged instructions
        # this is required to run catal and then delila again
        rename_lib1.newLib('lib1')
        rename_lib1.moveLib('lib1')
        # now run malign on the book and instruction files  
        pipe.runMALIGN('MERGED_book.txt', 'MERGED_INSTRUCTIONS.txt')
        # run malin on the malign results   
        pipe.runMALIN('MERGED_INSTRUCTIONS.txt')
        # run catal again prior to running delila  
        pipe.runCATAL()
    else:                                    # There is only one chromosome present
        with open('mybooks.txt', 'r') as f:  # get the name of the single book 
            malignBook = f.readline().rstrip()
        f.close()
        with open('instructions.list', 'r') as f:  # get the single instruction file name
            malignInst = f.readline().rstrip()
        f.close()
        
        pipe.runMALIGN(malignBook, malignInst)
        pipe.runMALIN(malignInst)
    
    # Finally we can run delila on the malin results
    book = pipe.runDELILA('cinst')
    
    pipe.runALIST(book , 'cinst' )
    pipe.runENCODE(book, 'cinst', 'encodep')
    pipe.runCOMP(book, 'compp')
    pipe.runRSEQ('cmp', 'encseq')
    pipe.runDALVEC('rsdata')
    # MAKE IN INITIAL LOGO TO COMPARE WITH THE FINAL LOG
    pipe.runMAKELOGO('symvec', prefix + '-initial.logo')       # make logo
    # RUN Ri, then rerun malign, malign ugh!
    pipe.runRi(book, 'cinst', 'rsdata', 'riplog' )
    pipe.runParseRI('list', 'rixyin', 'RI_out.txt')
     
    ### REFINE THE LOGO BY REMOVING SITES WITH AN RI score 0 or less     
    # Use the filtered/refined data set to run the pipeline again, 
    # creating the FINAL logo  
    if bookCount > 1:      
        logger.info('\nRunning removeRI_books.removeRI on MERGED_book.txt\n')     
        riIDs = removeRI_books.parseRI('RI_out.txt')
        removeRI_books.removeRI('MERGED_book.txt', riIDs) 

        logger.info('\nRunning removeRI_instructions.parseRI on MERGED_INSTRUCTIONS.txt\n')
        removeRI_instructions.removeRI('MERGED_INSTRUCTIONS.txt',riIDs)

        # now run malign on the book and instruction files  
        pipe.runMALIGN('NEW_MERGED_book.txt', 'NEW_MERGED_INSTRUCTIONS.txt')
        # run malin on the malign results   
        pipe.runMALIN('NEW_MERGED_INSTRUCTIONS.txt')
        
        # run catal again prior to running delila  
        pipe.runCATAL()
    else:                                    # There is only one chromosome present
        with open('mybooks.txt', 'r') as f:  # get the name of the single book 
            malignBook = f.readline().rstrip()
        f.close()

        with open('instructions.list', 'r') as f:  # get the single instruction file name
            malignInst = f.readline().rstrip()
        f.close()

        riIDs = removeRI_books.parseRI('RI_out.txt')
        removeRI_books.removeRI(malignBook, riIDs)   # FIX FILE NAME OUTPUT IN REMOVERI_BOOKS AND INSTRUCTIONS

        logger.info('\nRunning removeRI_instructions.parseRI on MERGED_INSTRUCTIONS.txt\n')
        removeRI_instructions.removeRI(malignInst ,riIDs)
        
        pipe.runMALIGN('NEW_MERGED_book.txt', 'NEW_MERGED_INSTRUCTIONS.txt')
        pipe.runMALIN('NEW_MERGED_INSTRUCTIONS.txt')
    
    # Finally we can run delila on the malin results
    book = pipe.runDELILA('cinst')
    pipe.runALIST(book , 'cinst' )
    pipe.runENCODE(book, 'cinst', 'encodep')
    pipe.runCOMP(book, 'compp')
    pipe.runRSEQ('cmp', 'encseq')
    pipe.runDALVEC('rsdata')
    pipe.runMAKELOGO('symvec', prefix + '-FINAL.logo')       # Make Final logo

    pipe.retrievePWM()
    pipe.ps2pdf()
    pipe.cleanUp()
    
    
if __name__ == "__main__":
    main()
