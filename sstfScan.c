#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * @brief                      	Peforms SSTF (Shortest-Service-Time-First) scheduling policy
 * @author              Dylan Martie
 * @date                6 Dec 21
 * @lastUpdated         6 Dec 21
 * @return              void (prints to command line)
 * @arg                                 startTrack - Starting track for policy, either 0, 100, or 199
 *                                      file - Input file of requests
 * @note                This function currently prints out requested values to command line only
 *****************************************************************************/
void sstfScan(int startTrack, FILE *file) {

	int bufferSize = 10;
	int currentTrack = startTrack;
	int requestBuffer[bufferSize];
	int tableResults[1000][2];

	// Initial population of buffer
	for (int i = 0; i < bufferSize; i++) {

		int buff[1];
		fscanf(file, "%d", buff);
		requestBuffer[i] = buff[0];
	}

	for (int i = 0; i < 1000; i++) {

		int nextTrack[3] = {201, 201, 0};
		for (int j = 0; j < bufferSize; j++) {

			int tracksTraversed = abs(requestBuffer[j] - currentTrack);
			if (tracksTraversed < nextTrack[1]) {
				nextTrack[0] = requestBuffer[j];
				nextTrack[1] = tracksTraversed;
				nextTrack[2] = j;
			}
		}

		// Store next track accessed and # of tracks travered
                tableResults[i][0] = nextTrack[0];
                tableResults[i][1] = nextTrack[1];

		// Set new current track and add new request in buffer
		currentTrack = nextTrack[0];
		int index = nextTrack[2];

		int buff[1];
		fscanf(file, "%d", buff);
		requestBuffer[index] = buff[0];
	}

//	for (int i = 0; i < 100; i++) {
//		printf("Next Track: %d\n", tableResults[i][0]);
//		printf("Tracks Traversed: %d\n", tableResults[i][1]);
//	}

}

int main(int argc, char** argv) {

// Read inputs from command line as strings
        char* inputFile = argv[1];

	char* startTrackStr = argv[2];

	// Convert from string to number
        int startTrack = atoi(startTrackStr);

	FILE *file = fopen(inputFile, "r");

	sstfScan(startTrack, file);

	fclose(file);

}
