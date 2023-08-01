/* -*- c++ -*- */
/*
 * Copyright 2023 Ivan Iudice.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "adsb_mod_async_impl.h"
#include <gnuradio/io_signature.h>
#include <pmt/pmt.h>
#include <iostream>
#include <sstream>
#include <streambuf>

namespace gr {
namespace spoof1090 {

adsb_mod_async::sptr adsb_mod_async::make()
{
    return gnuradio::make_block_sptr<adsb_mod_async_impl>();
}


/*
 * The private constructor
 */
adsb_mod_async_impl::adsb_mod_async_impl()
    : gr::block("adsb_mod_async",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(0, 0, 0))
{
    message_port_register_in(pmt::mp("in"));
    message_port_register_out(pmt::mp("out"));
    set_msg_handler(pmt::mp("in"), [this](const pmt::pmt_t& msg) { handler_function(msg); });
}

    void
    adsb_mod_async_impl::handler_function(pmt::pmt_t msg)
    {
      // Per l'ingresso da python
      /*std::string s = adsb_mod_async_impl::write_string(msg);
      unsigned char* packet = new unsigned char [14];
      adsb_mod_async_impl::string_list_to_byte_array((const std::string) s, packet);*/

      // per l'ingresso da C++
      pmt::pmt_t bit_data = pmt::cdr(msg);
      unsigned char* packet = new unsigned char [14];
      memcpy(packet, pmt::blob_data(bit_data), 14);

      unsigned char* burst = new unsigned char [240];
      adsb_mod_async_impl::ads_b_1090es_preamble(burst);
      adsb_mod_async_impl::ads_b_1090es_mapping(packet, burst+16);

      delete packet;

      //std::string out_str(burst,240);
      //pmt::pmt_t pdu = pmt::string_to_symbol(out_str);

      pmt::pmt_t vecpmt(pmt::make_blob(&burst[0], 240));
      pmt::pmt_t pdu(pmt::cons(pmt::PMT_NIL, vecpmt));

      delete burst;

      //pmt::pmt_t pdu = pmt::PMT_NIL;
      //pmt::print(pdu);

      message_port_pub(pmt::mp("out"), pdu);
    }

    void
    adsb_mod_async_impl::ads_b_1090es_preamble(unsigned char* ppm)
    {
      char preamble[17]="\x1\x0\x1\x0\x0\x0\x0\x1\x0\x1\x0\x0\x0\x0\x0\x0";
      //char preamble[17]="1010000101000000";
      for (int i = 0; i < 16; i++) {
	ppm[i] = preamble[i];
      }
    }

    void
    adsb_mod_async_impl::ads_b_1090es_mapping(unsigned char* data, unsigned char* ppm)
    {
      int i, j;
      for (i = 0; i < 14; i++) {
	for (j = 0; j < 8; j++) {
	  if ( ((data[i] >> 7-j) & 1) ) {
	    ppm[i*8*2+j*2]=1;
	    ppm[i*8*2+j*2+1]=0;
	  }
	  else {
	    ppm[i*8*2+j*2]=0;
	    ppm[i*8*2+j*2+1]=1;
	  }
        }
      }
    }

    std::string
    adsb_mod_async_impl::write_string(pmt::pmt_t obj)
    {
      std::ostringstream s;
      s << obj;
      return s.str();
    }

    void
    adsb_mod_async_impl::string_list_to_byte_array(const std::string str, unsigned char* byte_array)
    {
      char tmp[11];
      int array[14];
      tmp[0]='\0';
      int len = 0;
      int N = 0;
      bool store = 0;
      for (int i = 0; i < str.length(); i++) {
        if (str[i] == '-' | std::isdigit(str[i])) {
	  tmp[len++] = str[i];
          tmp[len] = '\0';
          store = 1;
        }
        else {
          if (store) {
            std::string num(tmp);
            std::istringstream num_stream(num);
	    num_stream >> array[N];
	    byte_array[N] = (unsigned char) array[N];
	    N++;
	    len = 0;
            tmp[0] = '\0';
            store = 0;
	  }
	}
      }
    }

/*
 * Our virtual destructor.
 */
adsb_mod_async_impl::~adsb_mod_async_impl() {}

} /* namespace spoof1090 */
} /* namespace gr */
