#ifndef ADS_B_NET_LIB_H
#define ADS_B_NET_LIB_H

#include <string>
#include <iostream>
#include "ads_b_draft_lib.h"

namespace cira {

  class aircraft {
    
    private:
      // Attributes
      std::string icao;
      float altitude;
      float longitude;
      float latitude;
      unsigned char ads_b_enc[14];
      unsigned char parity;
      // Private methods
      void hex2bytes(const std::string in, unsigned char* data);
      void ads_b_1090es_crc(unsigned char* msg, unsigned char* crc);
      void ads_b_1090es_crc(unsigned char* msg);

    public:
      // Public methods
      aircraft(const std::string &in_icao, const float in_altitude, const float in_longitude,
	const float in_latitude, unsigned char parity, const std::string &enc = "");
      ~aircraft();
      void print_bytes();
      void getMsg(unsigned char* msg);

  };

}

#endif
