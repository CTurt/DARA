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

unsigned short fifa06ecrc(unsigned char *data);
unsigned short fifa07ecrc(unsigned char *data);
unsigned short fifa08ecrc(unsigned char *data);
unsigned short fifa09ecrc(unsigned char *data);
unsigned short fifa10ecrc(unsigned char *data);

unsigned short fifaStreet2ucrc(unsigned char *data);

struct game games[] = {
	{
		name: "Fifa 06 E",
		magic: 0xF1FA06AA,
		crcOffset: 0x0A,
		crcCalculation: fifa06ecrc,
	},
	
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
	},
	
	{
		name: "Fifa 09 E",
		magic: 0x10071982,
		crcOffset: 0x04,
		crcCalculation: fifa09ecrc,
	},
	
	{
		name: "Fifa 10 E",
		magic: 0x10071983,
		crcOffset: 0x04,
		crcCalculation: fifa10ecrc,
	},
	
	{
		name: "Fifa Street 2 U",
		magic: 0xf1fa06ee,
		crcOffset: 0x0a,
		crcCalculation: fifaStreet2ucrc,
	}
};

unsigned short fifa06ecrc(unsigned char *data) {
	unsigned short crc = 64325;
	int m = 344;
	
	int i;
	for(i = 0x0000000C; i < 0x00000158; i++) {
		crc += (data[i] * ((m - (((i / 0x10) * 0x10))) - (((i / 0x4) * 0x4) - ((i / 0x10) * 0x10))));
	}
	
	return crc;
}

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

unsigned short fifa09ecrc(unsigned char *data) {
	unsigned short crc = 19;
	
	int i;
	for(i = 0x00000020; i < 0x0000010D; i++) {
		crc += (data[i] * (0x10C - i));
	}
	
	return crc;
}

unsigned short fifa10ecrc(unsigned char *data) {
	unsigned char crc = -133 - 104;
	unsigned short part2 = 0x10000;
	
	int i;
	for(i = 0x00000040; i < 0x00000110; i++) {
		crc += (data[i] * (0x10C - i + 4));
		part2 += (data[i] * (0x10C - i + 4));
	}
	
	part2 &= 0xFF00;
	
	return crc | part2;
}

unsigned short fifaStreet2ucrc(unsigned char *data) {
	unsigned int crc = -9649843;
	int m = 4052;
	
	int i;
	for(i = 0x00000010; i < 4051; i++) {
		crc += (data[i] * ((m - (((i / 0x10) * 0x10))) - (((i / 0x4) * 0x4) - ((i / 0x10) * 0x10))));
	}
	
	crc &= 0x0000FFFF;
	return (unsigned short)crc;
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
	free(data);
	
	printf("Calculated CRC : 0x%p\n", crc);
	
	//printf("Difference : %d\n", currentCrc - crc);
	
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
