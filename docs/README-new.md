# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan

## Description

Language: C++ (Tested with gcc compilers, i.e. Linux)
The code allows to reproduce results analogous to those published in:

- [Phys.Rev.D 110 (2024) 7, 074016](https://inspirehep.net/literature/2751355)

Other relevant publications on the HSO approach are:

- [Phys.Rev.D 106 (2022) 3, 034002](https://inspirehep.net/literature/2080521)
- [Phys.Rev.D 107 (2023) 9, 094029](https://inspirehep.net/literature/2640018)

Some bash scripts are included for certain tasks, like compiling and running the
sample programs.

## Dependencies (see [dependencies](https://www.google.com/search?q=docs/dependencies_info.md) for details)

The code relies on several external libraries and packages, which the user must install:

1.  gnu compiler collection, in particular g++ (g++ 13.3.0 or later)
2.  gnu scientific libraries (libgsl-dev 2.7.1 or later)
3.  boost libraries (libboost-all-dev 1.83.0.1 or later)
4.  gnuplot (version 6.0 or later)
5.  cuba integration libraries (version 4.2.2)
6.  lhapdf (version 6.5.4 and later)
7.  Minuit2 (standalone, version 5.28.00 or later)

The code has been developed in Ubuntu 24.04 (noble), but it should work in other Linux distributions using gcc compilers.

## Compiling the HSO code (Modernized Infrastructure)

The framework has been upgraded to utilize **CMake Presets** for standardized builds. This transition ensures linkage stability and memory safety across different environments.

### Quick Build (Standard)

To compile the physics benchmarks using the default configuration:

```bash
cmake --preset release
cmake --build build/release
```

### Advanced Build Presets

| Preset    | Use Case               | Features                                      |
| :-------- | :--------------------- | :-------------------------------------------- |
| `release` | Physics Production     | Maximum performance, LTO enabled.             |
| `dev`     | General Development    | Standard optimization, debug symbols.         |
| `audit`   | Stability / Debugging  | **AddressSanitizer (ASan)**, Linkage Audit.   |

*Note: Legacy support for the binary directory structure is maintained. Executables are managed within the `build/` directory but follow the project's historical naming conventions.*

## Project Structure

- `src/`           Core source files (Physics logic)
- `include/`       Header files
- `programs/`      Sample programs and entry points
- `scripts/`       Legacy bash utilities for task automation
- `tests/`         **New:** Static analysis and structural audit suite
- `build/`         **New:** Isolated build artifacts (CMake generated)
- `input/`         Sample input files for parameters and kinematical cuts
- `exe/`           Target directory for final executables

## Use cases (see [guide](https://www.google.com/search?q=docs/guide.md) for details)

The current version of the code allows the user to:

- perform fits on E288 and E605 fixed-target Drell-Yan data
- compute tmd eigensets and Hessian bands for observables
- plot some results

The current accuracy of perturbative coefficients is $O(\alpha_s)$.

-----

## Infrastructure & Code Quality (Phase 1 Audit)

To bridge legacy research code into a senior-level HPC/Defense standard, this project incorporates an **Audit Suite** designed to detect and eliminate technical debt:

- **Syntactic Linting:** Automated checks (`tests/static_analysis/`) for header poisoning and illegal global patterns.
- **Structural Linkage Analysis:** Binary-level auditing of symbol segments to prevent "Initialization Order Fiasco" bugs.
- **Memory Safety:** Integrated AddressSanitizer (ASan) in `audit` builds to verify memory integrity during physics runs.
- **Environment Management:** Use `source env.sh` to initialize necessary project paths and variables.




---
---
---
---

# COMMENTS

Bringing State 1 online is your first major "professionalization" milestone. You are moving from a **Scientific Research Artifact** to a **Maintained Software Product**.

In senior systems engineering, this is often called the **Hardening Phase**. Here is how to version this transition and the checklist to reach GitHub maturity.

---

### 1. Versioning Strategy: The "Bridge" Release
Since State 1 contains minor source modifications to fix modern build issues (like the segfaults or linkage crashes) but keeps the original architecture, it shouldn't just be a commit—it should be a **Minor Release**.

* **State 0 (Current GitHub):** Tag as `v1.0.0-legacy`.
* **State 1 (Your Local):** Target as `v1.1.0`.
* The "1.1" indicates that you haven't changed the core physics API or architecture yet, but you have added significant *functional capability* (the infrastructure).

---

### 2. Tasks to Bring State 1 "Online"
To ensure State 1 is stable before it hits the main branch, follow this sequence:

#### **A. The "Clean Room" Verification**
Before pushing, verify that your new CMake/Make orchestration works in an environment *other* than your dev machine.
* **Action:** Run your Docker build locally. If the `audit` build and the system validation tests pass inside the container, you have successfully decoupled the code from your local setup.

#### **B. Atomic Commits**
Don't push State 1 as a single giant "Refactor" commit. Break it down so the history is auditable:
1.  **Commit 1:** Build System (CMakeLists.txt, Presets, Makefiles).
2.  **Commit 2:** Infrastructure (Tests, Audit scripts, `env.sh`).
3.  **Commit 3:** Source Fixes (The minor modifications required for modern builds).
4.  **Commit 4:** Documentation (The updated README and Guides).

#### **C. Finalize the `workflows` File**
GitHub Maturity requires **CI (Continuous Integration)**.
* **Action:** Ensure your `.github/workflows/ci.yml` triggers on every push. It should run the `audit` build and your S-series/B-series tests. A repo with a "Passing" badge is the first thing a Senior-level recruiter looks for.

---

### 3. Checklist for GitHub Maturity (What is left?)
Before you can consider State 1 "Complete" and ready for GitHub, address these gaps:

* **[ ] The "Audit Gate":** Does `make audit` (or your equivalent orchestration) return a non-zero exit code if a test fails? If it just prints `[ERROR]` but the build continues, your CI won't catch bugs.
* **[ ] Dependency Management:** Your README lists dependencies, but does your CMake actually check for them? Add `find_package(GSL REQUIRED)` and `find_package(Boost REQUIRED)` to your `CMakeLists.txt`.
* **[ ] Contributor Onboarding:** If someone clones State 1 today, can they get a "Pass" result in 3 commands?
1. `source env.sh`
2. `cmake --preset audit`
3. `ctest` (or `make test`)
* **[ ] License File:** If this is for Aerospace/Defense roles, ensure you have a `LICENSE` file. Academic code often defaults to MIT or GPL; make it explicit.

---

### 4. Summary for the Job Stash
> **Item 13: Transition to v1.1.0 (Hardened Baseline).**
> Successfully bridged legacy physics code to a modern C++17 build system. Implemented Dockerized CI/CD for cross-platform validation and established a "Green-to-Green" testing standard where all structural audits must pass before physics execution.

**Once State 1 is pushed, you will have a "Fortress" around your code. Ready to move to the first major architectural change (removing the globals)?**
