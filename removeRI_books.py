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


def removeRI(inFile):
    '''
    Parse and retain sites with RI score greater than zero.

    Parameters
    ----------
    inFile : str
        Text file with a list of book files to merge

    '''
    # variables used to process and merge the books
    chromHeader = []     
    pieces = []
    first  = False
    organism = ''

    with open(inFile, 'r') as fl, open('MERGED_BOOK.txt','w') as out:
        for bookfile in fl:
            bookfile = bookfile.rstrip()
            with open( bookfile, 'r') as f:
                # gather header information from the first file, ignore the other file headers
                if not first:
                    out.write(f.readline())       # write title, organism out to file
                    out.write(f.readline())
                    org = f.readline()
                    out.write(org)
                    out.write(f.readline())
                    out.write(f.readline())
                    for ln in range(5):
                        chromHeader.append(f.readline())                   
                    first = True
                    organism = org.rstrip().split()[1]
                else: 
                    list(islice(f,10))                # unneeded header, send to ether
                
                # gather the pieces in the book
                while True:
                    eachPiece = list(islice(f,19))    # should be one entire piece
                    if not eachPiece:                 # end when nothing more to read
                        break
                    else:
                        if not eachPiece[0] == 'chromosome\n':   # if not end of file
                            pieces.append(eachPiece)

                chromHeader[1] = pieces[0][1]         # change chromosome name 
                for ch in chromHeader:                # write chromosome info
                    out.write(ch)
                for p in pieces:                      # write each pieces info
                    for i,ln in enumerate(p):
                        if i == 1:
                            ln = re.sub('\* ', '* ' + organism.rstrip() + '-', ln)  # need a unique name that differs from chromosome
                        out.write(ln)
                out.write('chromosome\n')
                pieces = []                           # reset for next book to use
        out.write('organism')                         # file end here

def main():
    
    cmdparser = argparse.ArgumentParser(description="Merge Delila chromosome book files.",
                                        usage='%(prog)s -f <input.txt> -r <RI_out.txt>'  ,prog='removeRI_books.py'  )  
    cmdparser.add_argument('-f', '--file', action='store', dest='FILE', help='Merged delila book file to parse.', metavar='')
    cmdparser.add_argument('-r', '--ri', action='store', dest="RIOUT",  help='RI_out.txt file', metavar='')
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)

    # get the input file, name of book file to process
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']     

    # get the RI_out.txt file
    if cmdResults['RIOUT'] if not None:
        riFile = cmdResults['RIOUT']

    #removeRI(inFile)    

if __name__ == "__main__":
    main()