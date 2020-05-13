#include "pnet_model.h"

void pnet_model::Decomp( site_struct * site,veg_struct* veg, clim_struct* clim, int rstep, share_struct* share)
{
	//
	//PnET-CN decomposition routine
	//

	double tEffSoil, TMult, WMult, KhoAct, DHO, GrossNMin, SoilPctN, NReten, GrossNImmob, NetNMin, NetNitr, RootNSinkStr, PlantNUptake, NH4Up, NO3Up;
	double FN2O,FNO,FN2,FNOd,FN2Od,NitLostGas,Kn,Fdeni;  // N gas emission
	double fNRatioNit=1.0; 		// factor to adjust daily net Nitrification.
	double R_NOx_N2O;  // ratio of NO to N2O through nitrification
	double Wnit; 		// water stress
	double den_T,Q10,den_W,Wcrit,R_N2_N2O,den_CO2,Ccrit,Cmax;
	int iday;



	FN2O = 0;
	FNO = 0;
	FN2 = 0;
	FN2Od =0;
	Kn=0.01; //Kn=0.02, Kn is the fraction of nitrified N lost as N2O/NOx  (or0.05) (Davidson,2000;parton,2001)



	//Temperature effect on all soil processes
	for (iday = 1; iday <= share->dayspan; iday++)
	{
		//Add atmospheric N deposition
		share->NO3 = share->NO3 + clim->NO3dep[rstep]/share->dayspan;
		share->NH4 = share->NH4 + clim->NH4dep[rstep]/share->dayspan;
		share->NdepTot = share->NdepTot + clim->NO3dep[rstep]/share->dayspan+clim->NH4dep[rstep]/share->dayspan;  //ZZX


		if (share->Tave > 1)
		{
			tEffSoil = share->Tave;
		}
		else
		{
			tEffSoil = 1;
		}

		TMult = (exp(0.1 * (share->Tave - 7.1)) * 0.68) * 1;
		WMult = share->MeanSoilMoistEff;

		//Add litter to Humus pool
		share->HOM = share->HOM + share->TotalLitterM/share->dayspan;
		share->HON = share->HON + share->TotalLitterN/share->dayspan;

		//Humus dynamics

		KhoAct = veg->Kho  / 365.0; //* share->dayspan;
	//	KhoAct = -log(veg->Kho) / 365.0; //* share->dayspan;
		DHO = share->HOM * (1 - exp(-KhoAct * TMult * WMult));
		GrossNMin = DHO * (share->HON / share->HOM);
		share->SoilDecResp = DHO * veg->CFracBiomass;
		share->SoilDecRespYr = share->SoilDecRespYr + share->SoilDecResp;
		share->GrossNMinYr = share->GrossNMinYr + GrossNMin;
		share->HON = share->HON - GrossNMin;
		share->HOM = share->HOM - DHO;

		share->NetCBal = share->NetCBal - share->SoilDecResp; // updating NetCBal

		//Immobilization and net mineralization
		SoilPctN = (share->HON / share->HOM) * 100;
		NReten = (veg->NImmobA + veg->NImmobB * SoilPctN) / 100;
		GrossNImmob = NReten * GrossNMin;
		share->HON = share->HON + GrossNImmob;
		share->GrossNImmobYr = share->GrossNImmobYr + GrossNImmob;
		NetNMin = GrossNMin - GrossNImmob;
		share->NH4 = share->NH4 + NetNMin;


		// nitrification========================================================

		fNRatioNit = 1/30.4;  // daily step;
		NetNitr = (share->NH4 * share->NRatioNit*fNRatioNit);
		// NetNitr = (share->NH4 * (-log(share->NRatioNit)*fNRatioNit));

		//===========================================================================
		// NOx/N2O emission from soil nitrification
		//===========================================================================

		share->WFPS = share->Water/site->WHC*0.75;  //water filled pore space = soilwater/porosity,*0.75 is for WHC is 0.75 porosity
		if (share->WFPS>1.0)share->WFPS=1.0;

		Wnit= 1.0;
		//	Wnit=1/(0.524+exp(4.27-16.51*share->WFPS+13.64*share->WFPS*share->WFPS));


		NitLostGas=Kn*NetNitr*Wnit;   //(Davidson,2000;parton,2001)

		//===================================================================
		// to discard N gas emission processes, set NitLostGas =0;  // zzx

		 NitLostGas =0;  // zzx
		//====================================================================

		R_NOx_N2O =pow(10.0, -3.79*share->WFPS + 2.73); //(Davidson,2000)
		FN2O=NitLostGas /(1.0+R_NOx_N2O);
		FNO=NitLostGas-FN2O;

		share->NO3 = share->NO3 + NetNitr;
		share->NH4 = share->NH4 - NetNitr - NitLostGas;


		//===========================================================================
		// soil denitrification
		//===========================================================================

		// temperature effect on denitrification(Andrews, 1997)
		Q10= 2.0;
		share->Tsoil = share->Tave ;
		if (0.0<share->Tsoil)  den_T= pow(Q10,0.1*(share->Tsoil-20.0)); // referrence temperature where den_T=1 is 20 oC
		else    den_T= 0.01;  //    from dndc  or if iced, it is 0.1

		// soil water effect on denitrification
		Wcrit=0.75;   // den_W =0.5
		den_W= 1.0-1.0/(1.0+exp(12.5*(share->WFPS-Wcrit)));

		// soil respiration effect as indicator of DOC
		//	den_CO2 = 24000
		Ccrit = 1.8; // 1.8 gC/m2/day for soil respiration where den-co2= 0.5
		Cmax = 3;		// gC/m2/day for soil respiration  where den-co2= 1
		den_CO2= 1.0-1.0/(1.0+exp(6.8*(share->SoilDecResp-Ccrit)/Cmax));
		den_CO2= 1.0 ;

		site->Kden = 0.03; //* share->dayspan;//0.025*30=mon-1,  default 0.03 (day-1) (McCray,2005)
//		site->Kden = 0.13; //* share->dayspan;//0.025*30=mon-1,  default 0.03 (day-1) (McCray,2005)  // for HB

		if (den_W>den_CO2) den_W = den_CO2;
		Fdeni= share->NO3*site->Kden*den_T* den_W;

		// N2/N2O emission from soil in denitrification
		R_N2_N2O =1.44*exp( 1.33*share->WFPS);    //(Del Grosso,2000)

		// sigmodal curve as an alternative function
		double RnA,RnB;

		share->RnMax = 10.0; // regression from (Schlesinger,2009)
		share->RnX1 = 0.7; // point 1
		share->RnY1 = 1.5;

		share->RnX2 = 1.0;	//point 2
		share->RnY2 = share->RnMax; // flooding //(Schlesinger,2009)


		//	share->RnY1 = share->RnY1/share->RnMax;   // relative to max
		//	share->RnY2 = share->RnY2/share->RnMax;
		//  Y=RnMax*x/(x+e(a-bx)) through (x1,y1),(x2,y2)
		//	RnB = (log(share->RnX1*(share->RnMax-share->RnY1)/share->RnY1)-log(share->RnX2*(share->RnMax-share->RnY2)/share->RnY2))/(share->RnX2-share->RnX1);
		//	RnA = log(share->RnX1*(share->RnMax-share->RnY1)/share->RnY1) + RnB * share->RnX1;
		//	R_N2_N2O = share->RnMax *share->WFPS/(share->WFPS + exp(RnA- RnB *share->WFPS )) ; //    //(Schlesinger,2009)


		//  Y=a*exp(bx) through (x1,y1),(x2,y2)

		RnB = log(share->RnY2/share->RnY1)/(share->RnX2-share->RnX1);
		RnA = share->RnY1*exp(-RnB * share->RnX1);

		R_N2_N2O = RnA*exp(RnB *share->WFPS ) ; //    //(Schlesinger,2009)


		FN2Od= Fdeni * 1/(1+R_NOx_N2O+R_N2_N2O) ;

		//===================================================================
		// to discard N gas emission processes, set FN2Od =0;  // zzx

		 FN2Od =0.0;
		//========================================================================

		FNOd = R_NOx_N2O * FN2Od;
		FN2=FN2Od*R_N2_N2O;

		share->FluxN2OYrDe += FN2Od;
		share->FluxNOYrDe += FNOd;

		share->FluxN2OYr += FN2O + FN2Od;
		share->FluxNOYr += FNO + FNOd;
		share->FluxN2Yr += FN2;

		share->NO3 -=FN2Od+FN2+FNOd;

		//Plant Uptake

		RootNSinkStr = share->RootNSinkEff * TMult;
		if (RootNSinkStr > 0.98)RootNSinkStr = 0.98;
		RootNSinkStr = RootNSinkStr / 30.4;// *1.5 to make daily to the same as monthly;  // daily step
//		RootNSinkStr =-log( RootNSinkStr) / 30.4;//  to make daily to the same as monthly;  // daily step


		PlantNUptake = (share->NH4 + share->NO3) * RootNSinkStr;
		if ((PlantNUptake + share->PlantN) > veg->MaxNStore)
		{
			PlantNUptake = veg->MaxNStore - share->PlantN;
			RootNSinkStr = PlantNUptake / (share->NO3 + share->NH4);
		}
		if (PlantNUptake < 0)
		{
			PlantNUptake = 0;
			RootNSinkStr = 0;
		}
		share->PlantN = share->PlantN + PlantNUptake;
		share->PlantNUptakeYr = share->PlantNUptakeYr + PlantNUptake;

		NH4Up = share->NH4 * RootNSinkStr;
		share->NH4 = share->NH4 - NH4Up;
		NO3Up = share->NO3 * RootNSinkStr;
		share->NO3 = share->NO3 - NO3Up;


		share->NetNMinYr = share->NetNMinYr + NetNMin;
		share->NetNitrYr = share->NetNitrYr + NetNitr;





	}





}


