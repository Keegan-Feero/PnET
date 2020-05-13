#include "pnet_model.h"

void pnet_model::ReadInput(site_struct* site,veg_struct* veg,share_struct* share, char* inputname)
{


	int i;

	FILE * f_input_load;
	f_input_load = fopen(inputname, "r");

	if(f_input_load==NULL)	
	{
		printf("Unable to open input file %s!\n",inputname);
		exit(0) ;
	}	

	char  note[200];

	//~ ~ ~ ~ ~ ~ skip model options information

	for (i=1; i<=5; i++)fgets(note, 200, f_input_load);
	
	//~ ~ ~ ~ ~ ~ read in site information
	for (i=1; i<=2; i++)fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &site->Lat);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%d", &site->O3EffectOnWUE);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%d", &site->CO2gsEffect);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%d", &site->Nabs);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%d", &site->nYearStart);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%d", &site->nYearEnd);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%s", site->ClimateFileName);fgets(note, 200, f_input_load);


	//~ ~ ~ ~ ~ ~ read in soil information
	fscanf(f_input_load, "%lf", &site->WHC);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->SnowPack);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->SWater);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->SoilMoistFact);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->FastFlowFrac);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->WaterReleaseFact);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->WaterStress);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->HOM);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->HON);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->NH4);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->Kho);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &site->SoilRespA);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->SoilRespB);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->NImmobA);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->NImmobB);fgets(note, 200, f_input_load);


	//~ ~ ~ ~ ~ ~ read in forest informtion
	for (i=1; i<=2; i++)fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%d", &veg->TreeCode);fgets(note, 200, f_input_load);
	veg->TreeCode --;	
	fscanf(f_input_load, "%d", &veg->Age);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->FolMass);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->WoodMass);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->DeadWoodM);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->RootMass);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->BudC);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->WoodC);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->PlantC);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->PlantN);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &share->NRatio);fgets(note, 200, f_input_load);
	
	fscanf(f_input_load, "%lf", &veg->AmaxA);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->AmaxB);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->HalfSat);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->BaseFolRespFrac);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RespQ10);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->PsnTMin);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->PsnTOpt);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->AmaxFrac);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &veg->FolReten);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->SLWmax);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->SLWdel);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->GDDFolStart);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->GDDFolEnd);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->GDDWoodStart);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->GDDWoodEnd);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->SenescStart);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &veg->FolMassMax);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->FolMassMin);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->k);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->FolNCon);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->FolRelGrowMax);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &veg->CFracBiomass);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RootAllocA);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RootAllocB);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->GRespFrac);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->WoodMRespA);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RootMRespFrac);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->PlantCReserveFrac);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->MinWoodFolRatio);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &veg->DVPD1);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->DVPD2);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->WUEConst);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->PrecIntFrac);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &veg->FLPctN);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RLPctN);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->WLPctN);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->FolNConRange);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->FolNRetrans);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->MaxNStore);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &veg->WoodTurnover);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RootTurnoverA);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RootTurnoverB);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->RootTurnoverC);fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%lf", &veg->WoodLitLossRate);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &veg->WoodLitCLoss);fgets(note, 200, f_input_load);

	//~ ~ ~ ~ ~ ~ read in management information
	for (i=1; i<=2; i++)fgets(note, 200, f_input_load);

	fscanf(f_input_load, "%d", &site->distyrs);fgets(note, 200, f_input_load);

	//allocate memory for disturbance variables
	site->distintensity = (double*)malloc((site->distyrs+1)*sizeof(double)); //use site->distyrs+1 so that the subscript can start from 1
	site->distyear = (int*)malloc((site->distyrs+1)*sizeof(int));
	site->distremove  = (double*)malloc((site->distyrs+1)*sizeof(double));
	site->distsoilloss = (double*)malloc((site->distyrs+1)*sizeof(double));
	site->folregen = (double*)malloc((site->distyrs+1)*sizeof(double));
	site->distdoy = (int*)malloc((site->distyrs+1)*sizeof(int));


	if (site->distyrs>0) 
	{
		for (i=1;i<=site->distyrs;i++)
		{


			fscanf(f_input_load, "%d %d %lf %lf %lf %lf\n",
					&site->distyear[i],
					&site->distdoy[i],
					&site->distintensity[i],
					&site->distremove[i],
					&site->distsoilloss[i],
					&site->folregen[i]
					);

			fgets(note, 200, f_input_load);


		}

	}


	fscanf(f_input_load, "%d", &site->agstart);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%d", &site->agstop);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf", &site->agrem);fgets(note, 200, f_input_load);
	
	fscanf(f_input_load, "%d %d", &site->FertYrStart,&site->FertDOYStart);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%d %d", &site->FertYrEnd,&site->FertDOYEnd);fgets(note, 200, f_input_load);
	fscanf(f_input_load, "%lf %lf %lf", &site->FertNH4, &site->FertNO3, &site->FertUrea);fgets(note, 200, f_input_load);

	fclose(f_input_load);

}

