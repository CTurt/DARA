#include <stdio.h>
#include <stdint.h>

struct fifa08save {
	union {
		unsigned char data[65536];
		
		struct {
			uint32_t magic;
			unsigned short crc;
		};
	};
};

struct fifa08save save;

unsigned short calculateCRC(unsigned char *data) {
	unsigned short crc = 18723;
	int m = 0x000002c4;
	
	int i;
	for(i = 0x00000044; i < 0x00000500; i++) {
		crc += (data[i] * ((m - (((i / 0x00000010) * 0x00000010))) - (((i / 4) * 4) - ((i / 0x00000010) * 0x00000010))));
	}
	
	return crc;
}

int main(int argc, char **argv) {
	printf("FIFA 08 DS Save CRC fixer\n");
	
	if(argc != 2) {
		printf("No input\n");
		return 1;
	}
	
	printf("File: %s\n", argv[1]);
	
	FILE *f = fopen(argv[1], "rb");
	
	if(!f) {
		printf("Failed to open\n");
		fclose(f);
		return 1;
	}
	
	fread(save.data, sizeof(struct fifa08save), 1, f);
	
	fclose(f);
	
	if(save.magic != 0x10071981) {
		printf("Not a valid FIFA 08 EU save file\n");
		return 1;
	}
	
	printf("Current CRC   : 0x%8lx\n", save.crc);
	
	unsigned short crc = calculateCRC(save.data);
	
	printf("Calculated CRC: 0x%8lx\n", crc);
	
	if(save.crc != crc) {
		printf("Incorrect CRC, press enter to fix\n");
		getchar();
		
		save.crc = crc;
		
		FILE *f = fopen(argv[1], "wb");
		if(!f) {
			printf("Failed to open\n");
			fclose(f);
			return 1;
		}
		
		fwrite(&save, 1, sizeof(struct fifa08save), f);
		fclose(f);
	}
	
	return 0;
}
