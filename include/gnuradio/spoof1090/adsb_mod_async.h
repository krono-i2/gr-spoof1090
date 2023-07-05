/* -*- c++ -*- */
/*
 * Copyright 2023 Ivan Iudice.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_SPOOF1090_ADSB_MOD_ASYNC_H
#define INCLUDED_SPOOF1090_ADSB_MOD_ASYNC_H

#include <gnuradio/spoof1090/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace spoof1090 {

/*!
 * \brief <+description of block+>
 * \ingroup spoof1090
 *
 */
class SPOOF1090_API adsb_mod_async : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<adsb_mod_async> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of spoof1090::adsb_mod_async.
     *
     * To avoid accidental use of raw pointers, spoof1090::adsb_mod_async's
     * constructor is in a private implementation
     * class. spoof1090::adsb_mod_async::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace spoof1090
} // namespace gr

#endif /* INCLUDED_SPOOF1090_ADSB_MOD_ASYNC_H */
