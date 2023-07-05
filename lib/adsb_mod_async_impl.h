/* -*- c++ -*- */
/*
 * Copyright 2023 Ivan Iudice.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_SPOOF1090_ADSB_MOD_ASYNC_IMPL_H
#define INCLUDED_SPOOF1090_ADSB_MOD_ASYNC_IMPL_H

#include <gnuradio/spoof1090/adsb_mod_async.h>

namespace gr {
namespace spoof1090 {

class adsb_mod_async_impl : public adsb_mod_async
{
private:
    void handler_function(pmt::pmt_t pmt);
    std::string write_string(pmt::pmt_t obj);
    void string_list_to_byte_array(const std::string str, unsigned  char* array);
    void ads_b_1090es_preamble(unsigned char* ppm);
    void ads_b_1090es_mapping(unsigned char* data, unsigned char* ppm);

public:
    adsb_mod_async_impl();
    ~adsb_mod_async_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace spoof1090
} // namespace gr

#endif /* INCLUDED_SPOOF1090_ADSB_MOD_ASYNC_IMPL_H */
