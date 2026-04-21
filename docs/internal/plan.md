# HSO-Engine: V&V Framework and Refactor Roadmap

## 1. Phase 1: Invariant Baselining (Discovery)
**Objective:** Establish the verification substrate and document the legacy physical state.

* **Branch:** `infra/baseline`
* **Action 1 (The Framework):** Commit `docs/taxonomy.md`, `tests/*.sh`, and `.gitignore`.
* *Logic:* Pairs the structural requirements with the interrogation scripts to ensure the "Requirement" and "Tool" are versioned together.
* **Action 2 (The Discovery Build):** Compile legacy artifacts with `-Werror` and `-Wshadow` suppressed to ensure a total linker pass despite existing debt.
* **Action 3 (State Capture):** Execute `get_globals_raw` and `get_globals_raw_app` to generate the legacy registries.
* **Action 4 (The Lock):** Commit `audit/global_registry.sym` and `audit/global_registry_app.sym`.
* **Action 5 (The Firewall):** Implement `.github/workflows/verify-syntax.yml`.
* **Enforcement Levels:** S1/S3: **HARD** | S5: **SOFT** | B1/B3: **ADVISORY**.

tasks:
tasks are:

-set up cmakelists.txt

-set up cmakepresets.json

-set up makefile

-set up docker file

-add all tests for developer and soft tests to replicate output (from research paper)

- set up the version 1.0.0 (both docker file and github=VERSION file)

-set up the initial snapshot of registry

-.dockerignore

-code entry point (for user)



assess. am ia missing something?



---

## 2. Phase 2: Structural Decoupling (The Vertical Refactor)
**Objective:** Eliminate parsing debt and execute the **Inversion of Control (IoC)** prototype.

* **Branch:** `refactor/fcn-ioc-prototype`
* **Action 1 (Parsing Unlock):** Resolve `include/pqcd.h` math debt (replace `std::pow` with optimized equivalents) to allow Clang-based AST analysis and IWYU (Include What You Use) graduation.
* **Action 2 (IoC Implementation):** Deploy `hso::FitterApp` facade. Refactor the `FCN` path via Constructor Injection to eliminate `extern` dependencies and global state linkage.
* **Action 3 (Convergence Proof):** Execute `find_resolved_globals` to quantify binary debt reduction against the Phase 1 Baseline.
* **Action 4 (Reporting):** Generate `docs/dependency_matrix.md` mapping remaining coupling.
* **Gate Update:** Escalate S5 (Inclusion DAG) to **HARD**.

---

## 3. Phase 3: Physical Truth (Binary Integrity Enforcement)
**Objective:** Formalize toolchain constraints and automate binary determinism gates.

* **Branch:** `infra/binary-gates`
* **Action 1 (Toolchain Hardening):** Update `CMakeLists.txt` with `-Wshadow -Werror -Wfatal-errors` and strict DAG-linkage constraints.
* **Action 2 (Binary Ratchet):** Integrate `find_new_globals` into CI/CD as a **HARD** gate. Any new mutable state beyond the current registry terminates the build.
* **Action 3 (Init-Array Audit):** Deploy `tests/b4_init_audit.sh` to verify zero-latency startup through `.init_array` interrogation.
* **Final Status:** All S/L/B Gates move to **HARD**. The system is officially classified as **Production Grade**.

---

## 4. Requirement Mapping & Tracking

| Milestone | Registry Mapping | Intent |
| :--- | :--- | :--- |
| **Recovery Baseline** | S1-S3, S5, B1-B3 | Documentation and snapshot of legacy technical debt. |
| **Logic Scoping** | L1, L2 | Resolution of symbol shadowing and circular dependencies. |
| **Namespace Isolation** | B3 | Physical verification of `hso::` encapsulation. |
| **Determinism Lock** | B1-B6 | Proof of thread-safe, stateless HPC-ready architecture. |

---

## 5. Execution Summary
1.  **Baseline:** Push framework and capture **Registry Snapshots**.
2.  **Refactor:** Fix `pqcd.h`, execute `FCN` refactor, and verify convergence via registries.
3.  **Harden:** Transition all observational gates to **Hard Enforcement** and lock the production toolchain.







