#include "pnet_model.h"

void pnet_model::CNTrans(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share)
{
	//
	//Carbon and nitrogen translocation routine
	//


	int i;
	double BiomLossFrac, RemoveFrac, RootTurnover, RootLitM, RootLitN, WoodLitM, WoodLitN, WoodMassLoss, WoodTransM;
	double WoodTransN, FolNLoss, Retrans, FolLitN;
//	int dist=1;

	//Check for a disturbance year
	BiomLossFrac = 0;
	RemoveFrac = 0;

//	if (clim->doy[rstep] > 335)  // need to modify for daily version
	if (site->distyrs > 0)
	{
		for (i = 1; i <= site->distyrs; i++)
		{
			if ((clim->year[rstep] == site->distyear[i]) && (clim->year[rstep] == site->distyear[i]))
			{
				BiomLossFrac = site->distintensity[i];  // mortality
				RemoveFrac = site->distremove[i];    // remove out of field
				share->HOM = share->HOM * (1 - site->distsoilloss[i]);
				share->HON = share->HON * (1 - site->distsoilloss[i]);
				break;
			}
		}
	}

	RootTurnover = veg->RootTurnoverA - (veg->RootTurnoverB * share->NetNMinLastYr) + (veg->RootTurnoverC * pow(share->NetNMinLastYr,2)); //Matlab to C
	if (RootTurnover > 2.5)
	{
		RootTurnover = 2.5;
	}
	if (RootTurnover < 0.1)
	{
		RootTurnover = 0.1;
	}
	RootTurnover = RootTurnover * (share->dayspan / 365.0); //Matlab to C
	if (BiomLossFrac > RootTurnover)
	{
		RootTurnover = BiomLossFrac;
	}
	RootLitM = share->RootMass * RootTurnover;
	RootLitN = RootLitM * (share->RootMassN / share->RootMass);
	share->RootMass = share->RootMass - RootLitM;
	share->RootMassN = share->RootMassN - RootLitN;

	if (BiomLossFrac > 0)
	{
		WoodLitM = share->WoodMass * BiomLossFrac * (1 - RemoveFrac);
		WoodLitN = share->WoodMassN * BiomLossFrac * (1 - RemoveFrac);
		share->WoodMass = share->WoodMass * (1 - BiomLossFrac);
		share->WoodMassN = share->WoodMassN * (1 - BiomLossFrac);
		//		share->PlantC = share->PlantC * (1 - BiomLossFrac);
		
	}
	else
	{

		WoodLitM = share->WoodMass * veg->WoodTurnover * (share->dayspan / 365.0); //Matlab to C
		WoodLitN = share->WoodMassN * veg->WoodTurnover * (share->dayspan / 365.0); //Matlab to C
		share->WoodMass = share->WoodMass - WoodLitM;
		share->WoodMassN = share->WoodMassN - WoodLitN;
	}

	share->DeadWoodM = share->DeadWoodM + WoodLitM;
	share->DeadWoodN = share->DeadWoodN + WoodLitN;
	WoodMassLoss = share->DeadWoodM * veg->WoodLitLossRate * (share->dayspan / 365.0); //Matlab to C
	WoodTransM = WoodMassLoss * (1 - veg->WoodLitCLoss);  // wood litter
	share->WoodDecResp = (WoodMassLoss - WoodTransM) * veg->CFracBiomass; // loss as CO2
	share->WoodDecRespYr = share->WoodDecRespYr + share->WoodDecResp;
	WoodTransN = (WoodMassLoss / share->DeadWoodM) * share->DeadWoodN;
	share->DeadWoodM = share->DeadWoodM - WoodMassLoss;
	share->DeadWoodN = share->DeadWoodN - WoodTransN;

	share->NetCBal = share->NetCBal - share->WoodDecResp;  // updating NetCBal

	FolNLoss = share->FolLitM * (veg->FolNCon / 100);
	Retrans = FolNLoss * veg->FolNRetrans;
	share->PlantN = share->PlantN + Retrans;
	FolLitN = FolNLoss - Retrans;

	if (BiomLossFrac > 0)
	{
		share->FolLitM = share->FolLitM + (share->FolMass * BiomLossFrac);
		FolLitN = FolLitN + (share->FolMass * BiomLossFrac * (veg->FolNCon / 100));
		share->FolMass = share->FolMass * (1 - BiomLossFrac);
		share->PlantC = share->PlantC * (1 - BiomLossFrac);
		share->PlantN = share->PlantN + (veg->MaxNStore - share->PlantN) * BiomLossFrac;  // increase PlantN?
	}

	share->TotalLitterM = share->FolLitM + RootLitM + WoodTransM;
	share->TotalLitterN = FolLitN + RootLitN + WoodTransN;

	//Agriculture
	if (clim->year[rstep]>=site->agstart && clim->year[rstep]<site->agstop)
	{
		share->TotalLitterM = share->TotalLitterM * (1 - site->agrem);
		share->TotalLitterN = share->TotalLitterN * (1 - site->agrem);

		share->WoodMass = share->WoodMass * (1 - site->agrem * (share->dayspan / 365.0));
		share->WoodMassN = share->WoodMassN * (1 - site->agrem * (share->dayspan / 365.0));
	}

	share->TotalLitterMYr = share->TotalLitterMYr + share->TotalLitterM;
	share->TotalLitterNYr = share->TotalLitterNYr + share->TotalLitterN;
}

