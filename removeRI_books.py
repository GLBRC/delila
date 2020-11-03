#!/home/mplace/anaconda3.7/bin/python
"""removeRI_books.py

Retain sites with RI greater than 0 from Merged delila book file.

Notes
----- 

Delila only handles one chromosome at a time, but can be "tricked" into using
multiple chromosomes.   

Method
------

Read merged book file, parse and match sites to be retained from the RI_out.txt file. 
    
Parameters
----------

f : str
    The previously generated merged_book file.

r : str
    The RI_out.txt file, listing sites to be retained.

Example
-------
    usage:

        removeRI_books.py -f MERGED_BOOK.txt -r RI_out.txt

Output
------

A delila book file with multiple chromosomes 

"""
import argparse
from itertools import islice 
import os
import re
import sys

def parseRI(riout):
    """
    Parse RI_out.txt file into dictionary.
    Key = gene ID  value = 1
    """
    ri = {}
    with open(riout, 'r') as f:
        for ln in f:
            id = ln.split('\t')[1]
            if id not in ri:
                ri[id] = 1
    return ri

def removeRI(inFile, riIDs):
    '''
    Parse and retain sites with RI score greater than zero.

    Parameters
    ----------
    inFile : str
        Text file with a list of book files to merge
    riIDs : dict
        Dictionary with gene ID's to retain.
    '''
    # variables used to process and merge the books
    chromTracker = set()     
    pieces = []
    organism = ''
    counter = 1


    with open(inFile, 'r') as f, open('NEW_MERGED_BOOK.txt','w') as out:
        # gather header information from the first file, ignore the other file headers
        for ln in range(10):
            out.write(f.readline())                   
            # gather the pieces in the book
        while True:
            peek = f.readline()                    # is the next line the start of a piece
            if peek.startswith('piece'):          
                wholePiece = list(islice(f,18))    # should be rest of the piece
                id =  wholePiece[1].rstrip().split()[1]   # check if the gene ID should be retained
                if id in riIDs:                           # gene ID found, now write piece
                    out.write(peek)
                    wholePiece[3] = f"* # {str(counter)}\n"
                    counter +=1
                    for row in wholePiece:
                        out.write(row)
            elif peek.startswith('chrom'):                # chromsome line
                out.write(peek)
                counter = 1                               # reset counter for each chromosome
                newChrom = list(islice(f,5)) 
                if len(newChrom) == 5:                    # write chromosome info
                    for c in newChrom:
                        out.write(c)
                else:                                     # we are at the end of the file
                    out.write(f.readline())
                    out.write("organism")
                    break                

def main():
    
    cmdparser = argparse.ArgumentParser(description="Merge Delila chromosome book files.",
                                        usage='%(prog)s -f <input.txt> -r <RI_out.txt>'  ,prog='removeRI_books.py'  )  
    cmdparser.add_argument('-f', '--file', action='store', dest='FILE', help='Merged delila book file to parse.', metavar='')
    cmdparser.add_argument('-r', '--ri', action='store', dest="RIOUT",  help='RI_out.txt file', metavar='')
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) < 3:
        print("")
        cmdparser.print_help()
        sys.exit(1)

    # get the input file, name of book file to process
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']     

    # get the RI_out.txt file
    if cmdResults['RIOUT'] is not None:
        riFile = cmdResults['RIOUT']

    riIDs = parseRI(riFile)
    removeRI(inFile, riIDs)    

if __name__ == "__main__":
    main()