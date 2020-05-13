#include "pnet_model.h"

void pnet_model::pnet_daily()
{

	char climname[200];
	char inputname[200];

	char climdayfile[200];	// for daily climate input
	char climhrfile[200] ;	// for hourly climate input to compare tower flux

	int CN_Mode = 1; //if running PnET-CN
	int i, rstep,nyrs,ndays;
	int ystep = 1; //unlike matlab code, ystep is initialized here instead of in initvars.c 
	int monspin;
	int doyEnd;  // end of year for doy, leap year:366, otherwise 365
	int YrStartSpin,YrEndSpin; // years of starting and end for spinning up
	

	veg_struct veg;				//structure to hold veg data
	site_struct site;			//structure to hold the site data
	clim_struct clim;			//structure to hold the input climate data
	out_struct out;				//structure to hold the output data
	share_struct share;			//structure to hold share data

	clim_struct climmon,climday,climhr,climdayspin;			//structure to hold the input climate data,monthly,daily,hourly

	FILE* fileoutM;		// output monthly result
	FILE* fileoutD;		// output daily result

	sprintf(inputname,"%s%s%s",exePath,PathInput,"input.txt");  // path of input files

	sprintf(climname,"%s%s%s",exePath,PathInput,"climate.clim"); //monthly climate, historical climate for spin up

	sprintf(climdayfile,"%s%s%s",exePath,PathInput,"climateday.clim");//daily climate
	sprintf(climdayfile,"%s%s%s",exePath,PathInput,"climateday_afshin.clim");//daily climate

//	sprintf(climhrfile,"%s%s%s",exePath,PathInput,"climatehour.clim");  // hourly input climate



	clim.timestep = 0;		//  monthly climate
	climday.timestep = 1;	// daily climate
	climdayspin.timestep = 1;	// daily climate


	// read in input data from input files

	ReadInput(&site,&veg,&share,inputname);

	ReadClimDay(&climdayspin, climdayfile);	// read daily climate

//	ReadClimDay(&climday, climdayfile);	// read daily climate

//	ReadClimHour(&climhr, climhrfile);	// read hourly climate




	nyrs = site.nYearEnd-site.nYearStart + 1;		// years of daily climate records

	//Initialize variables

	initvars(&site,&veg, &share);


	YrStartSpin = 1700;
	YrEndSpin = 2008;


	share.yrspin = YrEndSpin-YrStartSpin +1;		//actual number of year for spinning up
	climday.length = share.yrspin *365;     // 365 days for each year
	memset_climate(&climday);  // clim for spin up


	//allocate memory
	memset_out(share.yrspin+nyrs, &out);

	// pre-process of climate to generate monthly climate

	clim.length = share.yrspin*12;
	memset_climate(&clim);  // clim for spin up

/*
	ReadClim(&climmon, climname);

*/


	clim_day_fill(&site,&share,&climdayspin,&climday);
//	clim_day_mon(&site,&share,&climday,&clim);  // calculate monthly input based on daily input climate
	clim_day_mon(&site,&share,&climdayspin,&clim);  // calculate monthly input based on daily input climate
//		ReadClim(&clim, climname);			// read monthly climate, instead of calculated based on daily

		write_clim(&climday,0);
		write_clim(&clim,1);

		share.ifO3EffectOnPSN= 1;  // O3 effect
//		share.ifO3EffectOnPSN= 0;  // O3 effect

		share.kO3EffScalar = 1.12;
		share.kO3Eff = share.kO3Eff * share.kO3EffScalar; // 1.12;daily version
		share.WUEO3Eff =1;

	 // ============disturbance=============================================

//		 Disturbance(&site, &veg, &share,0);  // to allocate memory for disturbances
//		 Disturbance(&site, &veg, &share,-99);	//  skip any disturbance.
//		 Disturbance(&site, &veg, &share,share.distid);	// default disturbance



	// print info on the screen

	printf("             PnET model starts running\n");
	printf("   ===============================================\n\n");
	printf("             PnET model starts spinning up\n");


	//Main run loop to spin-up

//	monspin = share.yrspin*12;  //// control whether monthly version runs
	monspin = (share.yrspin-nyrs)*12; // ends up at the beginning year of daily records
	monspin = 0; // skip monthly spinup


	for (rstep = 1; rstep <= monspin; rstep++)
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
//		storeoutput(&veg, &share, &out, rstep, &ystep, 0);
//		WriteoutMo(&site,&veg,&clim, &share,rstep,fileoutM);

		//End-of-year activity
		if (clim.doy[rstep]>335)
		{
			AllocateYr(&site, &veg, &clim, rstep+1, &share, CN_Mode); // Note the rstep+1, not rstep
			storeoutput(&veg, &share, &out, rstep+1, &ystep, 1);
			YearInit(&share);
		}

		printf("       Executing year %d  doy  %d  \n",clim.year[rstep],clim.doy[rstep]);


	}// end of the spinup loop



	printf("   ===============================================\n\n");
	printf("             PnET model ends spinning up\n");

	printf("   ===============================================\n\n");
	printf("             PnET model starts daily run\n");


