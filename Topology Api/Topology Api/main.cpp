#include "Topo.h"
#include<iostream>

int main() {

	TopoLib::Topo topo;

	topo.readJSON("topology.json");
	//topo.queryTopologies().printTopologies();
	//topo.deleteTopology("top1");
	//topo.queryTopologies().printTopologies();
	cout << topo.queryDevices("top1") << endl;
	//cout << topo.queryDevicesWithNetlistNode("top1", "n1") << endl;
	//cout << topo.queryDevicesWithNetlistNode("top1", "vdd") << endl;

	return 0;
}