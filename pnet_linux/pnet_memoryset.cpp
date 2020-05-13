#include "pnet_model.h"

void pnet_model::memset_out(int nyrs, out_struct* out)
{
	//allocate memory for the output structure
	
	int nmonth = nyrs*12;
	int i;

	out->grosspsn = (double*)malloc((nmonth+1)*sizeof(double)); //use nmonth+1 so that rstep can start from 1
	out->netpsn = (double*)malloc((nmonth+1)*sizeof(double));
	out->netcbal = (double*)malloc((nmonth+1)*sizeof(double));
	out->vpd = (double*)malloc((nmonth+1)*sizeof(double));
	out->folmass = (double*)malloc((nmonth+1)*sizeof(double));
	out->plantnMo = (double*)malloc((nmonth+1)*sizeof(double));

	out->nppfol = (double*)malloc((nyrs+1)*sizeof(double)); //use (nyrs+1)+1 so that ystep can start from 1
	out->nppwood = (double*)malloc((nyrs+1)*sizeof(double));
	out->npproot = (double*)malloc((nyrs+1)*sizeof(double));
	out->nep = (double*)malloc((nyrs+1)*sizeof(double));
	out->gpp = (double*)malloc((nyrs+1)*sizeof(double));
	out->waterstress = (double*)malloc((nyrs+1)*sizeof(double));
	out->trans = (double*)malloc((nyrs+1)*sizeof(double));
	out->soilwater = (double*)malloc((nyrs+1)*sizeof(double));
	out->psn = (double*)malloc((nyrs+1)*sizeof(double));
	out->drain = (double*)malloc((nyrs+1)*sizeof(double));
	out->prec = (double*)malloc((nyrs+1)*sizeof(double));
	out->evap = (double*)malloc((nyrs+1)*sizeof(double));
	out->et = (double*)malloc((nyrs+1)*sizeof(double));
	out->plantc = (double*)malloc((nyrs+1)*sizeof(double));
	out->budc = (double*)malloc((nyrs+1)*sizeof(double));
	out->woodc = (double*)malloc((nyrs+1)*sizeof(double));
	out->rootc = (double*)malloc((nyrs+1)*sizeof(double));
	out->plantnYr = (double*)malloc((nyrs+1)*sizeof(double));
	out->budn = (double*)malloc((nyrs+1)*sizeof(double));
	out->ndrain = (double*)malloc((nyrs+1)*sizeof(double));
	out->netnmin = (double*)malloc((nyrs+1)*sizeof(double));
	out->grossnmin = (double*)malloc((nyrs+1)*sizeof(double));
	out->nplantuptake = (double*)malloc((nyrs+1)*sizeof(double));
	out->grossnimob = (double*)malloc((nyrs+1)*sizeof(double));
	out->littern = (double*)malloc((nyrs+1)*sizeof(double));
	out->netnitrif = (double*)malloc((nyrs+1)*sizeof(double));
	out->nratio = (double*)malloc((nyrs+1)*sizeof(double));
	out->foln = (double*)malloc((nyrs+1)*sizeof(double));
	out->litm = (double*)malloc((nyrs+1)*sizeof(double));
	out->litn = (double*)malloc((nyrs+1)*sizeof(double));
	out->rmresp = (double*)malloc((nyrs+1)*sizeof(double));
	out->rgresp = (double*)malloc((nyrs+1)*sizeof(double));
	out->decresp = (double*)malloc((nyrs+1)*sizeof(double));  

	
	out->folm = (double*)malloc((nyrs+1)*sizeof(double));
	out->deadwoodm = (double*)malloc((nyrs+1)*sizeof(double));
	out->woodm = (double*)malloc((nyrs+1)*sizeof(double));
	out->rootm = (double*)malloc((nyrs+1)*sizeof(double));
	out->hom = (double*)malloc((nyrs+1)*sizeof(double));
	out->hon = (double*)malloc((nyrs+1)*sizeof(double));  
	out->ndep = (double*)malloc((nyrs+1)*sizeof(double)); 


	out->fn2o = (double*)malloc((nyrs+1)*sizeof(double));
	out->fno = (double*)malloc((nyrs+1)*sizeof(double));
	out->fn2 = (double*)malloc((nyrs+1)*sizeof(double));
	out->fn2ode = (double*)malloc((nyrs+1)*sizeof(double));
	out->fnode = (double*)malloc((nyrs+1)*sizeof(double));


	out->v1 = (double*)malloc((nyrs+1)*sizeof(double));
	out->v2 = (double*)malloc((nyrs+1)*sizeof(double));
	out->v3 = (double*)malloc((nyrs+1)*sizeof(double));




	if (!out->grosspsn || !out->netpsn || !out->netcbal || !out->vpd || !out->folmass || !out->plantnMo || !out->nppfol || !out->nppwood || 
		!out->npproot || !out->nep || !out->gpp || !out->waterstress || !out->trans || !out->soilwater || !out->psn || !out->drain || !out->prec || 
		!out->evap || !out->et || !out->plantc || !out->budc || !out->woodc || !out->rootc || !out->plantnYr || !out->budn || !out->ndrain || 
		!out->netnmin || !out->grossnmin || !out->nplantuptake || !out->grossnimob || !out->littern || !out->netnitrif || !out->nratio || 
		!out->foln || !out->litm || !out->litn || !out->rmresp || !out->rgresp || !out->decresp)
	{
		printf("Unable to allocate memory for out_struct!\n");	
		exit(1) ;

	}

	init_out(nyrs, out); 								// initialize out structure

}

