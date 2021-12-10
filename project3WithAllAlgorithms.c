/*H**********************************************************************
* FILENAME : project3WithAllAlgorithms.c
*
* DESCRIPTION :
*       This file simulates the different scheduling algorithms and prints the results
*       to .csv files.
*
* PUBLIC FUNCTIONS :
*       int     main( int argc, char** argv )
*
* NOTES :
*       None as of yet
*
* AUTHOR :    Andrew Voss, Dylan Martie, Meagan Olson, and Joshua Molden       START DATE :    8 Dec 21
*
* VERSION:
*       1.0.0
*
* LAST UPDATED:
*       8 Dec 21
*H*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*****************************************************************************
 * @brief                       Peforms SSTF (Shortest-Service-Time-First) scheduling policy
 * @author              Dylan Martie
 * @date                6 Dec 21
 * @lastUpdated         6 Dec 21
 * @return              void (prints to command line)
 * @arg                                 startTrack - Starting track for policy, either 0, 100, or 199
 *                                      inputfile - Input file of requests
 *                                      size - Size of buffer
 * @note                This function currently prints out requested values to command line only
 *****************************************************************************/
double sstfScan(int startTrack, char* inputFile, int size) 
{
    int bufferSize = size;
    int currentTrack = startTrack;
    int requestBuffer[bufferSize];
    int tableResults[1000][2];

    FILE *file = fopen(inputFile, "r");

    // Initial population of buffer
    for (int i = 0; i < bufferSize; i++) 
    {
        int buff[1];
        fscanf(file, "%d", buff);
        requestBuffer[i] = buff[0];
    }

    for (int i = 0; i < 1000; i++) 
    {
        int nextTrack[3] = {201, 201, 0};
        for (int j = 0; j < bufferSize; j++) 
        {
            int tracksTraversed = abs(requestBuffer[j] - currentTrack);
            if (tracksTraversed < nextTrack[1]) 
            {
                nextTrack[0] = requestBuffer[j];
                nextTrack[1] = tracksTraversed;
                nextTrack[2] = j;
            }
        }

        // Store next track accessed and # of tracks traversed
        tableResults[i][0] = nextTrack[0];
        tableResults[i][1] = nextTrack[1];

        // Set new current track and add new request in buffer
        currentTrack = nextTrack[0];
        int index = nextTrack[2];

        // Final 10 processed requests do not read in new value
        if (i < 991) 
        {
            int buff[1];
            fscanf(file, "%d", buff);
            requestBuffer[index] = buff[0];
        }
        else
        {
            requestBuffer[index] = 999;
        }
    }

    fclose(file);

    // Calculate Average
    double total = 0;
    for (int i = 0; i < 1000; i++) 
    {
        total += tableResults[i][1];
    }
    double averageTraversedTracks = total / 1000;

    return averageTraversedTracks;
}

/*****************************************************************************
 * @brief               Peforms either SCAN or C-SCAN scheduling policies
 * @author              Dylan Martie
 * @date                5 Dec 21
 * @lastUpdated         8 Dec 21
 * @return              void (prints to command line)
 * @arg                 startTrack - Starting track for policy, either 0, 100, or 199
 *                      inputfile - Input file of requests
 *                      size - Size of buffer
 *                      cScan - Determines if running SCAN or C-Scan
 * @note                None
 *****************************************************************************/