//read *.clim file
void pnet_model::ReadClim(clim_struct* clim, char* climname)
{
	char buf[200];
	int i;

	FILE* fileClim;

	if ((fileClim = fopen(climname, "rt")) == NULL)
	{	
		printf("Unable to open the clim file!\n");
		exit(1) ;
	}

	i=0;
	while(NULL!=fgets(buf,200,fileClim))i++; 

	clim->length=i-1;

	rewind(fileClim);
	
	//allocate memory for the climate structure
	memset_climate(clim);

	fgets(buf, 200, fileClim);  // skip header

	for (i = 1; i <= clim->length; i++)
	{
		fscanf(fileClim, "%d",&clim->year[i]); //year
		fscanf(fileClim, "%d", &clim->doy[i]); //doy
		fscanf(fileClim, "%lf",&clim->tmax[i]); //tmax
		fscanf(fileClim, "%lf",&clim->tmin[i]); //tmin
		fscanf(fileClim, "%lf",&clim->par[i]); //par
		fscanf(fileClim, "%lf",&clim->prec[i]); //prec
		fscanf(fileClim, "%lf",&clim->O3[i]); //O3
		fscanf(fileClim, "%lf",&clim->CO2[i]); //CO2
		fscanf(fileClim, "%lf",&clim->NH4dep[i]); //NH4dep
		fscanf(fileClim, "%lf",&clim->NO3dep[i]); //NO3dep


	}
	fclose(fileClim);
}



