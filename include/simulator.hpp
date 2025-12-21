//
//  simulator.h
//  TP2
//
//  Created by Nolwen Dolléans on 23/10/2025
//

/**
 * @file simulator.hpp
 * @brief Declarations for simulation components: Array, GasField, Model, Simulator strategies, and Particles
 */

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
#include <chrono>
#include <thread>

#define N 50
#define NTHREADS 8

/*------------------------TOOLS------------------------*/
/**
 * @brief Simple dynamic array wrapper around std::vector<double> with convenient operators
 */
class Array{
	std::vector<double> data;
public:
    /** @brief Constructs an empty Array. */
	explicit Array() = default;
    /**
     * @brief Constructs an Array with a given size and initial value
     * @param size Number of elements
     * @param value Initial value for all elements
     */
	explicit Array(const unsigned long int size, const double value = 0.0) : data(size, value) {}
	
    /** @brief Returns the number of elements in the array. */
	unsigned long int size() const;
    /**
     * @brief Resizes the array to the specified number of elements
     * @param i New size (number of elements)
     */
	void resize(int i);
    /** @brief Destroys the Array. */
	~Array(){};
	
    /**
     * @brief Prints the array values to an output file stream
     * @param file Output file stream
     */
	void print(std::ofstream& file) const;
	
	//Operators
    /**
     * @brief Provides const access to an element by index
     * @param i Index of the element
     * @return Const reference to the element
     */
	const double& operator[](int i) const;
    /**
     * @brief Provides mutable access to an element by index
     * @param i Index of the element
     * @return Reference to the element
     */
	double& operator[](int i);
	
    /**
     * @brief Move-assigns from another Array
     * @param other Source array to move from
     * @return Reference to this Array
     */
	Array& operator=(Array&& other){
		data = std::move(other.data);
		return *this;
	}
    /**
     * @brief Copy-assigns from another Array
     * @param other Source array to copy from
     * @return Reference to this Array
     */
	Array& operator=(const Array& other){
		data = other.data;
		return *this;
	};
	
	//Iterators
    /** @brief Returns a const iterator to the beginning. */
	auto begin() const;
    /** @brief Returns a const iterator to the end. */
	auto end() const;
    /** @brief Returns an iterator to the beginning. */
	auto begin();
    /** @brief Returns an iterator to the end. */
	auto end();
	
};

/*------------------------GASFIELD------------------------*/
/**
 * @brief Abstract gas field interface providing velocity as a function of position and time
 */
struct GasField{
    /**
     * @brief Computes the velocity at a position and time
     * @param positions Position value
     * @param time Time step value
     * @return Velocity at the given position and time
     */
	virtual double velocity(double positions, double time) = 0;
	virtual ~GasField() = default;
};
/** @brief Gas field with constant velocity. */
struct ConstantGasField : GasField{
	/**
	 * @brief Computes the velocity at a position and time
	 * @param positions Position value
	 * @param time Time step value
	 * @return Velocity at the given position and time
	 */
	double velocity(double positions, double time) override;
};

/** @brief Gas field with spatially or temporally varying velocity. */
struct NonUniformGasField :  GasField{
	/**
	 * @brief Computes the velocity at a position and time
	 * @param positions Position value
	 * @param time Time step value
	 * @return Velocity at the given position and time
	 */
	double velocity(double positions, double time) override;
};

/*------------------------MODEL------------------------*/
/**
 * @brief Particle dynamics model using a selected gas field to update velocities and positions
 */
struct Model{
	std::unique_ptr<GasField> gastype = nullptr;
    /**
     * @brief Computes particle velocities from positions at a given time
     * @param velocities Output velocities array
     * @param positions Input positions array
     * @param time Time step value
     */
	void compute_velocities(Array& velocities, Array const& positions, double time);
    /**
     * @brief Updates particle positions using velocities at a given time
     * @param positions In/out positions array
     * @param velocities Input velocities array
     * @param time Time step value
     */
	void compute_positions(Array& positions, Array const& velocities, double time);
	
	~Model() {gastype.reset();}
};


