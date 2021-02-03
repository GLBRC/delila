Delila-PY
-----------
Delila-PY is a custom pipeline to make it quick and easy to run the Delila software packages required to construct a binding site logo while providing the high level of flexibilty and control present in the Deilia software.

Delila-PY uses programs developed in the lab of Tom Schneider and more information can be found at his [website](http://users.fred.net/tds/lab/) and in the references at the end of this readme.

Delila-PY uses standard (often default) values and allows the user to quickly and reproducibly process a file consisting of genomic locations to identify a potential logo.

You can download and compile the individual programs and run the Python script or you can use the (recommended) Docker Delila-PY image.

Either way, Delila-PY requires two files:

1) GenBank file for your genome of interest (obtained from [NCBI GenBank](https://www.ncbi.nlm.nih.gov/genbank/))
2) A sites file which is tab delimited with four columns (no column names): 
        
        chromosome name : must match the chromosome names in genbank file
        name : unique identifier for site
        strand :  forward or reverse (must use the words)
        position :  base pair position of site

        example:
        NC_007493.2   site_1   forward   409225

        where
        NC_007493.2 is a _R.sphaeroides_ chromosome name
        site_1 is the name
        forward is the strand 
        409225 is the base position of the site

The following basic command is used to run Delila-PY:

        delila_pipeline.py -g Rsphaeroides_GenBank.gbff -l -10 -r -10 -s Location_File_Example.txt

        where 
        -g indicates the genbank file
        -s indicates the sites file
        -l is the distance upstream to use as a boundary
        -r is the distance downstream to use as a boundary

Understanding Left and Right boundaries, which are always on the given strand from the sites file (forward or reverse):
   
    scenario 1 - searching around the given positions for a logo
    Left = -10 
    Right = +10 
    For site position 100, the bounds to find a logo are position 90 (Left) and position 110 (Right)

    scenario 2 - searching for the -10 promoter element upstream from a transcription start site position
    Left = -20
    Right = 0 (no sign required)
    For site position 100,  the bounds to find a logo are position 80 (Left) and position 100 (Right)

    
    scenario 3 - searching for the -35 promoter element upstream from a transcription start site position
    Left = +5
    Right = +25
    For site position 100,  the bounds to find a logo are position 105 (Left) and position 125 (Right)

Delila-PY has been tested on MacOS and Linux (Ubuntu and CentOS). The descriptions here are for running Delila-PY on one of these operating systems. Some additional work may be required to run Delila-PY on other operating systems.

Delila-PY Docker Image
------------
The recommended way to use Delila-PY is to download and run the Docker image. While you will need to install [Docker](https://docs.docker.com/engine/install/), this allows the user to run the pipeline without the need to install and compile all the individual programs and with minimal command line experience. Further, the Delila-PY Docker image can be used across varios computing platforms.
    
You can download the Delila-PY Docker image from the [Docker Hub](https://hub.docker.com/r/glbrc/delila)

Here is a step by step guide to running the Delila-PY Docker image for analysis

1) Install Docker

    https://docs.docker.com/engine/install/

    Test your Docker installation by running the following command:

        sudo docker run hello-world

    You should see something like in your terminal window:

        Hello from Docker!
        This message shows that your installation appears to be working correctly 
        ...

2) Pull the Delila-PY Docker image with this command:

        sudo docker pull glbrc/delila

    You should see something like the following in your terminal window:

        Using default tag: latest
        latest: Pulling from glbrc/delila
        da7391352a9b: Pull complete 
        14428a6d4bcd: Pull complete 
        2c2d948710f2: Pull complete 
        6e574971b428: Pull complete 
        98ae9b1c7d2a: Pull complete 
        400a97ff91ed: Pull complete 
        Digest: sha256:f865911122576824ebe331d0faf8989a4371350db8a9ff4121f4fb9342b37ea2
        Status: Downloaded newer image for glbrc/delila:latest
        docker.io/glbrc/delila:latest

  NOTE CONCERNING THE USE OF SUDO:

  If you don't want to use sudo everytime you run a Docker command, follow the instructions here:  https://docs.docker.com/engine/install/
  Note this has sercurity implications, you should be aware of:  https://docs.docker.com/engine/security/#docker-daemon-attack-surface                          

3) List Docker images to make sure Delila-PY was downloaded using the following command:

        sudo docker images
        glbrc/delila 
        REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
        glbrc/delila        latest              90845438bd28        6 days ago          620MB
        hello-world         latest              bf756fb1ae65        12 months ago       13.3kB

    You will see the hello-world image listed because you used it to make sure that Docker was installed correctly!

