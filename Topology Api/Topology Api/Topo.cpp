#include "Topo.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

namespace TopoLib {

	/* Result Sub Class *********************************************************/
	Topo::Result::Result(string id, DeviceList componentList)
	{
		this->id = id;
		this->componentList = componentList;
	}

	json Topo::Result::ConverttoJSON()
	{
		json j;
		j["id"] = this->id;
		j["components"] = this->componentList;
		return j;
	}

	string Topo::Result::getTopoID()
	{
		return this->id;
	}
	vector<json> Topo::Result::getDevicesList()
	{
		return this->componentList;
	}

	void Topo::Result::printTopo()
	{
		cout << setw(4) << this->ConverttoJSON() << endl;
	}

	/* TopologyList Sub Class ***************************************************/
	
	void Topo::TopologyList::addNewTopo(Result topo)
	{
		this->topologyList.push_back(topo);
	}

	Topo::Result Topo::TopologyList::getTopoWithID(string TopologyID)
	{
		for (auto topo : this->topologyList) {
			if (topo.getTopoID() == TopologyID) {
				return topo;
			}
		}
		perror("Not Found!!");
		exit(1);
	}

	void Topo::TopologyList::printTopologies()
	{
		for (auto topology : this->topologyList) {
			topology.printTopo();
		}
	}

	Topo::Result Topo::TopologyList::deletTopoWithId(string TopologyID)
	{
		Result result = this->getTopoWithID(TopologyID);
		for (int i = 0; i < this->topologyList.size(); i++) {
			if (this->topologyList[i].getTopoID() == TopologyID) {
				this->topologyList.erase(this->topologyList.begin()+i);
				return result;
			}
		}
		perror("Not Found!!");
		exit(1);
		
		return result;
	}

	/* Topo base Class **********************************************************/

	Topo::Topo()
	{
		this->memoryListPointer = unique_ptr<TopologyList>(new TopologyList());
	}

	// Read a topology from a given JSON file and store it in the memory.
	Topo::Result Topo::readJSON(string FileName)
	{
		ifstream fin(FileName);
		json j;
		fin >> j;
		try
		{
			string topoId = j["id"];
			DeviceList componentList = j["components"];
			Result result(topoId, componentList);
			this->memoryListPointer->addNewTopo(result);
			return result;
		}
		catch (const std::exception&)
		{
			perror("Error, Worng Json File Format!!");
			// Terminate C++ Program with Failure Status
			exit(1);
		}
	}
	// Write a given topology from the memory to a JSON file.
	Topo::Result Topo::writeJSON(string TopologyID) 
	{
		Result result = this->memoryListPointer->getTopoWithID(TopologyID);
		ofstream out(result.getTopoID()+".json");
		out << setw(4) << result.ConverttoJSON() << std::endl;
		return result;
	}
	// Query about which topologies are currently in the memory.
	Topo::TopologyList Topo::queryTopologies()
	{
		return *this->memoryListPointer;
	}
	// Delete a given topology from memory.
	Topo::Result Topo::deleteTopology(string TopologyID) {
		return this->memoryListPointer->deletTopoWithId(TopologyID);
	}
	// Query about which devices are in a given topology.
	DeviceList Topo::queryDevices(string TopologyID) {
		Result result = this->memoryListPointer->getTopoWithID(TopologyID);
		return result.getDevicesList();
	}
	// Query about which devices are connected to a given netlist node in a given topology.
	DeviceList Topo::queryDevicesWithNetlistNode(string TopologyID, string NetlistNodeID) {
		DeviceList devices = this->queryDevices(TopologyID);
		DeviceList result;
		for (auto device : devices) {
			string netlist = device["netlist"].dump();
			if (netlist.find(NetlistNodeID) != string::npos) {
				result.push_back(device);
			}
		}
		return result;
	}
}

