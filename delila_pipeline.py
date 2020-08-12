#!/home/mplace/anaconda3.7/bin/python
"""delila_pipeline.py

Create a sequence logo for Transcription Start sites using the Delila package.

Notes
----- 

The programs used in this pipeline have been converted from Pascal to C, by
Oliver Giramma.  They were then modified to use command line arguments.

(From the Delila Documentation)

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


Method
------

(From the Delila Documentation)

If you are working with binding sites in GenBank, you will need many programs.
They are given below in the order you use them, with a brief explanation of each.
Be sure to read the manual page of each one. In particular, note how the output
of each program generally becomes the input to the next. Also, it helps to read
various definitions in the glossary.

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

rseq: Compute Rsequence for each position in the aligned book, make an rsdata file.

dalvec: Convert the rsdata file into a symvec file, which the next program
can use to make the logo. 

makelogo: Finally! Make the sequence logo! 

rf: Once you have computed the information content of your binding site, 
compute Rfrequency. You will need to know how big the genome is 
(or the total number of binding positions availble to the recognizer)
and the number of binding sites in that region.
    
Parameters
----------

f : str
    A genbank file for a genome.

p : str
    prefix used for naming output files.

t : str
    Transcription start site information file.

    NC_007488.2     RSP_4039_1700   forward 1700
    NC_007488.2     RSP_4037_3543   forward 3543
    NC_007488.2     RSP_4025_19218  reverse 19218


Example
-------
    usage:

        delila_pipeline.py -f rhodo_genome.gbff -p rhodo -t all_rhodo_tss_for_delila.txt
        delila_pipeline.py -f rhodo_genome.gbff -p R.sphaeroides-2.4.1 -t all_rhodo_tss_for_delila.txt
        
Requirements
------------



References
----------

http://users.fred.net/tds/lab/delila.html

A design for computer nucleic-acid-sequence storage, retrieval, and manipulation
Thomas D. Schneider, Gary D. Stormo, Jeffrey S. Haemer, Larry Gold
Nucleic Acids Research, Volume 10, Issue 9, 11 May 1982, Pages 3013–3024, 
https://doi.org/10.1093/nar/10.9.3013


https://www.ncbi.nlm.nih.gov/Sitemap/samplerecord.html


"""
import argparse        # for command line args
import logging         
import os
import re              # for regex 
import subprocess      # used to call delila programs 
import sys
import chromosomeUtilities

