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
	const char* args[] = {"steady", "discretized", "nonuniform"};
	auto [compute_type, initializing_type, gas_type] = Simulator::userChoice(args);

	for(int i = 0;i<2;++i){
		EXPECT_EQ(p.position[i], 0);
	}

	
}

TEST(ParticlesTests, ComputeParticlesTest){
	Simulator::Particles p(1);
	const char* args[] = {"steady", "discretized", "nonuniform"};
	auto [compute_type, initializing_type, gas_type] = Simulator::userChoice(args);
	std::string path = "test";
	p.initialize(compute_type, initializing_type, gas_type, path);
	p.compute(path);
}



TEST(ArrayTests, ArrayTestsConstructor){
	Array arr(3,1);
	EXPECT_EQ(arr[0], 1);
	
}


TEST(ArrayTests, ArrayTestsOperators){
	Array arr(3,1);
	Array arr2(3,2);
	arr=arr2;
	EXPECT_EQ(arr[0], 2);
}

