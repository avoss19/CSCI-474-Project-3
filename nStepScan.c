#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/*****************************************************************************
 * @brief               Performs modified SCAN scheduling policy
 * @author              Dylan Martie, Meagan Olson
 * @date                5 Dec 21
 * @lastUpdated         9 Dec 21
 * @return              void (prints to command line)
 * @arg                 startTrack - Starting track for policy, either 0, 100, or 199
 *                      inputArr - Input array of requests
 *						bufferSize - Size of buffer
 *                      currentIndex - Index position of the next result to be placed in the results array
 *                      results - Array of number of tracks traversed, to be added to and returned
 * @note                This function currently prints out requested values to command line only
 *****************************************************************************/
int scan(int *currentTrack, int inputArr[], int bufferSize, int *currentIndex, int results[]) {

	int direction = 1; // 0 is to the left, 1 to right 

	int tracksProcessed = 0;

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

			// Store next track accessed and # of tracks travered
			results[*currentIndex] = nextTrack[1];
			*currentIndex++;
			tracksProcessed++;

			// Set new current track and add new request in buffer
			*currentTrack = nextTrack[0];
		}

		// Moving left
		else if (direction == 0) {
			int nextTrack[3] = {201, 201, 0};
			for (int j = 0; j < bufferSize; j++) {

				// If track is smaller than current Track, continue on
				if (requestBuffer[j] <= *currentTrack) {
					int tracksTraversed = (*currentTrack - requestBuffer[j]);

					// If track distance is smaller than what is stored, store at new next track
					if (tracksTraversed < nextTrack[1]) {
						nextTrack[0] = requestBuffer[j];
						nextTrack[1] = tracksTraversed;
						nextTrack[2] = j;
					}
				}
			}
			// No smaller Track found, switch directions
			if (nextTrack[0] == 201) {
				direction = 1;
				continue;
			}

			// Store next track accessed and # of tracks traversed
			results[*currentIndex] = nextTrack[1];
			*currentIndex++;
			tracksProcessed++;

			// Set new current track and add new request in buffer
			*currentTrack = nextTrack[0];
		}
	}
}

/*****************************************************************************
 * @brief               Calculates the average of a list of integers
 * @author              Meagan Olson
 * @date                9 Dec 21
 * @lastUpdated         9 Dec 21
 * @return              int - average of array of integers
 * @arg                 arr - Input array to be averaged
 *                      length - Length of input array
 *****************************************************************************/
int avg(int arr[], int length) {
	int total = 0;
	for (int i = 0; i <= length; i++) {
		total += arr[i];
	}
	return (int) (total / length);
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

	// Initialize variables to keep track of algorithm results
    int resultsIndex = 0;
	int results[totalRequests];

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
        for (int i = 0; i <= numRemainder) {
            int buff[1];
            fscanf(file, "%d", buff);
            lastBuffer[i] = buff[0];
        }
    }
    fclose(file);

	// LIFO requires the requests to be filled in reverse order, and will not have a remainder
    if (lifo) {
        for (int i = (numBuffers - 1); i >= 0; i--) {
            results = scan(&currentTrack, buffers[i], int bufferLength, &resultsIndex, results);
        }
        return avg(results, totalRequests);
    }

	// For all other algorithms, perform SCAN on the buffers in the order in which they were filled
    for (int i = 0; i <= numBuffers; i++) {
        results = scan(&currentTrack, buffers[i], int bufferLength, &resultsIndex, results);
    }
	// If there is no remainder buffer, return the results
	if (!remainderExists) return avg(results, totalRequests);

	// If there is a remainder buffer, perform SCAN on it as well and then return the results
	results = scan(&currentTrack, lastBuffer, int bufferLength, &resultsIndex, results);
	return avg(results, totalRequests);
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
