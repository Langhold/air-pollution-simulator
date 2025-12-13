//
//  simulator.cpp
//  TP2
//
//  Created by Nolwen Dolléans on 23/10/2025.
//

#include "simulator.hpp"


/*------------TOOLS------------*/
Simulator::ComputeType Simulator::userChoice(const char * arg){
	if (strcmp(arg, "steady")==0){
		return ComputeType::Steady;
	} else if (strcmp(arg, "unsteady")==0){
		return ComputeType::Unsteady;
	}
	return ComputeType::Steady;
}

/*------------ARRAY------------*/
unsigned long int Array::size() const{
	return data.size();
}

void Array::print(std::ofstream& file) const{
	if (!file) {
		std::cerr << "Error: File doesn't open.\n";
		exit(EXIT_FAILURE);
	}
	std::for_each(data.begin(), data.end(), [&file](auto& value){file << value <<",";});
	file << std::endl;
}

void Array::resize(int i){
	data.reserve(i);
	data.resize(i);
}
auto Array::begin() const{
	return data.begin();
}
auto Array::end() const{
	return data.end();
}
auto Array::begin(){
	return data.begin();
}
auto Array::end(){
	return data.end();
}

double& Array::operator[](int i){
	return data[i];
}

const double& Array::operator[](int i) const{
	return data[i];
}

/*------------GASFIELD------------*/
double ConstantGasField::velocity(double position, double time){
	return 1;
}

double NonUniformGasField::velocity(double position, double time){
	return sin(std::atan(1)*4*position);
}

/*------------MODEL------------*/
template<>
void Model<ConstantGasField>::compute_velocities(Array& velocities, Array const& positions, double time){
	std::transform(positions.begin(), positions.end(),velocities.begin(), [this, &time](auto& position){
		return this->gastype.velocity(position, time);
	});
}

template<>
void Model<ConstantGasField>::compute_positions(Array& positions, Array const& velocities, double time){
	std::transform(velocities.begin(), velocities.end(),positions.begin(), [&time](auto& velocitiy){
		return velocitiy * time;
	});
}

/*------------SIMULATOR------------*/
void Simulator::SteadySimulator::compute(Array& positions, Array& velocities, Model<ConstantGasField>& particle_model){
	std::cout << " --- compute particle evolution at time: " << 0 << "---" << std::endl;
	particle_model.compute_velocities(velocities, positions, 0);
	particle_model.compute_positions(positions,velocities, 0);
	
	std::cout << "--- Export particles positions at time t = 0 in /Results ---" << std::endl;
	std::ofstream file("Results/particles_positions");
	positions.print(file);
	file.close();
	
	std::cout << "--- Export particles velocities at time t = 0 in /Results  ---" << std::endl;
	std::ofstream file2("Results/particles_positions");
	velocities.print(file2);
	file2.close();
}

void Simulator::UnsteadySimulator::compute(Array& positions, Array& velocities, Model<ConstantGasField>& particle_model){
	const int N = 4;
	double t = 0;
	const double dt = 1.0/(double)N;
	while (t<=1) {
		
		std::cout << "--- Export particles positions at time t = " << t << " in /Results ---" << std::endl;
		std::ofstream file("Results/particles_positions", std::ios::app);
		positions.print(file);
		file.close();
		
		std::cout << "--- Export particles velocities at time t = " << t << " in /Results  ---" << std::endl;
		std::ofstream file2("Results/particles_velocities", std::ios::app);
		velocities.print(file2);
		file2.close();
		
		std::cout << "--- compute particle evolution at time: " << t << "---" << std::endl;
		t += dt;
		particle_model.compute_velocities(velocities, positions, t);
		particle_model.compute_positions(positions,velocities, t);
	}
}

/*------------PARTICLES------------*/
void Simulator::Particles::initialize(ComputeType const& Sim_type, ParticlesInit_mod const& Pos_type){
	std::fill(velocity.begin(), velocity.end(), 1);
	std::vector<int> index(nbpart);
	std::iota(index.begin(), index.end(),1);
	std::ofstream file("Results/particles_positions", std::ios::trunc);
	file.close();
	std::ofstream file2("Results/particles_velocity", std::ios::trunc);
	
	file2.close();
	switch (Pos_type){
		case ParticlesInit_mod::Discretized:
			std::cout << "--- init particles discretized ---" << std::endl;
			std::transform(index.begin(), index.end(), position.begin(), [this](auto& indx){
				return -1.0 + (double)indx*2.0/(double)nbpart;
			});
			break;
		case ParticlesInit_mod::Localized:
			std::cout << "--- init particles at 0 ---" << std::endl;
			std::fill(position.begin(), position.end(), 0.0);
			break;
	}
	switch (Sim_type){
		case ComputeType::Steady:
			sim = std::make_unique<SteadySimulator>();
			break;
		case ComputeType::Unsteady:
			sim = std::make_unique<UnsteadySimulator>();
			break;
	}
}

void Simulator::Particles::compute(){
	sim->compute(position, velocity, model);
}