double scanAndCScan(int startTrack, char* inputFile, int size, bool cScan){

    int bufferSize = size;
    int currentTrack = startTrack;
    int requestBuffer[bufferSize];
    int tableResults[1000][2];

    // 0 is to the left, 1 to right
    int direction = 1;

    FILE *file = fopen(inputFile, "r");

    // Initial population of buffer
    for (int i = 0; i < bufferSize; i++) 
    {
        int buff[1];
        fscanf(file, "%d", buff);
        requestBuffer[i] = buff[0];
    }

    int tracksProcessed = 0;

    // Additional variables for C-scan
    int resetDistance = 0;
    bool trackReset = false;

    // Start SCAN scheduling policy
    while (tracksProcessed < 1000) 
    {
        // Moving right
        if (direction == 1) {
            int nextTrack[3] = {201, 201, 0};
            for (int j = 0; j < bufferSize; j++) 
            {
                // If track is larger than current Track, continue on
                if (requestBuffer[j] >= currentTrack) 
                {
                    int tracksTraversed = (requestBuffer[j] - currentTrack);

                    // If track distance is smaller than what is stored, store at new next track
                    if (tracksTraversed < nextTrack[1]) {
                        nextTrack[0] = requestBuffer[j];
                        nextTrack[1] = tracksTraversed;
                        nextTrack[2] = j;
                    }
                }
            }
            // For C-Scan, resets current track back to left side
            if (nextTrack[0] == 201 && cScan) 
            {
		resetDistance = currentTrack;
                currentTrack = 0;
		trackReset = true;
                continue;
            }

            // No larger Track found, switch directions
            if (nextTrack[0] == 201) 
            {
                direction = 0;
                continue;
            }

	    // For C-scan, needed to calculate distance whenever track resets to 0
	    if (trackReset) {
		nextTrack[1] = resetDistance - nextTrack[1];
		trackReset = false;
	    }

            // Store next track accessed and # of tracks travered
            tableResults[tracksProcessed][0] = nextTrack[0];
            tableResults[tracksProcessed][1] = nextTrack[1];
            tracksProcessed++;

            // Set new current track and add new request in buffer
            currentTrack = nextTrack[0];
            int index = nextTrack[2];

            // Final 10 processed requests do not read in new value
            if (tracksProcessed < 991) 
            {
                int buff[1];
                fscanf(file, "%d", buff);
                requestBuffer[index] = buff[0];
            }
            else
            {
                requestBuffer[index] = -1;
            }
        }

        // Moving left
        else if (direction == 0) 
        {
            int nextTrack[3] = {201, 201, 0};
            for (int j = 0; j < bufferSize; j++) 
            {
                // If track is smaller than current Track, continue on
                if (requestBuffer[j] <= currentTrack) 
                {
                    int tracksTraversed = (currentTrack - requestBuffer[j]);

                    // If track distance is smaller than what is stored, store at new next track
                    if (tracksTraversed < nextTrack[1]) {
                        nextTrack[0] = requestBuffer[j];
                        nextTrack[1] = tracksTraversed;
                        nextTrack[2] = j;
                    }
                }
            }
            // No smaller Track found, switch directions
            if (nextTrack[0] == 201) 
            {
                direction = 1;
                continue;
            }

            // Store next track accessed and # of tracks travered
            tableResults[tracksProcessed][0] = nextTrack[0];
            tableResults[tracksProcessed][1] = nextTrack[1];
            tracksProcessed++;

            // Set new current track and add new request in buffer
            currentTrack = nextTrack[0];
            int index = nextTrack[2];

            // Final 10 processed requests do not read in new value
            if (tracksProcessed < 991) 
            {
                int buff[1];
                fscanf(file, "%d", buff);
                requestBuffer[index] = buff[0];
            }
            else
            {
               requestBuffer[index] = 201;
            }

        }
    }

    fclose(file);

    // Calculate Average
    double total = 0;
    for (int i = 0; i < 1000; i++) 
    {
        total += tableResults[i][1];
    }
    double averageTraversedTracks = total / 1000;

    return averageTraversedTracks;
}

/*****************************************************************************
 * @brief               prints contents of results array to a file
 * @author              Joshua Molden
 * @date                8 Dec 21
 * @lastUpdated         8 Dec 21
 * @return              void (prints to file)
 * @arg                 results - array holding values to print to file
 *						numOfRows - number of rows to print to file
 *						fileName - the name of the file to create
 * @note                None
 *****************************************************************************/
void printResultsToFile(double results[16][2], int numOfRows, char* fileName)
{
    
	FILE *file;
	file = fopen(fileName, "a");
	
	// create "numOfPagesInStreamInt" number of page numbers as ints between 0 and pagesInt
	for (int i = 0; i < numOfRows; ++i) {
		fprintf(file, "%.0f   ", results[i][0]);
		fprintf(file, "%.2f\n", results[i][1]);
	}

	// close file that was appended to
	fclose(file);
}

