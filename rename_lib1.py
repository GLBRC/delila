#!/home/mplace/anaconda3.7/bin/python
"""rename_lib1.py

Rename chromosome and piece tags in original lib1 for working with multiple chromosomes.

Notes
----- 

Delila only handles one chromosome at a time, but can be "tricked" into using
multiple chromosomes.   
    
Parameters
----------

None, it is assumed that lib1 exists in current directory.


Example
-------
    usage:

        rename_lib1.py 

Output
------

lib1 file with chromosome and piece tags rename to work with multiple chromosomes 

"""
import argparse
from itertools import islice 
import os
import re
import sys

def main():
    
    cmdparser = argparse.ArgumentParser(description="Rename chromosome & piece tags in lib1 file.",
                                        usage='%(prog)s '  ,prog='rename_lib1.py'  )  
    cmdResults = vars(cmdparser.parse_args())
        
    # get the input file, the list of book files to process
    if not os.path.exists('lib1'):
        print('\n\t lib1 file not found, lib1 is required to run delila a second time.\n') 
        cmdparser.print_help()
        sys.exit(1) 
    else:
        inFile = 'lib1'

    # variables used to process and merge the books
    chromHeader = []     
    pieces = []
    first  = False
    dna = False

    with open(inFile, 'r') as f, open('New_lib1.txt','w') as out:
        # gather header information from the first file, ignore the other file headers
        if not first:
            out.write(f.readline())       # title line
            out.write(f.readline())       # organism tag
            org = f.readline()            # capture organism name for later use
            out.write(org)                # organism name
            organism = org.split()[1]     # get organism name, remove the other stuff
            out.write(f.readline())       # *
            out.write(f.readline())       # bases
            first = True
        # get chromosome header
        for ln in range(5):
            chromHeader.append(f.readline())     # chromHeader[1] is the name

        # grab piece
        pieceInfo = list(islice(f,15))

        # now rename chromosome
        chromHeader[1] = pieceInfo[1]
        for ch in chromHeader:
            out.write(ch)

        # add orgamism name to piece tag
        pieceInfo[1] = '* ' + organism + '-' + pieceInfo[1].split()[1] + '\n'

        for p in pieceInfo:
            out.write(p)

        # now process the rest of the file
        while True:
            nextline = f.readline()
            if not line:
                break
            if nextline == 'dna':
                

        
        #nextline = f.readline().rstrip()     # should be "dna"
        #if nextline == "dna":
        #    while not dna:
        #        seq = f.readline()
        #        if re.match("\*  [g|a|t|c]", seq):
        #            print(seq)
         

                
    '''        for ln in range(5):
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
    '''

if __name__ == "__main__":
    main()

