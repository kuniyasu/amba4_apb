/*
 * TOP.h
 *
 *  Created on: 2013/12/31
 *      Author: Kuniyasu
 */

#ifndef TOP_H_
#define TOP_H_

#include <systemc.h>

class TOP : public sc_module{
public:
	TOP(const sc_module_name name):sc_module(name){
		end_module();
	}
};


#endif /* TOP_H_ */