void pnet_model::WriteoutMo(site_struct* site,veg_struct* veg,clim_struct* clim, share_struct* share,int rstep,FILE* &fileoutM)
{


	// write out header	
	if (rstep==1)
	{
		char foutput[200] ;

		sprintf(foutput,"%s%s%s",exePath,PathOutSite,"Output_monthly.csv");

		fileoutM = fopen(foutput, "w" );
		if (fileoutM==NULL)printf("Unable to open the Output_monthly.csv !\n");

		fprintf(fileoutM," year,   doy,  temp,   GrowthDD,  "
			"prec,     vpd, snowpack,   ET,    Drain, Soilwater, "
			"Water, WaterIx, WaterIx,  "
			"Foliage, LiveWood,  DeadWood,    "
			"Root,  PlantC,    BudC,     WoodC,    RootC, "
			"PosCBalMass, BalMassTot, BalMassIx, "
			"LAI,  LightEffMin, LightEffCBal, "
			"NetPsnMo, GrsPsnMo, NEE,    "
			"WoodMR,   WoodGR,   FolGR,   RootGR,   RootMR, SoilResp,  "
			"airT, NetNMinYr, NetNitrYr,NRatioNit,NDrain,NDrainMgL,FoliarN,"
			"N2O,     NO,      N2,  FluxN2OYrDe, FluxNOYrDe"
			"\n");
		
		fprintf(fileoutM,"   ,       ,    oC,      oC,       "
			"cm,      kpa,   cm,       cm,      cm,       cm,   "
			"Index,   Days,   total,   "
			"g/m2,     g/m2,     g/m2,      g/m2,   "
			"gC/m2,     gC/m2,    gC/m2,    gC/m2,    "
			"gC/m2,      gC/m2,     days,     "
			"m2/m2,        ,      gC/m2,    "
			"gC/m2,     gC/m2,    gC/m2,   "
			"gC/m2,    gC/m2,    gC/m2,    gC/m2,    gC/m2,    gC/m2, "
			"oC,gN/m2,gN/m2,gN/m2,gN/m2,mgN/l,%%,"
			"mgN/m2,mgN/m2,mgN/m2,mgN/m2,mgN/m2"  // Ngas
			"\n");	
	
	}

	// write out monthly results
	fprintf(fileoutM,"%5d, %5d, %6.1f, %8.1f,",
		clim->year[rstep],
		clim->doy[rstep],
		share->Tave,
		share->GDDTot);  // sequence of date
	fprintf(fileoutM,"%8.2f, %7.2f, %7.2f, %7.2f, %7.2f,  %7.2f,",
		clim->prec[rstep],
		share->VPD,
		site->SnowPack,
		share->ET,
		share->Drainage,
		share->Water);// water ballance
	fprintf(fileoutM,"%6.2f, %7.1f, %7.1f,",
		share->DWater,
		share->DwaterIx,
		share->Dwatertot);// water effect
	fprintf(fileoutM,"%8.2f, %9.2f, %9.2f, %7.2f,",
		share->FolMass,
		share->WoodMass,
		share->DeadWoodM,
		share->RootMass);// Carbon pools
	fprintf(fileoutM,"%9.2f, %8.2f, %8.2f, %8.2f,",
		share->PlantC,
		share->BudC,
		share->WoodC,
		share->RootC);// Carbon pools
	fprintf(fileoutM,"%8.1f, %8.1f, %10.1f,",
		share->PosCBalMass,
		share->PosCBalMassTot,
		share->PosCBalMassIx);// Carbon pools
	fprintf(fileoutM,"%8.1f, %8.2f, %8.2f,",
		share->LAI,
		share->LightEffMin,	
		share->LightEffCBal);// photosynthesis 
	fprintf(fileoutM,"%8.2f, %8.2f, %8.2f, ",
		share->NetPsnMo,
		share->GrsPsnMo,
		-share->NetCBal);// photosynthesis
	fprintf(fileoutM,"%8.1f, %8.1f, %8.1f, %8.1f, %8.1f, %8.1f,",
		share->WoodMRespYr,
		share->WoodGRespYr,
		share->FolGRespYr,
		share->RootGRespYr,
		share->RootMRespYr,
		share->SoilDecRespYr);

	fprintf(fileoutM,"%10.2f,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f,",
			share->Tave,
			share->NetNMinYr,
			share->NetNitrYr,
			share->NRatioNit,
			share->NDrain,
			share->NDrainMgL);//share->WFPSshare->DayLength

	fprintf(fileoutM,"%7.3f,",
			veg->FolNCon);//Foliar N

	fprintf(fileoutM,"%8.2f, %8.2f, %8.2f,",
		share->FluxN2OYr*1000,
		share->FluxNOYr*1000,
		share->FluxN2Yr*1000);
	fprintf(fileoutM,"%8.2f, %8.2f",
		share->FluxN2OYrDe*1000,
		share->CanopyDO3);
//		share->FluxNOYrDe*1000);



	fprintf(fileoutM,"\n");

	if (rstep== clim->length) fclose(fileoutM);

}


