/* -*- C++ -*- */

//=============================================================================
/**
 * @file   modAOS_VC_Mux_Interface.cpp
 * @author Tad Kollar  
 *
 * $Id: modAOS_VC_Mux_Interface.cpp 2018 2013-06-04 14:59:19Z tkollar $
 * Copyright (c) 2009.
 *      NASA Glenn Research Center.  All rights reserved.
 *      See COPYING file that comes with this distribution.
 */
//=============================================================================

#include "modAOS_VC_Mux.hpp"
#include "AOS_Interface_Macros.hpp"
#include "PeriodicHandler_Interface.hpp"

namespace nAOS_VC_Mux {

//=============================================================================
/**
 * @class modAOS_VC_Mux_Interface
 * @author Tad Kollar  
 * @brief Provide an XML-RPC interface to the Virtual Channel Multiplexing module.
 */
//=============================================================================
class modAOS_VC_Mux_Interface: public nasaCE::PeriodicHandler_Interface<modAOS_VC_Mux> {
public:
	modAOS_VC_Mux_Interface(xmlrpc_c::registryPtr& rpcRegistry):
		nasaCE::PeriodicHandler_Interface<modAOS_VC_Mux>(rpcRegistry, "modAOS_VC_Mux") {
		ACE_TRACE("modAOS_VC_Mux_Interface::modAOS_VC_Mux_Interface");

		register_methods(rpcRegistry);
	}

	void set_priority(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* retvalP) {
		ACE_TRACE("modAOS_VC_Mux_Interface::set_priority");
		const ACE_UINT8 VCID = paramList.getInt(2);
		const unsigned long priority = static_cast<unsigned long>(paramList.getInt(3));

		find_handler(paramList)->setPriority(VCID, priority);

		*retvalP = xmlrpc_c::value_nil();
	}

	void get_priority(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* retvalP) {
		ACE_TRACE("modAOS_VC_Mux_Interface::get_priority");
		const ACE_UINT8 VCID = paramList.getInt(2);

		*retvalP = xmlrpc_c::value_int(find_handler(paramList)->getPriority(VCID));
	}

	void get_input_links(xmlrpc_c::paramList const& paramList, xmlrpc_c::value* retvalP) {
		ACE_TRACE("modAOS_VC_Mux_Interface::get_input_links");

		modAOS_VC_Mux* handler = dynamic_cast<modAOS_VC_Mux*>(find_handler(paramList));

		std::list<std::string> inputList;
		handler->getInputNames(inputList);

		std::map<std::string, xmlrpc_c::value> linkDescs;

		std::list<std::string>::iterator pos;

		for ( pos = inputList.begin(); pos != inputList.end(); ++pos ) {
			linkDescs[*pos] = xmlrpc_c::value_string("Primary");
		}

		HandlerLink* auxInputLink = handler->getLink(BaseTrafficHandler::AuxInputLink);
		if (auxInputLink)
			linkDescs[auxInputLink->getTarget()->getName()] = xmlrpc_c::value_string("Aux");

		*retvalP = xmlrpc_c::value_struct(linkDescs);
	}

	GENERATE_INT_ACCESSORS( default_priority, setDefaultPriority, getDefaultPriority);

	AOS_PHYSICAL_CHANNEL_DEFINE_ACCESSORS();
	AOS_MASTER_CHANNEL_DEFINE_ACCESSORS();

	// Export specified methods via XML-RPC.
	void register_methods(xmlrpc_c::registryPtr&);

protected:
	void get_counters_(xmlrpc_c::paramList const& paramList, modAOS_VC_Mux* handler,
		xstruct& counters) {
		ACE_TRACE("modAOS_VC_Mux_Interface::get_counters_");

		nasaCE::PeriodicHandler_Interface<modAOS_VC_Mux>::get_counters_(paramList, handler, counters);

		GET_AOS_PHYSICAL_CHANNEL_COUNTERS();
		GET_AOS_MASTER_CHANNEL_COUNTERS();
	}

	void get_settings_(xmlrpc_c::paramList const& paramList, modAOS_VC_Mux* handler,
		xstruct& settings) {
		ACE_TRACE("modAOS_VC_Mux_Interface::get_settings_");

		nasaCE::PeriodicHandler_Interface<modAOS_VC_Mux>::get_settings_(paramList, handler, settings);

		GET_AOS_PHYSICAL_CHANNEL_SETTINGS();
		GET_AOS_MASTER_CHANNEL_SETTINGS();
	}
};

modAOS_VC_Mux_Interface* modAOS_VC_Mux_InterfaceP;

PERIODIC_METHOD_CLASSES(modAOS_VC_Mux_Interface, modAOS_VC_Mux_InterfaceP);

METHOD_CLASS(set_priority, "n:ii",
	"Sets the priority for the specified Virtual Channel identifier.", modAOS_VC_Mux_InterfaceP);
METHOD_CLASS(get_priority, "i:i",
	"Returns the priority of the specified Virtual Channel identifier.", modAOS_VC_Mux_InterfaceP);
GENERATE_ACCESSOR_METHODS(default_priority, i, "the default priority for each virtual channel.",
	modAOS_VC_Mux_InterfaceP);

AOS_PHYSICAL_CHANNEL_GENERATE_METHODS(modAOS_VC_Mux_InterfaceP);
AOS_MASTER_CHANNEL_GENERATE_METHODS(modAOS_VC_Mux_InterfaceP);

void modAOS_VC_Mux_Interface::register_methods(xmlrpc_c::registryPtr& rpcRegistry) {
	ACE_TRACE("modAOS_VC_Mux_Interface::register_methods");

	PERIODIC_REGISTER_METHODS(modAOS_VC_Mux, modAOS_VC_Mux_Interface);
	REGISTER_ACCESSOR_METHODS(priority, modAOS_VC_Mux, Priority);
	REGISTER_ACCESSOR_METHODS(default_priority, modAOS_VC_Mux, DefaultPriority);
	AOS_PHYSICAL_CHANNEL_REGISTER_METHODS(modAOS_VC_Mux);
	AOS_MASTER_CHANNEL_REGISTER_METHODS(modAOS_VC_Mux);
}

} // namespace nAOS_VC_Mux