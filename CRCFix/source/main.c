#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct game {
	char name[16];
	unsigned int magic;
	unsigned int crcOffset[2];
	unsigned short (*crcCalculation[2])(unsigned char *data);
};

unsigned short fifa06ecrc(unsigned char *data);
unsigned short fifa07ecrc(unsigned char *data);
unsigned short fifa08ecrc(unsigned char *data);
unsigned short fifa09ecrc(unsigned char *data);
unsigned short fifa10ecrc(unsigned char *data);
unsigned short fifa10etournamentscrc(unsigned char *data);

unsigned short fifaStreet2ucrc(unsigned char *data);

struct game games[] = {
	{
		name: "Fifa 06 E",
		magic: 0xF1FA06AA,
		crcOffset: { 0x0A },
		crcCalculation: { fifa06ecrc, NULL },
	},
	
	{
		name: "Fifa 07 E",
		magic: 0xF1FA07BD,
		crcOffset: { 0x0A },
		crcCalculation: { fifa07ecrc, NULL },
	},
	
	{
		name: "Fifa 08 E",
		magic: 0x10071981,
		crcOffset: { 0x04 },
		crcCalculation: { fifa08ecrc, NULL },
	},
	
	{
		name: "Fifa 09 E",
		magic: 0x10071982,
		crcOffset: { 0x04 },
		crcCalculation: { fifa09ecrc, NULL },
	},
	
	{
		name: "Fifa 10 E",
		magic: 0x10071983,
		crcOffset: { 0x04, 0x06 },
		crcCalculation: { fifa10ecrc, fifa10etournamentscrc },
	},
	
	{
		name: "Fifa Street 2 U",
		magic: 0xF1FA06EE,
		crcOffset: { 0x0A },
		crcCalculation: { fifaStreet2ucrc, NULL },
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
	unsigned short crc = 0x13;
	
	int i;
	for(i = 0x00000040; i < 0x00000110; i++) {
		crc += (data[i] * (0x10C - i + 4));
	}
	
	return crc;
}

unsigned short fifa10etournamentscrc(unsigned char *data) {
	unsigned short crc = 19;
	
	int i;
	for(i = 0x00000110; i < 0x000005f8; i++) {
		crc += (data[i] * (0x4E8 - i + 0x00000110));
	}
	
	return crc;
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
	
	int j;
	for(j = 0; j < 2; j++) {
		if(!games[i].crcCalculation) break;
		
		unsigned short currentCrc = *(unsigned short *)(data + games[i].crcOffset[j]);
		
		printf("Current CRC (%d)    : 0x%p\n", j, currentCrc);
		
		unsigned short crc = games[i].crcCalculation[j](data);
		
		printf("Calculated CRC (%d) : 0x%p\n", j, crc);
		
		printf("Difference : %d\n", currentCrc - crc);
		
		if(currentCrc != crc) {
			if(askBeforeFixing) {
				printf("Incorrect CRC, press enter to fix\n");
				getchar();
			}
			
			fseek(f, games[i].crcOffset[j], SEEK_SET);
			fwrite(&crc, sizeof(unsigned short), 1, f);
		}
	}
	
	free(data);
	fclose(f);
	return 0;
}
