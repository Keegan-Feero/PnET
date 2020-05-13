#include "pnet_model.h"

void pnet_model::Photosyn(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share)
{
	//
	// Photosynthesis routine for the PnET ecosystem model.
	//


	int  ix;
	double i;
	double PsnTMax, DTemp, ten9, Ca, CiCaRatio, Ci350, CiElev, Arel350, ArelElev, Delgs, gsSlope, gsInt, Amax, BaseFolResp; 
	double GrossAmax, CanopyNetPsnO3, O3Prof, Layer, SLWLayer, Il, LightEff, LayerGrossPsnRate, LayerGrossPsn;
	double LayerResp, LayerNetPsn, netPsnumol, RelLayer, RelO3, LayerG, LayerDO3, LayerNetPsnO3, CanopyNetPsnPot;
	double Gamma;  // ppm;CO2 compensation in the absence of dark respiration, influenced by leaf temperature
	double Ca0;		// ppm; 350;reference air CO2 concentration

	int nLayers;  // canopy layer number
	double iFolMass;  // foliage mass per layer
	double LightEffBttm;// light effect at the canopy bottom
	double LAIBttm; //actual LAI in tha canopy

	PsnTMax = veg->PsnTOpt + (veg->PsnTOpt - veg->PsnTMin);
	DTemp = ((PsnTMax - share->Tday) * (share->Tday - veg->PsnTMin)) / (pow(((PsnTMax - veg->PsnTMin) / 2.0),2));

	if ((clim->tmin[rstep] < 6) && (DTemp > 0) && (share->GDDTot >= veg->GDDFolEnd))
	{
		//Frost effect
		DTemp = DTemp * (1.0 - ((6.0 - clim->tmin[rstep]) / 6.0) * (share->dayspan / 30.0));  // need to verify zzx
	}

	if (DTemp < 0)
	{
		DTemp = 0;
	}

	share->DVPD = 1.0 - veg->DVPD1 * (pow(share->VPD,veg->DVPD2));
	ten9 = 1000000000.0;

	//Set atmospheric CO2 concentration
	Ca = clim->CO2[rstep];

	//CO2 effect on photosynthesis
	//Leaf internal/external CO2
	CiCaRatio = (-0.075 * veg->FolNCon) + 0.875;
	//Ci at present (350 ppm) CO2
	Ci350 = 350 * CiCaRatio;
	//Ci at RealYear CO2 level

	CiElev = Ca * CiCaRatio;

	//Areal - rate of photosynthesis at a given atmospheric CO2
	//concentration (Ca) relative to that which occurs at CO2 saturation
	Arel350 = 1.22 * ((Ci350 - 68) / (Ci350 + 136));
	ArelElev = 1.22 * ((CiElev - 68) / (CiElev + 136));
	share->DelAmax = 1 + ((ArelElev - Arel350) / Arel350);


	Gamma = 40; // 40; assume a mean leaf temperature during psn of 25 oC
	Ca0 = 350;  // 350
//	share->DelAmax = (Ca - Gamma) /(Ca + 2 *Gamma)*(Ca0 + 2 *Gamma)/(Ca0 - Gamma);  // (Franks,2013, New Phytologist, 197:1077-1094)



	//Calculate CO2 effect on conductance and set slope and intercept for A-gs relationship
	if (site->CO2gsEffect == 1)
	{
		Delgs = share->DelAmax / ((Ca - CiElev) / (350 - Ci350));
		share->DWUE = 1 + (1 - Delgs);					// used for effect on water use efficiency
		gsSlope = (-1.1309 * share->DelAmax) + 1.9762;  //used to determine conductance and then ozone uptake
		gsInt = (0.4656 * share->DelAmax) - 0.9701;

		//		Delgs = share->DelAmax / (Ca / Ca0);
		//		share->DWUE = 1/Delgs;	// (Franks,2013, New Phytologist, 197:1077-1094) need to refine with the downregulation


	}
	else
	{
		share->DWUE = 1;
		gsSlope = (-0.6157 * share->DelAmax) + 1.4582;
		gsInt = (0.4974 * share->DelAmax) - 0.9893;
	}


	double fAmax = 1.0;

//	fAmax = 1.46; // hw

	Amax = (veg->AmaxA + veg->AmaxB * veg->FolNCon) * share->DelAmax * fAmax;  //nmole CO2/g Fol.sec
//	Amax = (veg->AmaxA + veg->AmaxB * 1.8) * share->DelAmax * fAmax;  //nmole CO2/g Fol.sec
	//	Amax = pow(10.0,(0.74*log10(veg->FolNCon)-0.57*log10(SLWLayer)+2.96)) * share->DelAmax;  //nmole CO2/g Fol.sec
	//	veg->BaseFolRespFrac = 0.14 - 0.002 * share->Tave;		// Longterm leaf respiration acclimation (Wythers,2013,JGR,V118, 1?4,)
	//	veg->RespQ10 = 3.22 - 0.046 * (share->Tave+veg->PsnTOpt)/2.; // Longterm leaf respiration acclimation (Wythers,2013,JGR,V118, 1?4,)
	//veg->BaseFolRespFrac = 0.09;
	BaseFolResp = veg->BaseFolRespFrac * Amax * veg->FolRespFrac;
	Amax = Amax * veg->AmaxFrac;
	GrossAmax = Amax + BaseFolResp;
	GrossAmax = (GrossAmax * share->DVPD * DTemp * share->DayLength * 12.0) / ten9;  //g C/g Fol/day, 12 for C atom 

	if (GrossAmax < 0)
	{
		 GrossAmax = 0;
	}

	share->DayResp = ( BaseFolResp * (pow(veg->RespQ10,((share->Tday - veg->PsnTOpt) / 10.0))) * share->DayLength * 12.0) / ten9;
	share->NightResp = (BaseFolResp * (pow(veg->RespQ10,((share->Tnight - veg->PsnTOpt) / 10.0))) * share->NightLength * 12.0) / ten9; //   g C/g Fol/day

	//Initialize ozone effect
	CanopyNetPsnO3 = 0;
	CanopyNetPsnPot = 0;

	//Calculate canopy ozone extinction based on folmass
	O3Prof = 0.6163 + (0.00105 * share->FolMass);
	if (share->ifO3EffectOnPSN == 1)
	{
		if (clim->year[rstep]>=site->nYearStart )share->O3EffectOnPSN= 1;
		else share->O3EffectOnPSN= 0;

	}
	else
	{
		share->O3EffectOnPSN= 0;
	}



	share->LightEffCBal=0.0;
	nLayers = 50;
	if (share->FolMass > 0)
	{
		share->CanopyNetPsn = 0;
		share->CanopyGrossPsn = 0;
		share->LAI = 0;
		share->PosCBalMass = share->FolMass;  // posCBalMass: possible C Mass at balance point 
//		O3Effect = 0;
		Layer = 0; 
		share->LightEffCBal = 0;
		iFolMass = share->FolMass / (double)nLayers;
		ix = 0;  //

		//for (ix = 1; ix <= 50; ix++)
		while(1)
		{

			ix++;
			i = (double)ix * iFolMass;
			SLWLayer = veg->SLWmax - (veg->SLWdel * i);//SLW for this layer
			if (SLWLayer<20)SLWLayer = 20.0;  // need to update
			share->LAI = share->LAI + iFolMass / SLWLayer;
			Il = clim->par[rstep] * exp(-veg->k * share->LAI );
			LightEff = (1.0 - exp(-Il * log(2.0) / veg->HalfSat));
			LayerGrossPsnRate = GrossAmax * LightEff;					//gC/g Fol/day
			LayerGrossPsn = LayerGrossPsnRate * iFolMass;   //g C/m2 ground/day
			LayerResp = (share->DayResp + share->NightResp) * iFolMass;
			LayerNetPsn = LayerGrossPsn - LayerResp;
			if ((LayerNetPsn < 0) && (share->PosCBalMass == share->FolMass))
			{
				//if (ix < nLayers+1)
				share->PosCBalMass = (ix - 1.0) * iFolMass;
				share->LightEffCBal = LightEff;  // light effect at the light compensation point layer, could be larger than nlayers.

			}

			if (ix>nLayers)
			{

				if (ix > ((int)nLayers + 10)) break;  // the compensation point can not excess 30% of current foliar mass to avoid endless loop
				if ((LayerNetPsn > 0) && (DTemp > 0) && (share->GDDTot > veg->GDDFolEnd) && (clim->doy[rstep] < veg->SenescStart))
					continue ;
				else break;  // break while after getting the compensation point
			}

			if (ix==nLayers)
			{
				LightEffBttm = LightEff;  // light effect at the bottom of canopy
				LAIBttm = share->LAI;  // total LAI of canopy

			}


			share->CanopyNetPsn = share->CanopyNetPsn + LayerNetPsn;
			share->CanopyGrossPsn = share->CanopyGrossPsn + LayerGrossPsn;
	    
			//Ozone effects on Net Psn

//			if (clim->O3[rstep]>0)  // &&  ix<=nLayers; need to improve to match nlayers>50
			if ((share->O3EffectOnPSN == 1) && (share->GDDTot > veg->GDDFolStart))	 // O3 effect is on
			{
				// Convert netpsn to micromoles for calculating conductance


				netPsnumol = ((LayerNetPsn * 1000000) / (share->DayLength * 12)) / (iFolMass/ SLWLayer); //Matlab to C
				//Calculate ozone extinction throughout the canopy
				Layer = Layer + 1;
				RelLayer = Layer / (double)nLayers;
				RelO3 = 1 - (RelLayer * O3Prof) * (RelLayer * O3Prof) * (RelLayer * O3Prof); //Matlab to C
				if (RelO3<0) RelO3=0;  // improved by zzx

				//Calculate Conductance (mm/s): Conductance down-regulates with prior O3 effects on Psn
				LayerG = (gsInt + (gsSlope * netPsnumol)) * (1 - share->O3Effect[(int)Layer]);
				//For no downregulation use:    LayerG = gsInt + (gsSlope * netPsnumol);
				if (LayerG < 0)
				{
					LayerG = 0;
				}
				//Calculate cumulative ozone effect for each canopy layer with consideration that previous O3 effects were modified by drought
				share->O3Effect[(int)Layer] = (share->O3Effect[(int)Layer] * share->DroughtO3Frac) + (share->kO3Eff * LayerG * clim->O3[rstep] * RelO3);//
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


		if (share->LightEffCBal < LightEffBttm)  //excess light exists
		{
			share->LightEffCBal=LightEffBttm-share->LightEffCBal;  // growth potential
		}
		else									// too much leaf mass
		{
			share->LightEffCBal=0;   // can also be discarded. in the future, zzx

		}

		share->LightEffCBal=LightEffBttm;
		//share->LightEffCBal=LightEff;

		if ((DTemp > 0) && (share->GDDTot > veg->GDDFolEnd) && (clim->doy[rstep] < veg->SenescStart))
		{
			share->PosCBalMassTot = share->PosCBalMassTot + (share->PosCBalMass * share->dayspan);
			share->PosCBalMassIx = share->PosCBalMassIx + share->dayspan;

			share->LightEffCBalTot = share->LightEffCBalTot + (share->LightEffCBal * share->dayspan);
			share->LightEffCBalIx = share->LightEffCBalIx + share->dayspan;

		}
		if (share->LightEffMin > LightEffBttm)   // minimum light effect during growing season.
		{
			share->LightEffMin = LightEffBttm;
		}

		share->LAI=LAIBttm ;  // total LAI of canopy

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
//	if ((clim->O3[rstep]>0) && (share->CanopyGrossPsn>0))
	if ((share->O3EffectOnPSN == 1) && (share->GDDTot > veg->GDDFolStart) && (share->CanopyGrossPsn>0))

	{
		CanopyNetPsnPot = share->CanopyGrossPsn - (share->DayResp * share->FolMass) - (share->NightResp * share->FolMass);
		share->CanopyDO3Pot = CanopyNetPsnO3 / CanopyNetPsnPot;

		share->TotO3Dose = share->TotO3Dose + clim->O3[rstep];
	}
	else
	{
		share->CanopyDO3Pot = 1;
	}

	if (share->CanopyDO3Pot > 1)share->CanopyDO3Pot = 1;
	if (share->CanopyDO3Pot < 0)share->CanopyDO3Pot = 1;

}