namespace Simulator {

/*------------------------USERCHOICE------------------------*/
/** @brief Simulation compute mode (steady or unsteady). */
enum class ComputeType{
	Steady,
	Unsteady
};

/** @brief Particle initialization mode. */
enum class ParticlesInit_mod{
	Discretized,
	Localized
};

/** @brief Gas field type used by the model. */
enum class GasType{
	Constant,
	NonUniform
};

/**
 * @brief Reports invalid user choices
 * @param args The offending argument
 * @param message Explanation message
 */
void failed_choices(const char* args, const char* message);

/**
 * @brief Parses user arguments into simulation configuration
 * @param args Array of C-string arguments (compute, init, gas)
 * @return Tuple of ComputeType, ParticlesInit_mod, GasType
 */
std::tuple<ComputeType, ParticlesInit_mod, GasType> userChoice(const char ** args);

/**
 * @brief Parses the compute type from a string
 * @param arg Input string
 * @return Parsed ComputeType
 */
Simulator::ComputeType userChoice_ComputeT(const char * arg);

/**
 * @brief Parses the particles initialization mode from a string
 * @param arg Input string
 * @return Parsed ParticlesInit_mod
 */
Simulator::ParticlesInit_mod userChoice_ParticlesInit(const char * arg);

/**
 * @brief Parses the gas type from a string
 * @param arg Input string
 * @return Parsed GasType
 */
Simulator::GasType userChoice_GasType(const char * arg);

/*------------------------SIMULATOR------------------------*/
/**
 * @brief Abstract simulator strategy that advances the system state
 */
class Simulator{
public:
    /**
     * @brief Performs the simulation step(s)
     * @param positions In/out particle positions
     * @param velocities In/out particle velocities
     * @param particle_model Model used to compute updates
     * @param path Output path for results
     */
	virtual void compute(Array& positions, Array& velocities, Model& particle_model, std::string& path) = 0;
	virtual ~Simulator() = default;
	
};

/** @brief Simulator for steady computations. */
class SteadySimulator : public Simulator{
public:
	/**
	 * @brief Performs the simulation step(s)
	 * @param positions In/out particle positions
	 * @param velocities In/out particle velocities
	 * @param particle_model Model used to compute updates
	 * @param path Output path for results
	 */
	void compute(Array& positions, Array& velocities, Model& particle_model, std::string& path) override;
	
};

/** @brief Simulator for unsteady computations. */
class UnsteadySimulator : public Simulator{
public:
	/**
	 * @brief Performs the simulation steps
	 * @param positions particle positions
	 * @param velocities particle velocities
	 * @param particle_model Model used to compute updates
	 * @param path Output path for results
	 */
	void compute(Array& positions, Array& velocities, Model& particle_model, std::string& path) override;
	
};

/*------------------------PARTICLES------------------------*/
/**
 * @brief Particles state and simulation control
 */
class Particles{
	Model model;
	std::unique_ptr<Simulator> sim = nullptr;
	ComputeType __computeType;
	unsigned long int nbpart = 0;
public:
	Array position;
	Array velocity;
	
    /** @brief Constructs an empty Particles set. */
	Particles() = default;
    /**
     * @brief Constructs particles arrays with a given number of particles
     * @param i Number of particles
     */
	Particles(unsigned int i){
		nbpart = i;
		position.resize(i);
		velocity.resize(i);
	}
	
    /**
     * @brief Initializes the particles, model and simulator according to configuration
     * @param Sim_type Compute mode
     * @param Pos_type Particles initialization mode
     * @param Gas_type Gas field type
     * @param path Output path for results
     */
	void initialize(ComputeType const& Sim_type, ParticlesInit_mod const& Pos_type, GasType const& Gas_type, std::string& path);
	
	/**
     * @brief Initializes the particles, model and simulator according to configuration using std::threads
     * @param Sim_type Compute mode
     * @param Pos_type Particles initialization mode
     * @param Gas_type Gas field type
     * @param path Output path for results
     */
	void initialize_parallel(ComputeType const& Sim_type, ParticlesInit_mod const& Pos_type, GasType const& Gas_type, std::string& path);
    /**
     * @brief Runs the simulation using the configured simulator
     * @param path Output path for results
     */
	void compute(std::string& path);
	
    /**
     * @brief Runs the simulation using the configured simulator using std::threads
     * @param path Output path for results
     */
	void compute_parallel(std::string& path);
	
	~Particles() {sim.reset();}
};

/*------------------------CHRONO------------------------*/
/**
 * @brief Chronometer for runtime computing
 */
class Chrono{
	std::chrono::time_point<std::chrono::system_clock> time{};
	std::chrono::milliseconds rt{};
	bool running = false;
public:
	Chrono() = default;
	~Chrono() = default;
	/**
	 * @brief Store the actual time and start the chrono
	 */
	void start();
	/**
	 * @brief Store the runtime the chrono
	 */
	void stop();
	/**
	 * @brief Retrun the runtime computed
	 * @return runtime stored
	 */
	std::chrono::milliseconds runtime() const;
	
	void print() const;
};


/*------------------------PROBLEMS------------------------*/
struct Problem{
	unsigned int nb_particles;
	const char ** argv;
	Problem(const char ** args) : argv(args){nb_particles = 16;}
	Problem(unsigned int nbparticles, const char ** args) : nb_particles(nbparticles), argv(args){}
	~Problem() = default;
	
	void solve() const;
	void solve_parallel() const;
};



} //Simulator



#endif /* simulator_h */


