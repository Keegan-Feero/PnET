#include "pnet_model.h"

void pnet_model::Leach(share_struct* share)
{
	//
	//PnET-CN leaching routine
	//

	share->NDrain = share->FracDrain * share->NO3;
	share->NDrainYr = share->NDrainYr + share->NDrain;
	share->NDrainMgL = (share->NDrain * 1000) / (share->Drainage * 10+1.0E-6);  //to convert to mg/l
	share->NO3 = share->NO3 - share->NDrain;

}