void pnet_model::WriteoutYr(clim_struct* clim,out_struct* out)
{
	int nyrs = clim->year[clim->length]-clim->year[1]+1;

	char outputyr[200] ;
	FILE* fileoutY;

	sprintf(outputyr,"%s%s%s",exePath,PathOutSite,"Output_annual.csv");

	fileoutY = fopen(outputyr, "w" );
	if (fileoutY==NULL)
	{
		printf("Unable to open the Output_annual.csv !\n");
		exit(1) ;	
	}


	fprintf(fileoutY," year, folm,  deadwoodm,  livewoodm,  rootm,   som,       son,     "
		"nppfol, nppwood, npproot,  psn,      nep,    gpp,    "
		"prec,     evap,  trans,   et,   drain, wtrstress, "
		"plantc,   budc,    woodc,     rootc,    "
		"ndep, plantnYr, budn, ndrain, netnmin, netnitrif, nratio, foln, "
		"litm,    litn,    rmresp,   rgresp, decresp,  "
		"N2O,     NO,      N2,  N2Ode,  NOde,"   // ngas
		"v1, v2,v3"
		"\n");
	fprintf(fileoutY,"  ,    g/m2,   g/m2,        g/m2,     g/m2,    g/m2,      gN/m2,   "
		"g/m2,   g/m2,    g/m2,     gC/m2,   gC/m2,   gC/m2,   "
		"cm,       cm,     cm,    cm,      cm,         ,    "
		"gC/m2,  gC/m2,   gC/m2,     gC/m2,    "
		"gN/m2, gN/m2,  gN/m2, gN/m2,   gN/m2,   gN/m2,       ,   %%,   "
		"gC/m2,   gN/m2,   gC/m2,    gC/m2,   gC/m2,  "
		"mgN/m2, mgN/m2, mgN/m2,  mgN/m2, mgN/m2,"
		"v1,v2,v3"
		"\n");
	
	
	for (int i=1;i<nyrs+1;i++)
	{	
		
		fprintf(fileoutY,"%5d, ",i+clim->year[1]-1);  // sequence of year	double*	folm;

		fprintf(fileoutY,"%6.1f, %8.1f, %8.1f, %8.1f, %8.1f, %8.1f,",out->folm[i],out->deadwoodm[i],
			out->woodm[i],out->rootm[i],out->hom[i],out->hon[i]);// C ballance

		fprintf(fileoutY,"%8.1f, %7.1f, %7.1f, %7.1f, %7.1f, %7.1f,",out->nppfol[i],out->nppwood[i],
			out->npproot[i],out->psn[i],out->nep[i],out->gpp[i]);// C ballance

		fprintf(fileoutY,"%7.1f, %6.1f, %6.1f, %6.1f, %6.1f, %6.2f,",out->prec[i],out->evap[i],
			out->trans[i],out->et[i],out->drain[i],out->waterstress[i]);// water cycle

		fprintf(fileoutY,"%9.2f, %8.2f, %8.2f, %8.2f, ",out->plantc[i],out->budc[i],out->woodc[i],out->rootc[i]);// carbon cycle
		
		fprintf(fileoutY,"%6.2f, %6.2f, %6.2f, %6.2f, %6.2f, ",out->ndep[i],out->plantnYr[i],out->budn[i],out->ndrain[i],out->netnmin[i]);// N cycle

		fprintf(fileoutY,"%7.2f, %7.2f, %7.2f, ",out->netnitrif[i],out->nratio[i],out->foln[i]);// N cycle

		fprintf(fileoutY,"%7.1f, %7.1f, %7.1f, %7.1f, %7.1f,",out->litm[i],out->litn[i],out->rmresp[i],out->rgresp[i],out->decresp[i]);//
		
		fprintf(fileoutY,"%7.2f, %7.2f, %7.2f,",out->fn2o[i]*1000,out->fno[i]*1000,out->fn2[i]*1000);   // N Gas
		fprintf(fileoutY,"%7.2f, %7.2f,",out->fn2ode[i]*1000,out->fnode[i]*1000);   // N Gas

		fprintf(fileoutY,"%7.2f, %7.2f,%7.2f,",out->v1[i],out->v2[i],out->v3[i]);   // spare variables for future use

		fprintf(fileoutY,"\n");

	}


	fclose(fileoutY);


}

