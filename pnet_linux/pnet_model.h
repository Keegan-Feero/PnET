// pnet_model.h: interface for the pnet_model class.
//
//////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
//#include <cstddef.h>
//#include <stddef.h>
#include <algorithm>    // std::min max
//#include <io.h>
#include <float.h>
#include <string.h>

#include <string.h>
#include <math.h>


#include "pnet_constants.h"



class pnet_model  
{
public:
	pnet_model();
	virtual ~pnet_model();	
	

	// paths to files
	char sep[3];			// file separator. Wins:\\; Linux: /
	char exePath[300];		// program location
	char PathInput[30];		//input directory path
	char PathOutSite[30];	//output directory path for site mode
	char PathOutRegion[30];	//output directory path for regional mode
	char PathInter[30];		//intermediate directory path to store temporary files
	char PathLib[30];		// directory path to library folder. not used currently
	char PathRegion[30];	// path to regional input files.
	
	

	// model type

	int modelmode;  // 1: site modeling or 2:regional modeling
	int modeltype;  // model type, 0: pnet-day, 1: pnet-ii, 2: pnet-cn ,3 pnet_daily;

		
		
	typedef struct 
	{

		int	timestep;  // timestep of the climate data: 0: monthly; 1: daily; 2: hourly

		int	length; //length of climate record
		
		int*	year; 						// year
		int*	month;   					// month of the doy
		int*	day;  						// date of the doy
		double*	hour;						// hour of the doy  // only for hourly version
		int*	doy;						// day of year

		double*	tmax;						// oC
		double*	tmin;						// oC
		double*	par;						//  umol/m2/s 
		double*	prec;						// cm
		double*	O3;							// ppm-h, dose of hourly concentrations above a throshold  of 40 ppb ozone dose values summed over the growing season (may through oct)??
		//The D40 dose is accumulated over the entire growing season and is calculated as the sum of all
		//daytime hourly values > 40 ppb after subtracting 40 from from May through October.

		double*	CO2;						// ppm
		double*	NH4dep;						// g N/m2
		double*	NO3dep;						// g N/m2


	} clim_struct;
		
