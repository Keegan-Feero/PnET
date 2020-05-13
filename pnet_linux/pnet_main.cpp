#include "pnet_model.h"

int main()
{
	pnet_model pnet_case;
	strcpy(pnet_case.exePath,"..");  // for console platform
	pnet_case.pnet_run(0);			// 0: for linux system;1: windows system
	
	printf("Press any key to quit PnET\n");
//	scanf("Press any key to quit PnET");

	return 0;
}