4) It's highly recommended that you first run the Delila-PY using the provided example files to ensure everything is working correctly.
    
    In the terminal, type the following command to make it possible to share data between the Docker environment and your local computer:

        sudo docker run -dit -P --name delila -v /home/user/test/delila/container-data:/opt/delila/example/container-data  glbrc/delila
           
    -dit is `d` for detached mode,  `it` makes bash available in a pseudo terminal.

    -P reports ports to the host.
    
    –name give the container a name, (Optional)
    
    -v attach a volume for sharing data with host system

    The first directory path (prior to the ":") is the directory path on your local computer, where you can your Delila-PY input files (see below), they will be visible in the Docker container and can be used.
    
    In this example this path is:
                
        "/home/user/test/delila/container-data"
        
    if this doesn't exit it will be created.

    Also note that this path may need to be changed to match the file system on your local computer.
                
    The second directory path (after the ":") is working directory within the Docker container. 
    
    In this example the path is:   

        "/opt/delila/example/container-data"

    glbrc/delila is the image used for the Docker container

    After running the `sudo docker run` above command you should see a container ID written in the terminal.
    
    It will look something like the following (the actual digits and letters may be different):

        22d098e7b4cacdc346f5cadaea561a0f2d9f7bcb1833b301c603c598b409a355

    You can check that the container is running using the following command:

        sudo docker ps -a

    And you should see something like this in the terminal window, which lists the Docker images that are running:

        CONTAINER ID     IMAGE         COMMAND     CREATED          STATUS         PORTS   NAMES
        22d098e7b4ca   glbrc/delila   "/bin/bash"  42 seconds ago   Up 41 seconds          delila

    To run Delila-PY, you must first access the container by typing at least the first 4 characters of the Container ID using the following command:

        sudo docker attach 22d098
        
    Alternatively, you can use the following command and indicate the name of the image:

        sudo docker attach delila
        
    You will see a new command line prompt in your Terminal that should look similar to this (the digits and letters may be different):

        root@aaab660630ab:/#

    Your terminal is now pointing to the Delila-PY Docker image.
    
    You can run `ls` to show the directory structure of the image, which should look something like this:

        bin  boot  dev  etc  home  lib  lib32  lib64  libx32  media  mnt  opt ....

    Navigate to the folder you created above using the following command:

        cd /opt/delila/example/containter-data
        
    Finally, we can run Delila using the example files using the following command:

        python3 /opt/delila/delila_pipeline.py -g opt/delila/example/Rsphaeroides_GenBank.gbff -s opt/delila/example/Location_File_Example.txt  -l -10 -r +10
    
    This will produce the following files & directories within the `container-data` directory within the Docker image:

        R.sphaeroides-2.4.1-initial.logo 
        delila_pipeline.log
        R.sphaeroides-2.4.1_PWM.txt
        R.sphaeroides-2.4.1-FINAL.logo
        R.sphaeroides-2.4.1-initial.logo.pdf
        R.sphaeroides-2.4.1-FINAL.logo.pdf
        directories:
        parameters/
        other/
        instructions/
        books/
   
    You can then exit the Docker image command line using the `exit` command.

    This will then copy all the contents from the `/opt/delila/example/container-data` directory to your local computer directory `/home/user/test/delila/container-data` where you can view the resulting logo that was created.
    
    You can restart the same Delila-PY Docker image using the following commands:

       sudo docker start delila
       sudo docker attach delila