/*****************************************************************************
 * @brief               Simulates FIFO, LIFO, SSTF, SCAN, C-SCAN, F-SCAN, and N-step-SCAN scheduling
 *                      algorithms against 5 files with the starting track a 0, 100, and 199
 * @author              Joshua Molden
 * @date                8 Dec 21
 * @lastUpdated         8 Dec 21
 * @return              int (0 if exited normally, non-zero if issues)
 * @arg                 argc - number of command line arguments passed in, including name of program
 *                      argv - the actual arguments
 * @note                None
 *****************************************************************************/
int main(int argc, char** argv) {

	// read inputs from command line as strings
	// char* inputFile = argv[1];

    int rows = 16;

	// arrays to hold results
	double fifoResults[rows][2];
	double lifoResults[rows][2];
    double sstfResults[rows][2];
	double scanResults[rows][2];
    double cScanResults[rows][2];
	double fScanResults[rows][2];
	double nStepScanResults[rows][2];

    for (int i = 1; i <= rows; i++)
    {
        fifoResults[i - 1][0] = i;
        lifoResults[i - 1][0] = i;
        sstfResults[i - 1][0] = i;
        scanResults[i - 1][0] = i;
        cScanResults[i - 1][0] = i;
        fScanResults[i - 1][0] = i;
        nStepScanResults[i - 1][0] = i;
    }

    char* filePattern = "file%d.txt";

    char fileName[10];
    for (int fileNum = 1; fileNum <= 5; fileNum++)
    {
        sprintf(fileName, filePattern, fileNum);
        for (int position = 1; position <= 3; position ++)
        {
            int startingPosition;
            int indexIntoArray;

            if (fileNum == 1)
            {
                if (position == 1) { indexIntoArray = 0; startingPosition = 0; }
                else if (position == 2) { indexIntoArray = 1; startingPosition = 100; }
                else { indexIntoArray = 2; startingPosition = 199; }
            }
            else if (fileNum == 2)
            {
                if (position == 1) { indexIntoArray = 3; startingPosition = 0; }
                else if (position == 2) { indexIntoArray = 4; startingPosition = 100; }
                else { indexIntoArray = 5; startingPosition = 199; }
            }
            else if (fileNum == 3)
            {
                if (position == 1) { indexIntoArray = 6; startingPosition = 0; }
                else if (position == 2) { indexIntoArray = 7; startingPosition = 100; }
                else { indexIntoArray = 8; startingPosition = 199; }
            }
            else if (fileNum == 4)
            {
                if (position == 1) { indexIntoArray = 9; startingPosition = 0; }
                else if (position == 2) { indexIntoArray = 10; startingPosition = 100; }
                else { indexIntoArray = 11; startingPosition = 199; }
            }
            else
            {
                if (position == 1) { indexIntoArray = 12; startingPosition = 0; }
                else if (position == 2) { indexIntoArray = 13; startingPosition = 100; }
                else { indexIntoArray = 14; startingPosition = 199; }
            }

            scanResults[indexIntoArray][1] = scanAndCScan(startingPosition, fileName, 10, false);
            cScanResults[indexIntoArray][1] = scanAndCScan(startingPosition, fileName, 10, true);
            sstfResults[indexIntoArray][1] = sstfScan(startingPosition, fileName, 10);
        }
    }

    double totaledAveragesForSCAN = 0;
    double totaledAveragesForCSCAN = 0;
    double totaledAveragesForSSTF = 0;

    for (int i = 0; i < 15; i ++)
    {
        totaledAveragesForSCAN = scanResults[i][1] + totaledAveragesForSCAN;
        totaledAveragesForCSCAN = cScanResults[i][1] + totaledAveragesForCSCAN;
        totaledAveragesForSSTF =  sstfResults[i][1] + totaledAveragesForSSTF;
    }

    scanResults[15][1] = totaledAveragesForSCAN / 15.0;
    cScanResults[15][1] = totaledAveragesForCSCAN / 15.0;
    sstfResults[15][1] = totaledAveragesForSSTF / 15.0;

    printResultsToFile(scanResults, 16, "SCAN Results.csv");
    printResultsToFile(cScanResults, 16, "C-SCAN Results.csv");
    printResultsToFile(sstfResults, 16, "SSTF Results.csv");

	return 0;
}
