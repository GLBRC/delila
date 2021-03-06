#!/usr/bin/env python
"""filter_sites.py

Remove sites within user defined number of bases of each other.

Notes
----- 

Assumes the sites are sorted by chromosome and then by position low to high.   

Method
------

Read site file line by line.  Parse each line identifying the chromosome
and check the next position for "x" base overlap.
    
Parameters
----------

f : str
    A text file with each line representing a site.

h : str
    show help message and exit

n : int
    Number of base positions overlap to remove.

example input file:

    NC_007488.2     RSP_4039_1700   forward 1700
    NC_007488.2     RSP_4038_2627   forward 2627
    NC_007488.2     RSP_4037_3543   forward 3543
    NC_007488.2     RSP_4025_19218  reverse 19218

Example
-------
    usage:

        filter_sites.py -f input.txt -n 10  

Output
------

A site text file of the same format as the input. 
All sites within "X" bases of each other removed.

"""
import argparse 
import os
import sys

def filterSites(inFile, baseNum):
    '''
    Filter sites within "X" bases.

    Parameters
    ----------
    inFile : str
        Input site file, text file
    baseNum : int
        Number of bases to use as filter, i.e. if site A is within baseNum remove site.
        
    '''
    chrom     = ''    # keep track of the previous site's chrom
    strand    = ''    # keep track of the previous site's strand
    previous  = 0     # keep track of the previous site's position

    # open and parse tss file
    with open(inFile, 'r') as f:
        for line in f:
            site = int(line.rstrip().split('\t')[3])   # get position 
            dat  = line.split('\t')                    
            if dat[0] == chrom and dat[2] == strand:   # site on same chrom and strand
                    if site - previous < baseNum:      # if site is within X bases skip
                        chrom = dat[0]
                        strand = dat[2]
                        continue
                    else:                              # site is greater than X bases
                        print(line.rstrip())           # keep it
                        previous = site                # now set previous to current position
                        chrom = dat[0]                 
                        strand = dat[2]
            else:
                print(line.rstrip())                   # if not on the same chromosome print
                previous = site
                chrom = dat[0]
                strand = dat[2]
            
    f.close()

def main():
    
    cmdparser = argparse.ArgumentParser(description="Remove sites with x number of bases overlap, file MUST BE SORTED.",
                                        usage='%(prog)s -f <site_file.txt> -n <int>'  ,prog='filter_sites.py'  )
    cmdparser.add_argument('-f', '--file',  action='store', dest='FILE',
                            help='Text file, containing sites', metavar='')
    cmdparser.add_argument('-n', '--number', action='store', dest='NUM',help='Number of base overlap, default(15)', metavar='') 
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)

    # get the input tss site file
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']  
    else:
        print('\n\t-f Sites file not found')
        cmdparser.print_help()
        sys.exit(1)   
    
    # get the number of bases
    if cmdResults['NUM']:
        baseNum = int(cmdResults['NUM'])
    else:
        baseNum = 15
    
    filterSites(inFile, baseNum)
    
if __name__ == "__main__":
    main()
    