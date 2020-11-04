#!/home/mplace/anaconda3.7/bin/python
"""removeRI_instructions.py

Retain sites with RI greater than 0 from Merged instructions file.

Notes
----- 

Delila only handles one chromosome at a time, but can be "tricked" into using
multiple chromosomes.   

Method
------

Read merged instructions file, parse and match sites to be retained from the RI_out.txt file. 
    
Parameters
----------

f : str
    The previously generated merged_instructions file.

r : str
    The RI_out.txt file, listing sites to be retained.

Example
-------
    usage:

        removeRI_instructionss.py -f MERGED_INSTRUCTIONS.txt -r RI_out.txt

Output
------

A instructions file with multiple chromosomes. 

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
    # variables
    chromTracker = ''     
    piece = False
        
    with open(inFile, 'r') as f, open('NEW_MERGED_INSTRUCTIONS.txt','w') as out:
        # gather header information
        for ln in range(4):
            out.write(f.readline())                   
            # gather the pieces in the book
        for ln in f:
            if ln.startswith('chromosome'):
                out.write('\n' + ln)
            if ln.startswith('piece'):
                out.write(ln)
            if ln.startswith('name'):
                name = re.sub( '"','', ln.rstrip(';\n').split()[1])
                if name in riIDs:
                    out.write(ln)
                    out.write(f.readline())

                    

def main():
    
    cmdparser = argparse.ArgumentParser(description="Filter instructions file by ri score.",
                                        usage='%(prog)s -f <input.txt> -r <RI_out.txt>'  ,prog='removeRI_instructions.py'  )  
    cmdparser.add_argument('-f', '--file', action='store', dest='FILE', help='Merged instructions file to parse.', metavar='')
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