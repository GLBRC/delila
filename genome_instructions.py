#!/home/mplace/anaconda3.7/bin/python
"""genome_instructions.py

Create a single genome instruction file.

Notes
----- 

Delila normally only handles one chromosome at a time, but we need to handle
multiple chromosomes.  Idea is based on Tom's script (http://users.fred.net/tds/lab/ftp/delilagenome) 

Method
------

Read TSS file line by line.  Parse each line identifying the chromosome.
    
Parameters
----------

f : str
    A text file with each line representing a TSS site.

h : str
    show help message and exit

d : int
    Downstream base position

o : str
    Organism

p : int
    Position of the TSS site, generally upstream of gene start.

u : int
    Upstream base position

example input file:

    NC_007488.2     RSP_4039_1700   forward 1700
    NC_007488.2     RSP_4038_2627   forward 2627
    NC_007488.2     RSP_4037_3543   forward 3543
    NC_007488.2     RSP_4025_19218  reverse 19218


Example
-------
    usage:

        genome_instructions.py -f input.txt -d 10  -o rhodo -p 10 -u 10

Output
------

A text file with a 2 line header followed by site request information :

title "test grab";
organism hg19;

chromosome chr1;
piece hg19-chr1;
name "Example 1, sequence 1";
get from 1 to 5;
name "Example 1, sequence 2";
get from 1 to 10;
name "Example 1, sequence 3";
get from 6 to 10;

chromosome chrM;
piece hg19-chrM;
name "Example 2, sequence 1";
get from 1 to 5;
name "Example 2, sequence 2";
get from 1 to 10;
name "Example 2, sequence 3";
get from 6 to 10;

"""
from datetime import date
import argparse 
import os
import sys

def main():
    
    cmdparser = argparse.ArgumentParser(description="Create whole genome delila instruction file.",
                                        usage='%(prog)s -f <TSS_site_file.txt> -o organism -u <int> -d <int>'  ,prog='genome_instructions.py'  )
    cmdparser.add_argument('-d', '--down',  action='store', dest='DOWN',
                            help='Downstream base position', metavar='')
    cmdparser.add_argument('-f', '--file',  action='store', dest='FILE',
                            help='Text file, containing TSS sites', metavar='')
    cmdparser.add_argument('-o', '--organism', action='store', dest='ORGANISM',help='Organism', metavar='')
    cmdparser.add_argument('-p', '--position', action='store', dest='POS',
                            help='Position of the TSS site, generally upstream of gene start.')
    cmdparser.add_argument('-u', '--upstream', action='store', dest='UP',
                            help='Upstream base position', metavar='')    
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)

    # get the downstream base position
    if cmdResults['DOWN']:
        downPos = cmdResults['DOWN']
    else:
        downPos = 10

    # get the input tss site file
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']     
    
    # get the orgamism name
    if cmdResults['ORGANISM'] is not None:
        organism = cmdResults['ORGANISM']    

    # get the TSS position
    if cmdResults['POS']:
        tssPos = int(cmdResults['POS'])
    else:
        tssPos = 10
    
    # get the upstream base position
    if cmdResults['UP']:
        upPos = cmdResults['UP']
    else:
        upPos = 10    
    
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
    
    # create output file name
    outName = organism + '_TSS.inst'
    # open output file for writing
    with open(outName, 'w') as out:
        # each instruction file requires a 4 line header    
        out.write('title \"{} -10 elements TSS sites version 1.0 {}  {}\";\n'.format(organism, outName, currDate));
        out.write('organism {};\n'.format(organism))

        previous = ''     # use for printing chromosome and piece only once
        # write results with separate sections for chromosomes
        for chrom in data.keys():
            if previous != chrom:
                out.write('\nchromosome {};\n'.format(chrom))
                out.write('piece {};\n'.format(organism + '-' + chrom))
                previous = chrom

            # now process each site 
            for sites in data[chrom].values():
                dat = sites.rstrip().split('\t')
                out.write('name \"{}\";\n'.format(dat[1]))    # each site's info is precedded by the site name 
                
                # handle the strandedness and write request to instruction file
                if dat[2] == 'forward':
                    direction = '+'
                    pos = str(int(dat[3]) - tssPos)   
                    out.write('get from {} -{} to {} +{} direction {};\n'.format(pos, str(downPos), pos, str(upPos),direction )) 
                else:
                    direction = '-'
                    pos = str(int(dat[3]) + tssPos) 
                    out.write('get from {} +{} to {} -{} direction {};\n'.format(pos, str(upPos), pos, str(downPos) ,direction)) 
                
    out.close()

if __name__ == "__main__":
    main()

