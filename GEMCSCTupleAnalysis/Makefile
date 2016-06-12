LIBDIR        = lib
SRCDIR        = source
INTDIR        = include
CINTDIR       = cint
UNPACKDIR     = ../gem-light-dqm/gemtreewriter/lib/

include $(ROOTSYS)/etc/Makefile.arch

CXXFLAGS += -I../.. -I.

ifeq ($(PLATFORM),macosx)
CXXFLAGS += -std=c++11
endif

OBJS = $(LIBDIR)/BaseTupleAnalyzer.o $(LIBDIR)/GEMGeoInfo.o $(LIBDIR)/GEMInfo.o $(LIBDIR)/BaseTupleAnalyzer.o  $(LIBDIR)/BaseCSCAndGEMAnalyzer.o
INTS = $(INTDIR)/BaseTupleAnalyzer.h $(INTDIR)/GEMGeoInfo.h $(INTDIR)/GEMInfo.h   $(LIBDIR)/BaseTupleAnalyzer.h   $(LIBDIR)/BaseCSCAndGEMAnalyzer.h

libCSCTupleAnalysis.so: $(OBJS)
ifeq ($(PLATFORM),macosx)
	$(LD) $(SOFLAGS)$@ $(LDFLAGS) $(UNPACKDIR)libEvent.so $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
else
	$(LD) $(SOFLAGS) $(LDFLAGS) $(UNPACKDIR)libEvent.so $^ $(OutPutOpt) $@ $(EXPLLINKLIBS) 
endif

# no need for a dictionary
#CSCTupleAnalysisDict.cc: $(INTS) $(SRCDIR)/CSCTupleAnalysis_LinkDef.h
#	@echo "Generating dictionary $@..."
#	rootcling  -v -l -f $@ -c -I../.. $^ 
#$(LIBDIR)/CSCTupleAnalysisDict.o: CSCTupleAnalysisDict.cc
#	$(CXX)  $(CXXFLAGS) -c $<  -o $@	

$(LIBDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $<  -o $@

all:  libHistoPlotting.so
	
# =============================== 
clean: 
	rm -f $(LIBDIR)/* $(CINTDIR)/* CSCTupleAnalysisDict* libCSCTupleAnalysis.so
# =============================== # DO NOT DELETE
# DO NOT DELETE
