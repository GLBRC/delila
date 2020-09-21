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

def newLib(inFile):
    '''
    Rename the chromosome and all the piece tags to match the merged book and merged instructions.
    '''
     # variables used to process and merge the books
    chromHeader = []     
    pieces = []
    first  = False
    dna = False

    with open(inFile, 'r') as f, open('New_lib1.txt','w') as out:
        # gather header information 
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

        # grab the first piece (this is a tag, spans 15 lines)
        pieceInfo = list(islice(f,15))

        # now rename chromosome to match what is in the merged book & instructions
        chromHeader[1] = pieceInfo[1]
        for ch in chromHeader:
            out.write(ch)

        # add orgamism name to piece tag
        pieceInfo[1] = '* ' + organism + '-' + pieceInfo[1].split()[1] + '\n'

        # write out first piece tab
        for p in pieceInfo:
            out.write(p)

        # now process the rest of the file
        while True:
            nextline = f.readline().rstrip()
            if not nextline:                        # if end of file
                break
            elif nextline == 'organism':            # write organism
                out.write(nextline)
            elif nextline == 'dna':                 # dna section tag
                out.write(nextline + '\n')
            elif re.match('\*  [g|a|t|c]',nextline): # write all the sequence out
                out.write(nextline + '\n')
            elif nextline == 'piece':
                out.write(nextline + '\n')
                # grab piece
                pieceInfo = list(islice(f,15))
                # add orgamism name to piece tag
                if pieceInfo[0] == 'chromosome\n':   # handle the closing tags for the file
                    for p in pieceInfo:
                        out.write(p)
                else:
                    pieceInfo[1] = '* ' + organism + '-' + pieceInfo[1].split()[1] + '\n'
                    for p in pieceInfo:
                        out.write(p)
            else:
                out.write(nextline)

def moveLib(inFile):
    '''
    Move original lib1 to lib1_original and link new_lib1.txt to lib1
    '''
    os.rename(inFile, 'lib1_original.txt')
    os.unlink('l1')
    os.symlink('New_lib1.txt', 'l1')
    with open("lib1", 'w') as f:
        pass
        f.close()

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

    newLib(inFile)   
    moveLib(inFile)

if __name__ == "__main__":
    main()