		//a structure to hold site data
	typedef struct
	{
		double	WHC;						// cm
		double	Lat;						// decimal degree without minute and second
		double	WaterStress;				// 0 or 1: 1 for no stress on photosynthesis
		double	SnowPack;					// cm equivalent water
		int		CO2gsEffect;				// 1: Yes; 0: NO. CO2 effect on gs to modify WUE and O3 effect.
		int		O3EffectOnWUE;				// 1: Yes; 0: NO, if there is an effect of O3 on WUE
		int		Nabs;				// 1: Yes; 0: NO, N deposition has absulate unit, i.e, gN m-2

		int		nYearStart;  		// simulation first year
		int		nYearEnd;   		 // end year of simulation
		double	SWater;  			// soil water
		double	SoilMoistFact; 		// soil moisture factor to soil respiration
		double	FastFlowFrac;		//Fraction of water inputs lost directly to drainage through macro pore or runoff
		double  WaterReleaseFact;	//Soil water release ability for evapotranspiration
		double	DWater;				// water stress index
		double  HOM;			//soil organic matter,	g/m2
		double	HON;			//soil organic N, gNm2
		double	NH4;			//soil ammonium N, gNm2
		double	Kho;			// soil decomposition constant, yr-1

		double	SoilRespA;		//the slope in the linear relation between soil resp CO2 and mean monthly temp, only in PnET-II
		double	SoilRespB;		//the intercept in the linear relation (g/m^2) between soil resp and mean monthly temp,only in PnET-II
		double	NImmobA;		// N immoblization rate parameter
		double	NImmobB;		// linear coefficients for fraction of mineralized N reimmobilized as a function of SOM C:N

		char ClimateFileName[300];		// climate file path, only used in GUI version
		
		// disturbance
		int		agstart;					// agriculture start year
		int		agstop;						// agriculture end year
		double	agrem;						// removal fraction in agriculture

		int		distyrs;					 //number of disturbances, added by J.X.
		double*	distintensity;				// disturbance motality, 0-1
		int*	distyear;					//disturbance year
		double*	distremove;					// removal fraction for aboveground biomass
		double*	distsoilloss;				// removal fraction for soil biomass				
		double*	folregen;					//foliar regeneration rate next year, g/m-2
		int*	distdoy;						// disturbance day of the year.

		int FertYrStart;					// N fertilization start year
		int FertDOYStart;					// N fertilization start day of year
		int FertYrEnd;						// N fertilization end year
		int FertDOYEnd;						// N fertilization end day of year
		double FertNH4;						// N fertilization  NH4 rate gN/m2
		double FertNO3;						// N fertilization  NO3 rate gN/m2
		double FertUrea;					// N fertilization  urea rate gN/m2


		double Kden;

		int agnum;
	//	double

	} site_struct;

		
		//a structure to hold veg data (parameters)
	typedef struct 
	{
		double	AmaxA;				// the intercept in a linear relation max net Photosynthesis as a function of N.
		double	AmaxB;				//the slope in the function. Max rate units are umol CO2 m-2 leaf s-1
		double	AmaxFrac;			//Daily Amax as a fraction of the early morning instantaneous rate
		double	BaseFolRespFrac;	//respiration as a fraction of max photosynthesis
		double	CFracBiomass;		//Carbon as fraction of foliage mass
		double	DVPD1;				//Coefficients for converting VPD to DVPD (kPa-1	
		double	DVPD2;				//DVPD being fraction loss of photosynthesis
		double	FolMassMax;			//g/m2
		double	FolMassMin;			////g/m2
		double	GDDFolStart;		//growing degree days at which foliar production begins
		double	GRespFrac;			//Growth respiration, fraction of allocation
		double	GDDFolEnd;			//growing degree days at which foliar production ends
		double	HalfSat;			//Half saturation light level (umol m-2 s-1)
		double	PsnTMin;			//Minimum temperature for photosynthesis (C)
		double	PsnTOpt;			//Optimum temperature for photosynthesis (C)
		double	RespQ10;			//Q10 value for foliar respiration
		double	SLWdel;				//change in SLW with increasing foliar mass above (g/m-2 g-1)
		double	SLWmax;				//specific leaf weight (top of canopy) g/m-2
		double	SenescStart;		//first day of year when leaf drop could potentially start
		double	k;					//canopy light attenuation constant exp -kT
		double	FolNCon;			//2.2% for estimate from spectral data HF, HW, pine, spruce
		double	FolRelGrowMax;		//
		double	RootAllocA;			//Relationship between foliar and root allocation. Intercept
		double	RootAllocB;			//Relationship between foliar and root allocation. Slope
		double	WoodMRespA;			//Wood Maintenance respiration as a fraction of gross photowynthesis
		double	RootMRespFrac;		//Ratio of fine root maintenance respiration to biomass production
		double	PlantCReserveFrac;	//Fraction of PlantC held in reserve after allocation to BudC
		double	MinWoodFolRatio;	//Min ratio of carbon allocation to wood and foliage
		double	WUEConst;			//Constant in equation for water use efficiency as a function of VPD, g CO2/kg water
		double	PrecIntFrac;		//Fraction of precipitation intercepted and evaporated
		double	FastFlowFrac;		//Fraction of water inputs lost directly to drainage
		double	f;					//Soil water release parameter for evapotranspiration
		double	SoilRespA;			//the slope in the linear relation between soil resp CO2 and mean monthly temp
		double	SoilRespB;			//the intercept in the linear relation (g/m^2) between soil resp and mean monthly temp
		double	SoilMoistFact;		//effect of soil moisture
		double	WoodTurnover;		//fractional mortality of live wood per year, live wood to dead wood
		double	RootTurnoverA;		//constant term of 2-deg polynomial describing fine root turnover
		double	RootTurnoverB;		//as a function of annual net N mineralization. Linear coefficient
		double	RootTurnoverC;		//quadratic coefficient
		double	FolReten;			//Foliar retention time (yr)
		double	Kho;				//Decomposition constant for SOM pool yr-1 (SOM*(1-exp(-Kho*Tmult))
		double	NImmobA;			//linear coefficients for fraction of mineralized N
		double	NImmobB;			//reimmobilized as a function of SOM C:N
		double	MaxNStore;			//max N content in PlantN pool g N m-2
		double	GDDWoodStart;		//Growing degree days at which wood production begins
		double	GDDWoodEnd;			//Growing degree days at which wood production ends
		double	FolNConRange;		//max fractional increase in N concentration
		double	FolNRetrans;		//fraction of foliage N retransfer to plant N, remainder in litter 
		double	FLPctN;				//min % N concentration in foliar litter
		double	RLPctN;				//min N % cincentration in root litter
		double	WLPctN;				//min N % cincentration in wood litter
		double	WoodLitLossRate;	// fraction of dead wood to litter  yr-1
		double	WoodLitCLoss;		// fraction of litter decayed as CO2 

		int		TreeCode;			// not used currently.
		int		Age;				// not used currently.
		double	FolRespFrac;	//Daily respiration as a fraction of monthly temperature results
		

	}  veg_struct;
		
