#!/home/mplace/anaconda3.7/bin/python
"""delila_instructions.py

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

        delila_instructions.py -f input.txt -d 10  -o rhodo -p 10 -u 10

Output
------

A text file with a four line header followed by site request information :

title "rhodobacter -10 elements TSS sites version 1.0 NC_007490.2_TSS.txt  2020/07/14;
organism rhodobacter;
chromosome rhodobacter;
piece NC_007490.2;
name "RSP_4139_3142";
get from 3132 -8 to 3132 +5 direction +;
name "RSP_4139_3144";
get from 3134 -8 to 3134 +5 direction +;

"""
from datetime import date
import argparse 
import os
import sys

def main():
    
    cmdparser = argparse.ArgumentParser(description="Split TSS file by chromosome.",
                                        usage='%(prog)s -f <TSS_site_file.txt> -u <int> -d <int>'  ,prog='delila_instructions.py'  )
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
    else:
        print('\n\t-f TSS file not found')
        cmdparser.print_help()
        sys.exit(1)   
    
    # get the orgamism name
    if cmdResults['ORGANISM'] is not None:
        organism = cmdResults['ORGANISM']    
    else:
        print('\n\t-o organism not found')
        cmdparser.print_help()
        sys.exit(1)

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

    # instructions.list contains the instruction files for each chromosome in genome
    instructions = []

    # write results working with one chromosome at a time
    for chrom in data.keys():
        # create output file name
        outName = chrom + '_TSS.inst'
        instructions.append(outName)
        # open output file for writing
        with open(outName, 'w') as out:
            # each instruction file requires a 4 line header    
            out.write('title \"{} -10 elements TSS sites version 1.0 {}  {}\";\n'.format(organism, outName, currDate));
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
                    pos = str(int(dat[3]) - tssPos)   
                    out.write('get from {} -{} to {} +{} direction {};\n'.format(pos, str(upPos), pos, str(downPos),direction )) 
                else:
                    direction = '-'
                    pos = str(int(dat[3]) + tssPos) 
                    out.write('get from {} +{} to {} -{} direction {};\n'.format(pos, str(downPos), pos, str(upPos) ,direction)) 
                
        out.close()

    # delete a previously created file
    if os.path.exists('instructions.list'):
        os.unlink('instructions.list')
    
    # write new list to file
    with open('instructions.list', 'w') as out:
        for instFile in instructions:
            out.write('{}\n'.format(instFile))
    out.close()
    
if __name__ == "__main__":
    main()

