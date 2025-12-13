//
//  simulator.h
//  TP2
//
//  Created by Nolwen Dolléans on 23/10/2025.
//

#ifndef simulator_h
#define simulator_h

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <memory>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cmath>


/*------------TOOLS------------*/
class Array{
	std::vector<double> data;
public:
	explicit Array() = default;
	explicit Array(const unsigned long int size, const double value = 0.0) : data(size, value) {}
	
	unsigned long int size() const;
	void resize(int i);
	~Array(){};
	
	void print(std::ofstream& file) const;
	
	//Opérateurs
	const double& operator[](int i) const;
	double& operator[](int i);
	
	Array& operator=(Array&& other){
		data = std::move(other.data);
		return *this;
	}
	Array& operator=(const Array& other){
		data = other.data;
		return *this;
	};
	
	//Itérateurs
	auto begin() const;
	auto end() const;
	auto begin();
	auto end();
	
};

/*------------GASFIELD------------*/
struct ConstantGasField{
	double velocity(double positions, double time);
};

struct NonUniformGasField{
	double velocity(double positions, double time);
};

/*------------MODEL------------*/
template <typename T>
class Model{
	T gastype;
public:
	void compute_velocities(Array& velocities, Array const& positions, double time);
	void compute_positions(Array& positions, Array const& velocities, double time);
};


namespace Simulator {

/*------------USERCHOICE------------*/
enum class ComputeType{
	Steady,
	Unsteady
};
ComputeType userChoice(const char * arg);

/*------------SIMULATOR------------*/
class Simulator{
public:
	virtual void compute(Array& positions, Array& velocities, Model<ConstantGasField>& particle_model) = 0;
	virtual ~Simulator() = default;
	
};

class SteadySimulator : public Simulator{
public:
	void compute(Array& positions, Array& velocities, Model<ConstantGasField>& particle_model) override;
	
};

class UnsteadySimulator : public Simulator{
public:
	void compute(Array& positions, Array& velocities, Model<ConstantGasField>& particle_model) override;
	
};

/*------------PARTICLES------------*/
enum class ParticlesInit_mod{
	Discretized,
	Localized
};

class Particles{
	Model<ConstantGasField> model;
	std::unique_ptr<Simulator> sim = nullptr;
	ComputeType type;
	unsigned long int nbpart = 0;
public:
	Array position;
	Array velocity;
	
	Particles() = default;
	Particles(unsigned int i){
		nbpart = i;
		position.resize(i);
		velocity.resize(i);
	}
	
	void initialize(ComputeType const& Sim_type, ParticlesInit_mod const& Pos_type);
	void compute();
	
	~Particles() {sim.reset();}
};


} //Simulator



#endif /* simulator_h */
