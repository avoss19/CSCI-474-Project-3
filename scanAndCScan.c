#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*****************************************************************************
 * @brief                      	Peforms either SCAN or C-SCAN scheduling policies
 * @author              Dylan Martie
 * @date                5 Dec 21
 * @lastUpdated         5 Dec 21
 * @return              void (prints to command line)
 * @arg                                 startTrack - Starting track for policy, either 0, 100, or 199
 *                                      file - Input file of requests
 *                                      cScan - Determines if running SCAN or C-Scan
 * @note                This function currently prints out requested values to command line only
 *****************************************************************************/
void scanAndCScan(int startTrack, FILE *file, bool cScan){

	int bufferSize = 9;
	int currentTrack = startTrack;
	int requestBuffer[bufferSize];
	int tableResults[1000][2];
	int direction = 1; // 0 is to the left, 1 to right 


	// Initial population of buffer
	for (int i = 0; i < bufferSize; i++) {

		int buff[1];
		fscanf(file, "%d", buff);
		requestBuffer[i] = buff[0];
	}

	int tracksProcessed = 0;

	// Start SCAN scheduling policy
	while (tracksProcessed < 1000) {

		// Moving right
		if (direction == 1) {
			int nextTrack[3] = {201, 201, 0};
			for (int j = 0; j < bufferSize; j++) {

				// If track is larger than current Track, continue on
				if (requestBuffer[j] >= currentTrack) {
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
			if (nextTrack[0] == 201 && cScan) {
				currentTrack = 0;
				continue;
			}

			// No larger Track found, switch directions
			if (nextTrack[0] == 201) {
				direction = 0;
				continue;
			}

			// Store next track accessed and # of tracks travered
			tableResults[tracksProcessed][0] = nextTrack[0];
			tableResults[tracksProcessed][1] = nextTrack[1];
			tracksProcessed++;

			// Set new current track and add new request in buffer
			currentTrack = nextTrack[0];
			int index = nextTrack[2];

			// Final 8 processed requests do not read in new value
			if (tracksProcessed < 992) {
				int buff[1];
				fscanf(file, "%d", buff);
				requestBuffer[index] = buff[0];
			}
			else
				requestBuffer[index] = -1;
		}

		// Moving left
		else if (direction == 0) {
                        int nextTrack[3] = {201, 201, 0};
                        for (int j = 0; j < bufferSize; j++) {

                                // If track is smaller than current Track, continue on
                                if (requestBuffer[j] <= currentTrack) {
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
                        if (nextTrack[0] == 201) {
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

			// Final 8 processed requests do not read in new value
			if (tracksProcessed < 992) {
				int buff[1];
				fscanf(file, "%d", buff);
				requestBuffer[index] = buff[0];
			}
			else
				requestBuffer[index] = 201;

                }


	}
	//for (int i = 0; i < 100; i++) {
	//	printf("Next Track: %d\n", tableResults[i][0]);
	//	printf("Tracks Traversed: %d\n", tableResults[i][1]);
	//}

}


int main(int argc, char** argv) {

// Read inputs from command line as strings
        char* inputFile = argv[1];

	char* startTrackStr = argv[2];

	// Convert from string to number
        int startTrack = atoi(startTrackStr);

	FILE *file = fopen(inputFile, "r");

	scanAndCScan(startTrack, file, true);

	fclose(file);

}

