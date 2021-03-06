#!/usr/bin/env python

"""
Script to organize the Delila outputs of Malign_list and Ri Results and write
them to files.

The script requires outputs of the delila pipeline:  LIST and RIXYIN

It will write the files to a more readable format, combine them, and then
retain ONLY those that have a positive (>0) Ri value from RIXYIN file.

The surviving genes will be formatted into a file to use in the delila_instructions.py script.

@author: kevin myers
"""
import argparse
import sys

def processFiles( malign_file, rixyin_file, output_file ):
    with open('malign_list_organized.txt', 'w') as f:
        f.write("SiteID\tGeneID\tChromosome\tMinus10_Position\tStrand\tSequence\n")
    
    idDict = {}    
    idlist = []
    with open(malign_file, 'r') as f:
        for _ in range(14):
            next(f)
        for line in f:
            if line.startswith(" "):
                continue
            if line.startswith("\x0cpage"):
                continue
            else:
                geneID = line.rstrip().split()[0]         # RSP_4039_1700 in the case of R.sphaeroides
                chromosome = line.rstrip().split()[1].split('-')[-1]     # R.sphaeroides-2.4.1-NC_007488.2   organism plus chromosome ID
                minus10 = line.rstrip().split()[2]        # 1691  base pair position 
                strand = line.rstrip().split()[3]         # + or -
                siteID = line.rstrip().split()[4]         # row number from malign list file
                sequence = line.rstrip().split()[5]       # gctcagcgcatagtcgtcctt              
                if strand == "+":
                    strand2 = "forward"
                    position = int(minus10) + 9
                else:
                    strand2 = "reverse"
                    position = int(minus10) - 10
                idlist.append(f"{siteID}\t{geneID}\t{chromosome}\t{minus10}\t{strand}\t{sequence}\n")
                idDict[siteID] = []
                idDict[siteID].append(f"{chromosome}\t{geneID}\t{strand2}\t{position}\n")
                
    for each in idlist:
        with open("malign_list_organized.txt", 'a') as f:
            f.write(each)
    
    with open("ri_results_organized.txt", "w") as f:
        f.write("SiteID\tGeneID\tChromosome\tStrand\tPosition\tSequence\tLength\tMinus10_Position\tRi_Scoare\tValue\tStandard Deviation\n")
    
    rilist = []
    forInst = []
    with open(rixyin_file, 'r') as f:
        for line in f:
            if line.startswith("*"):
                continue
            else:
                siteID = line.rstrip().split()[0]        # row number from rixyin, = malign list row number
                chromosome = line.rstrip().split()[1]    # R.sphaeroide, automatically truncated by ri ?
                seq = line.rstrip().split()[2]           # gatcggcgtatacagcaatct
                length = line.rstrip().split()[3]        # length of seq, i.e. 21
                minus10 = line.rstrip().split()[4]       # minus 10 site, base pair position
                riScore = line.rstrip().split()[5]       # numeric score -1.205503
                value = line.rstrip().split()[6]         # value from the values file (0 if values is empty)
                stdDev = line.rstrip().split()[7]        # stsDev of Rindividual for that sequence
                if siteID in idDict:
                    geneID = idDict[siteID]
                    geneID2 = '\t'.join([str(elem) for elem in geneID])
                    if float(riScore) > 0:
                        rilist.append(f"{siteID}\t{geneID2}\t{chromosome}\t{seq}\t{length}\t{minus10}\t{riScore}\t{value}\t{stdDev}\n")
                        forInst.append(idDict[siteID][0])
                else:
                    continue
    # write parsed ri results for a check
    with open("ri_results_organized.txt", 'w') as f:
        for each in rilist:
            f.write(each)
    # write a new TSS site file for the refining stage of the pipeline
    with open(output_file, 'w') as f:
        for each in forInst:
            f.write(each)
    
def main():
    cmdparser = argparse.ArgumentParser(description="Removal of sites with negative Ri values in Delila.",
                                        usage='%(prog)s -f <fastq file list.txt> [optional args: -a -r -d -ref ]' ,prog='organizing_ri_delila_results.py'  )
    cmdparser.add_argument('-l', '--malign_list',    action='store', dest='MALIGN_LIST',    help='malign_list file from Delila pipeline.', metavar='')
    cmdparser.add_argument('-r', '--rixyin_file', action='store', dest='RIXYIN_FILE', help='Rixyin results file from Ri in the Delila pipeline', metavar='')
    cmdparser.add_argument('-d', '--detail',  action='store_true', dest='DETAIL',  help='Print a more detailed description of program.')
    cmdparser.add_argument('-o', '--output', action='store', dest='OUTPUT', help='Output file name. Default is "positive_Ri_updated_locations.txt".',metavar='')
    cmdResults = vars(cmdparser.parse_args())
    
        # if no args print help
    if len(sys.argv) == 1:
        print("")
        cmdparser.print_help()
        sys.exit(1)
        
    if cmdResults['DETAIL']:
        print("\norganizing_ri_delila_results.py")
        print("\nPurpose: Organization of Delila results to identify those with positive Ri values.\nWill write new instruction file to generate updated logo in the Delila pipeline.")
        print("\nInput : The malign_list result and the rixyin result files.")
        print("\nThis program will remove entries with negative Ri values using the two input files.")
        print("\nPlease use a dedicated directory for running pipeline.")
        print("This script is designed to run on the GLBRC scarcity servers.")
        print("See Kevin Myers for problems with this script.")
        print("Email:  bioinformaticshelp@glbrc.wisc.edu\n")
        sys.exit(1)
        
    if cmdResults['MALIGN_LIST'] is not None:
        malign_file = cmdResults['MALIGN_LIST']
    else:
        print("Please enter a malign results file\n")
        sys.exit(1)
        
    if cmdResults['RIXYIN_FILE'] is not None:
        rixyin_file = cmdResults['RIXYIN_FILE']
    else:
        print("Please enter a rixyin results file\n")
        sys.exit(1)
        
    if cmdResults["OUTPUT"] is not None:
        output_file = cmdResults['OUTPUT']
    else:
        output_file = "ositive_Ri_updated_locations.txt"   
        
    processFiles( malign_file, rixyin_file, output_file )
    
if __name__ == "__main__":
    main()


