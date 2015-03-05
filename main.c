#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ctc.h"

int main(int argc, char **argv) {
	enum {
		MODE_UNSPECIFIED,
		MODE_COMPRESS,
		MODE_DECOMPRESS,
	} mode = MODE_UNSPECIFIED;
	
	FILE *f;
	
	char *input = NULL;
	char *output = NULL;
	
	unsigned char *inputData = NULL;
	unsigned char *outputData = NULL;
	
	size_t inputSize;
	size_t outputSize;
	
	if(argc < 2) {
		printf("CTC Compressor / Decompressor\n");
		printf("Usage:\n");
		printf("CTC input.bin [-c] [-d] [-o output.bin]\n");
		printf(" -c: Compress\n");
		printf(" -d: Decompress\n");
		printf(" -o: Specify output\n");
		printf("If neither -c, nor -d is specified, compression will be used.\n");
		printf("If no output is specified, the input will be overwritten.\n");
		printf("EG: CTC data.bin -c -o data_compressed.bin\n");
		return 1;
	}
	
	int i;
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-c") == 0) {
			if(mode != MODE_UNSPECIFIED) {
				printf("Error! Multiple modes specified!\n");
				return 1;
			}
			
			mode = MODE_COMPRESS;
		}
		
		else if(strcmp(argv[i], "-d") == 0) {
			if(mode != MODE_UNSPECIFIED) {
				printf("Error! Multiple modes specified!\n");
				return 1;
			}
			
			mode = MODE_DECOMPRESS;
		}
		
		else if(strcmp(argv[i], "-o") == 0) {
			if(i + 1 > argc) {
				printf("Error! No filename after -o!\n");
				return 1;
			}
			
			i++;
			
			size_t length = strlen(argv[i]) + 1;
			output = malloc(length);
			strcpy(output, argv[i]);
		}
		
		else {
			if(input) {
				printf("Error! Multiple inputs not supported!\n");
			}
			
			size_t length = strlen(argv[i]) + 1;
			input = malloc(length);
			strcpy(input, argv[i]);
		}
	}
	
	if(mode == MODE_UNSPECIFIED) {
		mode = MODE_COMPRESS;
	}
	
	if(!output) {
		size_t length = strlen(input) + 1;
		output = malloc(length);
		strcpy(output, input);
	}
	
	f = fopen(input, "rb");
	if(!f) {
		printf("Error! Could not open \"%s\"\n", input);
		free(input);
		free(output);
		return 1;
	}
	
	fseek(f, 0, SEEK_END);
	inputSize = ftell(f);
	rewind(f);
	
	inputData = malloc(inputSize);
	if(!inputData) {
		printf("Error! Unable to allocate %d bytes for input!\n", inputSize);
		fclose(f);
		free(input);
		free(output);
		return 1;
	}
	
	fread(inputData, inputSize, 1, f);
	fclose(f);
	
	if(mode == MODE_COMPRESS) {
		printf("Compressing \"%s\"...\n", input);
		
		outputSize = CTC_Compress(NULL, inputData, inputSize);
		
		outputData = malloc(outputSize);
		
		CTC_Compress(outputData, inputData, inputSize);
		
		f = fopen(output, "wb");
		if(!f) {
			printf("Error! Could not write to \"%s\"!\n", output);
			free(input);
			free(output);
			free(inputData);
			free(outputData);
			return 1;
		}
		
		fwrite(outputData, outputSize, 1, f);
		fclose(f);
		
		printf("Done! Wrote output to \"%s\"!\n", output);
		
		double p = ((double)((signed long)outputSize - (signed long)inputSize) / (double)inputSize) * 100.0d;
		printf("%d bytes to %d bytes, %.2f%% %s!\n", inputSize, outputSize, fabs(p), p >= 0.0d ? "gain" : "reduction");
	}
	
	else if(mode == MODE_DECOMPRESS) {
		printf("Decompressing \"%s\"...\n", input);
		
		outputSize = CTC_Decompress(NULL, inputData, inputSize);
		
		outputData = malloc(outputSize);
		
		CTC_Decompress(outputData, inputData, inputSize);
		
		f = fopen(output, "wb");
		if(!f) {
			printf("Error! Could not write to \"%s\"!\n", output);
			free(input);
			free(output);
			free(inputData);
			free(outputData);
			return 1;
		}
		
		fwrite(outputData, outputSize, 1, f);
		fclose(f);
		
		printf("Done! Wrote output to \"%s\"!\n", output);
	}
	
	free(input);
	free(output);
	free(inputData);
	free(outputData);
	
	return 0;
}