void pnet_model::memset_climate(clim_struct* clim)
{
	//allocate memory for the climate structure

	clim->year = (int*)malloc((clim->length+1)*sizeof(int)); //use clim->length+1 so that rstep can start from 1
	clim->doy = (int*)malloc((clim->length+1)*sizeof(int));
	clim->tmax = (double*)malloc((clim->length+1)*sizeof(double));
	clim->tmin = (double*)malloc((clim->length+1)*sizeof(double));
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

	if (clim->timestep == 1)  // daily step
	{
		clim->month = (int*)malloc((clim->length+1)*sizeof(int));
		clim->day = (int*)malloc((clim->length+1)*sizeof(int));

	}


}


void pnet_model::memfree_climate(clim_struct* clim)
{

	//clim_struct
	free(clim->year);
	free(clim->doy);
	free(clim->tmax);
	free(clim->tmin);
	free(clim->par);
	free(clim->prec);
	free(clim->O3);
    free(clim->CO2);
	free(clim->NH4dep);
	free(clim->NO3dep);

	if (clim->timestep == 1)
	{
		free(clim->month);
		free(clim->day);

	}

}

void pnet_model::memfree_out(out_struct* out)
{

	//free memory

	//out_struct

	free(out->grosspsn);
	free(out->netpsn); 
	free(out->netcbal); 
	free(out->vpd); 
	free(out->folmass); 
	free(out->plantnMo); 

	free(out->nppfol); 
	free(out->nppwood);
	free(out->npproot); 
	free(out->nep);
	free(out->gpp); 

	free(out->waterstress); 
	free(out->trans); 
	free(out->soilwater); 
	free(out->psn); 
	free(out->drain); 
	free(out->prec);
	free(out->evap); 
	free(out->et); 
	free(out->plantc); 
	free(out->budc); 
	free(out->woodc); 
	free(out->rootc); 
	free(out->plantnYr); 
	free(out->budn); 
	free(out->ndrain);
	free(out->netnmin); 
	free(out->grossnmin); 
	free(out->nplantuptake); 
	free(out->grossnimob); 
	free(out->littern); 
	free(out->netnitrif); 
	free(out->nratio);
	free(out->foln); 
	free(out->litm); 
	free(out->litn); 
	free(out->rmresp); 
	free(out->rgresp); 
	free(out->decresp);

	free(out->folm); 
	free(out->deadwoodm); 
	free(out->woodm); 
	free(out->rootm); 
	free(out->hom); 
	free(out->hon);
	free(out->ndep);

	free(out->fn2o);
	free(out->fno);
	free(out->fn2);
	free(out->fn2ode);
	free(out->fnode);

	free(out->v1);
	free(out->v2);
	free(out->v3);




}


void pnet_model::memfree_site(site_struct* site)
{

	//free memory

	//site_struct

	free(site->distintensity);
	free(site->distyear);
	free(site->distremove);
	free(site->distsoilloss);
	free(site->folregen);
	free(site->distdoy);


}

void pnet_model::memfree_all(site_struct* site,clim_struct* clim, share_struct* share, out_struct* out)
{

	//free memory

	//site_struct
	memfree_site(site);

	//climate_structs
	memfree_climate(clim);

	//out_struct
	memfree_out(out);
}
