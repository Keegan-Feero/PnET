####################################################
## This makefile is to compile and link project and  create executable "pnet"
####################################################
#CPPFLAGS :=-Wall -Werror 
CPPFLAGS :=-g -Werror

OBJS= pnet_AllocateMo.o \
			pnet_AllocateYr.o \
			pnet_AtmEnviron.o \
			pnet_cn.o \
			pnet_CNTrans.o \
			pnet_Decomp.o \
			pnet_initvars.o \
			pnet_Leach.o \
			pnet_main.o \
			pnet_model.o \
			pnet_Phenology.o \
			pnet_Photosyn.o \
			pnet_ReadData.o \
			pnet_storeoutput.o \
			pnet_Waterbal.o \
			pnet_YearInit.o \
			pnet_run.o 	\
			pnet_memoryset.o \
			pnet_ii.o \
			pnet_day.o \
			pnet_region.o \
			pnet_daily.o	\
			
all: pnet
			
pnet: $(OBJS)
		g++ $(CPPFLAGS) -o pnet  $(OBJS)
								
pnet_AllocateMo.o : pnet_AllocateMo.cpp  pnet_model.h  
		g++ -c  $(CPPFLAGS) pnet_AllocateMo.cpp
pnet_AllocateYr.o : pnet_AllocateYr.cpp  pnet_model.h 
		g++ -c $(CPPFLAGS) pnet_AllocateYr.cpp
pnet_AtmEnviron.o : pnet_AtmEnviron.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_AtmEnviron.cpp
pnet_cn.o : pnet_cn.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_cn.cpp
pnet_CNTrans.o  : pnet_CNTrans.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_CNTrans.cpp
pnet_Decomp.o : pnet_Decomp.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_Decomp.cpp
pnet_initvars.o : pnet_initvars.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_initvars.cpp
pnet_Leach.o : pnet_Leach.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_Leach.cpp
pnet_main.o : pnet_main.cpp pnet_model.h
		g++ -c $(CPPFLAGS) pnet_main.cpp
pnet_model.o : pnet_model.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_model.cpp
pnet_Phenology.o  : pnet_Phenology.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_Phenology.cpp
pnet_Photosyn.o : pnet_Photosyn.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_Photosyn.cpp
pnet_ReadData.o  : pnet_ReadData.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_ReadData.cpp
pnet_storeoutput.o : pnet_storeoutput.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_storeoutput.cpp
pnet_Waterbal.o : pnet_Waterbal.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_Waterbal.cpp
pnet_YearInit.o  :	pnet_YearInit.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_YearInit.cpp				

pnet_run.o  :	pnet_run.cpp  pnet_model.h
		g++ -c $(CPPFLAGS) pnet_run.cpp			
		
pnet_memoryset.o  :	pnet_memoryset.cpp pnet_model.h
		g++ -c $(CPPFLAGS) pnet_memoryset.cpp	

pnet_ii.o  :	pnet_ii.cpp pnet_model.h
		g++ -c $(CPPFLAGS) pnet_ii.cpp		

pnet_day.o  :	pnet_day.cpp pnet_model.h
		g++ -c $(CPPFLAGS) pnet_day.cpp				

pnet_region.o :	pnet_region.cpp pnet_model.h
		g++ -c $(CPPFLAGS) pnet_region.cpp				

pnet_daily.o :	pnet_daily.cpp pnet_model.h
		g++ -c $(CPPFLAGS) pnet_daily.cpp		



	
clean:
		rm -f *.o #* *~
cleanall:
		rm -f *.o #* pnet pnet.exe				