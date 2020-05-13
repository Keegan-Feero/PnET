#include "pnet_model.h"

void pnet_model::GetRegionPath(char * PathProj)
{
	char inputname[200];
	FILE * f_input_load;
	char  note[200];

	sprintf(inputname,"%s%s%s",exePath,PathInput,"input.txt");

	f_input_load = fopen(inputname, "r");	

	if(f_input_load==NULL)	
	{
		printf("Unable to open input file %s!\n",inputname);
		exit(1) ;
	}	
		

	for (int i=1; i<=5; i++)fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%s\n", inputname);fgets(note, 200, f_input_load);	

	fclose(f_input_load);

	sprintf(PathProj,"%s%s%s%s",exePath,PathRegion,inputname,sep);


}

void pnet_model::ReadRegionFile()
{

	char inputname[200];
	FILE * f_input_load;
	char  note[200];

	sprintf(inputname,"%s%s%s",exePath,PathInput,"Region.rgn");

	f_input_load = fopen(inputname, "r");	

	if(f_input_load==NULL)	
	{
		printf("Unable to open input file %s!\n",inputname);
		exit(1) ;
	}	
		
  
	fgets(note, 200, f_input_load);

	for (int i=1; i<=5; i++)fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%s\n", inputname);fgets(note, 200, f_input_load);	

	fclose(f_input_load);


}


void pnet_model::pnet_region()
{
	//input files

	char climname[200] ;
	char inputname[200];
	char outname[200];
	char pathproj[200];
	int ngrid,grid,gridid;
	char gridname[30];
	char  note[200],filename[30];


	int CN_Mode = 1; //if running PnET-CN
	int ystep = 1;

	int rstep,nyrs;
	veg_struct veg;				//structure to hold veg data
	site_struct site;			//structure to hold the site data	
	clim_struct clim;			//structure to hold the input climate data
	out_struct out,outrgn;				//structure to hold the output data	
	share_struct share;			//structure to hold share data

	
	// print info on the screen

	printf("             PnET model starts running\n");
	printf("   ===============================================\n\n");

	GetRegionPath(pathproj);

	FILE * f_input_load;

	sprintf(inputname,"%s%s%s",exePath,PathInput,"Region.rgn");

	f_input_load = fopen(inputname, "r");	

	if(f_input_load==NULL)	
	{
		printf("Unable to open input file %s!\n",inputname);
		exit(1) ;
	}	
		

	fscanf(f_input_load, "%d\n", &ngrid);fgets(note, 200, f_input_load);
	
	memset_out(ngrid,&outrgn);

	for (grid=1;grid<=ngrid;grid++)
	{
		fscanf(f_input_load, "%d %s\n", &gridid,gridname);fgets(note, 200, f_input_load);	

		sprintf(filename,"%s_climate.clim",gridname);
		sprintf(climname,"%s%s",pathproj,filename);
	
		sprintf(filename,"%s_input.txt",gridname);
		sprintf(inputname,"%s%s",pathproj,filename);

		// read in input data from input files

		ReadInput(&site,&veg,&share,inputname);

		ReadClim(&clim, climname);

		nyrs= clim.year[clim.length]-clim.year[1]+1;

		//allocate memory
		
		memset_out(nyrs, &out);
		
		//Initialize variables

		initvars(&site,&veg, &share);

		//Main run loop

		for (rstep = 1; rstep <= clim.length; rstep++)
		{

			// Call subroutines
			AtmEnviron(&site, &clim, rstep, &share);
			Phenology(&veg, &clim, rstep, &share, 1);
			Photosyn(&site, &veg, &clim, rstep, &share);
			Waterbal(&site, &veg, &clim, rstep, &share);
			AllocateMo(&veg, &share, rstep, CN_Mode);
			Phenology(&veg, &clim, rstep, &share, 2);
			CNTrans(&site, &veg, &clim, rstep, &share);
			Decomp(&site,&veg, &clim, rstep, &share);
			Leach(&share);
			storeoutput(&veg, &share, &out, rstep, &ystep, 0);

			//End-of-year activity
			if (clim.doy[rstep]>335)
			{
				AllocateYr(&site, &veg, &clim, rstep+1, &share, CN_Mode); // Note the rstep+1, not rstep
				storeoutput(&veg, &share, &out, rstep+1, &ystep, 1);
				YearInit(&share);
			}

			printf("       Executing %d %d  doy  %d  \n",grid,clim.year[rstep],clim.doy[rstep]);


		}// end of the time loop

		// write out annual results

		//WriteoutYr(&clim,&out);
		StoreoutRgn(grid,nyrs,&out,&outrgn);

		//free memory and close files
		memfree_all(&site,&clim, &share, &out);


	}// end if the space loop


	sprintf(outname,"%s%s%s",exePath,PathOutRegion,"Output_annual.txt");
	WriteoutRgn(ngrid,&outrgn,outname);

	fclose(f_input_load);
	memfree_out(&outrgn);

	printf("   ===============================================\n");

}