//	ystep = 1;
//	ndays = 0;  // control whether daily version runs or not
	ndays = climday.length;
	veg.AmaxFrac =0.89 ;  // based on our previous analysis
	//veg.FolRespFrac = 1.14 ;  // based on our previous analysis

	for (rstep = 1; rstep <= ndays; rstep++)
	{

		//veg.FolNCon = 2.4;
		// Call subroutines

//		climday.O3[rstep] = climday.O3[rstep] *5;


		AtmEnviron(&site, &climday, rstep, &share);
		Phenology(&veg, &climday, rstep, &share, 1);
		Photosyn(&site, &veg, &climday, rstep, &share);
		Waterbal(&site, &veg, &climday, rstep, &share);
		AllocateMo(&veg, &share, rstep, CN_Mode);
		Phenology(&veg, &climday, rstep, &share, 2);
		CNTrans(&site, &veg, &climday, rstep, &share);
		Decomp(&site,&veg, &climday, rstep, &share);
		Leach(&share);
//		storeoutput(&veg, &share, &out, rstep, &ystep, 0);
		WriteoutDay(&site,&veg,&climday, &share,rstep,fileoutD);  // it is different from the monthly version

		//End-of-year activity

		if (climday.doy[rstep]>=365)
		{
			if (is_leapyear(climday.year[rstep]))
			{
				doyEnd = 366;
				doyEnd = 365;  // all years have 365 days
			} //leap year
			else doyEnd = 365;
			if  (climday.doy[rstep] == doyEnd )
			{

				AllocateYr(&site, &veg, &climday, rstep+1, &share, CN_Mode); // Note the rstep+1, not rstep
				storeoutput(&veg, &share, &out, rstep+1, &ystep, 1);
				YearInit(&share);
			}
		}

		printf("       Executing year %d  doy  %d  \n",climday.year[rstep],climday.doy[rstep]);


	}// end of the run loop


	// write out annual results
//	WriteoutYr(&clim,&out);
	WriteoutYr(&climday,&out);

	//free memory and close files
//	pnet_memfree(&site,&clim, &share, &out);
	memfree_all(&site,&clim, &share, &out);

	printf("   ===============================================\n");

}