		//a structure to hold share data
	typedef struct
	{
		double Tave;					// oC
		double Tday;					// oC
		double Tnight;					// oC
		double DayLength;				// second
		double NightLength;				// second
		double VPD;						// kpa
		double dayspan; 				// numbers of days of each timestep
		double GDDTot;					// total growth degree days
		double OldGDDFolEff;			// to calculate foliar growth
		double FolLitM;					// foliar litter mass, g/m2
		double PosCBalMass;				// possible C mass at balance point
		double PosCBalMassTot;			// total potential mass
		double PosCBalMassIx;			// total potential mass days
		double LAI;						// leaf area index
		double DVPD;					// vpd effect on photosynthesis
		double DayResp;					// foliar respiration at daytime
		double NightResp;				// foliar respiration at nighttime
		double CanopyNetPsn;			// canopy net photosynthesis, gpp-foliar respiration
		double CanopyGrossPsn;			// gpp,//g C/m2 ground/day
		double Dwatertot;				// total water stress at growing season
		double DwaterIx;				// total water stressed days
		double GrsPsnMo;				// monthly gpp
		double NetPsnMo;				// monthly net psn
		double FolGRespMo;				// foliar growth respiration
		double WoodMRespYr;				// yearly wood maintenance respiration
		double CanopyGrossPsnActMo;		//monthly gpp modified by water stress and other stress
		double FolProdCYr;				// foliar npp, g C m-2
		double FolProdCMo;				// foliar npp each time step, g C m-2
		double FolGRespYr;				// foliar yearly growth respiration, g C m-2
		double RootProdCYr;				// root npp, g C m-2
		double RootMRespYr;				// root yearly maintenance respiration, g C m-2
		double RootGRespYr;				// root yearly growth respiration, g C m-2
		double SoilRespMo;				// soil respiration, g C m-2
		double SoilRespYr;				// soil yearly respiration, g C m-2
		double OldGDDWoodEff;			// to calculate wood growth
		double WoodProdCYr;				// wood npp, g C m-2
		double WoodGRespYr;				// wood yearly growth respiration, g C m-2
		double TotPsn;					// total net psn
		double MeanSoilMoistEff;		// soil moisture effect on som decay
		double Drainage;				// water drainage, as runoff + leaching
		double TotDrain;				// total drainage
		double TotEvap;					// total evaporation
		double TotTrans;				// total transpiration
		double TotPrec;					// total precipitation
		double TotWater;				// total soil water
		double TotGrossPsn;				// total gpp
		double NPPFolYr;				// foliar npp, g dry matter m-2
		double NPPWoodYr;				// wood npp, g dry matter m-2
		double NPPRootYr;				// root npp, g dry matter m-2
		double ET;						// ET
		double NEP;						// net ecosystem production, -NEE
		double NetCBal;					// net C balance, NEE
		double SoilDecResp;				// soil decomposition
		double BudN;					// bud N or foliar total N
		double SoilDecRespYr;			// yearly soil decomposition
		double WoodDecRespYr;			// yearly deadwood decay loss as CO2
		double DelAmax;					// Amax adjustor for CO2 effect
		double DWUE;					// WUE adjustor for CO2 effect
		double CanopyDO3Pot;			// potential O3 effect on photosynthesis for the whole canopy
		double DroughtO3Frac;			// drought effect on O3 effect
		double TotO3Dose;				// total O3 dose
		double RootMassN;				//root N
		double TotalLitterMYr;			// total yearly litter mass
		double TotalLitterNYr;			// total yearly litter N
		double GrossNImmobYr;			// total yearly gross N immoblized
		double GrossNMinYr;				// total yearly gross N mineralization
		double PlantNUptakeYr;			// yearly plant uptake N
		double NetNitrYr;				// yearly net Nitrification rate
		double NetNMinYr;				// yearly net mineralization rate
		double FracDrain;				// proportion of drainage to total water
		double NDrainYr;				// yearly N drained out
		double NDrain;					// drained N, gN m-2
		double NDrainMgL;				// drained N concentration in water, mgN l-1
		double WoodDecResp;				// wood decay
		double TotalLitterM;			// total litter mass
		double TotalLitterN;			// total litter N
		double FolN;					// total foliar N
		double FolC;					// total foliar C
		double TotalN;					// total N
		double TotalM;					// total mass
		double NO3;						// NO3 content
		double NH4;						// NH4 content
		
		double FolNConOld; 				// to store FolN for output.
		double	NdepTot;				// total N deposition


		//Shared variables with initial conditions
		double FolMass;   //double FolMass=veg.FolMassMin;   In PnET-Day only,g/m2
		double BudC;    //double BudC=(veg.FolMassMax-double FolMass)*veg.CFracBiomass;  In PnET-Day only
		double Water;		// soil water
		double DeadWoodM;	// dead wood mass
		double WoodC;		//  wood C pool for wood growth
		double PlantC;		// plant C pool to store non structure C
		double RootC;		// Root C pool for root dynamic growth
		double LightEffMin;	// minimum light effect for next year foliar growth
		double NRatio;			// N stress index
		double PlantN;			// plant N pool
		double WoodMass;		// wood mass
		double RootMass;		// root mass
		double HOM;				// soil som
		double HON;				// soil son
		double RootNSinkEff;	// root N uptake capability
		double WUEO3Eff;		// O3 effect on WUE
		double WoodMassN;		// live wood total N
		double DeadWoodN;		// dead wood total N
		double NRatioNit;		// Nitrification constant determined by Nratio
		double NetNMinLastYr; 	// previous year net N mineralizatio rate
		double DWater;	  		// water stress for plant growth

		double LightEffCBal;	// light effect at foliar light compensation point.
		double LightEffCBalTot;	// total light effect at foliar light compensation point at growing season
		double LightEffCBalIx;	// number of days for LightEffCBal > 0.

		double O3Effect[51];	// O3 effect for each canopy layer

		double avgPCBM;			// average light effect
		double AvgDWater;		// average water stress

		double TaveYr;			// annual average air T, degree
		double PARYr;			// annual average PAR, umol m-2 s-1 at daytime

		double WoodProdCMo;
		double RootProdCMo;

		double Tsoil;
		double WFPS;

		double RnY2;
		double RnX2;
		double RnY1;
		double RnX1;
		double RnMax;

		double FluxN2OYrDe;
		double FluxNOYrDe;
		double FluxN2OYr;
		double FluxNOYr;
		double FluxN2Yr;

		double TotCanopyGrossPsnMax;

		double s1,s2;

		int O3EffectOnPSN;
		int ifO3EffectOnPSN;
		double kO3Eff;		// empirically derived ozone response coefficient, 2.6 * power(10,-6)/ ppb-O3 for hardwood forests
		double CanopyDO3;			//  O3 effect on photosynthesis for the whole canopy
		double CanopyDO3Avg;			// Avg O3 effect on photosynthesis for the whole canopy for the whole growing season
		double CanopyDO3Tot;			// Total O3 effect on photosynthesis for the whole canopy for the whole growing season
		int CanopyDO3TotDay;			// Total days of O3 effect on photosynthesis for the whole canopy for the whole growing season
		double kO3EffScalar;		// empirically derived ozone response coefficient, 2.6 * power(10,-6)/ ppb-O3 for hardwood forests


		int yrspin; // number of year for spin up
		int distid;

	} share_struct;