void pnet_model::StoreoutRgn(int grid,int nyrs,out_struct* out,out_struct* outrgn)
{

//	int nyrs = clim->year[clim->length]-clim->year[1]+1;
	int avgyrs; //number of years to average the output
	avgyrs=30;


//	for (int i=1;i<nyrs+1;i++)
	for (int i=nyrs;i>nyrs-avgyrs;i--)
	{	
		
		outrgn->folm[grid] +=out->folm[i]/avgyrs;
		outrgn->deadwoodm[grid] +=out->deadwoodm[i]/avgyrs;
		outrgn->woodm[grid] +=out->woodm[i]/avgyrs;
		outrgn->rootm[grid] +=out->rootm[i]/avgyrs;

		outrgn->hom[grid] +=out->hom[i]/avgyrs;
		outrgn->hon[grid] +=out->hon[i]/avgyrs;
/**/
		outrgn->nppfol[grid] +=out->nppfol[i]/avgyrs;
		outrgn->nppwood[grid] +=out->nppwood[i]/avgyrs;
		outrgn->npproot[grid] +=out->npproot[i]/avgyrs;
		outrgn->psn[grid] +=out->psn[i]/avgyrs;		// total net psn
		outrgn->nep[grid] +=out->nep[i]/avgyrs;
		outrgn->gpp[grid] +=out->gpp[i]/avgyrs;


		outrgn->prec[grid] +=out->prec[i]/avgyrs;
		outrgn->evap[grid] +=out->evap[i]/avgyrs;
		outrgn->trans[grid] +=out->trans[i]/avgyrs;
		outrgn->et[grid] +=out->et[i]/avgyrs;
		outrgn->drain[grid] +=out->drain[i]/avgyrs;
		outrgn->waterstress[grid] +=out->waterstress[i]/avgyrs;

		outrgn->budc[grid] +=out->budc[i]/avgyrs;
		outrgn->plantc[grid] +=out->plantc[i]/avgyrs;
		outrgn->woodc[grid] +=out->woodc[i]/avgyrs;
		outrgn->rootc[grid] +=out->rootc[i]/avgyrs;


		outrgn->ndep[grid] +=out->ndep[i]/avgyrs;
		outrgn->plantnYr[grid] +=out->plantnYr[i]/avgyrs;
		outrgn->budn[grid] +=out->budn[i]/avgyrs;
		outrgn->ndrain[grid] +=out->ndrain[i]/avgyrs;
		outrgn->netnmin[grid] +=out->netnmin[i]/avgyrs;
		outrgn->netnitrif[grid] +=out->netnitrif[i]/avgyrs;
		
		outrgn->nratio[grid] +=out->nratio[i]/avgyrs;
		outrgn->foln[grid] +=out->foln[i]/avgyrs;
		outrgn->litm[grid] +=out->litm[i]/avgyrs;
		outrgn->litn[grid] +=out->litn[i]/avgyrs;

		outrgn->rmresp[grid] +=out->rmresp[i]/avgyrs;
		outrgn->rgresp[grid] +=out->rgresp[i]/avgyrs;
		outrgn->decresp[grid] +=out->decresp[i]/avgyrs;

		outrgn->v1[grid] +=out->v1[i]/avgyrs;
		outrgn->v2[grid] +=out->v2[i]/avgyrs;
/**/
	}

	init_out(nyrs, out);

}


