#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*****************************************************************************
 * @brief       Reads ints in from a file and stores them in an array
 * @author      Joshua Molden
 * @date        3 Dec 21
 * @lastUpdated 3 Dec 21
 * @return      void (prints nums to command line)
 * @arg         fileName - name of the file to read. Needs to include '.txt'
 *              size - size of buffer
 *              startingTrackNum - where the initial position is of the arm (0, 100, 199)
 * @note        None
 *****************************************************************************/
void readFile(char* fileName, int size, int startingTrackNum)
{
    FILE *file;
    file = fopen(fileName, "r");

    int buffer[size];

    // holds number that is read from file
    int buff[1];

    int firstRun = 1;
    int lastTrackNum;
	int numOfLinesInFile = 0;
	
	int numOfValuesInBuffer = 0;
	
	// calculate number of lines in file
	do
	{
        int succRead = fscanf(file, "%d", buff);
		numOfLinesInFile++;
	}
	while (!feof(file));

    // close to reset file
    fclose(file);

    // reopen file
    file = fopen(fileName, "r");

    for (int lineInFile = 0; lineInFile < numOfLinesInFile; lineInFile++)
    {
        // buffer hasn't been populated yet
        if (lineInFile < size)   
        {
			int succRead = fscanf(file, "%d", buff);
			buffer[lineInFile] = buff[0];
			if (lineInFile == size - 1)
			{
				printf("------------- Printing first %d values -------------\n", size);
				for (int i = 0; i < size; ++i)
				{
					printf("Index: %d\tTrack Number: %d\n", i, buffer[i]);
				}
				printf("\n------------- Scanning and replacing values -------------\n");
			}
        }
        else if (lineInFile >= size && lineInFile < numOfLinesInFile - size)
        {
            int indexOfClosestNum = -1;
            int numOfTracksAway = 200;
            if (firstRun == 1)
            {
                lastTrackNum = startingTrackNum;
                firstRun = 0;
            }

            for (int i = 0; i < size; i++)
            {
                if (abs(buffer[i] - lastTrackNum) < numOfTracksAway)
                {
                    numOfTracksAway = abs(buffer[i] - lastTrackNum);
                    indexOfClosestNum = i;
                }
            }

            printf("Track Number to Remove: %d\t", buffer[indexOfClosestNum]);
            printf("Number of Tracks Traversed: %d\t", numOfTracksAway);

            // read int from line. fscanf retruns 0 if successfully read
            int succRead = fscanf(file, "%d", buff);
            printf("Track Number to Add to buffer: %d\n", buff[0]);
            buffer[indexOfClosestNum] = buff[0];
        }
        else 
        {
            printf("------------- Run Number %d -------------\n", lineInFile);
            int indexOfClosestNum = -1;
            int numOfTracksAway = 200;

            for (int j = 0; j < size; j++)
            {
                if (buffer[j] == -1000)
                {
                    // skip since this index has already been used
                }
                else if (abs(buffer[j] - lastTrackNum) < numOfTracksAway)
                {
                    numOfTracksAway = abs(buffer[j] - lastTrackNum);
                    indexOfClosestNum = j;
                }
                else
                {
                    // do nothing since track is a furthur distance away
                }
            }


            // set track number to invalid number since it shouldn't be used again
            buffer[indexOfClosestNum] = -1000;

            for (int j = 0; j < size; j++)
            {
                printf("Index in Buffer: %d\tValue at Index: %d\n", j, buffer[j]);
            }
            printf("\n");
        }
    }

    fclose(file);
}

/*****************************************************************************
 * @brief                       reads values from a file
 * @author              Joshua Molden
 * @date                5 Nov 21
 * @lastUpdated         5 Nov 21
 * @return              int (0 if exited normally, non-zero if issues)
 * @arg                 argc - number of command line arguments passed in, including name of program
 *                      argv - the actual arguments
 *                              argv[0] - name of program
 *                              argv[1] = name of file to read. Include the extension of the file you want to read
 * @note                None
 *****************************************************************************/
int main(int argc, char** argv) {
    // name of file (including .txt)
    char* fileName = argv[1];

    // size of array (probably 10 for this project?)
    char* sizeOfArray = argv[2];

    char* startingPosition = argv[3];

    // convert from string to number
    int size = atoi(sizeOfArray);
    int startingPositionInt = atoi(startingPosition);

    int sstfResults[16][2];
    
    char* firstHalfOfFileName = "file";
    
    for (int i = 1; i < 5; i ++)
    {
        char* fileToRead = firstHalfOfFileName;
        if (i == 1) { strcat(fileToRead, "1.txt"); }
        else if (i == 2) { strcat(fileToRead, "2.txt"); }
        else if (i == 3) { strcat(fileToRead, "3.txt"); }
        else if (i == 4) { strcat(fileToRead, "4.txt"); }
        else { strcat(fileToRead, "5.txt"); }
        
        printf("Name of File: %s\n", fileToRead);
    }

}
