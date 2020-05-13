#include "pnet_model.h"

void pnet_model::pnet_run(int wins)
{
	int i;
	char inputname[200];
	char  note[200];

	FILE * f_input_load;	

	PathDefine(wins);

	sprintf(inputname,"%s%s%s",exePath,PathInput,"input.txt");

	f_input_load = fopen(inputname, "r");	

	if(f_input_load==NULL)	
	{
		printf("Unable to open input file %s!\n",inputname);
		exit(1) ;
	}	
		
  	for (i=1; i<=3; i++)fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%d\n", &modelmode);fgets(note, 200, f_input_load);	
	fscanf(f_input_load, "%d\n", &modeltype);fgets(note, 200, f_input_load);

	fclose(f_input_load);

	if (modelmode==1)  pnet_site();			// PnET site scale

	if (modelmode==2)  pnet_region();		// PnET-CN regional scale


}


void  pnet_model::pnet_site()
{
	
	if (0==modeltype)  pnet_day(); // PnET-Day
	
	if (1==modeltype)  pnet_ii();// PnET-II

	if (2==modeltype)	pnet_cn(); // PnET-CN

	if (3==modeltype)	pnet_daily(); // PnET-daily

}

void pnet_model::PathDefine(int wins)
{

	if (wins==0)sprintf(sep,"%s","/");	 //for linux path
	else sprintf(sep,"%s","\\");	 // for windows path

	sprintf(PathInput,"%sInput%s",sep,sep);	
	sprintf(PathLib,"%sLibrary%s",sep,sep);	
	sprintf(PathInter,"%sInter%s",sep,sep);	
	sprintf(PathRegion,"%sRegion%s",sep,sep);	
	sprintf(PathOutSite,"%sResult%sSite%s",sep,sep,sep);	
	sprintf(PathOutRegion,"%sResult%sRegion%s",sep,sep,sep);	


}