void pnet_model::Photosyn_hour(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share)
{
	//
	// Photosynthesis routine for the PnET ecosystem model.
	//

	//global veg site clim share rstep;

	int  ix;
	double i;
	double PsnTMax, DTemp, ten9, Ca, CiCaRatio, Ci350, CiElev, Arel350, ArelElev, Delgs, gsSlope, gsInt, Amax, BaseFolResp; 
	double GrossAmax, CanopyNetPsnO3, O3Prof, Layer, SLWLayer, Il, LightEff, LayerGrossPsnRate, LayerGrossPsn;
	double LayerResp, LayerNetPsn, netPsnumol, RelLayer, RelO3, LayerG, LayerDO3, LayerNetPsnO3, CanopyNetPsnPot;

	PsnTMax = veg->PsnTOpt + (veg->PsnTOpt - veg->PsnTMin);
	//DTemp = ((PsnTMax - share->Tday) * (share->Tday - veg->PsnTMin)) / (power(((PsnTMax - veg->PsnTMin) / 2.0),2));
	DTemp = ((PsnTMax - share->Tday) * (share->Tday - veg->PsnTMin)) / (pow(((PsnTMax - veg->PsnTMin) / 2.0),2)); //Matlab to C
	if ((clim->tmin[rstep] < 6)&&(DTemp > 0)&&(share->GDDTot >= veg->GDDFolEnd))
	{
		//Frost effect
		DTemp = DTemp * (1.0 - ((6.0 - clim->tmin[rstep]) / 6.0) * (share->dayspan / 30.0));
	}
	//DTemp = max(DTemp, 0);
	if (DTemp < 0)
	{
		DTemp = 0;
	} //Matlab to C
	//share->DVPD = 1.0 - veg->DVPD1 * (power(share->VPD,veg->DVPD2));
	share->DVPD = 1.0 - veg->DVPD1 * (pow(share->VPD,veg->DVPD2)); //Matlab to C
	ten9 = 1000000000.0;

	//Set atmospheric CO2 concentration
	Ca = clim->CO2[rstep];

	//CO2 effect on photosynthesis
	//Leaf internal/external CO2
	CiCaRatio = (-0.075 * veg->FolNCon) + 0.875;
	//Ci at present (350 ppm) CO2
	Ci350 = 350 * CiCaRatio;
	//Ci at RealYear CO2 level
//	Ca=700;
	CiElev = Ca * CiCaRatio;

	//Areal - rate of photosynthesis at a given atmospheric CO2
	//concentration (Ca) relative to that which occurs at CO2 saturation
	Arel350 = 1.22 * ((Ci350 - 68) / (Ci350 + 136));
	ArelElev = 1.22 * ((CiElev - 68) / (CiElev + 136));
	share->DelAmax = 1 + ((ArelElev - Arel350) / Arel350);

	//Calculate CO2 effect on conductance and set slope and intercept for A-gs relationship
	if (site->CO2gsEffect == 1)
	{
		Delgs = share->DelAmax / ((Ca - CiElev) / (350 - Ci350));
		share->DWUE = 1 + (1 - Delgs);					// used for effect on water use efficiency
		gsSlope = (-1.1309 * share->DelAmax) + 1.9762;  //used to determine conductance and then ozone uptake
		gsInt = (0.4656 * share->DelAmax) - 0.9701;
	}
	else
	{
		share->DWUE = 1;
		gsSlope = (-0.6157 * share->DelAmax) + 1.4582;
		gsInt = (0.4974 * share->DelAmax) - 0.9893;
	}



	Amax = (veg->AmaxA + veg->AmaxB * veg->FolNCon) * share->DelAmax;  //nmole CO2/g Fol.sec

	BaseFolResp = veg->BaseFolRespFrac * Amax;
	Amax = Amax * veg->AmaxFrac;
	GrossAmax = Amax + BaseFolResp;
	GrossAmax = (GrossAmax * share->DVPD * DTemp * share->DayLength * 12.0) / ten9;  //g C/g Fol/day, 12 for C atom 

	if (GrossAmax < 0)
	{
		 GrossAmax = 0;
	}
	//share->DayResp = (BaseFolResp * (power(veg->RespQ10,((share->Tday - veg->PsnTOpt) / 10.0))) * share->DayLength * 12.0) / ten9;
	share->DayResp = (BaseFolResp * (pow(veg->RespQ10,((share->Tday - veg->PsnTOpt) / 10.0))) * share->DayLength * 12.0) / ten9; //Matlab to C
	//share->NightResp = (BaseFolResp * (power(veg->RespQ10,((share->Tnight - veg->PsnTOpt) / 10.0))) * share->NightLength * 12.0) / ten9;
	share->NightResp = (BaseFolResp * (pow(veg->RespQ10,((share->Tnight - veg->PsnTOpt) / 10.0))) * share->NightLength * 12.0) / ten9; //Matlab to C   g C/g Fol/day

	//Initialize ozone effect
	CanopyNetPsnO3 = 0;
	CanopyNetPsnPot = 0;

	//Calculate canopy ozone extinction based on folmass
	O3Prof = 0.6163 + (0.00105 * share->FolMass);




//===================================


	char outputtest[200] ;
	FILE* fileout;

	sprintf(outputtest,"mon.csv");// timestep,

	fileout = fopen(outputtest, "a" );
	if (fileout==NULL)
	{
		printf("Unable to open the file !\n");
		exit(1) ;	
	}





///============================================================







	share->LightEffCBal=0.0;
	if (share->FolMass > 0)
	{
		share->CanopyNetPsn = 0;
		share->CanopyGrossPsn = 0;
		share->LAI = 0;
		share->PosCBalMass = share->FolMass;  // posCBalMass: possible C Mass at balance point 
//		O3Effect = 0;
		Layer = 0; 
		
		  
		for (ix = 1; ix <= 50; ix++)
		{
			i = (double)ix * (share->FolMass / 50.0);				//SLW for this layer
			SLWLayer = veg->SLWmax - (veg->SLWdel * i);
			share->LAI = share->LAI + (share->FolMass / 50.0) / SLWLayer;
			Il = clim->par[rstep] * exp(-veg->k * share->LAI);
			LightEff = (1.0 - exp(-Il * log(2.0) / veg->HalfSat));
			LayerGrossPsnRate = GrossAmax * LightEff;					//gC/g Fol/day
			LayerGrossPsn = LayerGrossPsnRate * (share->FolMass / 50.0);   //g C/m2 ground/day
			LayerResp = (share->DayResp + share->NightResp) * (share->FolMass / 50.0);  
			LayerNetPsn = LayerGrossPsn - LayerResp;
			if ((LayerNetPsn < 0)&&(share->PosCBalMass == share->FolMass))
			{
				share->PosCBalMass = (ix - 1.0) * (share->FolMass / 50.0);
				share->LightEffCBal = LightEff;
			}
			share->CanopyNetPsn = share->CanopyNetPsn + LayerNetPsn;
			share->CanopyGrossPsn = share->CanopyGrossPsn + LayerGrossPsn;
	    
			//Ozone effects on Net Psn
			//clim->O3[rstep]=40;  //zzx
			if (clim->O3[rstep]>0)
			{
				// Convert netpsn to micromoles for calculating conductance
				//netPsnumol = ((LayerNetPsn * 10 ^ 6) / (share->DayLength * 12)) / ((share->FolMass / 50) / SLWLayer);
				netPsnumol = ((LayerNetPsn * 1000000) / (share->DayLength * 12)) / ((share->FolMass / 50) / SLWLayer); //Matlab to C
				//Calculate ozone extinction throughout the canopy
				Layer = Layer + 1;
				RelLayer = Layer / 50;
				//RelO3 = 1 - (RelLayer * O3Prof) ^ 3;
				RelO3 = 1 - (RelLayer * O3Prof) * (RelLayer * O3Prof) * (RelLayer * O3Prof); //Matlab to C
				if (RelO3<0) RelO3=0;
				//Calculate Conductance (mm/s): Conductance down-regulates with prior O3 effects on Psn
				LayerG = (gsInt + (gsSlope * netPsnumol)) * (1 - share->O3Effect[(int)Layer]);
				//For no downregulation use:    LayerG = gsInt + (gsSlope * netPsnumol);
				if (LayerG < 0)
				{
					LayerG = 0;
				}
				//Calculate cumulative ozone effect for each canopy layer with consideration that previous O3 effects were modified by drought
				share->O3Effect[(int)Layer] = (share->O3Effect[(int)Layer] * share->DroughtO3Frac) + (0.0026 * LayerG * clim->O3[rstep] * RelO3);// 
				if (share->O3Effect[(int)Layer]>1.0)share->O3Effect[(int)Layer]=1.0;
				LayerDO3 = 1 - share->O3Effect[(int)Layer];
			}
			else
			{
				LayerDO3 = 1;
			}

			LayerNetPsnO3 = LayerNetPsn * LayerDO3;
			CanopyNetPsnO3 = CanopyNetPsnO3 + LayerNetPsnO3;
		}// ix=51

		if ((DTemp > 0)&&(share->GDDTot > veg->GDDFolEnd)&&(clim->doy[rstep] < veg->SenescStart))
		{
			share->PosCBalMassTot = share->PosCBalMassTot + (share->PosCBalMass * share->dayspan);
			share->PosCBalMassIx = share->PosCBalMassIx + share->dayspan;
		}
		if (share->LightEffMin > LightEff)
		{
			share->LightEffMin = LightEff;
		}
	}
	else
	{
		share->PosCBalMass = 0;
		share->CanopyNetPsn = 0;
		share->CanopyGrossPsn = 0;
		share->LAI = 0;
		share->DayResp = 0;
		share->NightResp = 0;
	}

	//Calculate whole-canopy ozone effects before drought
	if ((clim->O3[rstep]>0)&&(share->CanopyGrossPsn>0))
	{
		CanopyNetPsnPot = share->CanopyGrossPsn - (share->DayResp * share->FolMass) - (share->NightResp * share->FolMass);
		share->CanopyDO3Pot = CanopyNetPsnO3 / CanopyNetPsnPot;
	}
	else
	{
		share->CanopyDO3Pot = 1;
	}


		fprintf(fileout,
			"%4d,%5.1f,%8.3f"
//			"%8.3f,%8.3f"
			"\n",
			clim->doy[rstep],clim->hour[rstep],share->CanopyGrossPsn
//			share->PosCBalMass,share->LightEffMin
			);// C ballance

		fclose(fileout);
		




}