# program home directory
pdir = '/home/mplace/scripts/delila/src/'

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
    def __init__(self, genbank, prefix, tss):
        """
        Set up delilaPipe object
        
        gnbk            Delila book, result of running makeDBBK
        dbbkChanges     Changes recored from makeDBBK
        l1              l1 is output of makeDBBK
        l2,l3           empty files required by Delila
        lib1,lib2,lib3  
        cat1,cat2,cat3
        instructions     A list of instruction files, one for each chromosome

        """
        self.gnbk        = genbank             # genbank file, the primary input file
        self.prefix      = prefix
        self.dbbkChanges = prefix + '_' + 'dbbk_changes.txt'  # record seq changes from dbbk
        self.catalParams = self.catalParameters()
        self.l1          = 'l1'                # l1,l2,l3 required by delila, only l1 contains info
        self.l2          = 'l2'
        self.l3          = 'l3'
        self.lib1        = 'lib1'              # lib1, lib2, lib3 required by delila, only lib1 contains info
        self.lib2        = 'lib2'
        self.lib3        = 'lib3'
        self.cat1        = 'cat1'              # cat1, cat2, cat3 required by delila, only cat1 contains info
        self.cat2        = 'cat2'
        self.cat3        = 'cat3'
        self.instructions = []                 # list of instruction files, one per chromosome
        self.tss         = tss                 # Transcription Start Site information file name
        self.delilaBOOK  = []                  # list of books, Output of Delila program   

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
        rep += 'TSS File {}'.format(self.tss)     
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
           i.e. bases Not GATC
        -o output file, delila book 

        '''
        program = pdir + 'dbbk'       # location of delila 
        # set up dbbk command parameters
        cmd = [program, '-f', self.gnbk, '-c', self.dbbkChanges, '-o', self.prefix + '_' + 'dbbk.txt']
        # log function call
        logger.info("Running makeDBBK ")
        logger.info(program + ' ' + ' '.join(cmd))
        # run dbbk
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)
        # create a symbolic link to l1 for running catal
        os.symlink(self.prefix + '_' + 'dbbk.txt', 'l1')

    def catalParameters(self):
        '''
        Write catal parameters file
        '''
        with open('catal_parameters.txt', 'w') as out:
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

        return 'catal_parameters.txt'

    def runCATAL(self):
        '''
        Call to Delila catal
        
        The catalogue program checks all the input libraries for correct
        structure.  Duplicated names are removed and a new set of library
        files is created, along with their catalogues for delila.
        
        catal expects a file named: l1 this is a symbolic link to the dbbk
        produced earlier in the pipeline.

        Need to create 3 empty files for delila catal
        l2, l3, catalp
        '''
        # create l2
        with open('l2', 'w') as l2:
            pass
        # create l3
        with open('l3', 'w') as l3:
            pass
        # create catalp
        with open('catalp', 'w') as catalp:
            pass

        program = pdir + 'catal'
        cmd = [program , '-f', self.catalParams ]
        logger.info("Running catal ")
        logger.info(program + ' ' + ' '.join(cmd))
        # run catal
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def splitTSS(self):
        '''
        Break up the input transcription start site input file into delila 
        instruction files by chromosome. 

        File is provided by the user.  Formatted as follows:

        NC_007488.2	RSP_4039_1700	forward	1700
        NC_007488.2	RSP_4038_2627	forward	2627

        creates instruction files for running delila by chromosome.


        '''
        program = '/home/mplace/scripts/delila/delila_instructions.py'
        cmd = [ program, '-f', self.tss, '-o', self.prefix ]
        logger.info('Running splitTSS ')
        logger.info( program + ' ' +  ' '.join(cmd))
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
        lib2: the second library
        cat2: the second catalogue, corresponding to lib2
        lib3: the third library
        cat3: the third catalogue, corresponding to lib3
        '''
        book    = 'BOOK.tmp'
        listing = 'LISTING.tmp' 

        program = pdir + 'delila'
        cmd = [program , '-b', book, '-i', inst, '-l' , listing ]
        logger.info("Running delila ")
        logger.info(program + ' ' + ' '.join(cmd))
        # run delila
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

        # rename temporary output files 
        chrom = re.sub('_TSS.inst', '', inst)
        os.rename(book, self.prefix + '_' + chrom + '_' + 'book.txt' )
        os.rename(listing, self.prefix + '_' + chrom + '_' + 'listing.txt')

    def createMalignp(self):
        '''
        Create the malign parameter file.
        Output a text file called malignp, used as input to malign.
        '''
        with open('malignp', 'w') as out:
            out.write('-5 +5\n')   # winleft, winright: left and right ends of window
            out.write('-5 +5\n')   # shiftmin, shiftmax: minimum and maximum shift of aligned base
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

        '''
        # set up malign
        program = pdir + 'malign'
        cmd = [ program , '-b', book, '-i', inst, '-m', 'malignp' ]
        logger.info("Running malign ")
        logger.info(program + ' ' + ' '.join(cmd))
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
        Output a text file called malignp, used as input to malign.
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

        malin -a optalign -i NC_007493.2_TSS.inst -o optinst -p malinp 
        '''
        # set up malin
        program = pdir + 'malin'
        cmd = [ program , '-a', 'optalign', '-i', inst, '-o', 'optinst' ,'-p','malinp' ]
        logger.info("Running malin ")
        logger.info(program + ' ' + ' '.join(cmd))
        # run malin
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)

    def colors(self):
        '''
        Create the colors parameter file.
        '''
        with open('colors', 'w') as cout:
            cout.write("* Color order is red-green-blue and these are the values\n")
            cout.write("* that PostScript uses to generate the figures.\n\n")
            cout.write("* green:\n")
            cout.write("a 0.182082 1 0.181899\n")
            cout.write("* blue:")
            cout.write("c 0 0.9372 1\n")
            cout.write("* organge\n")
            cout.write("g 1 0.7 0\n")
            cout.write("* red:\n")
            cout.write("t 1 0 0\n")
            cout.write("u 1 0 0\n")
            cout.write("* polar are GREEN\n")
            cout.write("G 0 1 0\n")
            cout.write("S 0 1 0\n")
            cout.write("T 0 1 0\n")
            cout.write("Y 0 1 0\n")
            cout.write("C 0 1 0\n")
            cout.write("* neutral are purple\n")
            cout.write("N 1 0 1\n")
            cout.write("Q 1 0 1\n")
            cout.write("* basic BLUE\n")
            cout.write("K 0 0 1\n")
            cout.write("R 0 0 1 \n")
            cout.write("H 0 0 1\n")
            cout.write("* acidic RED\n")
            cout.write("D 1 0 0\n")
            cout.write("E 1 0 0\n")
            cout.write("* the hydrophobic amino acids remain black\n")
            cout.write("P 0 0 0\n")
            cout.write("A 0 0 0\n")
            cout.write("W 0 0 0\n")
            cout.write("F 0 0 0\n")
            cout.write("L 0 0 0\n")
            cout.write("I 0 0 0\n")
            cout.write("M 0 0 0\n")
            cout.write("V 0 0 0\n")
            cout.write("\n")            
        cout.close()

    def makeALISTp(self):
        '''
        Create the alist parameter file
        '''
        with open('alistp', 'w') as out:
            out.write("6.64        version of alistp that this parameter file is designed for.\n")
            out.write("-10 +10       From and To\n")
            out.write("pl          display control p: piece&coordinate of zero base; l: long name\n")
            out.write("p           p: paging, n: no paging\n")
            out.write("i           f: first base, i: inst, b: book alignment\n")
            out.write("6  4  1     avalues: column, output width, output decimals\n")
            out.write("n 0 0 0 0   edgecontrol (p=page), edgeleft, edgeright, edgelow, edgehigh in cm\n")
            out.write("C 15        mapcontrol: C=do map, R= rotate, char height (72 points/inch)\n")
            out.write("1.0 -1.0 1.0 amount to move image in x and y (cm) and scale factor\n")
            out.write("h           headercontrol: h(eader); 0: no header, no numbar; else numbar\n")

    def runALIST(self, book, cinst, alistp):
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

        '''
        # create the required empty files   
        with open('avalues', 'w') as f:
            pass
        f.close()

        # create the colors parameter file
        self.colors()

        with open("namebook", 'w') as f:
            pass
        f.close()

        with open("namelist", 'w') as f:
            pass
        f.close()

        # set up alist
        program = pdir + 'alist'
        cmd = [ program , '-b', book, '-i', cinst, '-p', alistp ]
        logger.info("Running alist ")
        logger.info(program + ' ' + ' '.join(cmd))
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

        '''
        # set up encode
        program = pdir + 'encode'
        cmd = [ program , '-b', book, '-i', cinst, '-p', encodep ]
        logger.info("Running encode ")
        logger.info(program + ' ' + ' '.join(cmd))
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

        '''
        with open("compp", 'w') as f:
            pass
        f.close()

        # set up comp
        program = pdir + 'comp'
        cmd = [ program , '-b', book, '-p', compp ]
        logger.info("Running compp ")
        logger.info(program + ' ' + ' '.join(cmd))
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
        ''' 
        # set up rseq
        program = pdir + 'rseq'
        cmd = [ program , '-c', cmp, '-e', encseq ]
        logger.info("Running rseq ")
        logger.info(program + ' ' + ' '.join(cmd))
        # run rseq
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)       

    def runDALVEC(self, rsdata, dalvecp):
        '''
        from Delila documentation:

        Convert the rsdata file from rseq into a format that the makelogo program
        can use.  The format is a 'symbol vector'.
        
        dalvec -r rsdata -p dalvecp
        '''
        # set up dalvec
        program = pdir + 'dalvec'
        cmd = [ program , '-r', rsdata, '-p', dalvecp ]
        logger.info("Running dalvec ")
        logger.info(program + ' ' + ' '.join(cmd))
        # run dalvec
        output = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
        result1 = output[0].decode('utf-8')
        result2 = output[1].decode('utf-8')
        # log stdout and stderr 
        logger.info(result1)
        logger.info(result2)     

    def makeWaveFile(self):
        '''
        Make a wave input file
        '''
        with open('wave', 'w') as out:
            out.write("h\n")
            out.write("2.0\n")
            out.write("0.5\n")
            out.write("10.6\n")
            out.write("0.2\n")
            out.write("0\n")  

    def makeLOGOp(self):
        '''
        Create a default parameter file to make a logo
        '''
        with open('makelogop', 'w') as out:
            out.write("-5 +7\n")
            out.write("100\n")
            out.write("5.0 10.0\n")
            out.write("0\n")
            out.write("0.36\n")
            out.write("5 0.5\n")
            out.write("2\n")
            out.write("1.0\n")
            out.write("bt 2 10\n")
            out.write("no show\n")
            out.write("no outline\n")
            out.write("caps\n")
            out.write("14\n")
            out.write("1\n")
            out.write("1.0\n")
            out.write("n\n")
            out.write("1\n")
            out.write("1\n")
            out.write("-1000 5.2 1\n")
            out.write("1\n")            
    
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
        '''
        # set up makeLOGO
        program = pdir + 'makelogo'
        cmd = [ program , '-s', symvec, '-o', output ]
        logger.info("Running makelogo ")
        logger.info(program + ' ' + ' '.join(cmd))
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
    
    def updateTssPositions(self):
        '''
        Need to update positions for TSS sites, based on merged chromosome positions.
        '''
        with open('tss-test.txt', 'r') as tss:
            for ln in tss:
                 dat = ln.rstrip().split('\t')
                 newPos = int(dat[3]) + mySeq.chrInfo[dat[0]]['start']
                 dat.append(newPos)
                 print(dat)


def main():
    
    cmdparser = argparse.ArgumentParser(description="Delila pipeline to make sequence logo from Transcription start sites.",
                                        usage='%(prog)s -f genome.genbank -p prefix -t tss_file.txt -w -10 +10 ',
                                        prog='delila_pipeline.py'  )
    cmdparser.add_argument('-i', '--info', action='store_true', dest='INFO',
                            help='Print more information to stdout')                            
    cmdparser.add_argument('-f', '--file', action='store', dest='FILE', 
                            help='genome fasta file', metavar='')
    cmdparser.add_argument('-p', '--prefix', action='store', dest='PREFIX', 
                            help='Prefix names used on output files', metavar='')
    cmdparser.add_argument('-w', '--window', action='store', dest='WINDOW', 
                            help='Window to search for site, 2 numbers: -10 +10', nargs='+', metavar='' )
    cmdparser.add_argument('-t', '--tss',  action='store', dest='TSS',  
                            help='TSS site information text file.)', metavar='')    
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
        print("    Create a sequence logo for Transcription Start sites using the Delila package.")
        print("")
        print("To Run:\n")
        print("delila_pipeline.py -f genome.gbff -p ecoli -t ecoli_tss_info.txt")
        print("")
        print("    -f genome genbank file ")
        print("    -p prefix name to use for output files")
        print("    -t transcription start site information file")
        print("    -w window size to search, takes 2 numbers: -10 +10")
        print("")
        print("    TSS file provides chromosome, name, strand, position information in a tab delimited format.")
        print("")
        print("\t NC_007488.2     RSP_4039_1700   forward 1700")
        print("\t NC_007488.2     RSP_4025_19218  reverse 19218")
        print("")
        print("Output :")
        print("")
        print("    LOGO postscript file")
        print("")
        print("")
        print("")
        print("For help contact:  bioinformaticshelp@glbrc.wisc.edu\n")
        sys.exit(1)
    
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']        

    if cmdResults['PREFIX'] is not None:
        prefix = cmdResults['PREFIX']

    if cmdResults['TSS'] is not None:
        tssFile = cmdResults['TSS']
    
    if cmdResults['WINDOW'] is not None:
        window = cmdResults['WINDOW']
    else:
        window = ['-10', '+10']

    # log program start
    logger.info("Running delila_pipeline ")
    logger.info('Working Directory: ' + os.getcwd())
    logger.info('genbank file: {}'.format(inFile))
    logger.info('output prefix: {}'.format(prefix))
    logger.info('Transcription Start Site file: {}'.format(tssFile))
    logger.info('Search Window: {}'.format( ' '.join(window)))
    # create delila object and get to work
    pipe = delilaPipe(inFile, prefix, tssFile)
    pipe.makeDBBK()
    pipe.catalParameters()
    pipe.runCATAL()
    pipe.splitTSS()
    # Read instructions from file
    pipe.getInstructions()
    # Create delila instruction sets1
    for inst in pipe.instructions:
        pipe.runDELILA(inst)
    pipe.createMalignp()
    pipe.runMALIGN('R.sphaeroides-2.4.1_NC_007493.2_book.txt', 'NC_007493.2_TSS.inst')
    pipe.createMalinp()
    pipe.runMALIN('NC_007493.2_TSS.inst')
    pipe.runDELILA('cinst')
    pipe.runALIST( 'R.sphaeroides-2.4.1_cinst_book.txt', 'cinst', 'avalues' )
    pipe.runENCODE('R.sphaeroides-2.4.1_cinst_book.txt', 'cinst', 'encodep')
    pipe.runCOMP('R.sphaeroides-2.4.1_cinst_book.txt', 'compp')
    pipe.runRSEQ('cmp', 'encseq')
    pipe.runDALVEC('rsdata', 'dalvecp')
    
    # check if the user has provided a make logo parameter file if not make one
    if os.path.exists('makelogop'):
        continue
    else:
        pipe.makeLogop()

    pipe.runMAKELOGO('symvec', pipe.prefix + '.logo')
    pipe.retrievePWM()

if __name__ == "__main__":
    main()
