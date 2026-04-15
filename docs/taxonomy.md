# HSO-Engine: Phase 1 Technical Debt Taxonomy (S/B/L)
*Operational Realignment of Tier I (Source) and Tier II (Linkage).*

---

### **Operational Logic: The Three-Stage Firewall**

This taxonomy replaces the "conceptual" Tier I and Tier II registries with a **Functional Enforcement Pipeline**. Each category represents a specific mechanical barrier within the build process.

---

## Audit Categories

#### **1. Category S: Syntactic Integrity (Pre-process Layer)**
* **Mandate:** Eliminates **Source-Level Contamination** before the compiler is invoked.
* **Methodology:** High-velocity regex/string interrogation (S1-S4) and AST-based semantic analysis (S5)
* **Objective:** To enforce architectural constraints that are legally "valid" C++ but architecturally "invalid" for the HSO-Engine.
* **Verdict:** Failure indicates a **Structural Violation**. The code is rejected immediately to prevent "debt" from poisoning the object files.

#### **2. Category B: Binary Determinism (Physical Layer)**
* **Mandate:** Eliminates **Physical Persistent State** and initialization side effects.
* **Methodology:** Direct interrogation of object files (`*.o`) and the final executable (`hso_app`) using `nm`, `readelf`, and `ldd`.
* **Objective:** To prove the absence of mutable global state and verify that the startup payload is zero-latency.
* **Verdict:** Failure indicates a **Concurrency Risk**. Even if the code compiles, the binary is physically unsafe for high-concurrency HPC execution.

#### **3. Category L: Linker/Tooling Stability (Structural Layer)**
* **Mandate:** Eliminates **Dependency Deadlocks** and Scope Collisions.
* **Methodology:** Enforcement of toolchain-level verdicts (compiler warnings and linker resolution rules).
* **Objective:** To ensure the project maintains a strict Directed Acyclic Graph (DAG) for headers and prevents logical errors through scoping rigor.
* **Verdict:** Failure indicates a **Maintenance Deadlock**. The project cannot be reliably extended or linked against external physics libraries.

---

### **Execution Protocol**

| Phase | Tooling | Target | CI/CD Action |
| :--- | :--- | :--- | :--- |
| **Stage 1 (S)** | `grep`, `awk` | Source Code | **Abort Build** on failure. |
| **Stage 2 (L)** | `g++`, `ld` | Translation Units | **Abort Link** on warning/error. |
| **Stage 3 (B)** | `nm`, `readelf` | Binaries | **Reject Artifact** on symbol detection. |

### **The "Mean-Mode" Production Standard**
A build is considered **Production Grade** only when the Category B audit confirms **User Pointers = 0** and **Zero Disjoint Symbols**. The S-Category serves as a low-cost filter to ensure that the more rigorous B-Category interrogation remains focused on physical reality rather than avoidable source noise.

---

## Audit Points

### Category S: Syntactic Audit (Source-Level)

* **S1: Header State Poisoning**
  * **Audit:** Detection of `static` globals within `include/*.h`.
  * **Risk:** Linkage duplication and state fragmentation across translation units.
* **S2: Heavy Inclusion Poisoning**
  * **Audit:** Detection of `<iostream>`, `<vector>`, or `<map>` in headers.
  * **Risk:** Exponential pre-processor load; non-HPC compile times across all 44 files.
* **S3: Guard Integrity**
  * **Audit:** Verification of `#ifndef` or `#pragma once` consistency.
  * **Risk:** Recursive inclusion failures and "Symbol Not Found" errors.
* **S4: Magic Number Hardcoding**
  * **Audit:** Regex detection of raw numeric literals in physics logic.
  * **Risk:** Maintenance drift and loss of precision control in constants.
* **S5: Semantic Inclusion DAG**
  * **Audit:** AST-based analysis of header-to-source relationships.
  * **Risk:** Circular dependencies, transitive include debt, and monolithic rebuilds.


---

## Category L: Linker/Compiler Audit (Tool-Based)

* **L1: Circular Dependency Deadlock**
  * **Audit:** Build-time failure of the Directed Acyclic Graph (DAG).
  * **Risk:** Compilation deadlock; requires forward-declaration refactoring.