void pnet_model::ReadClimDay(clim_struct* clim, char* climname)
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
	

	//allocate memory for the climate structure  // zzx rearrange the code
	memset_climate(clim);


	fgets(buf, 200, fileClim);

	for (i = 1; i <= clim->length; i++)
	{
		fscanf(fileClim, "%d ", &clim->year[i]); //year
		fscanf(fileClim, "%d", &clim->month[i]); //mon
		fscanf(fileClim, "%d", &clim->day[i]); //day
		fscanf(fileClim, "%d", &clim->doy[i]); //doy
		fscanf(fileClim, "%lf",&clim->tmax[i]); //tmax
		fscanf(fileClim, "%lf",&clim->tmin[i]); //tmin
		fscanf(fileClim, "%lf",&clim->par[i]); //par
		fscanf(fileClim, "%lf",&clim->prec[i]); //prec
		fscanf(fileClim, "%lf",&clim->O3[i]); //O3
		fscanf(fileClim, "%lf",&clim->CO2[i]); //CO2
		fscanf(fileClim, "%lf",&clim->NH4dep[i]); //NH4dep
		fscanf(fileClim, "%lf",&clim->NO3dep[i]); //NO3dep
//		fscanf(fileClim, "%lf",&clim->tsoil[i]); //NO3dep

	}
	fclose(fileClim);
}



