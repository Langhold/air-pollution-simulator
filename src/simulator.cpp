//
//  simulator.cpp
//  TP2
//
//  Created by Nolwen Dolléans on 23/10/2025.
//

#include "simulator.hpp"


/*------------TOOLS------------*/
void Simulator::failed_choices(const char* args, const char* message){
	std::cout << "Using: " << args << " " << message << std::endl;
	exit(EXIT_FAILURE);
}
std::tuple<Simulator::ComputeType, Simulator::ParticlesInit_mod, Simulator::GasType> Simulator::userChoice(const char ** args){
	return {userChoice_ComputeT(args[1]), userChoice_ParticlesInit(args[2]), userChoice_GasType(args[3])};
}

Simulator::ComputeType Simulator::userChoice_ComputeT(const char * arg){
	if (strcmp(arg, "steady")==0){
		return ComputeType::Steady;
	} else if (strcmp(arg, "unsteady")==0){
		return ComputeType::Unsteady;
	}
	else{
		failed_choices(arg, "rather than: (steady or unsteady)");
	}
	return ComputeType::Steady;
}

Simulator::ParticlesInit_mod Simulator::userChoice_ParticlesInit(const char * arg){
	if (strcmp(arg, "localized")==0){
		return ParticlesInit_mod::Localized;
	} else if (strcmp(arg, "discretized")==0){
		return ParticlesInit_mod::Discretized;
	}
	else{
		failed_choices(arg, "rather than: (localized/discretized)");
	}
	return ParticlesInit_mod::Localized;
}

Simulator::GasType Simulator::userChoice_GasType(const char * arg){
	if (strcmp(arg, "constant")==0){
		return GasType::Constant;
	} else if (strcmp(arg, "nonuniform")==0){
		return GasType::NonUniform;
	}
	else{
		failed_choices(arg, "rather than: (constant, nonuniform)");
	}
	return GasType::Constant;
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
	std::for_each(data.begin(), data.end(), [&file](auto& value){file << (double)value <<",";});
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
	return sin(-M_PI*position);
}

/*------------MODEL------------*/
void Model::compute_velocities(Array& velocities, Array const& positions, double time){
	std::transform(positions.begin(), positions.end(),velocities.begin(), [this, &time](auto& position){
		return this->gastype->velocity(position, time);
	});
}

void Model::compute_positions(Array& positions, Array const& velocities, double time){
	std::transform(velocities.begin(), velocities.end(),positions.begin(), positions.begin(), [&time](auto& velocitiy, auto& position){
		return position+velocitiy * time;
	});
}

/*------------SIMULATOR------------*/
void Simulator::SteadySimulator::compute(Array& positions, Array& velocities, Model& particle_model, std::string& path){
	std::cout << " --- compute particle evolution at time: " << 0 << "---" << std::endl;
	particle_model.compute_velocities(velocities, positions, 0);
	particle_model.compute_positions(positions,velocities, 0);
	
	std::cout << "--- Export particles positions at time t = 0 in /Results ---" << std::endl;
	std::ofstream file(path+"_positions.csv");
	positions.print(file);
	file.close();
	
	std::cout << "--- Export particles velocities at time t = 0 in /Results  ---" << std::endl;
	std::ofstream file2(path+"_velocities.csv");
	velocities.print(file2);
	file2.close();
}

void Simulator::UnsteadySimulator::compute(Array& positions, Array& velocities, Model& particle_model, std::string& path){
	double t = 0;
	const double dt = 1.0/(double)N;
	while (t<1) {
		std::cout << "--- Export particles positions at time t = " << t << " in /Results ---" << std::endl;
		std::ofstream file(path+"_positions.csv", std::ios::app);
		positions.print(file);
		file.close();
		
		std::cout << "--- Export particles velocities at time t = " << t << " in /Results ---" << std::endl;
		std::ofstream file2(path+"_velocities.csv", std::ios::app);
		velocities.print(file2);
		file2.close();
		
		std::cout << "--- compute particle evolution at time: " << t << " ---" << std::endl;
		t += dt;
		particle_model.compute_velocities(velocities, positions, dt);
		particle_model.compute_positions(positions,velocities, dt);
	}
}

/*------------PARTICLES------------*/
void Simulator::Particles::initialize(ComputeType const& Sim_type, ParticlesInit_mod const& Pos_type, GasType const& Gas_type, std::string& path){
	std::fill(velocity.begin(), velocity.end(), 1);
	std::vector<int> index(nbpart);
	std::iota(index.begin(), index.end(),1);
	
	std::ofstream file(path+"_positions.csv", std::ios::trunc);
	file.close();
	std::ofstream file2(path+"_velocities.csv", std::ios::trunc);
	
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
	switch (Gas_type){
		case GasType::Constant:
			model.gastype = std::make_unique<ConstantGasField>();
			break;
		case GasType::NonUniform:
			model.gastype = std::make_unique<NonUniformGasField>();
			break;
	}
}

void Simulator::Particles::compute(std::string& path){
	sim->compute(position, velocity, model, path);
}
