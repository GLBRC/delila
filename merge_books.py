#!/home/mplace/anaconda3.7/bin/python
"""merge_books.py

Merge delila book files to handle multiple chromosomes.

Notes
----- 

Delila only handles one chromosome at a time, but can be "tricked" into using
multiple chromosomes.   

Method
------

Read each book file and parse. Write each chromosome's lines to 
to the same new file.  The original chromosome name and piece tag values have 
to be made unique.
    
Parameters
----------

f : str
    A text file with each line representing a TSS site.


Example
-------
    usage:

        merge_books.py -f input.txt 

Output
------

A delila book file with multiple chromosomes 

"""
import argparse
from itertools import islice 
import os
import sys

def main():
    
    cmdparser = argparse.ArgumentParser(description="Merge Delila chromosome book files.",
                                        usage='%(prog)s -f <input.txt>'  ,prog='merge_books.py'  )  
    cmdparser.add_argument('-f', '--file', action='store', dest='FILE', help='List of delila book files to parse.', metavar='')
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)

    # get the input file, the list of book files to process
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']     

    # variables used to process and merge the books
    chromHeader = []     
    pieces = []
    first  = False

    with open(inFile, 'r') as fl, open('MERGED_BOOK_TEST.txt','w') as out:
        for bookfile in fl:
            bookfile = bookfile.rstrip()
            with open( bookfile, 'r') as f:
                # gather header information from the first file, ignore the other file headers
                if not first:
                    for ln in range(5):
                        out.write(f.readline())       # write title, organism out to file
                    for ln in range(5):
                        chromHeader.append(f.readline())                   
                    first = True
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
                            ln = ln.rstrip() + 'x' + '\n'   # need a unique name that differs from chromosome
                        out.write(ln)
                out.write('chromosome\n')
                pieces = []                           # reset for next book to use
        out.write('organism')                         # file end here

if __name__ == "__main__":
    main()

