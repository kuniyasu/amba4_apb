/*
 * TOP.h
 *
 *  Created on: 2013/12/31
 *      Author: Kuniyasu
 */

#ifndef TOP_H_
#define TOP_H_

#include <systemc.h>

#include "amba4_apb_if.h"

class InitiatorDUT:public sc_module{
public:
	sc_in<bool> clk;
	sc_in<bool> nrst;

	apb4_port<32U,32U,TLM2LT> port;

	SC_HAS_PROCESS(InitiatorDUT);
	InitiatorDUT(const sc_module_name name):sc_module(name){
		end_module();
	}
};

class TargetDUT:public sc_module{
public:
	sc_in<bool> clk;
	sc_in<bool> nrst;

	apb4_export<32U,32U,TLM2LT> ex_port;

	SC_HAS_PROCESS(TargetDUT);
	TargetDUT(const sc_module_name name):sc_module(name){
		end_module();
	}
};

class TOP : public sc_module{
public:
	sc_clock clk;
	sc_signal<bool> nrst;

	InitiatorDUT initiatorDUT;
	TargetDUT targetDUT;
	apb4_simple_bus<32U,32U,TLM2LT> bus;

	SC_HAS_PROCESS(TOP);
	TOP(const sc_module_name name):sc_module(name),
		clk("CLK", 25, SC_NS, 0.5, 5, SC_NS, true ),nrst("nrst"),
		initiatorDUT("initiatorDUT"),targetDUT("targetDUT"),bus("bus"){
		initiatorDUT.clk(clk);
		initiatorDUT.nrst(nrst);
		initiatorDUT.port(bus.initiator_port);

		targetDUT.clk(clk);
		targetDUT.nrst(nrst);
		targetDUT.ex_port(bus.target_port);
		//initiatorDUT.port(targetDUT.ex_port);

		SC_THREAD(timeout_thread);
		end_module();
	}

	void timeout_thread(){
		wait(100,SC_MS);

		sc_stop();
	}
};


#endif /* TOP_H_ */
