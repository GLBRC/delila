#!/home/mplace/anaconda3.7/bin/python
"""define_site_position.py

Shift the position up or downstream for a sites file by a user defined number of bases.

Method
------

Read sites file line by line.  Parse each line and shift
the position by +/- value taking account of the strand.
    
Parameters
----------

f : str
    A text file with each line representing a Site.

h : str
    show help message and exit

n : int
    Number of base positions to shift

example input file:

    NC_007488.2     RSP_4039_1700   forward 1700
    NC_007488.2     RSP_4038_2627   forward 2627
    NC_007488.2     RSP_4037_3543   forward 3543
    NC_007488.2     RSP_4025_19218  reverse 19218

Example
-------
    usage:

    define_site_position.py -f input.txt -n -10  

    This results in a site file of the same format as the input.
    All sites shifted upstream by 10 bases.

    alternatively:

    define_site_position.py -f input.txt -n 10

    This results in a site file of the same format as the input.
    All sites shifted downstream by 10 bases.

"""
import argparse 
import os
import sys

def shiftSites(inFile, baseNum):
    '''
    Shift sites by "X" bases taking into account the strand.

    Parameters
    ----------
    inFile : str
        Input Sites file, text file
    baseNum : int
        Number of bases to shift.
        
    '''
    # open and parse Sites file
    with open(inFile, 'r') as f:
        for line in f:
            fixedPosition = 0                         # clear fixedPosition each time
            site = line.rstrip().split('\t')          
            position = int(site[3])                   # last value is the position
            if baseNum < 0:                           # move positions upsteam 
                if site[2] == 'forward':                  
                    fixedPosition = position - abs(baseNum)   
                else:
                    fixedPosition = position + abs(baseNum)
            else:
                if site[2] == 'forward':             # move positions downstream     
                    fixedPosition = position + abs(baseNum)   # move 
                else:
                    fixedPosition = position - abs(baseNum)               

            site[-1] = str(fixedPosition)
            outLine = '\t'.join(site)
            print(outLine)          
    f.close()

def main():
    
    cmdparser = argparse.ArgumentParser(description="Shift sites by x number of bases.",
                                        usage='%(prog)s -f <site_file.txt> -n <int>'  ,prog='filter_Sites.py'  )
    cmdparser.add_argument('-f', '--file',  action='store', dest='FILE',
                            help='Text file, containing sites', metavar='')
    cmdparser.add_argument('-n', '--number', action='store', dest='NUM',help='Number of base to shift (negative = upstream)', metavar='') 
    cmdResults = vars(cmdparser.parse_args())
        
    # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)

    # get the input sites file
    if cmdResults['FILE'] is not None:
        inFile = cmdResults['FILE']  
    else:
        print('\n\t-f Site file not found')
        cmdparser.print_help()
        sys.exit(1)   
    
    # get the number of bases
    if cmdResults['NUM']:
        baseNum = int(cmdResults['NUM'])
    else:
        print('\n\t-n number of bases to shift is required')
        cmdparser.print_help()
        sys.exit(1)
    
    shiftSites(inFile, baseNum)
    
if __name__ == "__main__":
    main()
    