* **L2: Primitive Shadowing**
  * **Audit:** Enforcement via `-Wshadow -Werror` compiler flags.
  * **Risk:** Logic errors where local scope "hides" intended global constants.

---

## Category B: Binary Audit (Interrogation-Based)

* **B1: Explicit Global Debt**
  * **Audit:** `get_globals` detecting uppercase `[DBGS]` symbols in `*.o`.
  * **Risk:** Direct violation of stateless architecture; high-level pollution.
* **B2: Persistent Mutation State**
  * **Audit:** Identifying non-const globals in the `.data` segment.
  * **Risk:** Thread-safety violations; allows one file to "poison" engine state.
* **B3: Namespace & Linkage Purity**
  * **Audit:** `get_globals_app` delta check; verifying `hso::` encapsulation.
  * **Risk:** Name collisions with ROOT/LHAPDF; leakage of "Phantom" state.
* **B4: Initialization Penalty & Payload**
  * **Audit:** Interrogation of `.init_array` and non-primitive constructors.
  * **Metric:** Generation of **Table 2** (Target: User Pointers = 0).
  * **Risk:** Startup bottlenecks; "Thundering Herd" performance hits on HPC nodes.
* **B5: Thread-Safe Guard Audit**
  * **Audit:** Detection of `_ZGV` (Guard Variable) symbols in the app binary.
  * **Risk:** Hidden mutex overhead and locking penalties in local static singletons.
* **B6: External Library Tax**
  * **Audit:** `ldd` count and dependency path verification.
  * **Risk:** Inherited debt from unsafe, unoptimized, or non-HPC third-party libraries.

---

## Audit Gates

### Category S: Syntactic Audit (Source-Level)
*Enforced via High-velocity regex/string interrogation (S1-S4) and AST-based semantic analysis (S5)*

* **S1: Header State Poisoning**
  * `test_S1-1.sh` : **HARD** (Fails on static symbol detection)
* **S2: Heavy Inclusion Poisoning**
  * `test_S2-1.sh` : **SOFT** (Baseline reporting: `<iostream>`, `<vector>`)
* **S3: Guard Integrity**
  * `test_S3-1.sh` : **HARD** (Fails if guard is not found or inconsistent)
* **S4: Magic Number Hardcoding**
  * **deferred:**: limited efficiency of bash, need use dedicated tools: Clang-Tidy (readability-magic-numbers)
* **S5: Header Inclusion DAG**
  * `test_S5-1.sh` : **SOFT** (Enforced via `iwyu_tool`; advisory until `pqcd.h` fatal errors are resolved).



---

## Category L: Linker/Compiler Audit (Tool-Based)
*Enforced natively by the C++ toolchain settings. These ensure structural integrity.*

  * **L1: Circular Dependency Deadlock**
    * `CMakeLists.txt`: **HARD** (enforced by `target_link_libraries` hierarchy, failure if non-DAG subgraphs are found in target graph)
    * **Optimization Note**: Current enforcement is limited to the Application/Library boundary. A transition to a Split-Header architecture (`/include` vs `/src`) is required to extend DAG validation to internal components.
  * **L2: Primitive Shadowing**
    * `CMakeLists.txt`: **HARD** (flags `-Wshadow -Werror -Wfatal-errors` enforce failure if shadow symbols found)

---

## Category B: Binary Audit (Interrogation-Based)
*Enforced via `nm`, `readelf`, and `ldd`. These provide the absolute "Physical Truth" of the build.*

* **B1: Explicit Global Debt / B2: Persistent Mutation State (translation unit level)**
  * `test_B1_B2.sh`: **SOFT** (fail on incomplete build, new violations found and violations resolved. Relies on comparison to `audit/global_registry.sym`)
* **B3: Namespace & Linkage Purity (application level)**
  * `test_B3.sh`: **SOFT** (fail on incomplete build, new violations found and violations resolved. Relies on comparison to `audit/global_registry.sym`)
* **B4: Initialization Penalty & Payload**
* **B5: Thread-Safe Guard Audit**
* **B6: External Library Tax**

---


