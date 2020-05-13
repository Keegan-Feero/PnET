#include "pnet_model.h"

void pnet_model::Waterbal(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share)
{
	//
	// PnET ecosystem water balance routine.
	//


	int wday;
	double	CanopyGrossPsnMG, WUE, PotTransd, Evap, precrem, SnowFrac, Tavew, SnowMelt, waterin, FastFlow, waterind;
	double	Trans, TotSoilMoistEff, Transd, CanopyGrossPsnAct;
	double	dTemp,SnowTCrit;

	//Potential transpiration
	CanopyGrossPsnMG = share->CanopyGrossPsn * 1000.0 * (44.0 / 12.0);  // mg CO2/m2.day
	WUE = (veg->WUEConst / share->VPD) * share->DWUE;  // g CO2/kg water
    //DelAmax added to the following eqn to prevent increased transpiration at high CO2
	PotTransd = (CanopyGrossPsnMG / share->DelAmax / WUE) / 10000.0;   //cm water/day

	//Precip input and snow/rain partitioning
	Evap = clim->prec[rstep] * veg->PrecIntFrac; // all intercepted rain evaporated
	precrem = clim->prec[rstep] - Evap;


	if (share->Tave > 2.0)
	{
		SnowFrac = 0;
	}
	if (share->Tave < -5.0)
	{
		SnowFrac = 1.0;
	}
	if ((share->Tave >= -5.0) && (share->Tave <= 2.0))
	{
		SnowFrac = (share->Tave - 2.0) / -7.0;
	}




	if ( clim->timestep == 1 )  //daily version
	{
		SnowTCrit = -6.0;
		SnowMelt =0.0;

		Tavew = share->Tave;
		if (share->Tave < 1.0)Tavew = 1.0;

		if (clim->tmin[rstep] > SnowTCrit)
		{
			SnowFrac = 0;
			SnowMelt = 0.15 * Tavew ;

		}
		else SnowFrac = 1.0;

	}


	//Snowmelt
	site->SnowPack = site->SnowPack + precrem * SnowFrac;
	if (site->SnowPack > 0)
	{

		if ( clim->timestep == 0 )  //monthly version
		{
			Tavew = share->Tave;
			if (share->Tave < 1.0)
			{
				Tavew = 1.0;
			}

			SnowMelt = 0.15 * Tavew * share->dayspan;
		}

		if (SnowMelt > site->SnowPack)
		{
			SnowMelt = site->SnowPack;
		}

	}
	else
	{
		SnowMelt = 0;
	}


	site->SnowPack = site->SnowPack - SnowMelt;

	//Fast flow
	waterin = SnowMelt + precrem * (1.0 - SnowFrac);
	FastFlow = veg->FastFlowFrac * waterin;
	waterin = waterin - FastFlow;
	waterind = waterin / share->dayspan;   // daily water in

	//Transpiration
	Trans = 0;
	if (PotTransd > 0)
	{
		TotSoilMoistEff = 0;
		for (wday = 1; wday <= share->dayspan; wday++)
		{
			share->Water = share->Water + waterind;
			if (share->Water >= PotTransd/veg->f)
			{
				Transd = PotTransd;
			}
			else
			{
				Transd = share->Water * veg->f;
			}
			share->Water = share->Water - Transd;
			Trans = Trans + Transd;

			if (share->Water < site->WHC)
			{
				dTemp = share->Water;
			}
			else
			{
				dTemp = site->WHC;
			}
			TotSoilMoistEff = TotSoilMoistEff + pow((dTemp / site->WHC),(1.0 + veg->SoilMoistFact));
		}// end of daily loop



		share->MeanSoilMoistEff = TotSoilMoistEff / share->dayspan;  // bug correct, zzx
		if (share->MeanSoilMoistEff>1)share->MeanSoilMoistEff = 1.0;


		//Water stress
		share->DWater = Trans / (PotTransd * share->dayspan);

		/////////////Manually turn water stress off  //
		if (site->WaterStress == 0) share->DWater = 1;

		if (share->DWater>1.0) share->DWater = 1.0;

		share->Dwatertot = share->Dwatertot + (share->DWater * share->dayspan);
		share->DwaterIx = share->DwaterIx + share->dayspan;
	}
	else
	{
		share->DWater = 1.0;
		share->Water = share->Water + waterin;
		share->MeanSoilMoistEff = 1.0;
	}

	//Calculate actural ozone effect and NetPsn with drought stress
	
	if ((share->O3EffectOnPSN == 1) && (share->CanopyGrossPsn>0))
	{
		if (share->WUEO3Eff == 0)
		{
			//no O3 effect on WUE (assumes no stomatal impairment)
			share->CanopyDO3 = share->CanopyDO3Pot + ((1 - share->CanopyDO3Pot) * (1 - share->DWater));  // drought offset to O3 effect
		}
		else
		{
			//reduce the degree to which drought offsets O3 (assumes stomatal impairment in proportion to effects on psn)
			share->CanopyDO3 = share->CanopyDO3Pot + (1 - share->CanopyDO3Pot) * (1 - share->DWater / share->CanopyDO3Pot);
		
		}
		share->DroughtO3Frac = share->CanopyDO3Pot / share->CanopyDO3;  // relative drought offset
	}
	else
	{
		share->CanopyDO3 = 1;
		share->DroughtO3Frac = 1;
	}

	if(share->CanopyDO3 >1)  share->CanopyDO3 = 1 ;

	if (share->CanopyGrossPsn>0)
	{
		share-> CanopyDO3Tot = share-> CanopyDO3Tot + share->CanopyDO3 * share->dayspan;
		share-> CanopyDO3TotDay = share-> CanopyDO3TotDay + share->dayspan;

	}


	CanopyGrossPsnAct = share->CanopyGrossPsn * share->DWater;   // ' g C/m2.day
	share->CanopyGrossPsnActMo = CanopyGrossPsnAct * share->dayspan;  // g C/m2.month
	share->GrsPsnMo = share->CanopyGrossPsnActMo * share->CanopyDO3;
	share->NetPsnMo = (CanopyGrossPsnAct - (share->DayResp + share->NightResp) * share->FolMass) * share->dayspan * share->CanopyDO3;

	share->TotCanopyGrossPsnMax= share->TotCanopyGrossPsnMax + share->CanopyGrossPsn * share->dayspan ;  // psn without water stress


	if (share->Water > site->WHC)
	{
		share->Drainage = share->Water - site->WHC;
		share->Water = site->WHC;
	}
	else
	{
		share->Drainage = 0;
	}

	share->Drainage = share->Drainage + FastFlow;
	share->FracDrain = share->Drainage / (share->Water + clim->prec[rstep]);
	share->TotTrans = share->TotTrans + Trans;
	share->TotWater = share->TotWater + share->Water;
	share->TotPsn = share->TotPsn + share->NetPsnMo;
	share->TotDrain = share->TotDrain + share->Drainage;
	share->TotPrec = share->TotPrec + clim->prec[rstep];
	share->TotEvap = share->TotEvap + Evap;
	share->TotGrossPsn = share->TotGrossPsn + share->GrsPsnMo;
	share->ET=Trans+Evap;

}