void pnet_model::WriteoutDay(site_struct* site,veg_struct* veg,clim_struct* clim, share_struct* share,int rstep,FILE* &fileoutM)
{


	// write out header
	if (rstep==1)
	{
		char foutput[200] ;

		sprintf(foutput,"%s%s%s",exePath,PathOutSite,"Output_daily.csv");

		fileoutM = fopen(foutput, "w" );
		if (fileoutM==NULL)printf("Unable to open the Output_daily.csv !\n");

		fprintf(fileoutM," year,   doy,  temp,   GrowthDD,  "
			"prec,     vpd, snowpack,   ET,    Drain, Soilwater, "
			"DWater, WaterIx, Dwatertot,"
			"Foliage, LiveWood,  DeadWood,    "
			"Root,  PlantC,    BudC,     WoodC,    RootC, "
			"PosCBalMass, BalMassTot, BalMassIx, "
			"LAI,  LightEffMin, LightEffCBal, "
			"NetPsnMo, GrsPsnMo, NEE,    "
			"WoodMR,   WoodGR,   FolGR,   RootGR,   RootMR, SoilResp,  "
			"airT, NetNMinYr, NetNitrYr,NRatioNit,NDrain,NDrainMgL,FoliarN,"
			"N2O,     NO,      N2,  FluxN2OYrDe, FluxNOYrDe"
			"\n");

		fprintf(fileoutM,"   ,       ,    oC,      oC,       "
			"cm,      kpa,   cm,       cm,      cm,       cm,   "
			"Index,   Days,   total,   "
			"g/m2,     g/m2,     g/m2,      g/m2,   "
			"gC/m2,     gC/m2,    gC/m2,    gC/m2,    "
			"gC/m2,      gC/m2,     days,     "
			"m2/m2,        ,      gC/m2,    "
			"gC/m2,     gC/m2,    gC/m2,   "
			"gC/m2,    gC/m2,    gC/m2,    gC/m2,    gC/m2,    gC/m2, "
			"oC,gN/m2,gN/m2,gN/m2,gN/m2,mgN/l,%%,"
			"mgN/m2,mgN/m2,mgN/m2,mgN/m2,mgN/m2"  // Ngas
			"\n");

	}

	// write out monthly results
	fprintf(fileoutM,"%5d, %5d, %6.1f, %8.1f,",
		clim->year[rstep],
		clim->doy[rstep],
		share->Tave,
		share->GDDTot);  // sequence of date
	fprintf(fileoutM,"%8.2f, %7.2f, %7.2f, %7.2f, %7.2f,  %7.2f,",
		clim->prec[rstep],
		share->VPD,
		site->SnowPack,
		share->ET,
		share->Drainage,
		share->Water);// water ballance
	fprintf(fileoutM,"%6.2f, %7.1f, %7.1f,",
		share->DWater,
		share->DwaterIx,
		share->Dwatertot);// water effect
	fprintf(fileoutM,"%8.2f, %9.2f, %9.2f, %7.2f,",
		share->FolMass,
		share->WoodMass,
		share->DeadWoodM,
		share->RootMass);// Carbon pools
	fprintf(fileoutM,"%9.2f, %8.2f, %8.2f, %8.2f,",
		share->PlantC,
		share->BudC,
		share->WoodC,
		share->RootC);// Carbon pools
	fprintf(fileoutM,"%8.1f, %8.1f, %10.1f,",
		share->PosCBalMass,
		share->PosCBalMassTot,
		share->PosCBalMassIx);// Carbon pools
	fprintf(fileoutM,"%8.1f, %8.2f, %8.2f,",
		share->LAI,
		share->LightEffMin,
		share->LightEffCBal);// photosynthesis
	fprintf(fileoutM,"%8.2f, %8.2f, %8.2f, ",
		share->NetPsnMo,
		share->GrsPsnMo,
		-share->NetCBal);// photosynthesis
	fprintf(fileoutM,"%8.1f, %8.1f, %8.1f, %8.1f, %8.1f, %8.1f,",
		share->WoodMRespYr,
		share->WoodGRespYr,
		share->FolGRespYr,
		share->RootGRespYr,
		share->RootMRespYr,
		share->SoilDecRespYr);

	fprintf(fileoutM,"%10.2f,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f,",
			share->Tave,
			share->NetNMinYr,
			share->NetNitrYr,
			share->NRatioNit,
			share->NDrain,
			share->NDrainMgL);//share->WFPSshare->DayLength

	fprintf(fileoutM,"%7.3f,",
			veg->FolNCon);//Foliar N

	fprintf(fileoutM,"%8.2f, %8.2f, %8.2f,",
		share->FluxN2OYr*1000,
		share->FluxNOYr*1000,
		share->FluxN2Yr*1000);
	fprintf(fileoutM,"%8.2f, %8.2f",
//			share->CanopyDO3,
		share->FluxN2OYrDe*1000,
		share->FluxNOYrDe*1000);
//		share->CanopyGrossPsn);



	fprintf(fileoutM,"\n");

	if (rstep== clim->length) fclose(fileoutM);

}





