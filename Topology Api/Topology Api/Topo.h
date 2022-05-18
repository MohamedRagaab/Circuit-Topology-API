#pragma once
#include <iostream>
#include <vector>
#include "./nlohmann/json.hpp"

using json = nlohmann::json;
#define DeviceList vector<json> 
using namespace std;

/* Topology Api Library  *********************************************************************/

namespace TopoLib {
	// Base Class 
	class Topo
	{
		/* Result sub class (one circuit topology) *****************************************************************/
		class Result {
			string id;
			DeviceList componentList;

		public:
			// Constructor
			Result(string id, DeviceList componentList);
			json ConverttoJSON();
			//int cntDevices();
			string getTopoID();
			DeviceList getDevicesList();
			void printTopo();
			//bool operator==(const Result& other);
		
		};
		/* TopologyList sub class (list of circuit topology) ********************************************************/

		class TopologyList {
			// Topologies Memory Vector
			vector<Result> topologyList;
		public:
			void addNewTopo(Result topo);
			Result getTopoWithID(string TopologyID);
			void printTopologies();
			Result deletTopoWithId(string TopologyID);
		};

		// Topologies Memory Vector Pointer
		unique_ptr<TopologyList> memoryListPointer;

	public:
		Topo();
		/* API ***************************************************************************************************/
		// Read a topology from a given JSON file and store it in the memory.
		Result readJSON(string FileName);

		// Write a given topology from the memory to a JSON file.
		Result writeJSON(string TopologyID);

		// Query about which topologies are currently in the memory.
		TopologyList queryTopologies();

		// Delete a given topology from memory.
		Result deleteTopology(string TopologyID);

		// Query about which devices are in a given topology.
		DeviceList queryDevices(string TopologyID);

		// Query about which devices are connected to a given netlist node in a given topology.
		DeviceList queryDevicesWithNetlistNode(string TopologyID, string NetlistNodeID);
	};
}

