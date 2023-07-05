/*
 * funzioni.cpp
 *
 *  Created on: 5 giu 2019
 *      Author: Ivan Iudice
 */

#include "ads_b_draft_lib.h"

int latz = 15;

int encode_alt_modes(float alt, int bit13) {

	int mbit = 0;
    int qbit = 1;
	int tmp1, tmp2;
	int encalt = (int(alt) + 1000) / 25;

	if (bit13) {
		tmp1 = (encalt & 0xfe0) << 2;
		tmp2 = (encalt & 0x010) << 1;
	}
	else {
		tmp1 = (encalt & 0xff8) << 1;
		tmp2 = 0;
	}
	return (encalt & 0x0F) | tmp1 | tmp2 | (mbit << 6) | (qbit << 4);
}

int nz(int ctype) {

	return (4 * latz - ctype);

}

float dlat(int ctype, int surface) {

	float tmp;
	int nzcalc = nz(ctype);

	if (surface == 1)
		tmp = 90.0;
	else
		tmp = 360.0;

	if (nzcalc == 0)
		return tmp;
	else
		return tmp / nzcalc;
}

float nl(float declat_in) {

	if (abs(declat_in) > 87.0)
		return 1.0;
	else if (abs(declat_in) < 87.0)
		//return floor((2.0 * PI) * acos(1.0 - 1 / (1.0 - cos(PI / (2.0 * latz))) / (cos((PI / 180.0) * abs(declat_in)) * cos((PI / 180.0) * abs(declat_in)))));
		return floor((2.0 * PI) / acos(1.0 - (1.0 - cos(PI / (2.0 * latz))) / (cos((PI / 180.0) * declat_in) * cos((PI / 180.0) * declat_in))));
	else if (declat_in == 0)
		return 59.0;
	else
		return 2.0;
}

float dlon(float declat_in, int ctype, int surface) {

	float tmp, nlcalc;

	if (surface)
		tmp = 90.0;
	else
		tmp = 360.0;

	nlcalc = nl(declat_in) - ctype;

	if (nlcalc < 1.0)
		nlcalc = 1.0;

	return tmp / nlcalc;
}

coordinate cpr_encode(float lat, float lon, int ctype, int surface) {

	float scalar, dlati, dloni, rlat;
	coordinate coord;

	if (surface)
		scalar = pow(2.0, 19);
	else
		scalar = pow(2.0, 17);

	//encode using 360 constant for segment size.
	dlati = dlat(ctype, 0);
	coord.yz = floor(scalar * (fmod(lat, dlati) / dlati) + 0.5);
	rlat = dlati * ((coord.yz / scalar) + floor(lat / dlati));

	//encode using 360 constant for segment size.
	dloni = dlon(lat, ctype, 0);
	coord.xz = floor(scalar * (fmod(lon, dloni) / dloni) + 0.5);

	coord.yz = (int)coord.yz & (int)(pow(2,17) -1);
	coord.xz = (int)coord.xz & (int)(pow(2,17) -1);

	return coord; //lat, lon
}

void hex2bin(char* hexstr, int hexstr_len, char* binstr) {

	//Convert a hexdecimal string to binary string, with zero fillings

	//Lettere in maiuscolo
	int binstr_len = 0;
	char tmp_str[5];
	binstr[0] = '\0';
	for (int i = 0; i < hexstr_len;i++) {
		switch (hexstr[i]) {
		case '0':
			strcpy(tmp_str, "0000");
			break;
		case '1':
			strcpy(tmp_str, "0001");
			break;
		case '2':
			strcpy(tmp_str, "0010");
			break;
		case '3':
			strcpy(tmp_str, "0011");
			break;
		case '4':
			strcpy(tmp_str, "0100");
			break;
		case '5':
			strcpy(tmp_str, "0101");
			break;
		case '6':
			strcpy(tmp_str, "0110");
			break;
		case '7':
			strcpy(tmp_str, "0111");
			break;
		case '8':
			strcpy(tmp_str, "1000");
			break;
		case '9':
			strcpy(tmp_str, "1001");
			break;
		case 'A':
			strcpy(tmp_str, "1010");
			break;
		case 'B':
			strcpy(tmp_str, "1011");
			break;
		case 'C':
			strcpy(tmp_str, "1100");
			break;
		case 'D':
			strcpy(tmp_str, "1101");
			break;
		case 'E':
			strcpy(tmp_str, "1110");
			break;
		default:
			strcpy(tmp_str, "1111");
			break;
}

		strcat(binstr, tmp_str);
	}

	return;
}

int bin2int(char* binstr,int binstr_len) {

	//Convert a binary string to integer
	int x = 0;
	for (int i = 0;i < binstr_len;i++)

		if (binstr[i] == '1')
			x += 1 << (binstr_len - 1 - i);

	return x;

}

int get_parity(char* msg, int extended) {

	int msg_len=sizeof(msg);

	char payload[msg_len - 24], parity[24];

	for (int i = 0;i < msg_len - 24;i++)
		payload[i] = msg[i];

	for (int i = msg_len - 24;i < msg_len;i++)
		parity[i] = msg[i];

	int data, data1, data2;
	data = bin2int(payload,32);

	if (extended) {
		data1 = bin2int(payload+32,32);
		data2 = bin2int(payload+64,msg_len-24-64) << 8;
	}

	int hex_id = bin2int(parity,24) << 8;

    for (int i = 0;i < (msg_len - 24);i++) {

		if ((data & 0x80000000) != 0)
			data ^= 0xFFFA0480;
		data <<= 1;

		if (extended) {
			if ((data1 & 0x80000000) != 0)
				data |= 1;
			data1 <<= 1;

			if ((data2 & 0x80000000) != 0)
				data1 = data1 | 1;
			data2 <<= 1;

		}

	}

	return data;

}

void df17_pos_rep_encode(unsigned char* df17, unsigned char parity, int ca, int icao, int tc,
	int ss, int nicsb, float alt, int time, float lat, float lon, int surface) {

		int format = 17;
		int enc_alt = encode_alt_modes(alt, surface);

		coordinate enc = cpr_encode(lat, lon, (int) (parity & 1), surface);

		df17[0] = ((format << 3) | ca);
		df17[1] = ((icao >> 16) & 0xff);
		df17[2] = ((icao >> 8) & 0xff);
		df17[3] = ((icao) & 0xff);

		// data
		df17[4] = ((tc << 3) | (ss << 1) | nicsb);
		df17[5] = ((enc_alt >> 4) & 0xff);
		df17[6] = ((enc_alt & 0xf) << 4 | (time << 3) | (parity << 2) | (enc.yz >> 15));
		df17[7] = ((enc.yz >> 7) & 0xff);
		df17[8] = (((enc.yz & 0x7f) << 1) | (enc.xz >> 16));
		df17[9] = ((enc.xz >> 8) & 0xff);
		df17[10] = ((enc.xz) & 0xff);

}