5) Once you have verified that the Delila-PY runs successfully with the included example files, you can run Delila-PY with your own files 

    Create a working directory (`mywork` in this example) for your data on the local system using the following command:
        
        mkdir mywork

    Then you can copy the input files into the newly created mywork either in the GUI or with `mv` command in the Terminal

    Once coppied, you can start the Delila-PY container using detached mode as above using the following commands:

        sudo docker run -dit -P --name delila -v /mywork:/opt/delila/test/mywork glbrc/delila

    The path before the `:` is the path to the mywork directory on your local computer and the path after the `:` is the path to where the directory will be created within the Delila-PY Docker image.

        sudo docker attach delila

    Finally, you can run the Delila-PY

        cd /opt/delila/test/mywork
        python3 /opt/delila/delila_pipeline.py -g opt/delila/mywork/GenBank.gbff -s opt/delila/mywork/Location_File.txt  -l -10 -r +10

    Where the -l and -r can be changed to whatever values you prefer for your data.
    
    After exiting the Delila-PY Docker image, the results file will be placed on your local computer, in the `mywork` directory.


Delila-PY Scripts
-----------

You are also able to download the individual scripts and run the Delila-PY script independent of Docker. Note this requires downloading and compiling the required programs and compiling them from Pascal to C. You can download the required programs from the [Delila website](http://users.fred.net/tds/lab/delila/delilaprograms.html). This requires extensive knowledge of programing and experience with the command line. You can find all the scripts within this GitHub repository.

1) The script directory must be manually chaged within the `delila_pipeline.py` file

    Define the script directory by editing the "scriptDir" varible in the delila_pipeline.py file.

    scriptDir = '/home/<userName>/bin/delila/' to match the path on your local computer.

2) The required programs must be compilied from Pascal to C. Programs can be found at the [Delila website](http://users.fred.net/tds/lab/delila/delilaprograms.html):
        
    - alist
    - catal
    - comp
    - dalvec
    - dbbk
    - delila
    - encode
    - makelogo
    - malign
    - malin
    - mkdb
    - ri
    - rseq
    
    
3) The required GenBank file must be downloaded from [NCBI GenBank](https://www.ncbi.nlm.nih.gov/genbank/) and the genomic location file must be constructed as indicated below and matching the example files included in this repository.

    We have provided example files in the Git repository as a guide both to how to run the Delila-PY
    and the format of the required files. The example files represent genomic coordiates for binding
    sites of the transcription factor FnrL from the bacterium *Rhodobacter sphaeroides*.
    	- Location_File_Example.txt is the genmoic location example file
    	- Rsphaeroides_GenBank.gbff is the *R. sphaeroides* GenBank file from NCBI

Finally, the Delila-PY script requires [ps2pdf](http://web.mit.edu/ghostscript/www/Ps2pdf.htm) to be installed on your local comptuer. If it is not, you must install it before running Delila-PY.

Create logo
-----------

You need to provide the following:

1) GenBank file for your genome of interest 
2) A sites file which is tab delimited with columns: 
        
    chromosome name : must match the chromosome names in genbank file
    name : unique identifier for site
    strand :  forward or reverse 
    position :  base pair position of site

    example:
    NC_007493.2   site_1   forward   409225

    where NC_007493.2 is a R.sphaeroides chromosome name
    site_1 is the name
    forward is the strand 
    409225 is the base position of the site

To run delila and create a logo:

    delila_pipeline.py -g Rsphaeroides_GenBank.gbff -l -10 -r -10 -s Location_File_Example.txt
    
Running individual Delila programs
----------------------------------

