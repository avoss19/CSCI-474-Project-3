Our algorithms that simulate the 7 different scheduling policies are in the project3WithAllAlgorithms.c file.
In order to compile this file, you will need to run the gcc compiler on the file: 

  gcc project3WithAllAlgorithms.c
  
We have the file set up to read in the random track requests from files that are named file1.txt, file2.txt, file3.txt, file4.txt and file5.txt.
Thus, make sure the files are nammed accordingly. Also, we created our algorithm to read files that are delineated by the new line character, so
please ensure that the files are new line delineated.

The last thing to take note of is that when you run the file (by using the ./a.out command with no parameters), it will create 5 output files.
These files contain the average tracks traversed for each run. There will be 16 rows of output. The first 15 rows will be the average seek times
for the individual runs. Runs 1-3 hold the average seek times for the runs on file1.txt with a starting position of 0, 100, and 199 respectively.
The next runs, 4-6, are the average seek times for the runs on file2.txt with a starting position of 0, 100, and 199 respectively. This pattern
continues. The 16th row of data is average of all the average tracks traversed over the 15 runs. You can use the command cat fileName to view the
contnents of these files.