void pnet_model::ReadClimHour(clim_struct* clim, char* climname)
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
	

	//allocate memory for the climate structure  // zzx rearrange the code
	clim->year = (int*)malloc((clim->length+1)*sizeof(int)); //use clim->length+1 so that rstep can start from 1
	clim->month = (int*)malloc((clim->length+1)*sizeof(int)); //use clim->length+1 so that rstep can start from 1
	clim->day = (int*)malloc((clim->length+1)*sizeof(int)); //use clim->length+1 so that rstep can start from 1
	clim->hour = (double*)malloc((clim->length+1)*sizeof(double));

	clim->doy = (int*)malloc((clim->length+1)*sizeof(int));
//	clim->ifday = (int*)malloc((clim->length+1)*sizeof(int));

	clim->tmax = (double*)malloc((clim->length+1)*sizeof(double));
	clim->tmin = (double*)malloc((clim->length+1)*sizeof(double));
//	clim->tmean = (double*)malloc((clim->length+1)*sizeof(double));
//	clim->vpd = (double*)malloc((clim->length+1)*sizeof(double));

	clim->par = (double*)malloc((clim->length+1)*sizeof(double));
	clim->prec = (double*)malloc((clim->length+1)*sizeof(double));
	clim->O3 = (double*)malloc((clim->length+1)*sizeof(double));
	clim->CO2 = (double*)malloc((clim->length+1)*sizeof(double));
	clim->NH4dep = (double*)malloc((clim->length+1)*sizeof(double));
	clim->NO3dep = (double*)malloc((clim->length+1)*sizeof(double));
	if (!clim->year || !clim->doy || !clim->tmax || !clim->tmin || !clim->par || !clim->prec || !clim->O3 || !clim->CO2 || !clim->NH4dep || !clim->NO3dep)
	{
		printf("Unable to allocate memory for clim_struct!\n");
		exit(1) ;
	}


	fgets(buf, 200, fileClim);

	for (i = 1; i <= clim->length; i++)
	{
		fscanf(fileClim, "%d",&clim->year[i]); //year
		fscanf(fileClim, "%d", &clim->month[i]); //doy
		fscanf(fileClim, "%d", &clim->day[i]); //doy
		fscanf(fileClim, "%lf", &clim->hour[i]); //doy
		fscanf(fileClim, "%d", &clim->doy[i]); //doy
//		fscanf(fileClim, "%d",&clim->ifday[i]); //doy
//		fscanf(fileClim, "%lf",&clim->tmean[i]); //tmax
//		fscanf(fileClim, "%lf",&clim->vpd[i]); //tmin
		fscanf(fileClim, "%lf",&clim->par[i]); //par
		fscanf(fileClim, "%lf",&clim->prec[i]); //prec
		fscanf(fileClim, "%lf",&clim->O3[i]); //O3
		fscanf(fileClim, "%lf",&clim->CO2[i]); //CO2
		fscanf(fileClim, "%lf",&clim->NH4dep[i]); //NH4dep
		fscanf(fileClim, "%lf\n",&clim->NO3dep[i]); //NO3dep
/**/

	}
	fclose(fileClim);
}

