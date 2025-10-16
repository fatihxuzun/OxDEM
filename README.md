# OxDEM
OxDEM is a software tool for Discrete Element Method (DEM) simulations, designed for modeling granular and powder systems in engineering applications. This repository contains the source code, scripts, build configurations, and proprietary precompiled binary components for OxDEM. Please note that this is an ongoing project. The repository will be updated periodically with new developments, improvements to the methodology, and additional features. For more information see the citing paper given below and contact with the authors of this paper.

## License
This project is licensed under the MIT License with restrictions on proprietary binary components. See the LICENSE file for details. The open-source parts can be freely used, modified, and distributed, while the proprietary binaries in the lib/ directory (e.g., OxDEM_library.o) are for academic and research use only and cannot be reverse-engineered or redistributed without permission.

## Download and Installation
To download and install OxDEM, it is recommended to use the terminal for cloning the repository via Git.

Clone the repository:

```bash
git clone https://github.com/fatihxuzun/OxDEM.git
```

## Usage

Run OxDEM via the automated script:

```bash
bash OxDEM.sh
```

This single command will:

* Create a temporary working directory
* Copy required components (src/, include/, lib/, and scripts/)
* Build the OxDEM executable using CMake
* Run the solver with default parameters
* Redirect all console output to a timestamped log file
* Perform Python post-processing automatically
* Archive the entire run into a compressed .zip file

## Outputs

The resulting output (e.g. pbg_iTk6Z.zip) contains:

* simulation_output.log
* solution.log
* time_frame.pvd

## Scientific Usage
This software has been developed as part of the research introducing the OxDEM framework, which formulates the Discrete Element Method based on strain energy principles to model granular and powder systems with enhanced physical consistency.Please cite:

Uzun, F., Korsunsky, A.M. OxDEM: Discrete Element Method based on strain energy formulation. Advanced Powder Technology, 36(11), 105082 (2025). https://doi.org/10.1016/j.apt.2025.105082

## Publications
