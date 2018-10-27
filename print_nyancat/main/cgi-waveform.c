#include <esp8266.h>
#include "printcart_i2s.h"
#include "httpd.h"

int cgiWaveform(HttpdConnData *connData) {
	int len;
	char buff[1024];
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "wv", buff, sizeof(buff));
	if (len>0) {
		char *p=buff;
		int i=0;
		printf("Recved: %s\n", buff);
		uint16_t wf_desc[64]={0};
		while (*p!=0) {
			wf_desc[i]=strtol(p, NULL, 16);
			p+=5;
			i++;
		}
		i2s_set_wfdesc(wf_desc, i);
	}

	httpdStartResponse(connData, 200);
	return HTTPD_CGI_DONE;
}