void pnet_model::WriteoutRgn(int ngrid,out_struct* out,char * filename)
{

	FILE* fileoutY;

	fileoutY = fopen(filename, "w" );
	if (fileoutY==NULL)
	{
		printf("Unable to open the Output_annual.txt !\n");
		exit(1) ;	
	}


	fprintf(fileoutY," grid, folm, deadwoodm, livewoodm, rootm, som, son,"
		"nppfol, nppwood, npproot, psn, nep, gpp,"
		"prec, evap, trans, et, drain, wtrstress,"
		"plantc, budc, woodc, rootc, ndep, plantnYr, budn, ndrain, netnmin, netnitrif, nratio, foln,"
		"litm, litn, rmresp, rgresp, decresp,"
		"temp, par"
		"\n");
	fprintf(fileoutY," ,g/m2,g/m2,g/m2,g/m2,g/m2,gN/m2,"
		"g/m2,g/m2,g/m2,gC/m2,gC/m2,gC/m2,"
		"cm,cm,cm,cm,cm, ,"
		"gC/m2,gC/m2,gC/m2,gC/m2,gN/m2,gN/m2,gN/m2,gN/m2,gN/m2,gN/m2, , %%,"
		"gC/m2, gN/m2,gC/m2,gC/m2,gC/m2,"
		"oC, umol/m2/s  "
		"\n");


	
	
	for (int i=1;i<ngrid+1;i++)
	{	
		
		fprintf(fileoutY,"%5d, ",i);  // sequence of grid	double*	folm;

		fprintf(fileoutY,"%8.2f, %8.2f, %8.2f",out->nppfol[i],out->nppwood[i],out->npproot[i]);// C ballance

		fprintf(fileoutY,"%8.2f, %8.2f, %8.2f, %8.2f, %8.2f, %8.2f,",out->folm[i],out->deadwoodm[i],
			out->woodm[i],out->rootm[i],out->hom[i],out->hon[i]);// C ballance

		fprintf(fileoutY,"%8.2f, %8.2f, %8.2f, %8.2f, %8.2f, %8.2f,",out->nppfol[i],out->nppwood[i],
			out->npproot[i],out->psn[i],out->nep[i],out->gpp[i]);// C ballance

		fprintf(fileoutY,"%8.2f, %8.2f, %8.2f, %8.2f, %8.2f, %8.2f,",out->prec[i],out->evap[i],
			out->trans[i],out->et[i],out->drain[i],out->waterstress[i]);// water cycle

		fprintf(fileoutY,"%8.2f, %8.2f, %8.2f, %8.2f, ",out->plantc[i],out->budc[i],out->woodc[i],out->rootc[i]);// carbon cycle
		
		fprintf(fileoutY,"%8.2f, %8.2f, %8.2f, %8.2f, %8.2f, ",out->ndep[i],out->plantnYr[i],out->budn[i],out->ndrain[i],out->netnmin[i]);// N cycle

		fprintf(fileoutY,"%7.2f, %7.2f, %7.2f, ",out->netnitrif[i],out->nratio[i],out->foln[i]);// N cycle

		fprintf(fileoutY,"%8.2f, %8.2f, %8.2f, %8.2f, %8.2f",out->litm[i],out->litn[i],out->rmresp[i],out->rgresp[i],out->decresp[i]);// N cycle

		fprintf(fileoutY,"\n");// N cycle


	}


	fclose(fileoutY);


}







