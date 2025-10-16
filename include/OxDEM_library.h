#ifndef OXDEM_LIBRARY_H
#define OXDEM_LIBRARY_H

// =====================================================================================
// OxDEM Core Solver Header
// Handles particle-particle, particle-bed, and particle-wall interactions.
// Developed as part of the OxDEM model (Uzun & Korsunsky, 2025).
// =====================================================================================

#include <cmath>

// =============================
// External global definitions
// =============================
extern const int total_number_of_particles;

extern double time_step_size;
extern double BED[];

// =============================
// External functions
// =============================
double EpD(double normalStrain);
double GpD(double shearStrain);

// =============================
// Core solver declaration
// =============================
double core_solver(int k, double P[]);

#endif // OXDEM_core_solver_H