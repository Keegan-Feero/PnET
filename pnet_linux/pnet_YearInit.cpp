#include "pnet_model.h"

void pnet_model::YearInit(share_struct* share)
{
	//
	// Annual initialization for the PnET ecosystem model.
	//

	share->GDDTot = 0;
	share->WoodMRespYr = 0;
	share->SoilRespYr = 0;
	share->TotTrans = 0;
	share->TotPsn = 0;
	share->TotGrossPsn = 0;
	share->TotDrain = 0;
	share->TotPrec = 0;
	share->TotEvap = 0;
	share->TotWater =0;
	share->FolProdCYr = 0;
	share->WoodProdCYr = 0;
	share->RootProdCYr = 0;
	share->RootMRespYr = 0;
	share->FolGRespYr = 0;
	share->WoodGRespYr = 0;
	share->RootGRespYr = 0;
	share->OldGDDFolEff = 0;
	share->OldGDDWoodEff = 0;
	share->PosCBalMassTot = 0;
	share->PosCBalMassIx = 0;
	share->Dwatertot = 0;
	share->DwaterIx = 0;
	share->NDrainYr=0;

	share->GrossNMinYr=0;
	share->PlantNUptakeYr=0;
	share->GrossNImmobYr=0;
	share->TotalLitterMYr=0;
	share->TotalLitterNYr=0;
	share->NetNitrYr=0;
	share->LightEffMin=1;
	share->SoilDecRespYr=0;
	share->WoodDecRespYr=0;
	share->NetNMinLastYr = share->NetNMinYr;
	share->NetNMinYr=0;
	share->NdepTot=0.0; //ZZX

	share->LightEffCBalTot=0;
	share->LightEffCBalIx=0;

	share->TaveYr=0;
	share->PARYr=0;

	share->FluxN2OYrDe=0;
	share->FluxNOYrDe=0;
	share->FluxN2OYr=0;
	share->FluxNOYr=0;
	share->FluxN2Yr=0;

	share->TotCanopyGrossPsnMax=0;
	share-> CanopyDO3Tot = 0;			// Total O3 effect on photosynthesis for the whole canopy for the whole growing season
	share-> CanopyDO3TotDay =0;			// Total days of O3 effect on photosynthesis for the whole canopy for the whole growing season
	for (int i = 0; i<51; i++)share->O3Effect[i]=0.0;

	share->TotO3Dose=0;



}
