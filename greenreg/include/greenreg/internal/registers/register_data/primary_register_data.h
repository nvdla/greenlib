/*
Copyright (c) 2008, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// ChangeLog GreenSocs
// 2009-01-21 Christian Schroeder: changed namespace
// 2009-01-29 Christian Schroeder: renamings
// 2009-01-29 FURTHER CHANGES SEE SVN LOG!


#ifndef PRIMARY_REGISTER_DATA_H_
#define PRIMARY_REGISTER_DATA_H_

#include <boost/config.hpp>
#include <systemc>

#include "greenreg/utils/storage/data_store_accessor.h"

#include "greenreg/internal/registers/register_data/I_register_data.h"

namespace gs {
namespace reg {

class register_container;

////////////////////////////////////////////
/// primary_register_data
/// register data that is bound to the block.
/// all register types will ultimately have one and only
/// one of these type of data definitions.  This allows
/// the seperation of input and output buffers by
/// utilizing the input and output buffer designations
/// in I_register for callback handling while never knowing
/// how the regsiters data buffer(s) are actually configured.
/// @author 
/// @since 
////////////////////////////////////////////
class primary_register_data : public I_register_data, public sc_core::sc_module
{
public:

	SC_HAS_PROCESS( primary_register_data);

	////////////////////////////////////////////
	/// Constructor
	/// Width is unimportant here, as it is controlled by the actual register
	///
	///	@param _name ? name of register data
	/// @param _register ? reference to register that will own this data
	/// @param _register_container ? container register is bound to
	/// @param _offset ? offset of register within device
	/// @param _data ? default data
	/// @param _write_mask ? write mask for this data buffer
	/// @param _lock_mask ? lock mask for this data buffer
	///
	/// @see I_register
	////////////////////////////////////////////
	primary_register_data( sc_core::sc_module_name _name,
		I_register & _register, register_container & _register_container,
		uint_gr_t _offset, uint_gr_t _data, uint_gr_t _write_mask, uint_gr_t _lock_mask);

	////////////////////////////////////////////
	/// Destructor
	////////////////////////////////////////////
	virtual ~primary_register_data();

	/// @see I_register_data
	inline virtual operator uint_gr_t ()
	{ return( m_data.read()); }

	/// @see I_register_data
	inline virtual I_register_data& operator = ( const uint_gr_t & _val)
	{
		set( _val);
		return( *this);
	}

	/// @see I_register_data
	inline virtual uint_gr_t get()
	{ return( m_data.read()); }

	/// @see I_register_data
	virtual void set( uint_gr_t _data, bool _warn_write_mask = true);

	/// @see I_register_data
	inline virtual void put( uint_gr_t _data)
	{ set( _data);}

	/// @see I_register_data
	inline virtual void set_write_mask( uint_gr_t _mask)
	{ m_write_mask = _mask; }

	/// @see I_register_data
	inline virtual uint_gr_t & get_write_mask()
	{ return( m_write_mask); }

	/// @see I_register_data
	inline virtual void set_lock_mask( uint_gr_t _mask)
	{ m_lock_mask = _mask; }

	/// @see I_register_data
	inline virtual uint_gr_t & get_lock_mask()
	{ return( m_lock_mask); }

protected:
	friend class gr_register_sharedio;
	friend class gr_register_splitio;
	friend class register_container;

	gs::reg_utils::data_store_accessor m_data;

	////////////////////////////////////////////
	/// on_received_write_request
	/// callback associated with this register data
	/// which occurs when ther register block receives a
	/// write request.
	///
	/// @param _value ? value to be written
  /// @param _transaction ? transaction initiating this call
  /// @param _delayed ? if the notifications shall be delayed or called directly
	////////////////////////////////////////////
	void on_received_write_request(uint_gr_t _value, transaction_type* _transaction, bool _delayed);

	////////////////////////////////////////////
	/// on_received_read_request
	/// callback associated with this register data
	/// which occurs when ther register block receives a
	/// read request.
	///
  /// @param _transaction ? transaction initiating this call
  /// @param _delayed ? if the notifications shall be delayed or called directly
	/// @return value to return
	////////////////////////////////////////////
	uint_gr_t on_received_read_request(transaction_type* _transaction, bool _delayed);
};

} // end namespace gs:reg
} // end namespace gs:reg

#endif /*PRIMARY_REGISTER_DATA_H_*/
