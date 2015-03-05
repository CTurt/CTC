#pragma once

struct ctcHeader {
	size_t contentLength;
	unsigned short tableLength;
};

// You can pass NULL as the destination of any compression / decompression function to get the size of the data after compression / decompression.

// Groups together consecutive bytes of 1
size_t rleBytesOf1Encode(unsigned char *destination, unsigned char *source, size_t length);
size_t rleBytesOf1Decode(unsigned char *destination, unsigned char *source, size_t length);

// Makes each byte relative to the previous, giving better RLE compression on the table
void diffEncode(unsigned char *destination, unsigned char *source, size_t length);
void diffDecode(unsigned char *destination, unsigned char *source, size_t length);

// Sorts a frequency table
void sortFrequencies(unsigned char *sortedListDestination, unsigned char *frequencyTableSource);

// Gets a list of how frequent each character is
void getFrequencies(unsigned char *frequencyTable, unsigned char *data, size_t length);

size_t CTC_Compress(unsigned char *destination, unsigned char *source, size_t length);
size_t CTC_Decompress(unsigned char *destination, unsigned char *source, size_t length);
