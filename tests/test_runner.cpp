//
//  test_runner.cpp
//  TP1
//
//  Created by Nolwen Dolléans on 09/10/2025.
//

#include "gtest/gtest.h"
#include "simulator.hpp"

TEST(ParticlesTests, InitParticlesTest){
	Simulator::Particles p(2);
	Simulator::ComputeType type = Simulator::ComputeType::Steady;
	Simulator::ParticlesInit_mod __type = Simulator::ParticlesInit_mod::Discretized;
	p.initialize(type, __type);
	for(int i = 0;i<2;++i){
		bool test = p.position[i] == 0;
	}

	for(int i = 0;i<2;++i){
		bool test = p.position[i] == 0;
	}

	
}

TEST(ParticlesTests, ComputeParticlesTest){
	Simulator::Particles p;
	Simulator::ComputeType type = Simulator::ComputeType::Steady;
	Simulator::ParticlesInit_mod __type = Simulator::ParticlesInit_mod::Discretized;
	p.initialize(type, __type);
	p.compute();
}



TEST(ArrayTests, ArrayTestsConstructor){
	Array arr(3,1);
	bool test = arr[0]==1;
	
}


TEST(ArrayTests, ArrayTestsOperators){
	Array arr(3,1);
	Array arr2(3,2);
	arr=arr2;
	bool test = arr[0] == 2;
}
