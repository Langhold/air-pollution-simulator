//
//  main.cpp
//  TP2
//
//  Created by Nolwen Dolléans on 23/10/2025.
//

#include <iostream>
#include <cstring>
#include "simulator.hpp"





int main(int argc, const char * argv[]) {
	if (argc != 2){
		std::cout << "Please Using: " << argv[0] << " SimulatorType(steady or unsteady)\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if (!(strcmp(argv[1], "steady") == 0 || strcmp(argv[1], "unsteady") == 0)){
		std::cout << "Using: " << argv[0] << " SimulatorType(steady or unsteady)\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Model<ConstantGasField> cons;
	Simulator::Particles p(10);
	Simulator::ComputeType compute_type = Simulator::userChoice(argv[1]);
	Simulator::ParticlesInit_mod initializing_type = Simulator::ParticlesInit_mod::Localized;
	p.initialize(compute_type, initializing_type);
	p.compute();
	std::cout << "Hello, World!\n";
	return 0;
}
