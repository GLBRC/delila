#!/home/mplace/anaconda3.7/bin/python
"""splitTSS.py

Split input Transcription Start site file by chromosome.

Notes
----- 

Delila only handles one chromosome at a time.   

Method
------

Read TSS file line by line.  Parse each line identifying the chromosome.
Write each chromosome's lines to a new file, named by chromosomeName_TSS.txt.
    
Parameters
----------

f : str
    A text file with each line representing a TSS site.

ex.
    NC_007488.2     RSP_4039_1700   forward 1700
    NC_007488.2     RSP_4038_2627   forward 2627
    NC_007488.2     RSP_4037_3543   forward 3543
    NC_007488.2     RSP_4025_19218  reverse 19218


Example
-------
    usage:

        splitTSS.py -f input.txt

"""
from datetime import date
import argparse 
import os
import sys
           


def main():
    
    cmdparser = argparse.ArgumentParser(description="Split TSS file by chromosome.",
                                        usage='%(prog)s -f <TSS_site_file.txt>' ,prog='splitTSS.py'  )
    cmdparser.add_argument('-f', '--file',     action='store', dest='FILE',    help='Text file, containing TSS sites', metavar='')
    cmdparser.add_argument('-o', '--organism', action='store', dest='ORGANISM',help='Organism', metavar='')
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)
        
    # get the input tss site file
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']     
    # get the orgamism name
    if cmdResults['ORGANISM'] is not None:
        organism = cmdResults['ORGANISM']      
    # today's date for a time stamp
    currDate = date.today().strftime("%Y/%m/%d")
    # dictionary to hold input data, as a dict of dicts
    data = {}
    # open and parse tss file
    with open(inFile, 'r') as f:
        for line in f:
            site = line.rstrip().split('\t')
            if site[0] not in data:           # is the chrom in the dict, if not add it
                data[site[0]] = {}
                data[site[0]][site[1]] = line # use the location info as 2nd key
            else:
                data[site[0]][site[1]] = line

    f.close()

    # write results working with one chromosome at a time
    for chrom in data.keys():
        # create output file name
        outName = chrom + '_TSS.inst'
        # open output file for writing
        with open(outName, 'w') as out:
            # each instruction file requires a 4 line header    
            out.write('title \"{} -10 elements TSS sites version 1.0 {}  {};\n'.format(organism, outName, currDate));
            out.write('organism {};\n'.format(organism))
            out.write('chromosome {};\n'.format(organism))
            out.write('piece {};\n'.format(chrom))
            # now process each site 
            for sites in data[chrom].values():
                dat = sites.rstrip().split('\t')
                out.write('name \"{}\";\n'.format(dat[1]))    # each site's info is precedded by the site name 
                
                # handle the strandedness and write request to instruction file
                if dat[2] == 'forward':
                    direction = '+'
                    pos = str(int(dat[3]) - 10)
                    out.write('get from {} -8 to {} +5 direction {};\n'.format(pos, pos, direction )) 
                else:
                    direction = '-'
                    pos = str(int(dat[3]) + 10) 
                    out.write('get from {} +8 to {} -5 direction {};\n'.format(pos, pos, direction )) 
                
        out.close()

if __name__ == "__main__":
    main()

