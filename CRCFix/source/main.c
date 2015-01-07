#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct game {
	char name[16];
	unsigned int magic;
	unsigned int crcOffset;
	unsigned short (*crcCalculation)(unsigned char *data);
};

unsigned short fifa07ecrc(unsigned char *data);
unsigned short fifa08ecrc(unsigned char *data);

struct game games[] = {
	{
		name: "Fifa 07 E",
		magic: 0xF1FA07BD,
		crcOffset: 0x0A,
		crcCalculation: fifa07ecrc,
	},
	
	{
		name: "Fifa 08 E",
		magic: 0x10071981,
		crcOffset: 0x04,
		crcCalculation: fifa08ecrc,
	}
};

unsigned short fifa07ecrc(unsigned char *data) {
	unsigned short crc = 24;
	int m = 0x0000026C;
	
	int i;
	for(i = 0x00000028; i < 0x0000026B; i++) {
		crc += (data[i] * ((m - (((i / 0x10) * 0x10))) - (((i / 0x4) * 0x4) - ((i / 0x10) * 0x10))));
	}
	
	return crc;
}

unsigned short fifa08ecrc(unsigned char *data) {
	unsigned short crc = 18723;
	int m = 0x000002C4;
	
	int i;
	for(i = 0x00000044; i < 0x00000500; i++) {
		crc += (data[i] * ((m - (((i / 0x10) * 0x10))) - (((i / 0x4) * 0x4) - ((i / 0x10) * 0x10))));
	}
	
	return crc;
}

int main(int argc, char **argv) {
	bool askBeforeFixing = true;
	
	if(argc < 2) {
		printf("FIFA DS save CRC fixer\n");
		printf("Usage:\n");
		printf("CRCFix save.sav [-f]\n");
		printf(" -f: fix CRC without asking confirmation\n");
		return 1;
	}
	
	if(argc == 3 && !strcmp(argv[2], "-f")) {
		askBeforeFixing = false;
	}
	
	printf("File: %s\n", argv[1]);
	
	FILE *f = fopen(argv[1], "r+wb");
	
	if(!f) {
		printf("Failed to open\n");
		fclose(f);
		return 1;
	}
	
	unsigned int magic;
	fread(&magic, sizeof(unsigned int), 1, f);
	
	int i;
	for(i = 0; i < sizeof(games) / sizeof(struct game); i++) {
		if(magic == games[i].magic) {
			break;
		}
		else if(i == sizeof(games) / sizeof(struct game) - 1) {
			printf("Unrecognised game save\n");
			printf("(magic %p)\n", magic);
			fclose(f);
			return 1;
		}
	}
	
	printf("%s save file detected\n", games[i].name);
	
	fseek(f, 0, SEEK_END);
	size_t length = ftell(f);
	unsigned char *data = malloc(length);
	
	if(!data) {
		printf("Couldn't allocate %d bytes\n", length);
		fclose(f);
		return 1;
	}
	
	rewind(f);
	fread(data, length, 1, f);
	
	unsigned short currentCrc = *(unsigned short *)(data + games[i].crcOffset);
	
	printf("Current CRC    : 0x%p\n", currentCrc);
	
	unsigned short crc = games[i].crcCalculation(data);
	
	printf("Calculated CRC : 0x%p\n", crc);
	
	free(data);
	
	if(currentCrc != crc) {
		if(askBeforeFixing) {
			printf("Incorrect CRC, press enter to fix\n");
			getchar();
		}
		
		fseek(f, games[i].crcOffset, SEEK_SET);
		fwrite(&crc, sizeof(unsigned short), 1, f);
	}
	
	fclose(f);
	return 0;
}
