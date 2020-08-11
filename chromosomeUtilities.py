#!/home/mplace/anaconda3.7/bin/python
"""chromosomeUtilities.py

Utilities to work with multi-sequence fasta files.

Notes
----- 

Delila only handles one chromosome at a time, one of the utilities will merge 
all chromosomes in fasta into a single sequence, recording the start and
stop positions to split later.

Method
------
   
Parameters
----------

f : str
    Fasta file

Example
-------
    usage:

        chromosomeUtilities.py -f genome.fna
    
References
----------
 
"""
import os
import re
import sys
import argparse    

from Bio import SeqIO
from Bio.Seq import Seq
from Bio.Alphabet import IUPAC

class chromosomeUtilities(object):
    '''
    Methods and data structures for working with fasta files, specifically with
    the delila pipeline.
    '''
    def __init__(self, fasta):
        '''
        Set up class object
        '''
        self.fasta   = fasta            #  input fasta sequence
        self.chrInfo = {}
        self.mergedSeq = None

    def combineSeq(self):
        '''
        Merge all sequences in a fasta file into a single string.
        '''
        pass

    def getPositions(self):
        '''
        Get the lengths of each of the fasta sequences, calculate new chrom start & stop positions 
        and store them in self.chrInfo dictionary.  The start and end positions are additive.
        For example here is a list of sequence names found in a fasta file and the sequence length.        

        NC_007493.2 3188524 # start = 0  (as this is the 1st chrom) end = 3188524
        NC_007494.2 943018  # start = 3188525 (previous end +1) end = 4131543 (start + length of seq)
        NC_009007.1 114045  # start = 4131544 end = 4245589
        NC_007488.2 114179  # start = 4245590 end = 4359769
        NC_007489.1 105284  # start = 4359770 end = 4465054
        NC_007490.2 100827  # start = 4465055 end = 4565882
        NC_009008.1 37100   # start = 4565883 end = 4602983

        self.chrInfo, key = chromosome name  value = dicionary { 'start': 1000, 'end': 5000  }

        '''
        first = 0
        end   = 0
        start = 0
        previous = 0
        for seqRec in SeqIO.parse(self.fasta, 'fasta'):
            if first == 0:
                start = 0
                end   = len(seqRec.seq)
                previous = len(seqRec.seq)
                first = 1
            else:
                start += previous  + 1
                end   += len(seqRec.seq) + 1
                previous = len(seqRec.seq)

            if seqRec.id not in self.chrInfo:
                self.chrInfo[seqRec.id] = { 'start':start, 'end':end }
            
            #print(seqRec.id, len(seqRec.seq), start, end)
        

def main():
    
    cmdparser = argparse.ArgumentParser(description="Utilities for manipulating fasta files inside the Delila peline.",
                                        usage='%(prog)s -f <fastq file list.txt>' ,prog='chromosomeUtilities.py'  )
    cmdparser.add_argument('-f', '--fasta',    action='store', dest='FASTA',    help='fastq file name', metavar='')
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)
        
    if cmdResults['FASTA'] is not None:
        fasta = cmdResults['FASTA']   

    
    mySeq = chromosomeUtilities(fasta)
    mySeq.getPositions()
    print(mySeq.chrInfo)

         

if __name__ == "__main__":
    main()

