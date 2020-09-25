#!/home/mplace/anaconda3.7/bin/python
"""filter_TSS.py

Remove TSS site within user defined number of bases of each other.

Notes
----- 

Assumes the sites are sorted, low to high.   

Method
------

Read TSS file line by line.  Parse each line identifying the chromosome
and check the next position for "x" base overlap.
    
Parameters
----------

f : str
    A text file with each line representing a TSS site.

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

        filter_TSS.py -f input.txt -n 10  

Output
------

A TSS site text file of the same format as the input.


"""
import argparse 
import os
import sys

def main():
    
    cmdparser = argparse.ArgumentParser(description="Filter TSS file by number of bases overlap.",
                                        usage='%(prog)s -f <TSS_site_file.txt> -n <int>'  ,prog='filter_TSS.py'  )
    cmdparser.add_argument('-f', '--file',  action='store', dest='FILE',
                            help='Text file, containing TSS sites', metavar='')
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
        print('\n\t-f TSS file not found')
        cmdparser.print_help()
        sys.exit(1)   
    
    # get the number of bases
    if cmdResults['NUM']:
        baseNum = int(cmdResults['NUM'])
    else:
        baseNum = 15
    
    chrom = ''          # keep track of the previous site's chrom
    strand = ''         # keep track of the previous site's strand
    previous = 0        # keep track of the previous site's position
    # open and parse tss file
    with open(inFile, 'r') as f:
        for line in f:
            site = int(line.rstrip().split('\t')[3])   # get position 
            dat  = line.split('\t')                    
            if dat[0] == chrom and dat[2] == strand:                   # compare strands
                    if site - previous < baseNum:      # if site is within X bases skip
                        previous = site
                        chrom = dat[0]
                        strand = dat[2]
                        continue
                    else:
                        print(line.rstrip())
                        previous = site
                        chrom = dat[0]
                        strand = dat[2]
            else:
                print(line.rstrip())
                previous = site
                chrom = dat[0]
                strand = dat[2]
            
    f.close()
    
if __name__ == "__main__":
    main()

