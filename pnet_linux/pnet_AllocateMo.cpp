#include "pnet_model.h"

void pnet_model::AllocateMo(veg_struct* veg, share_struct* share, int rstep, int CN_Mode)
{
	//
	// C allocation for the PnET ecosystem model.
	//


	double WoodMRespMo, GDDWoodEff, delGDDWoodEff, WoodProdCMo, WoodGRespMo, TMult, RootCAdd, RootAllocCMo, RootProdCMo, RootMRespMo, RootGRespMo;

	share->PlantC = share->PlantC + share->NetPsnMo - share->FolGRespMo;
	WoodMRespMo = share->CanopyGrossPsnActMo * veg->WoodMRespA;
	share->WoodMRespYr = share->WoodMRespYr + WoodMRespMo;
	share->FolProdCYr = share->FolProdCYr + share->FolProdCMo;
	share->FolGRespYr = share->FolGRespYr + share->FolGRespMo;

	if (share->GDDTot >= veg->GDDWoodStart)
	{
		GDDWoodEff = (share->GDDTot - veg->GDDWoodStart) / (veg->GDDWoodEnd - veg->GDDWoodStart);
		if (GDDWoodEff > 1.0)GDDWoodEff = 1;
		if (GDDWoodEff < 0)GDDWoodEff = 0;

		delGDDWoodEff = GDDWoodEff - share->OldGDDWoodEff;
		WoodProdCMo = share->WoodC * delGDDWoodEff;
		WoodGRespMo = WoodProdCMo * veg->GRespFrac;
		share->WoodProdCYr = share->WoodProdCYr + WoodProdCMo;
		share->WoodGRespYr = share->WoodGRespYr + WoodGRespMo;
		share->OldGDDWoodEff = GDDWoodEff;
	}
	else
	{
		WoodProdCMo = 0;
		WoodGRespMo = 0;
	}

	share->WoodProdCMo=WoodProdCMo;

	TMult = (exp(0.1 * (share->Tave - 7.1)) * 0.68) * 1.0;

	RootCAdd = veg->RootAllocA * (share->dayspan / 365.0) + veg->RootAllocB * share->FolProdCMo;
	share->RootC = share->RootC + RootCAdd;
	RootAllocCMo = (share->dayspan / 365.0) * TMult;   // modified from matlab version
	if (RootAllocCMo > 1.0) RootAllocCMo = 1.0;

	RootAllocCMo = RootAllocCMo *  share->RootC;
		
	share->RootC = share->RootC - RootAllocCMo;
	RootProdCMo = RootAllocCMo / (1.0 + veg->RootMRespFrac + veg->GRespFrac);
	share->RootProdCYr = share->RootProdCYr + RootProdCMo;
	RootMRespMo = RootProdCMo * veg->RootMRespFrac;
	share->RootMRespYr = share->RootMRespYr + RootMRespMo;
	RootGRespMo = RootProdCMo * veg->GRespFrac;
	share->RootGRespYr = share->RootGRespYr + RootGRespMo;
	share->PlantC = share->PlantC - RootCAdd - WoodMRespMo - WoodGRespMo;
	share->NetCBal = share->NetPsnMo - share->SoilRespMo - WoodMRespMo - WoodGRespMo - share->FolGRespMo;

	share->RootProdCMo=RootProdCMo;

	//PnET-CN Only -----------------------------------------------------------------
	if (CN_Mode == 1)
	{
		share->WoodMass = share->WoodMass + (WoodProdCMo / veg->CFracBiomass);
		share->WoodMassN = share->WoodMassN + ((WoodProdCMo / veg->CFracBiomass) * veg->WLPctN * share->NRatio);
		share->PlantN = share->PlantN - ((WoodProdCMo / veg->CFracBiomass) * veg->WLPctN * share->NRatio);
		share->RootMass = share->RootMass + (RootProdCMo / veg->CFracBiomass);
		share->RootMassN = share->RootMassN + ((RootProdCMo / veg->CFracBiomass) * veg->RLPctN * share->NRatio);
		share->PlantN = share->PlantN - ((RootProdCMo / veg->CFracBiomass) * veg->RLPctN * share->NRatio);
		share->NetCBal = share->NetPsnMo - WoodMRespMo - WoodGRespMo - share->FolGRespMo - RootMRespMo - RootGRespMo;
		// needs -share->SoilDecResp - share->WoodDecResp, and will be updated in the respective routine.

	}
	//------------------------------------------------------------------------------
}