void pnet_model::clim_day_mon(site_struct* site,share_struct* share,clim_struct* climday,clim_struct* clim)
{

	int i,j,jj;
	int doy[13]={0,15,46,74,105,135,166,196,227,258,288,319,349};
	double m_tmax,m_tmin,m_preci,m_par,m_co2,m_nh4,m_no3,m_o3;
	int month;

	j=clim->length;
	jj=0;
	month = 12 ;
	m_tmax =0;
	m_tmin =0;
	m_preci =0.0;
	m_par = 0.0;
	m_co2 = 0.0;
	m_nh4=0.0;
	m_no3 = 0.0;
	m_o3 = 0.0;

	for (i = climday->length; i >0; i--)
	{
		if (climday->month[i] != month || i == 1 )
		{

			if ( i==1)
			{
				jj++;
				m_tmax += climday->tmax[i];
				m_tmin += climday->tmin[i];
				m_par += climday->par[i];
				m_preci += climday->prec[i];
				m_o3 += climday->O3[i];
				m_co2 += climday->CO2[i];
				m_nh4 += climday->NH4dep[i];
				m_no3 += climday->NO3dep[i];

			}


			m_tmax = m_tmax/jj;
			m_tmin = m_tmin/jj;
			m_par = m_par/jj;
		//	m_o3 = m_o3;
			m_co2 = m_co2/jj;


			clim->year[j]=climday->year[i+1];
			clim->doy[j]=doy[month];
			clim->tmin [j]= m_tmin;
			clim->tmax [j]= m_tmax;
			clim->par[j] = m_par;
			clim->prec [j]= m_preci; //

			clim->O3[j] = m_o3;
			clim->CO2[j] = m_co2;
			clim->NH4dep[j] = m_nh4;
			clim->NO3dep[j] = m_no3;

			m_tmax =0;
			m_tmin =0;
			m_preci =0.0;
			m_par = 0.0;

			m_co2 = 0.0;
			m_nh4=0.0;
			m_no3 = 0.0;
			m_o3 = 0.0;

			month = climday->month[i];  // previous month

			j--;	// next monthly record

			m_tmax += climday->tmax[i];
			m_tmin += climday->tmin[i];
			m_par += climday->par[i];
			m_preci += climday->prec[i];
			m_o3 += climday->O3[i];
			m_co2 += climday->CO2[i];
			m_nh4 += climday->NH4dep[i];
			m_no3 += climday->NO3dep[i];

			jj=1;  // reinitialization


		}

		else
		{
			m_tmax += climday->tmax[i];
			m_tmin += climday->tmin[i];
			m_par += climday->par[i];
			m_preci += climday->prec[i];
			m_o3 += climday->O3[i];
			m_co2 += climday->CO2[i];
			m_nh4 += climday->NH4dep[i];
			m_no3 += climday->NO3dep[i];
			jj++;
		}


	}  // end of record loop


	//fill clim
	month = (climday->year[climday->length]-climday->year[1]+1) * 12;  // number of month with observations
	for (j=clim->length - month; j>0; j--)
	{
		clim->year[j]=clim->year[j+12]-1;
		clim->doy[j]=clim->doy[j+month];
		clim->tmin [j]= clim->tmin[j+month];
		clim->tmax [j]= clim->tmax[j+month];
		clim->par[j] = clim->par[j+month];
		clim->prec [j]= clim->prec[j+month]; //

		clim->O3[j] = clim->O3[j+month];
		clim->CO2[j] = 282.23 + exp(clim->year[j]/51.35)*1.03*1.0e-15; //(Franks,2013, New Phytologist, 197:1077-1094);
	//	clim->NH4dep[j] = clim->NH4dep[j+month];
	//	clim->NO3dep[j] = clim->NO3dep[j+month];

		if (clim->year[j]<1860)clim->NH4dep[j] = 0.00928;
		else clim->NH4dep[j] = (4.1176*clim->year[j]- 7508.8)/1000/12*0.742;
		clim->NO3dep[j] = clim->NH4dep[j] * 0.34771;


	}


}


