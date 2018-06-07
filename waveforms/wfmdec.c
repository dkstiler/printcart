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
	int bt[3]={0};
	int cst=0, cv=0, lst=0, p=0, del=0;
	while ((n=fread(buf, 4, 1024, f))!=0) {
		for (int i=0; i<n; i++) {
			uint16_t j=buf[i*4+2]+(buf[i*4+1]<<8);
			int clk=(j&(1<<8));
			if (clk!=cv) {
				del=20; //delay
				cv=clk;
			}
			if (del!=0) del--;
			if (del==1) {
				if (p-lst>1000) {
					//printf("%02X %02X %02X\n", bt[0], bt[1], bt[2]);
//					printf("%02X ", bt[1]);
					bt[0]=0; bt[1]=0; bt[2]=0;
				}
				for (int k=0; k<3; k++) bt[k]<<=1;
				if (j&(1<<0)) bt[0]|=1;
				if (j&(1<<1)) bt[1]|=1;
				if (j&(1<<2)) bt[2]|=1;
				lst=p;
				printf("%c", (j&(1<<1))?'*':'.');
			}
			p++;
		}
	}
	printf("\n");
	fclose(f);
}