	//a structure to hold output data
	typedef struct
	{
		int length;			// length of years

		double*	grosspsn;			// monthly gross psn
		double*	netpsn;				// monthly net psn
		double*	netcbal;
		double*	vpd;
		double*	folmass;
		double*	plantnMo;
		double*	nppfol;
		double*	nppwood;
		double*	npproot;
		double*	nep;
		double*	gpp;
		double*	waterstress;
		double*	trans;
		double*	soilwater;
		double*	psn;				// yearly net psn
		double*	drain;
		double*	prec;
		double*	evap;
		double*	et;
		double*	plantc;
		double*	budc;
		double*	woodc;
		double*	rootc;
		double*	plantnYr;
		double*	budn;
		double*	ndrain;
		double*	netnmin;
		double*	grossnmin;
		double*	nplantuptake;
		double*	grossnimob;
		double*	littern;
		double*	netnitrif;
		double*	nratio;
		double*	foln;
		double*	litm;
		double*	litn;
		double*	rmresp;
		double*	rgresp;
		double*	decresp;
		
		double*	folm;
		double*	deadwoodm;
		double*	woodm;
		double*	rootm;
		double*	hom;
		double*	hon;
		double*	ndep;
		

		double*	fn2o;
		double*	fno;
		double*	fn2;
		double*	fn2ode;
		double*	fnode;


		double* v1;			// spare variables for temporary use // =Tavg
		double* v2;			// par
		double* v3;			// par





	} out_struct;




