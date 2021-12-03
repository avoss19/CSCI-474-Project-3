#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*****************************************************************************
 * @brief               reads values in from command line, makes random scheduling stream
 * @author              Joshua Molden
 * @date                3 Dec 21
 * @lastUpdated         3 Dec 21
 * @return              int (0 if exited normally, non-zero if issues)
 * @arg                 argc - number of command line arguments passed in, including name of program
 *                      argv - the actual arguments
 *                              argv[0] = name of program
 *                              argv[1] = name of file you want to create. Do not need to specify what type of file it is, the program will
 *                                         make it a .txt file
 *                              argv[2] = max number of track that can be created - 1
 *                              argv[3] = how long the simulated scheduling stream should be. 1000 for this project
 * @note                None
 *****************************************************************************/
int main(int argc, char** argv) {
    // read inputs from command line as strings
    char* fileToCreate = argv[1];

    // how many track numbers to simulate. This number controls what range of numbers can be generated.
    // i.e 0 to maxNumber
    char* maxNumberString = argv[2];

    // number of random pages to make, 1000 for this project
    char* numOfTrackNumbersInStream = argv[3];

    // convert from string to number
    int maxNum = atoi(maxNumberString);
    int numOfTrackNumbersInStreamInt = atoi(numOfTrackNumbersInStream);

    // apend ".txt" to file name so it is always a .txt file
    strcat(fileToCreate, ".txt");

    FILE *file;

    // open a file to append to
    file = fopen(fileToCreate, "a");

    int seed = 847;
    // use pagesInt for seeder
    // srand(seed);
    srand(time(NULL));

    // create "numOfTrackNumbersInStreamInt" number of page numbers as ints between 0 and maxNum
    for (unsigned long i = 0; i < numOfTrackNumbersInStreamInt; ++i) {
        // random number between 0 inclusive and "maxNum" exclusive
        int page = ((int) rand() % maxNum);
        // write number to file and start new line
        if (i != numOfTrackNumbersInStreamInt - 1)
        {
                fprintf(file, "%d\n", page);
        }
        else
        {
                fprintf(file, "%d", page);
        }
    }

    // close file that was appended to
    fclose(file);
}
