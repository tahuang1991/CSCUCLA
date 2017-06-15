#include "TF1.h"
#include "TGraphErrors.h"

#include "DataFormats/Math/interface/deltaPhi.h"
#include "CSCUCLA/CSCDigiTuples/include/PtassignmentHelper.h"
//#include "GEMCode/GEMValidation/interface/DisplacedMuonTriggerPtassignment.h"
#include <iostream>
#include <math.h>       /* atan */



void PtassignmentHelper::calculateAlphaBeta(const std::vector<float>& v, 
                        const std::vector<float>& w, 
                        const std::vector<float>& ev, 
                        const std::vector<float>& ew, 
                        const std::vector<float>& status,
                        float& alpha, float& beta)
{
  //std::cout << "size of v: "<<v.size() << std::endl; 
  
  if (v.size()>=3) {
  
  float zmin;
  float zmax;
  if (v.front() < v.back()){
    zmin = v.front();
    zmax = v.back();
  }
  else{
    zmin = v.back();
    zmax = v.front();
  }

  TF1 *fit1 = new TF1("fit1","pol1",zmin,zmax); 
  //where 0 = x-axis_lowest and 48 = x_axis_highest 
  TGraphErrors* gr = new TGraphErrors(v.size(),&(v[0]),&(w[0]),&(ev[0]),&(ew[0]));
  gr->SetMinimum(w[2]-5*0.002);
  gr->SetMaximum(w[2]+5*0.002);
 
  gr->Fit(fit1,"RQ"); 
  
  alpha = fit1->GetParameter(0); //value of 0th parameter
  beta  = fit1->GetParameter(1); //value of 1st parameter

  delete fit1;
  delete gr;
  }
  else {alpha = -99; beta= 0.0;}
}



float PtassignmentHelper::normalizePhi(float phi) { 
    float result = phi;
    if(result > float(M_PI)) result -= float(2*M_PI);
    else if (result <= -float(M_PI)) result += float(2*M_PI);
    return result;
}

