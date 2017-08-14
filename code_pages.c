/*
 * Brute force extraction of Windows code pages.
 */

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
 
#define STR_SIZE 8

void printMapping(char *multiByteString, int multiByteStringLength, wchar_t *wideCharString, int wideCharStringLength) {
	int i;

	fprintf(stdout, "0x");
	for (i = 0; i < multiByteStringLength; i++) {
		fprintf(stdout, "%02X", multiByteString[i]);
	}
	fprintf(stdout, "\t0x");
	for (i = 0; i < wideCharStringLength; i++) {
		fprintf(stdout, "%04X", wideCharString[i]);
	}
	fprintf(stdout, "\t#");
}

int main(int argv, char **argv) {
	int codePage;
	CPINFOEX cpInfo;
	int success;
	char multiByteString[STR_SIZE];
	wchar_t wideCharString[STR_SIZE];
	int onlyByte;
	int resultSize;
	int firstByte;
	int secondByte;

	//TODO: At top, should print the version of Windows being used.

	for (codePage = 0; codePage < 65536; codePage++) {

		success = GetCPInfoEx(codePage, &cpInfo);
		if (success) {

			fprintf(stdout, "\n# Code page %d (%s)\n\n", codePage, cpInfo.CodePageName);

			//XXX: DefaultChar, UnicodeDefaultChar may be useful, maybe not. :/

			switch (cpInfo.MaxCharSize) {
				case 1:
				    for (onlyByte = 0; onlyByte < 256; onlyByte++) {
				    	multiByteString[0] = onlyByte;

				    	resultSize = MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS, multiByteString, 1, wideCharString, STR_SIZE);
				    	if (resultSize > 0) {
					    	printMapping(multiByteString, 1, wideCharString, resultSize);
					    }
				    }
				    break;

				case 2:
					//XXX: Could use cpInfo.LeadByte to get this information without making a call.
				    for (firstByte = 0; firstByte < 256; firstByte++) {
				        if (IsDBCSLeadByteEx(codePage, firstByte)) {
				            for (secondByte = 0; secondByte < 256; secondByte++) {
						    	multiByteString[0] = firstByte;
						    	multiByteString[1] = secondByte;

						    	resultSize = MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS, multiByteString, 2, wideCharString, STR_SIZE);
						    	if (resultSize > 0) {
							    	printMapping(multiByteString, 2, wideCharString, resultSize);
							    }
				            }
				        } else {
					    	multiByteString[0] = firstByte;

					    	resultSize = MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS, multiByteString, 1, wideCharString, STR_SIZE);
					    	if (resultSize > 0) {
						    	printMapping(multiByteString, 1, wideCharString, resultSize);
						    }
			            }
				    }
				    break;

				default:
					fprintf(stderr, "MaxCharSize was %d", cpInfo.MaxCharSize);
					return 1;
			}
		}
	}

	return 0;
}
