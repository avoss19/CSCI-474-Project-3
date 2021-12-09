#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*****************************************************************************
 * @brief               Performs modified SCAN scheduling policy
 * @author              Dylan Martie, Meagan Olson
 * @date                5 Dec 21
 * @lastUpdated         9 Dec 21
 * @return              int - total number of tracks traversed in the course of the algorithm
 * @arg                 startTrack - Starting track for policy, either 0, 100, or 199
 *                      inputArr - Input array of requests
 *						bufferSize - Size of buffer
 *****************************************************************************/
int scan(int *currentTrack, int inputArr[], int bufferSize) {

	int direction = 1; // 0 is to the left, 1 to right 

	int tracksProcessed = 0;
	int totalTracksTraversed = 0;

	// Start SCAN scheduling policy
	while (tracksProcessed < bufferSize) {

		// Moving right
		if (direction == 1) {
			int nextTrack[3] = {201, 201, 0};
			for (int j = 0; j < bufferSize; j++) {

				// If track is larger than current Track, continue on
				if (inputArr[j] >= *currentTrack) {
					int tracksTraversed = (inputArr[j] - *currentTrack);

					// If track distance is smaller than what is stored, store at new next track
					if (tracksTraversed < nextTrack[1]) {
						nextTrack[0] = inputArr[j];
						nextTrack[1] = tracksTraversed;
						nextTrack[2] = j;
					}
				}
			}

			// No larger track found, switch directions
			if (nextTrack[0] == 201) {
				direction = 0;
				continue;
			}

			// Store next track accessed and # of tracks traversed
			totalTracksTraversed += nextTrack[1];
			printf("Next track: %d\tNum tracks: %d\n", nextTrack[0], nextTrack[1]);
			tracksProcessed++;

			// Set new current track and add new request in buffer
			*currentTrack = nextTrack[0];
			int index = nextTrack[2];
			inputArr[index] = -1;
			printf("Tracks Processed: %d\n", tracksProcessed);
			printf("\tArray: {%d, %d, %d, %d, %d, %d, %d, %d, %d}\n\n", inputArr[0], inputArr[1], inputArr[2], inputArr[3], inputArr[4], inputArr[5], inputArr[6], inputArr[7], inputArr[8]);
		}

		// Moving left
		else if (direction == 0) {
			int nextTrack[3] = {201, 201, 0};
			for (int j = 0; j < bufferSize; j++) {

				// If track is smaller than current Track, continue on
				if (inputArr[j] <= *currentTrack) {
					int tracksTraversed = (*currentTrack - inputArr[j]);

					// If track distance is smaller than what is stored, store at new next track
					if (tracksTraversed < nextTrack[1]) {
						nextTrack[0] = inputArr[j];
						nextTrack[1] = tracksTraversed;
						nextTrack[2] = j;
					}
				}
			}
			// No smaller Track found, switch directions
			if (nextTrack[0] == 201) {
				direction = 1;
				int index = nextTrack[2];
				inputArr[index] = -1;
				continue;
			}

			// Store next track accessed and # of tracks traversed
			totalTracksTraversed += nextTrack[1];
			printf("Next track: %d\tNum tracks: %d\n", nextTrack[0], nextTrack[1]);
			tracksProcessed++;

			// Set new current track
			*currentTrack = nextTrack[0];
			int index = nextTrack[2];
			inputArr[index] = 201;
			printf("Tracks Processed: %d\n", tracksProcessed);
			printf("\tArray: {%d, %d, %d, %d, %d, %d, %d, %d, %d}\n\n", inputArr[0], inputArr[1], inputArr[2], inputArr[3], inputArr[4], inputArr[5], inputArr[6], inputArr[7], inputArr[8]);
		}
	}
	return totalTracksTraversed;
}

/*****************************************************************************
 * @brief               Models N-STEP-SCAN scheduling policy, as well as FSCAN, FIFO, and LIFO
 * @author              Meagan Olson
 * @date                7 Dec 21
 * @lastUpdated         9 Dec 21
 * @return              void (prints to command line)
 * @arg                 inputfile - Input file of requests
 *                      currentTrack - Starting track for policy, either 0, 100, or 199
 *						bufferLength - Buffer length (1 = FIFO; 500 or half the total request size = FSCAN)
 *                      totalRequests - Number of total track requests to process
 * @note                This function currently prints out requested values to command line only
 *****************************************************************************/
int nStepScan(char* inputFile, int currentTrack, int bufferLength, int totalRequests, bool lifo) {
	// "Remainder" refers to any requests left over after buffers have been filled completely
	// May occur if the number of requests does not divide among the buffers equally
    int numRemainder = totalRequests % bufferLength;
    bool remainderExists = numRemainder == 0;
    int numBuffers = (int) floor(totalRequests / bufferLength);
    int buffers[numBuffers][bufferLength];
    int lastBuffer[numRemainder]; // Will have length of zero if no remainder

	FILE *file = fopen(inputFile, "r");

    for (int i = 0; i <= numBuffers; i++) {
        for (int j = 0; j <= bufferLength; j++) {
            int buff[1];
            fscanf(file, "%d", buff);
            buffers[i][j] = buff[0];
        }
    }
	// Fill the remainder buffer if it needs to exist
    if (remainderExists) {
        for (int i = 0; i <= numRemainder; i++) {
            int buff[1];
            fscanf(file, "%d", buff);
            lastBuffer[i] = buff[0];
        }
    }
    fclose(file);

	// LIFO requires the requests to be filled in reverse order, and will not have a remainder
    if (lifo) {
		int totalTracksTraversed = 0;
        for (int i = (numBuffers - 1); i >= 0; i--) {
            totalTracksTraversed += scan(&currentTrack, buffers[i], bufferLength);
        }
        return ((int) (totalTracksTraversed / totalRequests));
    }

	// For all other algorithms, perform SCAN on the buffers in the order in which they were filled
	int totalTracksTraversed = 0;
    for (int i = 0; i <= numBuffers; i++) {
        totalTracksTraversed += scan(&currentTrack, buffers[i], bufferLength);
    }

	// If there is no remainder buffer, return the results
	if (!remainderExists) return ((int) (totalTracksTraversed / totalRequests));

	// If there is a remainder buffer, perform SCAN on it as well and then return the results
	totalTracksTraversed += (&currentTrack, lastBuffer, bufferLength);
	return ((int) (totalTracksTraversed / totalRequests));
}	

int main(int argc, char** argv) {

    // Read inputs from command line as strings
    char* inputFile = argv[1];

	char* startTrackStr = argv[2];
	// Convert from string to number
    int startTrack = atoi(startTrackStr);

    char* bufferLengthStr = argv[3];
    int bufferLength = atoi(bufferLengthStr);

    char* totalRequestsStr = argv[4];
    int totalRequests = atoi(totalRequestsStr);

    char* lifoStr = argv[5];
    int lifoInt = atoi(lifoStr);
    bool lifo = lifoInt == 0;

	int average = nStepScan(inputFile, startTrack, bufferLength, totalRequests, lifo);

	printf("Average Tracks Traversed: %d\n", average);
}
