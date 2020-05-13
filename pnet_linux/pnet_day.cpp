#include "pnet_model.h"

void pnet_model::pnet_day()
{
	//input files
	char climname[200] ;
	char inputname[200];


	int i, rstep,nyrs;
	int ystep = 1; //unlike matlab code, ystep is initialized here instead of in initvars.c 
	int CN_Mode = 0; //if running PnET-Day

	veg_struct veg;				//structure to hold veg data
	site_struct site;			//structure to hold the site data	
	clim_struct clim;			//structure to hold the input climate data
	out_struct out;				//structure to hold the output data	
	share_struct share;			//structure to hold share data


	sprintf(climname,"%s%s%s",exePath,PathInput,"climate.clim");
	sprintf(inputname,"%s%s%s",exePath,PathInput,"input.txt");


	// read in input data from input files

	ReadInput(&site,&veg,&share,inputname);

	ReadClim(&clim, climname);
	clim.timestep = 0; // for monthly time step

	nyrs= clim.year[clim.length]-clim.year[1]+1;

	//allocate memory
	
	memset_out(nyrs, &out);

	
	//Initialize variables

	initvars(&site,&veg, &share);

	FILE* fileoutM;
//	FILE* fileoutY;



	// print info on the screen

	printf("             PnET model starts running\n");
	printf("   ===============================================\n\n");


	//Main run loop

	for (rstep = 1; rstep <= clim.length; rstep++)
	{

		// Call subroutines
		AtmEnviron(&site, &clim, rstep, &share);
		Phenology(&veg, &clim, rstep, &share, 1);
//		share.DWater = 1.0;                            // assuming no water stress
		site.WaterStress = 0;							// assuming no water stress
		Photosyn(&site, &veg, &clim, rstep, &share);
		Waterbal(&site, &veg, &clim, rstep, &share);
//		SoilResp(&veg, &share, rstep);
//		AllocateMo(&veg, &share, rstep, CN_Mode);
		Phenology(&veg, &clim, rstep, &share, 2);
//		CNTrans(&site, &veg, &clim, rstep, &share);
//		Decomp(&veg, &clim, rstep, &share);
//		Leach(&share);
		storeoutput(&veg, &share, &out, rstep, &ystep, 0);
		WriteoutMo(&site,&veg,&clim, &share,rstep,fileoutM);

		//End-of-year activity
		if (clim.doy[rstep]>335)
		{
//			AllocateYr(&site, &veg, &clim, rstep+1, &share, CN_Mode); // Note the rstep+1, not rstep
			storeoutput(&veg, &share, &out, rstep+1, &ystep, 1);
			YearInit(&share);
		}

		printf("       Executing year %d  doy  %d  \n",clim.year[rstep],clim.doy[rstep]);


	}// end of the run loop

	// write out annual results

	WriteoutYr(&clim,&out);

	printf("   ===============================================\n");


	//free memory and close files
	memfree_all(&site,&clim, &share, &out);


}





