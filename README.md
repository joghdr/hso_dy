# HSO-DY: Modernization of a Legacy Physics Engine
A C/C++-based architectural framework undergoing systematic modernization to C++20 standards and High-Performance Computing (HPC).

## Engineering Vision & Modernization
This framework is currently transitioning from a research prototype to an industrial-grade engine. The primary focus is the elimination of global state to enable thread-safety, high-performance concurrency, and HPC scalability.

## Roadmap Progress
- [x] Pillar 1: **Infrastructure Foundations** [Stabilized]  
- [ ] Pillar 2: **Global State & Configuration Management** [Current Focus]  
- [ ] Pillar 3: **Architectural Decoupling** [Ongoing]  
- [ ] Pillar 4: **Library Abstraction** [Planned]  
- [ ] Pillar 5: **Modern C++20 & RAII** [Ongoing]  
- [ ] Pillar 6: **High-Performance Optimization** [Planned]  

*For real-time progress and activity ledger, see [Development Notes](hso_engine/docs/development_notes.md).*

## Roadmap Details
### Pillar 1: Infrastructure Foundations
Deployment of a modern CMake build, containerized Docker environment, and integrated CI/CD pipelines to ensure deterministic, scalable builds.
### Pillar 2: Global State & Configuration Management 
Systematic removal of global symbols, replacing scattered I/O and configuration state with a centralized, JSON-based configuration registry. This provides a unified interface for runtime behavior and physical constants, ensuring thread-safe re-entrancy.
### Pillar 3: Architectural Decoupling (DI/Interfaces) 
Implementation of Interface-based design and Dependency Injection to break monolithic interdependencies between Data, Stat, and Theory components, enabling unit testability in isolation.
### Pillar 4: Library Abstraction (Facade Pattern) 
Decoupling the codebase from third-party libraries (e.g., GSL, Minuit2, LHAPDF). This includes encapsulating legacy C-style void* callback interfaces behind type-safe C++ wrappers to enforce memory safety and maintain hardware agnosticism.
### Pillar 5: Modern C++20 & RAII 
Leveraging C++20 constructs (RAII, std::span, concepts) to formalize memory ownership and type safety across the newly abstracted interfaces.
### Pillar 6: High-Performance Optimization 
Profiling and optimizing hot paths for cache locality, eliminating branch divergence, and preparing the infrastructure for future GPU/CUDA acceleration.

## HSO-DY Engine Use Cases
The current `hso-dy` version allows the user to:
- Perform fits on E288 and E605 fixed-target Drell-Yan data.
- Compute TMD eigensets and Hessian bands for observables.
- Plot results (requires Gnuplot).

The current accuracy of perturbative coefficients is $O(\alpha_s)$ A bash script is provided to run the executables inside the Docker container. See [guide](hso_engine/docs/guide.md) for execution details.

## Build Requirements
The build system requires `CMake(3.28)`, `Ninja(1.11)` and `Docker(29.4)`, which the user must pre-install.

## Building the Executables
A containerized build environment is provided via Docker for portability (see [build](hso_engine/docs/build_info.md)). This enables a simple one-step build. Advanced users may refer to `CMakePresets.json` for custom presets and `CMakeLists.txt` for target-specific configuration.

## Dependencies (see [dependencies](hso_engine/docs/dependencies_info.md) for details)
*NOTE: when using the containerized build, all dependencies are automatically handled.*

The code relies on several external libraries and packages:
1. GNU Compiler Collection (g++ 13.3.0+)
2. GNU Scientific Library (libgsl-dev 2.7.1+)
3. Cuba integration libraries (4.2.2)
4. LHAPDF (6.5.4+)
5. Minuit2 (standalone, 5.28.00+)
6. Gnuplot (optional, 6.0+)

The code has been developed in Ubuntu 24.04 (noble), but it should work in other Linux distributions using GCC.

## Publications
The code allows to reproduce results analogous to those published in:
- [Phys.Rev.D 110 (2024) 7, 074016](https://inspirehep.net/literature/2751355)

Other relevant publications on the HSO approach are:
- [Phys.Rev.D 106 (2022) 3, 034002](https://inspirehep.net/literature/2080521)
- [Phys.Rev.D 107 (2023) 9, 094029](https://inspirehep.net/literature/2640018)



