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
from Bio.Alphabet import IUPAC
from Bio.SeqFeature import SeqFeature, FeatureLocation
from Bio.Seq import Seq
from Bio.SeqRecord import SeqRecord

class chromosomeUtilities(object):
    '''
    Methods and data structures for working with fasta files, specifically with
    the delila pipeline.
    '''
    def __init__(self, fasta):
        '''
        Set up class object
        '''
        self.fasta     = fasta            #  input fasta sequence
        self.chrInfo   = {}
        self.mergedSeq = Seq('', alphabet=IUPAC.ambiguous_dna)

    def combineSeq(self):
        '''
        Merge all sequences in a fasta file into a single string.
        '''
        # open and merge all sequence in fasta file into a giant "pseudo" chromosome
        for seqRec in SeqIO.parse(self.fasta, 'fasta', alphabet=IUPAC.ambiguous_dna  ):
            self.mergedSeq = self.mergedSeq + seqRec.seq     

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
        first = 0               # handle the first sequence in fasta file differently
        start = 0               # start position of sequence     
        end   = 0               # end of sequence
        previous = 0            # keep track of previous sequence length
        # process each sequence in a fasta file
        for seqRec in SeqIO.parse(self.fasta, 'fasta'):
            # handle 1st sequence differenlty, as start must equal zero
            if first == 0:
                start = 0
                end   = len(seqRec.seq)
                previous = len(seqRec.seq)
                first = 1        # done handling the first sequence
            else:
                start += previous  + 1       # new start position is all previous sequence lengths plus 1
                end   += len(seqRec.seq) + 1 # end position is sum of the previous sequence lengths
                previous = len(seqRec.seq)   

            # record postion information for later use with delila result positions
            if seqRec.id not in self.chrInfo:
                self.chrInfo[seqRec.id] = { 'start':start, 'end':end }    

    def makeGenBank(self):
        '''
        Create a genbank file for the newly created merged chromosome
        '''
        outName = re.sub('.fasta|.fna|.fa', '', self.fasta)
        record = SeqRecord(self.mergedSeq, id=outName, name=outName,
        description='Merged-Chromosome for delila pipeline')

        # add annotation
        feature = SeqFeature(FeatureLocation(start=0,end=len(self.mergedSeq)),type='exon')
        record.features.append(feature)
        # save as GenBank file
        outputFile = open(outName + '.gnbk', 'w')
        SeqIO.write(record, outputFile, 'genbank')
        outputFile.close()

    

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
    mySeq.combineSeq()
    mySeq.makeGenBank()
    # set up to write sequence to fasta file
    #outSeqRec = SeqRecord(mySeq.mergedSeq, id='mergedChromosome',
    #description='Merged Chromosome for use with delila pipeline')
    #SeqIO.write(outSeqRec, "mergedChromosome.fa", 'fasta')

if __name__ == "__main__":
    main()

