#!/home/mplace/anaconda3.7/bin/python
"""delila_pipeline.py

Create a sequence logo for Transcription Start sites using the Delila package.

Notes
----- 
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
    A text file with a list of RNA-Seq fastq files to be processed one file 
    name per line. Files maybe gzip or decompressed.


Example
-------
    usage:

        rnaSeqPipelineGLBRC.py -f input.txt
    

Requirements
------------

    1. bam2wig.pl
    2. Bowtie2, Including reference bowtie2 built reference and index files.
    3. Bwa, Including reference Bwa built reference and index files.

References
----------

http://users.fred.net/tds/lab/delila.html

A design for computer nucleic-acid-sequence storage, retrieval, and manipulation
Thomas D. Schneider, Gary D. Stormo, Jeffrey S. Haemer, Larry Gold
Nucleic Acids Research, Volume 10, Issue 9, 11 May 1982, Pages 3013–3024, 
https://doi.org/10.1093/nar/10.9.3013




"""
import os
import re
import sys
import argparse            

class delilaPipe( object ):
    """
    Methods and data structures for delila pipeline
    """
    def __init__(self, genbank):
        """
        Set up delilaPipe object
             
        """
        self.gnbk        = genbank             # genbank file, the primary input file
        self.dbbkChanges = 'dbbk_changes.txt'  # record seq changes from dbbk
        self.l1          = 'l1'
        self.l2          = 'l2'
        self.l3          = 'l3'
        self.lib1        = 'lib1'
        self.lib2        = 'lib2'
        self.lib3        = 'lib3'
        self.cat1        = 'cat1'
        self.cat2        = 'cat2'
        self.cat3        = 'cat3'
        self.instructions = 'delila_instructions.txt'

       



def main():
    
    cmdparser = argparse.ArgumentParser(description="RNA-Seq alignment, HTSeq & RPKM pipeline.",
                                        usage='%(prog)s -f <fastq file list.txt> [optional args: -a -r -d -ref ]' ,prog='rnaSeqPipelineGLBRC.py'  )
    cmdparser.add_argument('-f', '--file',    action='store', dest='FILE',    help='Text file, one fastq file name per line. (Forward file name only)', metavar='')
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)
        
    if cmdResults['DETAIL']:
        print("\n")
        print("This script is designed to run on the GLBRC scarcity servers 6-10.")
        print("See Mike Place for problems with this script.")
        sys.exit(1)
    
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']        

if __name__ == "__main__":
    main()

