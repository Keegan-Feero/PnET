#include "pnet_model.h"

void pnet_model::initvars(site_struct* site,veg_struct* veg, share_struct* share)
{
	//
	//Initialize internal shared variable structures for the PnET ecosystem model
	//


	int i;

	//Build "share" structure
	share->Tave=0;
	share->Tday=0;
	share->Tnight=0;
	share->DayLength=0;
	share->NightLength=0;
	share->VPD=0;
	share->dayspan=1;  // initial for daily
	share->GDDTot=0;
	share->OldGDDFolEff=0;
	share->FolLitM=0;
	share->PosCBalMass=0;
	share->PosCBalMassTot=0;
	share->PosCBalMassIx=0;
	share->LAI=0;
	share->DVPD=0;
	share->DayResp=0;
	share->NightResp=0;
	share->CanopyNetPsn=0;
	share->CanopyGrossPsn=0;
	share->Dwatertot=0;
	share->DwaterIx=0;
	share->GrsPsnMo=0;
	share->NetPsnMo=0;
	share->FolGRespMo=0;
	share->WoodMRespYr=0;
	share->CanopyGrossPsnActMo=0;
	share->FolProdCYr=0;
	share->FolProdCMo=0;
	share->FolGRespYr=0;
	share->RootProdCYr=0;
	share->RootMRespYr=0;
	share->RootGRespYr=0;
	share->SoilRespMo=0;
	share->SoilRespYr=0;
	share->OldGDDWoodEff=0;
	share->WoodProdCYr=0;
	share->WoodGRespYr=0;
	share->TotPsn=0;
	share->MeanSoilMoistEff=0;
	share->Drainage=0;
	share->TotDrain=0;
	share->TotEvap=0;
	share->TotTrans=0;
	share->TotPrec=0;
	share->TotWater=0;
	share->TotGrossPsn=0;
	share->NPPFolYr=0;
	share->NPPWoodYr=0;
	share->NPPRootYr=0;
	share->ET=0;
	share->NEP=0;
	share->NetCBal=0;
	share->SoilDecResp=0;
	share->BudN=0;
	share->SoilDecRespYr=0;
	share->WoodDecRespYr=0;
	share->DelAmax=0;
	share->DWUE=0;
	share->CanopyDO3Pot=0;
	share->DroughtO3Frac=0;
	share->TotO3Dose=0;
	share->RootMassN=0;
	share->TotalLitterMYr=0;
	share->TotalLitterNYr=0;
	share->GrossNImmobYr=0;
	share->GrossNMinYr=0;
	share->PlantNUptakeYr=0;
	share->NetNitrYr=0;
	share->NetNMinYr=0;
	share->FracDrain=0;
	share->NDrainYr=0;
	share->WoodDecResp=0;
	share->TotalLitterM=0;
	share->TotalLitterN=0;
	share->FolN=0;
	share->FolC=0;
	share->TotalN=0;
	share->TotalM=0;
	share->NO3=0;
	share->NH4=0;



	share->NdepTot=0.0;  // ZZX
	share->LightEffCBalTot=0;
	share->LightEffCBalIx=0;

	//Shared variables with initial conditions

//	share->FolMass=0;   //share->FolMass=veg->FolMassMin;   In PnET-Day only
//	share->FolMass=100;
//	share->BudC=135;    //share->BudC=(veg->FolMassMax-share->FolMass)*veg->CFracBiomass;  In PnET-Day only
//	share->Water=12;
	share->DWater=1;
//	share->DeadWoodM=11300;
//	share->WoodC=300;
//	share->PlantC=900;
	share->RootC=share->WoodC/3;
	share->LightEffMin=1;
	share->LightEffCBal=0;
//	share->NRatio=1.3993;
//	share->PlantN=1;
//	share->WoodMass=20000;
//	share->RootMass=6;
//	share->HOM=13500;
//	share->HON=390;
	share->RootNSinkEff=.5;

	share->WoodMassN=share->WoodMass*veg->WLPctN*share->NRatio;  //
	share->DeadWoodN=share->DeadWoodM*veg->WLPctN*share->NRatio;
	share->NRatioNit=1;  // share->NRatio;	
//	share->NRatioNit= share->NRatio;
	share->NetNMinLastYr=10;

// the input replaces the initials

	veg->FastFlowFrac=site->FastFlowFrac;
	veg->f=site->WaterReleaseFact;
	share->Water=site->SWater;
//	share->DWater=site->WaterStress;
	share->HOM=site->HOM;
	share->HON=site->HON;
	share->NH4=site->NH4;
	veg->Kho=site->Kho;

	veg->SoilRespA=site->SoilRespA;
	veg->SoilRespB=site->SoilRespB;
	veg->SoilMoistFact=site->SoilMoistFact;
	veg->NImmobA=site->NImmobA;
	veg->NImmobB=site->NImmobB;

	share->WUEO3Eff=site->O3EffectOnWUE;

	share->FolNConOld=0;
	share-> avgPCBM = 1.0;					// average light effect
	share-> AvgDWater =1.0;					// average water stress

	for (i = 0; i<51; i++)share->O3Effect[i]=0.0;

	share->NDrain=0;
	share->NDrainMgL=0;

	share->TaveYr=0;
	share->PARYr=0;


	share->yrspin = 1000;

	share->WoodProdCMo=0.0;
	share->RootProdCMo=0.0;
	share->Tsoil=0.0;
	share->WFPS=0.0;

	share->RnY2=0;
	share->RnX2=0;
	share->RnY1=0;
	share->RnX1=0;
	share->RnMax=0;

	share->FluxN2OYrDe=0;
	share->FluxNOYrDe=0;
	share->FluxN2OYr=0;
	share->FluxNOYr=0;
	share->FluxN2Yr=0;

	share->TotCanopyGrossPsnMax=0;
	share->O3EffectOnPSN=0;
	share->ifO3EffectOnPSN=0;
	share->kO3Eff=0.0026;   // per ppm
	share->kO3EffScalar=1.;   //


	share-> CanopyDO3Avg = 0;			// Avg O3 effect on photosynthesis for the whole canopy for the whole growing season
	share-> CanopyDO3Tot = 0;			// Total O3 effect on photosynthesis for the whole canopy for the whole growing season
	share-> CanopyDO3TotDay =0;			// Total days of O3 effect on photosynthesis for the whole canopy for the whole growing season




	veg->FolRespFrac = 1;  // based on our previous analysis

	share->distid = 0;
	site->distyrs = 0;

	share->s1 = 0;
	share->s2 = 0;





}