	void PathDefine(int wins);			// to define the directory path
	void pnet_run(int wins);			// control specific pnet run


	void pnet_site();					// site mode pnet
	void pnet_region();					// regional pnet
	void ReadRegionFile();				//
	void GetRegionPath(char *);


	// memory set and free functions
	void memset_out(int nyrs, out_struct* out);
	void memset_climate(clim_struct* clim);
	void memfree_all(site_struct* site,clim_struct* clim, share_struct* share, out_struct* out);
	void memfree_climate(clim_struct* clim);
	void memfree_out(out_struct* out);
	void memfree_site(site_struct* site);
	//	void memfree_share(share_struct* share);



	// specific for PnET-Day 
	void pnet_day();

	// specific for PnET-II 
	void pnet_ii();
	void SoilResp(veg_struct* veg, share_struct* share,int rstep);


// specific for PnET-CN
	void pnet_cn();
	int getdays(int year,int doy); // calculate number of days for each month
	void AllocateMo(veg_struct* veg, share_struct* share, int rstep, int CN_Mode);
	void AllocateYr(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share, int CN_Mode);
	void AtmEnviron(site_struct* site, clim_struct* clim, int rstep, share_struct* share);
	void CNTrans(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share);
	void Decomp( site_struct * site,veg_struct* veg, clim_struct* clim, int rstep, share_struct* share);
	void Leach(share_struct* share);
	void Phenology(veg_struct* veg, clim_struct* clim, int rstep, share_struct* share, int GrowthPhase);
	void Photosyn(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share);
	void Waterbal(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share);

	void initvars(site_struct* site,veg_struct* veg, share_struct* share);
	void init_out(int nyrs, out_struct* out);
	void YearInit(share_struct* share);

	void ReadInput(site_struct* site,veg_struct* veg, share_struct* share,char* inputname);
	void ReadClim(clim_struct* clim, char* climname);
	
	void storeoutput(veg_struct* veg, share_struct* share, out_struct* out, int rstep, int* ystep, int NewYear);
	void WriteoutMo(site_struct* site,veg_struct* veg,clim_struct* clim, share_struct* share,int rstep,FILE* &fileoutM);
	void WriteoutYr(clim_struct* clim,out_struct* out);
	void WriteoutRgn(int ngrid,out_struct* out,char* filename);
	void StoreoutRgn(int grid,int nyrs,out_struct* out,out_struct* outrgn);




	// specific for PnET-Daily
	void pnet_daily();
	void ReadClimDay(clim_struct* clim,char* climname);
	void WriteoutDay(site_struct* site,veg_struct* veg,clim_struct* clim, share_struct* share,int rstep,FILE* &fileout);
	int getmonth(int year,int doy);  // get the month of doy
	int is_leapyear(int year); // 1: leap year; 0: non leap year

	void Photosyn_hour(site_struct* site, veg_struct* veg, clim_struct* clim, int rstep, share_struct* share);
	void ReadClimHour(clim_struct* clim, char* climname);

	// pre and post processes
	void clim_day_mon(site_struct* site,share_struct* share,clim_struct* climday,clim_struct* climmon); // daily climate to monthly
	void clim_day_fill(site_struct* site,share_struct* share,clim_struct* climday,clim_struct* climmon); // daily climate to monthly
	void write_clim(clim_struct* clim,int i);


	int Disturbance(site_struct* site, veg_struct* veg, share_struct* share,int scenario);

};

