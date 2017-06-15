#ifndef GEMCode_GEMValidation_Ptassignment_h
#define GEMCode_GEMValidation_Ptassignment_h

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

namespace PtassignmentHelper {


void calculateAlphaBeta(const std::vector<float>& v, 
                        const std::vector<float>& w, 
                        const std::vector<float>& ev, 
                        const std::vector<float>& ew, 
                        const std::vector<float>& status,
                        float& alpha, float& beta);

float normalizePhi(float phi);

}

#endif
