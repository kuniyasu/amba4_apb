/*
 * amba4_apb_if.h
 *
 *  Created on: 2013/12/31
 *      Author: Kuniyasu
 */

#ifndef AMBA4_APB_IF_H_
#define AMBA4_APB_IF_H_

#include<systemc.h>
#include<tlm.h>
using namespace tlm;

class PIN{};
class TLM2LT{};
class TLM2AT{};

#define TR_NAME(_name) (std::string(name())+"."+std::string(_name)).c_str()
#define PIN_NAME(bname,_name_) (std::string(bname)+"."+std::string(_name_)).c_str()

template<unsigned int ADWIDTH, unsigned int BUSWIDTH> class apb4_interface;
template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>class apb4_base_chain;
template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>class apb4_base_port;
template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>class apb4_base_export;
template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>class apb4_port;
template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>class apb4_export;
template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>class apb4_simple_bus;


template<unsigned int ADWIDTH, unsigned int BUSWIDTH>
class apb4_interface:public sc_interface{
public:
	typedef sc_uint<BUSWIDTH> data_type;
	typedef sc_uint<BUSWIDTH/8> strb_type;
	typedef sc_uint<ADWIDTH> address_type;
	typedef sc_uint<3> prot_type;

	virtual void apb_reset() = 0;
	virtual bool apb_write(const prot_type& prot, const address_type& ad, const strb_type& strb, const data_type& dt) = 0;
	virtual bool  apb_read(const prot_type& prot, const address_type& ad, data_type& dt) = 0;

};

template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>
class apb4_base_port:public tlm_initiator_socket<BUSWIDTH>{
public:

	typedef tlm_initiator_socket<BUSWIDTH> base_type;
	apb4_base_port(const sc_module_name name=sc_gen_unique_name("apb4_base_port")):base_type(name){}
};


template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>
class apb4_base_export:public tlm_target_socket<BUSWIDTH>{
public:
	typedef tlm_target_socket<BUSWIDTH> base_type;
	apb4_base_export(const sc_module_name name=sc_gen_unique_name("apb4_base_export")):base_type(name){}
};

template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>
class apb4_port:public tlm_initiator_socket<BUSWIDTH>, public tlm_bw_transport_if<>{
public:
	typedef tlm_initiator_socket<BUSWIDTH> base_type;
	apb4_port(const sc_module_name name=sc_gen_unique_name("apb4_port")):base_type(name){
		m_export(*this);
	}

	virtual tlm::tlm_sync_enum nb_transport_bw( tlm::tlm_generic_payload& trans, tlm::tlm_phase &phase, sc_time& time ){
	    // not support
	    trans.set_response_status( tlm::TLM_GENERIC_ERROR_RESPONSE );
	    return tlm::TLM_COMPLETED;
	}

	virtual void invalidate_direct_mem_ptr( sc_dt::uint64 a, sc_dt::uint64 b){
	    // DMI unused
	}

};

template<unsigned int ADWIDTH, unsigned int BUSWIDTH, class MODE>
class apb4_export:public tlm_target_socket<BUSWIDTH>, public tlm_fw_transport_if<>{
public:
	typedef tlm_target_socket<BUSWIDTH> base_type;
	apb4_export(const sc_module_name name=sc_gen_unique_name("apb4_export")):base_type(name){
		m_port(*this);
	}

	void b_transport( tlm::tlm_generic_payload &trans, sc_time &time ) {
	    unsigned int address = static_cast<unsigned int>(trans.get_address());
	    unsigned int *data_ptr = reinterpret_cast<unsigned int*>(trans.get_data_ptr());

	    unsigned int idx = address/4;

	    trans.set_response_status( tlm::TLM_OK_RESPONSE );
        //trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE  );

	}

	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &time ){
	    cout << "nb_transport_fw is not supported." << endl;
	    trans.set_response_status( tlm::TLM_GENERIC_ERROR_RESPONSE );

	    return tlm::TLM_COMPLETED;
	}

	unsigned int transport_dbg( tlm::tlm_generic_payload &trans ){
	    return 0;  // Debug not supported
	}

	bool get_direct_mem_ptr( tlm::tlm_generic_payload &trans, tlm::tlm_dmi& dmi){
	    return false; // DMI not supported
	}
};

#endif /* AMBA4_APB_IF_H_ */