void pnet_model::init_out(int nyrs, out_struct* out)
{
	//allocate memory for the output structure

	int nmonth = nyrs*12;
	int i;

	// intialize arrays
	for (i=1;i<nmonth+1;i++)
	{
		out->grosspsn[i] = 0.0;
		out->netpsn[i] = 0.0;
		out->netcbal[i] = 0.0;
		out->vpd[i] = 0.0;
		out->folmass[i] = 0.0;
		out->plantnMo[i] = 0.0;

	}
/**/
	for (i=1;i<nyrs+1;i++)
	{

		out->nppfol[i] = 0.0;
		out->nppwood[i] = 0.0;
		out->npproot[i] = 0.0;
		out->nep[i] = 0.0;
		out->gpp[i] = 0.0;
		out->waterstress[i] = 0.0;
		out->trans[i] = 0.0;
		out->soilwater[i] = 0.0;
		out->psn[i] = 0.0;
		out->drain[i] = 0.0;
		out->prec[i] = 0.0;
		out->evap[i] = 0.0;
		out->et[i] = 0.0;
		out->plantc[i] = 0.0;
		out->budc[i] = 0.0;
		out->woodc[i] = 0.0;
		out->rootc[i] = 0.0;
		out->plantnYr[i] = 0.0;
		out->budn[i] = 0.0;
		out->ndrain[i] = 0.0;
		out->netnmin[i] = 0.0;
		out->grossnmin[i] = 0.0;
		out->nplantuptake[i] = 0.0;
		out->grossnimob[i] = 0.0;
		out->littern[i] = 0.0;
		out->netnitrif[i] = 0.0;
		out->nratio[i] = 0.0;
		out->foln[i] = 0.0;
		out->litm[i] = 0.0;
		out->litn[i] = 0.0;
		out->rmresp[i] = 0.0;
		out->rgresp[i] = 0.0;
		out->decresp[i] = 0.0;


		out->folm[i] = 0.0;
		out->deadwoodm[i] = 0.0;
		out->woodm[i] = 0.0;
		out->rootm[i] = 0.0;
		out->hom[i] = 0.0;
		out->hon[i] = 0.0;
		out->ndep[i] = 0.0;

		out->v1[i] = 0.0;
		out->v2[i] = 0.0;
		out->v3[i] = 0.0;



	}




}


