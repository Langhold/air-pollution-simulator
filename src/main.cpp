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
	if (argc != 4){Simulator::failed_choices(argv[0], "SimulatorType(steady or unsteady) InitializationParticlesType(localized/discretized) GasType(constant, nonuniform)\n");}
	
	if (!(strcmp(argv[1], "steady") == 0 || strcmp(argv[1], "unsteady") == 0)){Simulator::failed_choices(argv[0], "SimulatorType(steady or unsteady) InitializationParticlesType(localized/discretized) GasType(constant, nonuniform)\n");}
	
	
	if (!(strcmp(argv[2], "localized") == 0 || strcmp(argv[2], "discretized") == 0)){Simulator::failed_choices(argv[0], "SimulatorType(steady or unsteady) InitializationParticlesType(localized/discretized) GasType(constant, nonuniform)\n");}
	
	
	if (!(strcmp(argv[3], "constant") == 0 || strcmp(argv[3], "nonuniform") == 0)){Simulator::failed_choices(argv[0], "SimulatorType(steady or unsteady) InitializationParticlesType(localized/discretized) GasType(constant, nonuniform)\n");}
	
	std::string path1 = "Results/particles_unsteady_discretized_nonuniform";
	
	Simulator::Particles p(10);
	auto [compute_type, initializing_type, gas_type] = Simulator::userChoice(argv);
	p.initialize(compute_type, initializing_type, gas_type, path1);
	p.compute(path1);
	
	std::string path2 = "Results/particles_unsteady_localized_constant";
	const char * args[] = {"", "unsteady","localized","constant"};
	auto [compute_type2, initializing_type2, gas_type2] = Simulator::userChoice(args);
	p.initialize(compute_type2, initializing_type2, gas_type2, path2);
	p.compute(path2);
	return 0;
}
