#include <windows.h>
#include <wincon.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define AdapterList 50

#pragma comment(lib, "netapi32.lib")

typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff[30];
}ASTAT, *PASTAT;

ASTAT Adapter;

void main(void)
{
	for (int i = 0; i < AdapterList; i++) {
		NCB ncb;
		UCHAR uRetCode;
		memset(&ncb, 0, sizeof(ncb));
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = i;

		uRetCode = Netbios(&ncb);
		//printf("The NCBRESET return code is: 0x%x \n", uRetCode);

		memset(&ncb, 0, sizeof(ncb));
		ncb.ncb_command = NCBASTAT;
		ncb.ncb_lana_num = i;

		memcpy(&ncb.ncb_callname, "*               ", 16);
		ncb.ncb_buffer = (UCHAR*)&Adapter;
		ncb.ncb_length = sizeof(Adapter);

		uRetCode = Netbios(&ncb);
		//printf("The NCBASTAT return code is: 0x%x \n", uRetCode);
		if (uRetCode == 0) {
			printf("The Ethernet Number is: %02x%02x%02x%02x%02x%02x\n",
			Adapter.adapt.adapter_address[0],
			Adapter.adapt.adapter_address[1],
			Adapter.adapt.adapter_address[2],
			Adapter.adapt.adapter_address[3],
			Adapter.adapt.adapter_address[4],
			Adapter.adapt.adapter_address[5]);
		}
	}
	system("pause");
}
