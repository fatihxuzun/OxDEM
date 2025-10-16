# include "OxDEM_library.h"
# include <iostream>
# include <cmath>
# include <cstdlib>
# include <fstream>
# include <string>
# include <chrono>
# include <vector>
# include <sstream>
# include <iterator>
# include <tuple>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////

// particle properties

double Pr = 0.3;								// Particle Poisson's ratio
double Ep = 200e9;								// Particle elastic modulus (Pa)
double Gp = Ep / (2 * Pr + 2);					// Particle shear modulus
double density = 8000.0;						// Density

/////////////////////////////////////////////////////////////////////////////////////////////

double EpD_elastic_total = 0.5 * Ep / density;
double GpD_elastic_total = 0.5 * Gp / density;

/////////////////////////////////////////////////////////////////////////////////////////////

// sequential (0) or OpenMP (1)
const int SeqOrPar = 1;

// model paramters
const int total_time_steps = 100e5;
double time_step_size = 1e-5;

// frame step size
const int frame_step_size = 1e5;

// powder bed wall positions
double xBed = 7.5;
double yBed = 7.5;
double zBed = 1.0;

double BED[30] = { -xBed, 0.0, 0.0, 1.0, 0.0, 0.0, xBed, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -yBed, 0.0, 0.0, 1.0, 0.0, 0.0, yBed, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -zBed, 0.0, 0.0, 1.0 };

int xBoxRange = 5; // number of particles along the x-range
int yBoxRange = 5; // number of particles along the y-range
int zBoxRange = 10; // number of particles along the z-range

const int total_number_of_particles = xBoxRange * yBoxRange * zBoxRange;

double particle_size_range[2] = { 0.5, 1.0 }; // particle radius range

double EpD(double normalStrain)
{	
	return (EpD_elastic_total * normalStrain);
}

double GpD(double shearStrain)
{	
	return (GpD_elastic_total * shearStrain);
}

int main()
{	
	srand(5);

	double P[total_number_of_particles * 34] = { 0.0 };

	for (int i = 0; i < total_number_of_particles; i++) 
	{
		P[i * 27 + 0] = { -xBoxRange + ((i % xBoxRange + 1) * particle_size_range[1] * 2) - particle_size_range[1] };
		P[i * 27 + 1] = { -yBoxRange + (((i / xBoxRange) % yBoxRange + 1) * particle_size_range[1] * 2) - particle_size_range[1] };
		P[i * 27 + 2] = { 0.0 + ((i / (xBoxRange * yBoxRange) + 1) * particle_size_range[1] * 2) - particle_size_range[1] };
		P[i * 27 + 3] = { particle_size_range[0] + (particle_size_range[1] - particle_size_range[0]) * (rand() / ((double)RAND_MAX)) };
		P[i * 27 + 0] = { P[i * 27 + 0] - (particle_size_range[1] - P[i * 27 + 3]) + 2 * (particle_size_range[1] - P[i * 27 + 3]) * (rand() / ((double)RAND_MAX)) };
		P[i * 27 + 1] = { P[i * 27 + 1] - (particle_size_range[1] - P[i * 27 + 3]) + 2 * (particle_size_range[1] - P[i * 27 + 3]) * (rand() / ((double)RAND_MAX)) };
		P[i * 27 + 4] = { density * 4 * 3.1415926535 * P[i * 27 + 3] * P[i * 27 + 3] * P[i * 27 + 3] / 3 };
		P[i * 27 + 20] = { -9.8 };
		P[i * 27 + 10] = { 0.0 };
		P[i * 27 + 11] = { 0.0 };
		P[i * 27 + 12] = { 0.0 };
		P[i * 27 + 17] = { 0.0 };
		P[i * 27 + 18] = { 0.0 };
		P[i * 27 + 19] = { 0.0 };
	}

	double c[9] = { 0.0 };

	auto time_start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < total_time_steps; i++)
	{
		double total_velocity = 0.0;

		if (i % frame_step_size == 0.0) 
		{
			std::cout << i * time_step_size;

			for (int print_i = 0; print_i < total_number_of_particles; print_i++)
			{
				std::cout << '\t' << P[print_i * 27 + 0] << '\t' << P[print_i * 27 + 1] << '\t' << P[print_i * 27 + 2]; // coordinates
				std::cout << '\t' << P[print_i * 27 + 3] << '\t' << P[print_i * 27 + 4]; // radius and mass
				std::cout << '\t' << P[print_i * 27 + 17] << '\t' << P[print_i * 27 + 18] << '\t' << P[print_i * 27 + 19]; // velocity
				std::cout << '\t' << P[print_i * 27 + 21] << '\t' << P[print_i * 27 + 22] << '\t' << P[print_i * 27 + 23]; // acceleration due to external forces
			}

			std::cout << std::endl;
		}

		if (SeqOrPar == 0) 
		{
			for (int j = 0; j < total_number_of_particles; j++) 
			{
				core_solver(j, P);
			}
		}

		if (SeqOrPar == 1) 
		{
			#pragma omp parallel for
			for (int j = 0; j < total_number_of_particles; j++) 
			{
				core_solver(j, P);
			}
		}

		for (int j = 0; j < total_number_of_particles; j++)
		{
			P[j * 27 + 0] = { P[j * 27 + 24] };
			P[j * 27 + 1] = { P[j * 27 + 25] };
			P[j * 27 + 2] = { P[j * 27 + 26] };
		}
	}

	auto time_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<long double> elapsed = time_end - time_start;

	ofstream wsp;
	wsp.open("solution.log", ios::app);
	string Wsp = "elapsed time: "s + to_string(elapsed.count());
	wsp << Wsp;
	wsp.close();

	return 0;
}