void pnet_model::clim_day_fill(site_struct* site,share_struct* share,clim_struct* climday,clim_struct* clim)
{

	int i,j,jj;

	//fill clim
	i = clim->length - climday->length;

	for (j=i+1; j<=clim->length; j++)
	{
		clim->year[j]=climday->year[j-i];
		clim->doy[j]=climday->doy[j-i];
		clim->month[j]=climday->month[j-i];
		clim->tmin [j]= climday->tmin[j-i];
		clim->tmax [j]= climday->tmax[j-i];
		clim->par[j] = climday->par[j-i];
		clim->prec [j]= climday->prec[j-i]; //

		clim->O3[j] = climday->O3[j-i];
		clim->CO2[j] = climday->CO2[j-i];
		clim->NH4dep[j] = climday->NH4dep[j-i];
		clim->NO3dep[j] = climday->NO3dep[j-i];

	//	clim->CO2[j] = 282.23 + exp(clim->year[j]/51.35)*1.03*1.0e-15; //(Franks,2013, New Phytologist, 197:1077-1094);
	//	clim->NH4dep[j] = clim->NH4dep[j+month];
	//	clim->NO3dep[j] = clim->NO3dep[j+month];

	//	if (clim->year[j]<1860)clim->NH4dep[j] = 0.00928;
	//	else clim->NH4dep[j] = (4.1176*clim->year[j]- 7508.8)/1000/12*0.742;
	//	clim->NO3dep[j] = clim->NH4dep[j] * 0.34771;


	}

	for (j=i; j>0; j--)
	{
		clim->year[j]=clim->year[j+365]-1;
		clim->doy[j]=clim->doy[j+climday->length];
		clim->month[j]=clim->month[j+climday->length];
		clim->tmin [j]= clim->tmin[j+climday->length];
		clim->tmax [j]= clim->tmax[j+climday->length];
		clim->par[j] = clim->par[j+climday->length];
		clim->prec [j]= clim->prec[j+climday->length]; //

		clim->O3[j] = clim->O3[j+climday->length];
		clim->CO2[j] = 282.23 + exp(clim->year[j]/51.35)*1.03*1.0e-15; //(Franks,2013, New Phytologist, 197:1077-1094);

		if (clim->year[j]<1860)clim->NH4dep[j] = 0.00928/30.0;
		else clim->NH4dep[j] = (4.1176*clim->year[j]- 7508.8)/1000/12*0.742/30.0;
		clim->NO3dep[j] = clim->NH4dep[j] * 0.34771;


	}

//	write_clim(clim,0);

}



void pnet_model::write_clim(clim_struct* clim,int i)
{

	int j;
	char climname[200];
	FILE *outfile;  // test zzx
	sprintf(climname,"%s%sclim_print_%d.csv",exePath,PathInput,i);
	outfile = fopen(climname, "w");

	fprintf(outfile,"Year,DOY,MaxT(oc),MinT(oc),PAR(umol/m2/s),Precip(cm),O3,CO2(ppm),NH4(gN/m2*yr),NO3(gN/m2*yr)\n");

	for (j=1; j<=clim->length; j++)

	{

		fprintf(outfile,"%6d,%6d",clim->year[j],clim->doy[j]);
		fprintf(outfile,",%8.3lf,%8.3lf,%8.3lf,%8.3lf,%9.3lf,%9.3lf,%9.6lf,%9.6lf",
				clim->tmax[j],clim->tmin[j],clim->par[j],clim->prec[j],clim->O3[j],clim->CO2[j],
				clim->NH4dep[j],clim->NO3dep[j]);
		fprintf(outfile,"\n");
	}


	fclose(outfile);


}

