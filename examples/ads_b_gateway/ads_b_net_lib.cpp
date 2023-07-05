#include "ads_b_net_lib.h"

namespace cira {

  void aircraft::hex2bytes(const std::string in, unsigned char* data) {

    int character;
	
    for (int i = 0, j = 0; i < in.length(); i += 2, ++j) {
      sscanf(&in[i], "%2x", &character);
      data[j] = static_cast<char>(character);
    }

  }

  aircraft::aircraft(const std::string &in_icao, const float in_altitude, const float in_longitude,
    const float in_latitude, unsigned char parity, const std::string &enc) {

    icao = in_icao;
    altitude = in_altitude;
    longitude = in_longitude;
    latitude = in_latitude;

    if (enc.length() == 28) {
      
      hex2bytes(enc, ads_b_enc);

      unsigned char* crc = new unsigned char [3];
      ads_b_1090es_crc(ads_b_enc, crc);

      if ((crc[0] | crc[1] | crc[2]) == 0)
	return;

      delete crc;

    }

    // Using functions from Aniello Oliviero
    //#######################################//

    int ca = 5;
    int tc = 11;
    int ss = 0;
    int nicsb = 0;
    int time = 0;
    bool surface = false;

    unsigned char* icao_bytes = new unsigned char [3];
    hex2bytes(icao, icao_bytes);

    int icao_int = ((icao_bytes[0] << 16) & 0xff0000) | ((icao_bytes[1] << 8) & 0xff00) | (icao_bytes[2] & 0xff);

    df17_pos_rep_encode(ads_b_enc, parity, ca, icao_int, tc,
	ss, nicsb, altitude, time, latitude, longitude, surface);

    //#######################################//

    delete icao_bytes;

    for (int i = 0; i < 3; i++)
      ads_b_enc[11+i] = 0;

    ads_b_1090es_crc(ads_b_enc);

  }

  aircraft::~aircraft() {}

  void aircraft::print_bytes() {
    for (int i = 0; i < 14; i++) {
      for (int j = 0; j < 8; j++) {
	std::cout << (int)((ads_b_enc[i] >> (7-j)) & 1);
      }
      std::cout << std::endl;
    }

  }

  void aircraft::ads_b_1090es_crc(unsigned char* msg, unsigned char* crc) {
    
    int i, j;
    const int N_bytes_crc = 3, N_bytes_data = 14;
    char pol[] = "\xff\xf4\x09";	// Non riporto il primo bit dando per scontato sia 1

    for (i = 0; i < N_bytes_crc; i++)	
      crc[i]= msg[i];

    for (i = 0; i < (N_bytes_data-N_bytes_crc)*8; i++) {
      if ((crc[0] >> 7) & 1) {
	for (j = 0; j < N_bytes_crc - 1; j++)
	  crc[j] = (((crc[j] << 1) & 0xfe) | ((crc[j+1] >> 7) & 1)) ^ (pol[j]);

	crc[N_bytes_crc - 1] = (((crc[N_bytes_crc - 1] << 1) & 0xfe) | ((msg[i/8+N_bytes_crc] >> (7-(i+N_bytes_crc*8)%8)) & 1)) ^ (pol[N_bytes_crc-1]);
      }
      else {
	for (j = 0; j < N_bytes_crc - 1; j++)
	  crc[j] = ((crc[j] << 1) & 0xfe) | (crc[j+1] >> 7 & 1);

	  crc[N_bytes_crc - 1] = ((crc[N_bytes_crc - 1] << 1) & 0xfe) | ((msg[i/8+N_bytes_crc] >> (7-(i+N_bytes_crc*8)%8)) & 1);
      }
    }

  }

  void aircraft::ads_b_1090es_crc(unsigned char* msg) {

    unsigned char* crc = new unsigned char [3];
        
    int i, j;
    const int N_bytes_crc = 3, N_bytes_data = 14;
    char pol[] = "\xff\xf4\x09";	// Non riporto il primo bit dando per scontato sia 1

    for (i = 0; i < N_bytes_crc; i++)	
      crc[i]= msg[i];

    for (i = 0; i < (N_bytes_data-N_bytes_crc)*8; i++) {
      if ((crc[0] >> 7) & 1) {
	for (j = 0; j < N_bytes_crc - 1; j++)
	  crc[j] = (((crc[j] << 1) & 0xfe) | ((crc[j+1] >> 7) & 1)) ^ (pol[j]);

	crc[N_bytes_crc - 1] = (((crc[N_bytes_crc - 1] << 1) & 0xfe) | ((msg[i/8+N_bytes_crc] >> (7-(i+N_bytes_crc*8)%8)) & 1)) ^ (pol[N_bytes_crc-1]);
      }
      else {
	for (j = 0; j < N_bytes_crc - 1; j++)
	  crc[j] = ((crc[j] << 1) & 0xfe) | (crc[j+1] >> 7 & 1);

	  crc[N_bytes_crc - 1] = ((crc[N_bytes_crc - 1] << 1) & 0xfe) | ((msg[i/8+N_bytes_crc] >> (7-(i+N_bytes_crc*8)%8)) & 1);
      }
    }

    for (i = 0; i < 3; i++)
      msg[11+i] = crc[i];

  }

  void aircraft::getMsg(unsigned char* msg) {

    for (int i = 0; i < 14; i++)
      msg[i] = ads_b_enc[i];

  }

}
