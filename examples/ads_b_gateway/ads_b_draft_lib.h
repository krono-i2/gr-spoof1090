/*
 * funzioni.h
 *
 *  Created on: 5 giu 2019
 *      Author: Ivan Iudice
 */

#ifndef FUNZIONI_H_
#define FUNZIONI_H_

#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>

using namespace std;

#define PI 3.14

struct coordinate {
	int xz;
	int yz;
};

int encode_alt_modes(float alt, int bit13);
int nz(int ctype);
float dlat(int ctype, int surface);
float nl(float declat_in);
float dlon(float declat_in, int ctype, int surface);
coordinate cpr_encode(float lat, float lon, int ctype, int surface);

#define GENERATOR  "1111111111111010000001001"

void hex2bin(char* hexstr, int hexstr_len, char* binstr);
int bin2int(char* binstr,int binstr_len);

#define REMINDER_LEN 24

void df17_pos_rep_encode(unsigned char* df17, unsigned char parity, int ca, int icao, int tc,
	int ss, int nicsb, float alt, int time, float lat, float lon, int surface);

#endif /* FUNZIONI_H_ */
