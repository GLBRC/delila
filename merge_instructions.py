#!/home/mplace/anaconda3.7/bin/python
"""merge_books.py

Merge delila instruction files to handle multiple chromosomes.

Notes
----- 

Delila only handles one chromosome at a time, but can be "tricked" into using
multiple chromosomes.   

Method
------

Read each instruction file and parse. Write each chromosome's lines to 
to the same new file.  The original chromosome name and piece tag values have 
to be made unique.
    
Parameters
----------

f : str
    A text file with each line representing a TSS site.


Example
-------
    usage:

        merge_instructions.py -f input.txt 

Output
------

A delila instruction file with multiple chromosomes 

"""
import argparse
from itertools import islice 
import os
import re
import sys

def main():
    
    cmdparser = argparse.ArgumentParser(description="Merge Delila instruction files.",
                                        usage='%(prog)s -f <input.txt>'  ,prog='merge_instructions.py'  )  
    cmdparser.add_argument('-f', '--file', action='store', dest='FILE', help='List of delila instruction files to parse.', metavar='')
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)

    # get the input file, the list of book files to process
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']     

    # variables used to process and merge the instructions
    first  = False
    organism = ''

    with open(inFile, 'r') as fl, open('MERGED_INSTRUCTIONS.txt','w') as out:
        for inst in fl:
            inst = inst.rstrip()
            with open( inst, 'r') as f:
                # gather header information from the first file, ignore the other file headers
                if not first:
                    out.write(f.readline())       # write title, organism out to file  
                    organism = f.readline()
                    out.write(organism)
                    organism = organism.rstrip().split()[1]
                    out.write('\n')                
                    first = True
                else: 
                    list(islice(f,2))                # unneeded header, send to ether
                
                # gather the pieces in the book
                f.readline()
                piece = f.readline()
                chrom = 'chromosome ' + piece.split()[1] + '\n'
                piece =  piece.split()[0] + ' ' + organism.rstrip(';') + '-' + piece.split()[1] + '\n'
                out.write(chrom)
                out.write(piece)

                for lines in f:
                    out.write(lines)
                out.write('\n')  

if __name__ == "__main__":
    main()