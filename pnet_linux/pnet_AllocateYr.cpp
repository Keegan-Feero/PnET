#include "pnet_model.h"

void pnet_model::AllocateYr(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share, int CN_Mode)
{
	//
	//Annual C allocation for the PnET ecosystem model.
	//

	int i;
	double EnvMaxFol, SppMaxFol, FolRegen, BiomLossFrac, TotalC, FolNConOld, folnconnew, nr;
	double PotLightEff;  // potential light for growth

	share->NPPFolYr = share->FolProdCYr / veg->CFracBiomass;
	share->NPPWoodYr = share->WoodProdCYr / veg->CFracBiomass;
	share->NPPRootYr = share->RootProdCYr / veg->CFracBiomass;

	if (share->DwaterIx > 0)
	{
		share->AvgDWater = share->Dwatertot / share->DwaterIx;
	}
	else
	{
		share->AvgDWater = 1.0;
	}


	if (share->PosCBalMassIx > 0)
	{
		share->avgPCBM = (share->PosCBalMassTot / share->PosCBalMassIx);
	}
	else
	{
		share->avgPCBM = share->FolMass;
	}


	share-> CanopyDO3Avg = share-> CanopyDO3Tot/ share-> CanopyDO3TotDay;


	if (share->LightEffCBalIx > 0)
	{
		PotLightEff = (share->LightEffCBalTot / share->LightEffCBalIx);
	}
	else
	{
		PotLightEff = 1.0;
	}

	share->LightEffMin = PotLightEff;

	EnvMaxFol = (share->AvgDWater * share->avgPCBM) * (1.0 + (veg->FolRelGrowMax * share->LightEffMin));
	SppMaxFol = share->avgPCBM * (1.0 + (veg->FolRelGrowMax * share->LightEffMin));

	if (EnvMaxFol < SppMaxFol)
	{
		veg->FolMassMax = EnvMaxFol;
	}
	else
	{
		veg->FolMassMax = SppMaxFol;
	}

	//Check for a disturbance year
	FolRegen = 100;
	BiomLossFrac = 0;

	for (i = 1; i <= site->distyrs; i++)
	{
		if (clim->year[rstep] == site->distyear[i])
		{
			BiomLossFrac = site->distintensity[i];
			veg->FolMassMax = veg->FolMassMax * (1 - BiomLossFrac);
			if (veg->FolMassMax < site->folregen[i])veg->FolMassMax = site->folregen[i];
			break;
		}
	}

	veg->FolMassMin = (veg->FolMassMax - veg->FolMassMax * (1.0 / veg->FolReten));
	share->BudC = ((veg->FolMassMax - share->FolMass) * veg->CFracBiomass);
	  

	if (share->BudC < 0)
	{
		share->BudC = 0;
		if(veg->FolReten>1) share->BudC = share->FolMass * 1/(veg->FolReten-1)*veg->CFracBiomass*0.5;  // evergreen has half budc

	}

	share->PlantC = share->PlantC - share->BudC;
	share->WoodC = (1.0 - veg->PlantCReserveFrac) * share->PlantC;
	share->PlantC = share->PlantC - share->WoodC;

	if (share->WoodC < (veg->MinWoodFolRatio * share->BudC))
	{
		TotalC = share->WoodC + share->BudC;
		share->WoodC = TotalC * (veg->MinWoodFolRatio / (1.0 + veg->MinWoodFolRatio));
		share->BudC = TotalC - share->WoodC;
		veg->FolMassMax = share->FolMass + (share->BudC / veg->CFracBiomass);
		veg->FolMassMin = (veg->FolMassMax - veg->FolMassMax * (1.0 / veg->FolReten));
	}



	//NEP calculation for PnET-II
	share->NEP = share->TotPsn - share->WoodMRespYr - share->WoodGRespYr - share->FolGRespYr - share->SoilRespYr;
	// save current foliar N for output
	share->FolNConOld = veg->FolNCon;

	// PnET-CN Only -----------------------------------------------------------------
	if (CN_Mode == 1)
	{
		if (share->PlantN > veg->MaxNStore)
		{
			share->NH4 = share->NH4 + (share->PlantN - veg->MaxNStore);  // ZZX revised
			share->PlantN = veg->MaxNStore;
		}


		share->NRatio = 1 + (share->PlantN / veg->MaxNStore) * veg->FolNConRange;

		if (share->NRatio < 1)
		{
			share->NRatio = 1;
		}

		if (share->NRatio > (1 + veg->FolNConRange))
		{
			share->NRatio = 1 + veg->FolNConRange;
		}
		

		share->BudN = (share->BudC / veg->CFracBiomass) * veg->FLPctN * (1 /(1-veg->FolNRetrans)) * share->NRatio;//ZZX



		if (share->BudN > share->PlantN)
		{
			if (share->PlantN < 0)
			{
				share->BudC = share->BudC * 0.1;
				share->BudN = share->BudN * 0.1;
			}
			else
			{
				share->BudC = share->BudC * (share->PlantN / share->BudN);
				share->BudN = share->BudN * (share->PlantN / share->BudN);
			}
		}


		folnconnew = (share->FolMass * (veg->FolNCon / 100) + share->BudN) / (share->FolMass + (share->BudC / veg->CFracBiomass)) * 100;
		veg->FolNCon = folnconnew;


		share->PlantN = share->PlantN - share->BudN;

		if (share->NRatio < 1)
		{
			share->NRatioNit = 0;
		}
		else
		{

			nr = share->NRatio - 1 - (veg->FolNConRange / 3);
			if (nr < 0)nr = 0;

			share->NRatioNit = (nr / (0.6667 * veg->FolNConRange))*(nr / (0.6667 * veg->FolNConRange));
			if (share->NRatioNit > 1)share->NRatioNit = 1;

		}


		share->RootNSinkEff = sqrt(1 - (share->PlantN / veg->MaxNStore));   // this is based on monthly rate

		//Annual total variables for PnET-CN
		share->NEP = share->TotPsn - share->SoilDecRespYr - share->WoodDecRespYr -share->WoodMRespYr
			- share->WoodGRespYr - share->FolGRespYr - share->RootMRespYr - share->RootGRespYr;
		share->FolN = (share->FolMass * veg->FolNCon / 100);
		share->FolC = share->FolMass * veg->CFracBiomass;
		share->TotalN = share->FolN + share->WoodMassN + share->RootMassN + share->HON 
			+ share->NH4 + share->NO3 + share->BudN + share->DeadWoodN + share->PlantN;
		share->TotalM = (share->BudC / veg->CFracBiomass) + share->FolMass + (share->WoodMass + share->WoodC / veg->CFracBiomass)
			+ share->RootMass + share->DeadWoodM + share->HOM + (share->PlantC / veg->CFracBiomass)+(share->RootC / veg->CFracBiomass);
	}


}
