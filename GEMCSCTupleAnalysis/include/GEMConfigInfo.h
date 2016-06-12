/*
 * GEMConfigInfo.h
 *
 *  Created on: Jun 12, 2016
 *      Author: nmccoll
 */

#ifndef GEMCSCTUPLEANALYSIS_INCLUDE_GEMCONFIGINFO_H_
#define GEMCSCTUPLEANALYSIS_INCLUDE_GEMCONFIGINFO_H_

#include <string>

struct GEMConfigInfo {
  std::string dataDir               = "data/";
  std::string geoName               = "gemGeo.txt";
  std::string vFATChanMapName       = "slot_table_904_june09.csv";
  std::string stripChannelMap_0_1   = "v2b_schema_chips0-1.csv";
  std::string stripChannelMap_16_17 = "v2b_schema_chips16-17.csv";
  std::string stripChannelMap_18_23 = "v2b_schema_chips18-23.csv";
  std::string stripChannelMap_2_15  = "v2b_schema_chips2-15.csv";
};


#endif /* GEMCSCTUPLEANALYSIS_INCLUDE_GEMCONFIGINFO_H_ */
