# Project 3

## Generating the input files

The input for project3WithAllAlgorith.c uses 5 input files called file1.txt, file2.txt, file3.txt, file4.txt, and file5.txt.

Compile makeTrackNumberStreamFile.c by running:

`gcc makeTrackNumberStreamFile.c -o makeTrackStream`

Generate each of the files by running the following:

```
./makeTrackStream file1 200 1000
./makeTrackStream file2 200 1000
./makeTrackStream file3 200 1000
./makeTrackStream file4 200 1000
./makeTrackStream file5 200 1000
```
## Compiling and running the simulation algorithms

Run the gcc compiler on the file containing all algorithms: 

`gcc project3WithAllAlgorithms.c`

Then run it with the following command: 

`./a.out`

The file will create seven output files, one for each of the algorithms. These files contain the average tracks traversed for each run. There will be 16 rows of output. The first 15 rows will be the average seek times for the individual runs. Runs 1-3 hold the average seek times for the runs on file1.txt with a starting position of 0, 100, and 199 respectively.
The next runs, 4-6, are the average seek times for the runs on file2.txt with a starting position of 0, 100, and 199 respectively. This pattern continues. The 16th row of data is average of all the average tracks traversed over the 15 runs. You can use the command `cat fileName` to view the contents of these files.

