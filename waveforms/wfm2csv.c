#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) {
	if (argc<2) {
		printf("Usage: %s file.wmv > out.csv\n", argv[0]);
		exit(0);
	}
	FILE *f=fopen(argv[1], "r");
	if (f==NULL) {
		perror(argv[1]);
		exit(1);
	}
	fseek(f, 0xC90, SEEK_SET);
	size_t n;
	uint8_t buf[4*1024];
	while ((n=fread(buf, 4, 1024, f))!=0) {
		for (int i=0; i<n; i++) {
			printf("%d,%d,", buf[i*4]>64?1:0, buf[i*4+3]>64?1:0);
			uint16_t j=buf[i*4+2]+(buf[i*4+1]<<8);
			for (int b=1; b!=(1<<16); b<<=1) {
				printf("%d", (j&b)?1:0);
				if (b!=(1<<15)) printf(",");
			}
			printf("\n");
		}
	}
	fclose(f);
}