It is possible to run each C program on it's own outside of the pipeline.
To show a program's help, just run the program without any parameters.
This is not recommended as the pipeline aims to reduce the complexity of
Delila programs.  If you desire to do this, read the help for each 
program and refer to the original [Delila website](http://users.fred.net/tds/lab/delila/delilaprograms.html) for detailed information and instructions.

Note only the following delila programs are available in this repository:
    
    alist, catal, comp, dalvec, dbbk, delila, encode,makelogo, malign, malin, mkdb, ri, rseq
----------------------------------------------------------------------------------------

Description of Delila programs and pipeline
-----------

Create a sequence logo for user defined sites using the Delila package.
Supports single or multiple chromosomes.
    
Notes
----- 
    
The programs used in this pipeline have been converted from Pascal to C, by
Oliver Giramma.  They were then modified to use command line arguments.
    
The descriptions are all from the [Delila Documentation](http://users.fred.net/tds/lab/software.html) and it is highly recommended users consult that website for complete information.
    
> DELILA stands for DEoxyribnucleaic-acid LIbrary LAnguage.
> 
> It is a language for extracting DNA fragments from a large collection of sequences,
> invented around 1980 (T. D. Schneider, G. D. Stormo, J. S. Haemer, and L. Gold, A
> design for computer nucleic-acid sequence storage, retrieval and manipulation, 
> Nucl. Acids Res., 10: 3013-3024, 1982). The idea is that there is a large database
> containing all the sequences one would like, which we call a `library'. 
> 
> One would like a particular subset of these sequences, so one writes up some 
> instructions and gives them to the librarian, Delila, which returns a 'book' 
> containing just the sequences one wants for a particular analysis. So 'Delila' 
> also stands for the program that does the extraction (delila.p). Since it is 
> easier to manipulate Delila instructions than to edit DNA sequences, one makes
> fewer mistakes in generating one's data set for analysis, and they are trivial
> to correct. Also, a number of programs create instructions, which provides a 
> powerful means of sequence manipulation. One of Delila's strengths is that it
> can handle any continuous coordinate system. The 'Delila system' refers to a 
> set of programs that use these sequence subsets for molecular information 
> theory analysis of binding sites and proteins. In the spring of 1999 Delila 
> became capable of making sequence mutations, which can be displayed graphically
> along with sequence walkers on a lister map. A complete definition for the
> language is available https://alum.mit.edu/www/toms/libdef.html, although not
> all of it is implemented. There are also tutorials on building Delila libraries
> and using Delila instructions. 
> 
> Delila is a modular set of programs which produce results that feed into the next
> program.  It is highly configurable using parameter files.  The delila programs
> used are listed below with general descripbtions.
> 
Delila programs used to make logo (descriptions from the [Delila Documentation](http://users.fred.net/tds/lab/software.html))
------------------------------------------------------------------------------
> dbbk: 
> Convert GenBank flat file format to Delila format, which is called a `book'
> (db = database, bk = book). This produces an 'l1' file which contains the book.
> (l1 is a lower case L [standing for 'Library'] followed by the symbol 'one').
> 
> catal: 
> Catalogue the contents of a book, usually l1. If you are not using them
> (which is generally the case) make empty l2, l3 and catalp files, then run catal.
> This will produce six files: lib1, lib2, lib3 and cat1, cat2, cat3. These make 
> up the library used by Delila in the next step.
> 
> delila:
> Extract fragments of sequences from a library of sequences and create 
> a subset, a book. This is the core of the Delila system. You give Delila 
> instructions (inst file) and those are used to create the subset desired. 
> For a logo, generally one makes instructions that look like this:
> 
> get from 5600 -200 to same +200 direction +;
> 
> This means to get 200 bases before position 5600 to 200 bases afterwards, 
> for a total of 401 bases. I recommend that you use a wide range like this to
> be able to see the background noise around the binding site. By the way, the 
> number 5600 becomes the zero coordinate of the binding site. I try to pick a
> position that is strongly conserved (high information content). 
> 
> malign: 
> Given a book of aligned sequences, this program searches for the alignment
> of the sequences that has the lowest uncertainty, i.e. the highest value of
> Rsequence. The user specifies the "window" of bases within which
> uncertainty is calculated, and the maximum number of bases that each
> sequence is allowed to shift from the original alignment.  The program
> considers each sequence in turn, shifting it to an alignment with minimum
> uncertainty while holding the other sequences fixed.  A "pass" is complete
> when all sequences have been considered.  A "run" is complete when no
> alignments have changed in the preceding pass, and the alignment is then
> considered "optimal".  The first run starts with the original alignment;
> every run after that starts with a "shuffled" alignment obtained by shifting
> each sequence independently by a random amount between the allowed limits.
> The program maintains a list of all of the unique optimal alignments
> achieved from these starting alignments, and it outputs them in order of
> increasing uncertainty.
> 
> malin:
> This program allows one to select one of the alignments created by malign
> and to make the corresponding Delila instructions.  Because it copies the
> inst file it keeps the organism and chromosome information (along with all
> comments) so it is better than the "bestinst" file created by malign!
> 
> alist:
> Make an aligned listing of sequences using the Delila instructions 
> and the book created by Delila. The pair of the book and inst file is a set
> called an 'aligned book'. I create a listing of the sequences to be sure 
> that they are aligned correctly. If this works, making the logo is fast
> and a piece of cake.  Even if you have a large range defined in the delila
> instructions, you can use a smaller one for the aligned listing. Note that
> running this program does not affect the inst or book files, so can't
> affect later steps. The program is controled by a file `alistp', which 
> stands for alist-parameters. 
> 
> encode:
> Convert the book/inst into 0 and 1's. This is historically the way
> we did it, but it is fast so we still do it this way. The output is encseq,
> which is a bit large. To save space, you can delete the encseq file after
> running the next program. Note that the range can be reduced in the
> parameter file, so I normally set this to -200 to 200.
> 
> comp:
> Comp counts the number of each oligonucleotide (from length 1 to
> compmax) in the book and prints that to file "cmp".  The output is
> printed in order of increasing length of oligonucleotide (i.e., first
> the monos, then the dis, ...).  If there are no occurences of an
> oligonucleotide, but its one-shorter parent did occur, it will be given
> a zero.  None of its descendants will be printed in the composition
> file.
> 
> rseq:
> Compute Rsequence for each position in the aligned book, make an rsdata file.
> 
> dalvec:
> Convert the rsdata file into a symvec file, which the next program
> can use to make the logo.
> 
> ri:
> The program determines the individual informations of the sites in the book
> as aligned by the instructions, according to the frequency table given in
> the rsdata file.  The program calculates the Ri(b,l) table:
> 
> Ri(b,l) := 2 - (- log2( f(b,l)))
> 
> and sums this up for each sequence.  Ri is defined so that the average of
> the Ri's for a set of sequences is Rsequence.
> 
> makelogo:
> Finally! Make the sequence logo!
> 
Steps of Delila-PY when running the `delila_pipeline.py` script
------

1. dbbk    -- convert genbank file into a delila book
1. catal   -- create catalogue and library files
1. delila  -- extract a subset of sequences to examine, store in a book
1. malign  -- alignment of the sequences that has the lowest uncertainty
1. malin   -- select malign's best alignment
1. delila  -- create a new book from best alignment (from malign)
1. alist   -- make an aligned listing of sequences
1. encode  -- convert the book/inst into 0 and 1's
1. comp    -- counts the number of each oligonucleotide
1. rseq    -- compute Rsequence
1. dalvec  -- convert the rsdata file into a symvec file
1. ri      -- calculate site information
1. make initial logo -- for comparison to the final logo
1. filter  -- remove sites that ri identified as <= 0 information
1. repeat steps 3 through 11 with filtered sites
1. make final logo 
    
Parameters of Delila-PY when running the `delila_pipeline.py` script
----------

    g : str
    A genbank file for a genome.

    s : str
    Transcription start site information file.

    NC_007488.2	site_24	forward	24419
    NC_007490.2	site_25	forward	51745
    NC_007494.2	site_23	reverse	78076

    l : int
    Left boundary relative to center of the site, defaults to -10

    r : int
    Right boundary relative to the center of the site, defaults to +10

Example
-------
general usage:

    delila_pipeline.py -g Rsphaeroides_GenBank.gbff -s Location_File_Example.txt -l -10  -r +10

Other scripts which are part of the pipeline
----------------------------------------------
delila_instructions.py

- Split site files by chromosome


        delila_instructions.py -f <sites_file.txt> -l <int> -r <int>

        optional arguments:
        -h, --help       Show this help message and exit
        -f, --file       Text file, containing site positions
        -l, --left       Left boundary base position
        -o, --organism   Organism
        -r, --right      Right boundary base position

merge_books.py

- Merge Delila chromosme book files

        merge_books.py -f <input.txt>

        optional arguments:
        -h, --help   Show this help message and exit
        -f, --file   Delila book file to parse.

merge_instructions.py

- Merge Delila instruction files.

        merge_instructions.py -f <input.txt>

        optional arguments:
        -h, --help   Show this help message and exit
        -f, --file   List of delila instruction files to parse.

organizing_ri_delila_results.py

- Removal of sites with negative Ri values in Delila.

        organizing_ri_delila_results.py -f <fastq file list.txt> [optional args: -a -r -d -ref ]

        optional arguments:
        -h, --help          Show this help message and exit
        -l, --malign_list   Malign_list file from Delila pipeline.
        -r, --rixyin_file   Rixyin results file from Ri in the Delila pipeline
        -d, --detail        Print a more detailed description of program.
        -o, --output        Output file name. Default is "positive_Ri_updated_locations.txt".

removeRI_books.py

- Filter book file by ri score.

        removeRI_books.py -f <input.txt> -r <RI_out.txt>

        optional arguments:
        -h, --help   Show this help message and exit
        -f, --file   Merged delila book file to parse.
        -r, --ri     RI_out.txt file

removeRI_instructions.py

- Filter instructions file by ri score.

        removeRI_instructions.py -f <input.txt> -r <RI_out.txt>

        optional arguments:
        -h, --help   Show this help message and exit
        -f, --file   Merged instructions file to parse.
        -r, --ri     RI_out.txt file

rename_lib1.py

- Rename chromosome & piece tags in lib1 file.

        rename_lib1.py 

        optional arguments:
        -h, --help  show this help message and exit

Useful Python scripts not in pipeline 
-------------------------------------

filter_sites.py 


- Not part of the pipeline, but can be used prior to running `delila_pipeline.py` to remove overlapping sites.
- Remove sites with x <int> number of bases overlap, file MUST BE SORTED by site position.

To Run:

        filter_sites.py -f <site_file.txt> -n <int>

        arguments:
        -h, --help     Show this help message and exit
        -f, --file     Text file, containing sites
        -n, --number   Number of base overlap, default(15)


define_site_position.py

- Read sites file line by line.  Parse each line and shift the position by +/- value taking account of the strand.

To Run:

        filter_Sites.py -f <site_file.txt> -n <int>
            
        arguments:
        -h, --help      show this help message and exit
        -f , --file     Text file, containing sites
        -n , --number   Number of base to shift (negative = upstream)

        example input file:

        NC_007488.2     RSP_4039_1700   forward 1700
        NC_007488.2     RSP_4038_2627   forward 2627

example command:

        define_site_position.py -f input.txt -n -10  

This results in a site file of the same format as the input.
All sites shifted upstream by 10 bases.

alternatively:

        define_site_position.py -f input.txt -n 10

This results in a site file of the same format as the input.
All sites shifted downstream by 10 bases.

References
----------------------------------------------
1. Schneider TD, Stormo GD, Haemer JS, Gold L. 1982. A design for computer nucleic-acid-sequence storage, retrieval, and manipulation. Nucleic Acids Res 10:3013-24.
1. Schneider TD, Stormo GD, Yarus MA, Gold L. 1984. Delila system tools. Nucleic Acids Res 12:129-40.
1. Schneider TD, Stephens RM. 1990. Sequence logos: a new way to display consensus sequences. Nucleic Acids Res 18:6097-100.
1. Schneider TD. 2006. Twenty Years of Delila and Molecular Information Theory: The Altenberg-Austin Workshop in Theoretical Biology Biological Information, Beyond Metaphor: Causality, Explanation, and Unification Altenberg, Austria, 11-14 July 2002. Biol Theory 1:250-260.
1. Schneider TD. 2010. A brief review of molecular information theory. Nano Commun Netw 1:173-180.
1. Dufour YS, Kiley PJ, Donohue TJ. 2010. Reconstruction of the core and extended regulons of global transcription factors. PLoS Genet 6:e1001027.
1. Delila Software:  http://users.fred.net/tds/lab/delila.html
1. GenBank Flat File format information:  https://www.ncbi.nlm.nih.gov/Sitemap/samplerecord.html