int pnet_model:: Disturbance(site_struct* site, veg_struct* veg, share_struct* share, int scenario)
{

	int i;
//	int ndist;
//	ndist = 15;

	if (scenario == 0) // pre-define 15 disturbance events
	{
		site->distyrs = 15;
		//allocate memory for disturbance variables
		site->distintensity = (double*)malloc((site->distyrs+1)*sizeof(double)); //use site->distyrs+1 so that the subscript can start from 1
		site->distyear = (int*)malloc((site->distyrs+1)*sizeof(int));
		site->distremove  = (double*)malloc((site->distyrs+1)*sizeof(double));
		site->distsoilloss = (double*)malloc((site->distyrs+1)*sizeof(double));
		site->folregen = (double*)malloc((site->distyrs+1)*sizeof(double));

	}

	if(scenario == -99)  // skip  disturbance
	{
		site->distyrs = 0;
		site->agnum = 0;

	}

	if(scenario == 100)  // release memory
	{

		free(site->distintensity);
		free(site->distyear);
		free(site->distremove);
		free(site->distsoilloss);
		free(site->folregen);
		site->distyrs = 0;

	}


	if (scenario == 1) //HB
	{

		site->distyrs = 5;

		site->distyear[1] = 1850;
		site->distintensity[1] = 0.5;
		site->distremove[1] = 0.8;
		site->distsoilloss[1] =0.0;
		site->folregen[1]=100;

		site->distyear[2] = 1906;
		site->distintensity[2] = 0.2;
		site->distremove[2] = 0.8;
		site->distsoilloss[2] =0.0;
		site->folregen[2]=100;

		site->distyear[3] = 1917;
		site->distintensity[3] = 0.6;
		site->distremove[3] = 0.8;
		site->distsoilloss[3] =0.0;
		site->folregen[3]=100;

		site->distyear[4] = 1938;
		site->distintensity[4] = 0.2;
		site->distremove[4] = 0.4;
		site->distsoilloss[4] =0.0;
		site->folregen[4]=100;

//		site->distyear[5] = 1998;
//		site->distintensity[5] = 0.1;
//		site->distremove[5] = 0.;
//		site->distsoilloss[5] =0.0;
//		site->folregen[5]=100;

	}




	if (scenario == 2) //HF
	{

		site->distyrs = 5;

		site->distyear[1] = 1750; // vegatation removal
		site->distintensity[1] = 0.9;
		site->distremove[1] = 0.6;
		site->distsoilloss[1] =0.0;
		site->folregen[1]=100;

		site->distyear[2] = 1938; //hurricane salvage
		site->distintensity[2] = 0.2;
		site->distremove[2] = 0.4;
		site->distsoilloss[2] =0.0;
		site->folregen[2]=100;


		site->distyear[3] = 1945;  //timber harvest
		site->distintensity[3] = 0.8;
		site->distremove[3] = 0.8;
		site->distsoilloss[3] =0.0;
		site->folregen[3]=100;

//		site->distyear[4] = 1998;  // ice storm
//		site->distintensity[4] = 0.1;
//		site->distremove[4] = 0.;
//		site->distsoilloss[4] =0.0;
//		site->folregen[4]=200;

		site->agnum = 1;
		site->agstart = 1750;		// agriculture starts
		site->agstop = 1850;		// agriculture abandons
		site->agrem = 0.05;   // yearly loss

// assuming no oi+oe removal. could be 0.05




		}




	return 0;
}





