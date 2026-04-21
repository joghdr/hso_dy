### Description
#### **11-Point Audit**
**Point #1: Extern Variables**
* **Audit:** Trace global variables defined in headers with the `extern` keyword.
* **Risk:** Triggers **Multiple Definition** linker errors during the final build phase.

**Point #2: Static Variables in Headers**
* **Audit:** Detection of `static` globals within `.h` files.
* **Risk:** Creates a redundant private copy in every translation unit, bloating binary size and fragmenting state.

**Point #3: Persistent Global State**
* **Audit:** Identification of non-const globals (e.g., `Parton_charge`).
* **Risk:** Thread-safety violation; allows one file to "poison" constants for the entire engine.

**Point #4: Namespace Encapsulation**
* **Audit:** Check for variables/functions residing in the global namespace.
* **Risk:** Name collisions with external physics libraries (ROOT, LHAPDF).

**Point #5: Transitive Inclusion Poisoning**
* **Audit:** Identification of "heavy" headers (`<iostream>`, `<map>`, `LHAPDF.h`) inside `.h` files.
* **Risk:** Exponentially increases pre-processor load and compile times across all 24 files.

**Point #6: Header Guard Integrity**
* **Audit:** Verification of `#ifndef` or `#pragma once` consistency.
* **Risk:** Recursive inclusion failures and "Symbol Not Found" errors.

**Point #7: Circular Dependencies**
* **Audit:** Checking if `A.h` includes `B.h` while `B.h` includes `A.h`.
* **Risk:** Compilation deadlock; requires forward declaration refactoring.

**Point #8: Global Constructors (The "Hidden" Penalty)**
* **Audit:** Identification of non-primitive globals (e.g., `std::map`, `std::vector`).
* **Risk:** Forces heap allocation before `main()`; creates "thundering herd" bottlenecks on HPC nodes.

**Point #9: Primitive Shadowing**
* **Audit:** Checking if local variables in `.cpp` files share names with header-level globals.
* **Risk:** Logical errors where local scope "hides" the intended global constant.

**Point #10: Magic Number Hardcoding**
* **Audit:** Detection of raw literals (e.g., `3.14159`) instead of `constexpr` symbolic names.
* **Risk:** Maintenance failure and precision drift across the physics engine.

**Point #11: Linkage Purity & Symbol Visibility**

* **Audit Protocol:**
1.  **Function Bodies:** Every function defined within a header (`.h`/`.hpp`) **must** be explicitly marked with the `inline` keyword to allow the linker to merge duplicates across translation units.
2.  **Stateful Objects:** Raw `extern` declarations of complex, non-primitive objects (e.g., `StatNMin`, `std::vector`, `std::map`) are prohibited. These must be replaced with **Meyers Singletons** (static local variables within an `inline` accessor function).
3.  **Internal Linkage:** Prohibition of `static` variables in headers. These create independent "memory islands" in every `.cpp` file, leading to silent numerical divergence and memory bloat.

* **Risk Assessment:**
* **The Static Initialization Order Fiasco:** The primary cause of `SIGSEGV` (Segmentation Faults) under Link-Time Optimization (`-flto`). Without a functional wrapper, the compiler cannot guarantee that a global object's constructor (e.g., allocating a `std::vector`) runs before the physics logic attempts to access it.
* **Linker Collisions:** Hard-stop failures during the final bind stage due to "Multiple Definition" errors.
* **Binary Inconsistency:** Prevents the optimizer from safely deduplicating symbols, which slows down the 20-core build process and causes the "Ghost Binary" effect where code runs by luck rather than design.

---

### **Audit Point #1: Global Dependency Registry**
| Unique ID | Symbol Name | Definition Site | Data Type | Access (R/W) | Impacted Files |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **P1-001** | `chi2_total` | `src/objects_stat.cpp` | `std::vector<hso::Stat*>` | RW | `src/objects_stat.cpp(W)`, `src/models_fcn-tmd-gaussx1-CS-gauss.cpp(R)`, `src/models_fcn-tmd-gaussx2-CS-gauss.cpp(R)`, `programs/hessian_DY_hso.c(R)`, `programs/fit_fixed_target_DY.c(R)` |
| **P1-002** | `chi2_e288` | `src/objects_stat.cpp` | `hso::StatNMin` | RW | `src/objects_stat.cpp(W)`, `src/utilities.cpp(RW)` |
| **P1-003** | `chi2_e605` | `src/objects_stat.cpp` | `hso::StatNMin` | RW | `src/objects_stat.cpp(W)`, `src/utilities.cpp(RW)` |
| **P1-004** | `chi2_cdf_I` | `src/objects_stat.cpp` | `hso::StatNMin` | RW | `src/objects_stat.cpp(W)` |
| **P1-005** | `chi2_cdf_II` | `src/objects_stat.cpp` | `hso::StatNMin` | RW | `src/objects_stat.cpp(W)` |
| **P1-006** | `chi2_d0_I` | `src/objects_stat.cpp` | `hso::StatNMin` | RW | `src/objects_stat.cpp(W)` |
| **P1-007** | `ABC_coeff_mode` | `src/collinear_factorization.cpp` | `enum class GetCoefficients` | RW | `src/behavior.cpp(W)`, `src/collinear_factorization.cpp(RW)`, `src/pdf_cutoff_from_msbar.cpp(R)` |
| **P1-008** | `fit_iteration_counter` | `src/FCN.cpp` | `int` | RW | `src/FCN.cpp(RW)`, `src/models_fcn-tmd-gaussx1-CS-gauss.cpp(W)`, `src/models_fcn-tmd-gaussx2-CS-gauss.cpp(W)`, `src/utilities.cpp(W)` |
| **P1-009** | `e288_200_4` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-010** | `e288_200_5` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-011** | `e288_200_6` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-012** | `e288_200_7` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-013** | `e288_200_8` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-014** | `e288_200_9` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-015** | `e288_200_10` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-016** | `e288_300_4` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-017** | `e288_300_5` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-018** | `e288_300_6` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-019** | `e288_300_7` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-020** | `e288_300_8` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-021** | `e288_300_9` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-022** | `e288_300_10` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-023** | `e288_300_11` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-024** | `e288_400_5` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-025** | `e288_400_6` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-026** | `e288_400_7` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-027** | `e288_400_8` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-028** | `e288_400_9` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-029** | `e288_400_10` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-030** | `e288_400_11` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(W)` |
| **P1-031** | `e288_400_12` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-032** | `e288_400_13" | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-033** | `e605_800_7` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-034** | `e605_800_8` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-035** | `e605_800_10p5` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-036** | `e605_800_11p5` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-037** | `e605_800_13p5` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-038** | `cdfI_Z0pt_1991_raw` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-039** | `cdfI_Z0pt_1999_raw` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-040** | `d0I_Z0pt_1999_raw` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-041** | `cdfII_Z0pt_2012_raw` | `src/objects_data.cpp` | `hso::Data` | RW | `src/objects_data.cpp(RW)` |
| **P1-042** | `e288` | `src/objects_data.cpp` | `std::vector<hso::Data*>` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(RW)` |
| **P1-043** | `e605` | `src/objects_data.cpp` | `std::vector<hso::Data*>` | RW | `src/objects_data.cpp(RW)`, `src/utilities.cpp(RW)` |
| **P1-044** | `cdfI_Z0pt_1999` | `src/objects_data.cpp` | `std::vector<hso::Data*>` | RW | `src/objects_data.cpp(RW)` |
| **P1-045** | `cdfII_Z0pt_2012` | `src/objects_data.cpp` | `std::vector<hso::Data*>` | RW | `src/objects_data.cpp(RW)` |
| **P1-046** | `d0I_Z0pt_1999` | `src/objects_data.cpp` | `std::vector<hso::Data*>` | RW | `src/objects_data.cpp(RW)` |
| **P1-047** | `tevatron_I_Z0` | `src/objects_data.cpp` | `std::vector<hso::Data*>` | RW | `src/objects_data.cpp(RW)` |
| **P1-048** | `tevatron_II_Z0` | `src/objects_data.cpp` | `std::vector<hso::Data*>` | RW | `src/objects_data.cpp(RW)` |
| **P1-049** | `observable_drell_yan_1` | `src/objects_theory.cpp` | `hso::Theory` | RW | `src/objects_theory.cpp(W)`, `src/objects_data.cpp(R)` |
| **P1-050** | `observable_drell_yan_4_xFavg_qTavg` | `src/objects_theory.cpp` | `hso::Theory` | RW | `src/objects_theory.cpp(W)`, `src/objects_data.cpp(R)` |
| **P1-051** | `observable_drell_yan_Z0_1` | `src/objects_theory.cpp` | `hso::Theory` | RW | `src/objects_theory.cpp(W)`, `src/objects_data.cpp(R)` |
| **P1-052** | `hso::read_data::verbose` | `src/read_data.cpp` | `bool` | RW | `src/read_data.cpp(RW)` |
| **P1-053** | `hso::read_kin::verbose` | `src/read_kin.cpp` | `bool` | RW | `src/read_kin.cpp(RW)` |
| **P1-054** | `hso::read_para::verbose` | `src/read_para.cpp` | `bool` | W | `src/read_kin.cpp(W)` |
| **P1-055** | `store_values_stat` | `src/stat_class.cpp` | `bool` | RW | `src/stat_class.cpp(W)`, `src/utilities.cpp(W)`, `include/stat_class.h(R)` |
| **P1-056** | `store_values_theory` | `src/theory_class.cpp` | `bool` | RW | `src/theory_class.cpp(W)`, `src/utilities.cpp(W)`, `include/theory_class.h(R)` |
| **P1-057** | `eigen_index` | `src/theory_class.cpp` | `int` | RW | `src/theory_class.cpp(W)`, `src/utilities.cpp(RW)`, `include/theory_class.h(R)` |
| **P1-058** | `Apdf_lhapdf_pointer` | `src/collinear_factorization.cpp` | `LHAPDF::PDF*` | RW | `src/collinear_factorization.cpp(RW)` |
| **P1-059** | `Apdf_g_lhapdf_pointer` | `src/collinear_factorization.cpp` | `LHAPDF::PDF*` | RW | `src/collinear_factorization.cpp(RW)` |
| **P1-060** | `Bpdf_lhapdf_pointer` | `src/collinear_factorization.cpp` | `LHAPDF::PDF*` | RW | `src/collinear_factorization.cpp(RW)` |
| **P1-061** | `FtildeCore` | `src/models_fcn-tmd-gaussx*-CS-gauss.cpp` | `double (double, double, void *)` | EXEC | `src/models_fcn... (Internal)`, `src/tmdPDF.cpp(External)` |
| **P1-062** | `hso::util::verbose` | `src/utilities.cpp` | `bool` | W | `src/utilities.cpp(W)` |

---

### **Audit Point #2: Static Variables in Headers**: PASSED

---

### **Audit Point #3: Persistent Global State**
| Unique  ID | Symbol Name | Location | Violation Details | HPC Risk Profile |
| :--- | :--- | :--- | :--- | :--- |
| **P1-001** | `chi2_total` | `src/objects_stat.cpp` | **Mutable Vector:** Read/Write access across 5+ translation units. | **Non-Deterministic:** Simultaneous writes to the same vector from different MPI ranks will corrupt memory. |
| **P1-007** | `ABC_coeff_mode` | `src/collinear_factorization.cpp` | **Global Enum Switch:** Changes core physics logic globally. | **Race Condition:** One thread changing the mode affects every other thread's cross-section calculation. |
| **P1-008** | `fit_iteration_counter` | `src/FCN.cpp` | **Shared Integer State:** Tracks iteration globally across 24 files. | **Sync Failure:** Prevents independent parallel fits from reporting correct local iteration counts. |
| **P1-042 - P1-048** | `e288`, `e605`, etc. | `src/objects_data.cpp` | **Mutable Data Containers:** Global vectors of pointers to raw data. | **Pointer Poisoning:** Allows any file to reallocate or clear the raw data during a run. |
| **P1-058 - P1-060** | `...lhapdf_pointer` | `src/collinear_factorization.cpp` | **Mutable Global Pointers:** Unprotected access to PDF objects. | **Critical Failure:** If one file initializes/swaps the pointer, all other 23 files are forced to use the new set instantly. |
| **P3-063** | `min` | `src/utilities.cpp` | **File-Scope Static State:** Persistent across function calls via `static`. | **Non-Reentrant:** Only one minimizer can exist in the memory space at a time; prevents parallel fitting. |
| **P3-065** | `Parton_charge` | `include/pqcd.h` | **Mutable Map in Header:** Global map of physics constants. | **Logical Poisoning:** Constants are not `const`; they can be altered mid-fit, invalidating all subsequent physics. |
| **P1-055** | `store_values_stat` | `src/stat_class.cpp` | **Global Control Toggle:** Boolean state for class behavior. | **Execution Divergence:** Modifies logic flow globally; hidden dependency makes debugging side-effects impossible. |
| **P1-057** | `eigen_index` | `src/theory_class.cpp` | **Shared Integer:** Tracks the current eigenvalue index. | **Parallel Blocker:** Multiple threads cannot process different eigenvalues simultaneously if they share this index. |

---

### **Audit Point #4: Namespace Encapsulation** PASSED

---

## **Audit Point #5: Transitive Inclusion Poisoning**

* **Total Headers Scanned:** 26
* **Failures:** 17 (65.4%)
* **Passes:** 9 (34.6%)

| ID | Header File | LC | Status | All Identified Poisons (Includes) | Audit Note |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **P5-001** | `utilities.h` | 74,949 | **FAIL** | `data_class.h`, `stat_class.h`, `FCN.h`, `Minuit2/FunctionMinimum.h`, `<vector>`, `<string>` | **Critical:** Aggregates solver, stats, and data stacks. |
| **P5-002** | `tmdPDF.h` | 76,189 | **FAIL** | `pqcd.h`, `LHAPDF/LHAPDF.h` | **Critical:** Highest LC; transitively pulls LHAPDF framework. |
| **P5-003** | `FCN.h` | 48,990 | **FAIL** | `stat_class.h`, `Minuit2/FCNBase.h`, `<string>`, `<vector>`, `<iostream>`, `<fstream>` | Leaks solver and disk I/O into physics logic. |
| **P5-004** | `collinear_factorization.h` | 40,250 | **FAIL** | `pqcd.h`, `<string>` | Propagates pQCD logic and string parsing overhead. |
| **P5-005** | `debug.h` | 36,584 | **FAIL** | `<iostream>` | Injects static initializer overhead on all inclusions. |
| **P5-006** | `WDY_hso.h` | 36,457 | **FAIL** | `pqcd.h`, `<vector>` | Heavy math + STL container bloat for constants. |
| **P5-007** | `objects_stat.h` | 35,133 | **FAIL** | `stat_class.h`, `<vector>` | Propagates `chi2_total` vector bloat. |
| **P5-008** | `stat_class.h` | 35,111 | **FAIL** | `data_class.h`, `theory_class.h`, `<string>`, `<vector>` | High-order coupling node (Data + Theory). |
| **P5-009** | `objects_theory.h` | 34,939 | **FAIL** | `theory_class.h` | Transitive bloat from theory object declarations. |
| **P5-010** | `theory_class.h` | 34,924 | **FAIL** | `behavior.h`, `data_class.h`, `cuba.h`, `<initializer_list>`, `<string>`, `<vector>` | Leaks numerical integration (Cuba) into domain. |
| **P5-011** | `objects_data.h` | 34,768 | **FAIL** | `data_class.h` | Propagates data class bloat via composition. |
| **P5-012** | `data_class.h` | 34,708 | **FAIL** | `<vector>`, `<string>`, `<initializer_list>`, `<map>`, `<tuple>` | **Root Poison:** Foundational STL container bloat. |
| **P5-013** | `read_data.h` | 34,677 | **FAIL** | `<string>`, `<vector>`, `<map>`, `<tuple>` | Redundant heavy-container (STL) poisoning. |
| **P5-014** | `pdf_cutoff_from_msbar.h` | 31,185 | **FAIL** | `pqcd.h` | Propagates math/map dependencies via `pqcd.h`. |
| **P5-015** | `pqcd.h` | 31,148 | **FAIL** | `<cmath>`, `<map>` | Injects `<map>` into foundational math logic. |
| **P5-016** | `read_kin.h` | 30,017 | **FAIL** | `<string>`, `<vector>` | Container bloat in kinematic input headers. |
| **P5-017** | `read_para.h` | 30,015 | **FAIL** | `<string>`, `<vector>` | Container bloat in parameter input headers. |
| --- | `behavior.h` | 26 | **PASS** | None | Compliant; minimal overhead. |
| --- | `csec_DY_css_hso_integrands_cuba.h` | 65 | **PASS** | None | Compliant; specific integrand logic. |
| --- | `CSkernel.h` | 290 | **PASS** | `<cstddef>` | Compliant; minimal overhead. |
| --- | `cuba_settings.h` | 75 | **PASS** | None | Compliant; primitive configuration. |
| --- | `evo.h` | 20 | **PASS** | None | Compliant; evolution interface. |
| --- | `gnuplot_script_names.h` | 14 | **PASS** | None | Compliant; string literals only. |
| --- | `gsl_settings.h` | 36 | **PASS** | None | Compliant; primitive setup. |
| --- | `math_functions.h` | 14 | **PASS** | None | Compliant; lightweight wrapper. |
| --- | `scale_setting.h` | 37 | **PASS** | `behavior.h` | Compliant; low-impact inclusion. |

---

## **Audit Point #6: Header Guard Integrity**

* **Total Headers Scanned:** 26
* **Failures:** 7 (26.9%)
* **Passes:** 19 (73.1%)

| ID | Header File | Current Guard Macro | Status | Violation Note |
| :--- | :--- | :--- | :--- | :--- |
| **P6-001** | `debug.h` | `_MACROS_H_` | **FAIL** | Complete name mismatch; high collision risk with system/external macros. |
| **P6-002** | `csec_DY_css_hso_integrands_cuba.h` | `_CSEC_RG_IMPROVED_INTEGRANDS_H_` | **FAIL** | Content-descriptive mismatch; does not follow filename. |
| **P6-003** | `pdf_cutoff_from_msbar.h` | `_PDF_H_` | **FAIL** | Excessive truncation; high collision risk with LHAPDF or other PDF libs. |
| **P6-004** | `collinear_factorization.h` | `_COLLINEAR_H_` | **FAIL** | Truncated; deviates from `_FNAME_H_` convention. |
| **P6-005** | `gsl_settings.h` | `_GLS_SETTINGS_H_` | **FAIL** | **Typo:** "GLS" instead of "GSL". Breaks automated refactoring. |
| **P6-006** | `gnuplot_script_names.h` | `_GNUPLOT_SCRIPT_NAMES_` | **FAIL** | Missing trailing `H_` suffix. |
| **P6-007** | `objects_theory.h` | `_OBJECTS_Theory_H_` | **FAIL** | Case-sensitivity violation (`Theory` vs `THEORY`). |
| --- | `behavior.h` | `_BEHAVIOR_H_` | **PASS** | Compliant. |
| --- | `CSkernel.h` | `_CSKERNEL_H_` | **PASS** | Compliant. |
| --- | `cuba_settings.h` | `_CUBA_SETTINGS_H_` | **PASS** | Compliant. |
| --- | `data_class.h` | `_DATA_CLASS_H_` | **PASS** | Compliant. |
| --- | `evo.h` | `_EVO_H_` | **PASS** | Compliant. |
| --- | `FCN.h` | `_FCN_H_` | **PASS** | Compliant. |
| --- | `math_functions.h` | `_MATH_FUNCTIONS_H_` | **PASS** | Compliant. |
| --- | `objects_data.h` | `_OBJECTS_DATA_H_` | **PASS** | Compliant. |
| --- | `objects_stat.h` | `_OBJECTS_STAT_H_` | **PASS** | Compliant. |
| --- | `pqcd.h` | `_PQCD_H_` | **PASS** | Compliant. |
| --- | `read_data.h` | `_READ_DATA_H_` | **PASS** | Compliant. |
| --- | `read_kin.h` | `_READ_KIN_H_` | **PASS** | Compliant. |
| --- | `read_para.h` | `_READ_PARA_H_` | **PASS** | Compliant. |
| --- | `scale_setting.h` | `_SCALE_SETTING_H_` | **PASS** | Compliant. |
| --- | `stat_class.h` | `_STAT_CLASS_H_` | **PASS** | Compliant. |
| --- | `theory_class.h` | `_THEORY_CLASS_H_` | **PASS** | Compliant. |
| --- | `tmdPDF.h` | `_TMDPDF_H_` | **PASS** | Compliant. |
| --- | `utilities.h` | `_UTILITIES_H_` | **PASS** | Compliant. |
| --- | `WDY_hso.h` | `_WDY_HSO_H_` | **PASS** | Compliant. |

---

## **Audit Point #7: Circular Dependencies PASSED**
Infrastructure is stable ( directed acyclic graph)

---

### **Audit Point #8: Global Constructor Registry**
* **Total Pointers in `.init_array`:** 15
* **Critical Offenders:** `hso::chi2_e288` (objects_stat.cpp), `hso::AlphaStrong` (pqcd.cpp), `hso::home_dir` (utilities.cpp).

#### **Table 1: User-Defined Static Constructors (Check 1)**
*Internal translation units triggering pre-main() execution.*

| Symbol Type | Symbol Name | Source File | Line |
| :--- | :--- | :--- | :--- |
| t | hso::collinear::ABC_coeff_mode | collinear_factorization.cpp | 438 |
| t | hso::csec_no_rg_improved::integrand_drell_yan_1 | csec_DY_css_hso_integrands_cuba.cpp | 517 |
| t | hso::IntegrateGamma | CSkernel.cpp | 151 |
| t | hso::BesselK0 | math_functions.cpp | 39 |
| t | hso::e288_200_4 | objects_data.cpp | 91 |
| t | hso::chi2_e288 | objects_stat.cpp | 27 |
| t | _ZN3hso9chi2_e288E.cold | stl_vector.h | 370 |
| t | hso::observable_drell_yan_1 | objects_theory.cpp | 48 |
| t | hso::collinear::fr1Oas1_lhapdf_pointer | pdf_cutoff_from_msbar.cpp | 283 |
| t | hso::AlphaStrong | pqcd.cpp | 24 |
| t | hso::RGScaleTransformation | scale_setting.cpp | 23 |
| t | hso::FtildeInputOrder_1_msbar | tmdPDF.cpp | 75 |
| t | hso::home_dir[abi:cxx11] | utilities.cpp | 2151 |
| t | hso::drell_yan::MaxLeptonRapidity | WDY_hso.cpp | 494 |

#### **Table 2: Binary Startup Payload (Check 2)**
*The physical footprint of the .init_array section in the executable.*

| Metric | Value | Technical Meaning |
| :--- | :--- | :--- |
| **Total Pointers** | 15 | Total functions executed before `main()` |
| **Section Size** | 120 Bytes | 15 pointers * 8 bytes (x86_64) |
| **User vs System** | 14 vs 1 | 14 explicit user targets + 1 CRT/System pointer |

#### **Table 3: Guard Variable Audit (Check 3)**
*Evidence of inline variables and thread-safe local statics (Implicit Debt).*

| Object File | Target Symbol (Guard) | Type | Status |
| :--- | :--- | :--- | :--- |
| src/pqcd.o | hso::Parton_charge | u | **Inline Map** (Header-defined) |
| src/pqcd.o | hso::nucleon_number | u | **Inline Map** (Header-defined) |
| src/utilities.o | hso::Minimize::min | b | **Local Static** (Function-scope) |
| src/utilities.o | ROOT::Minuit2::gStackAllocator | u | **External Static** (Library-scope) |
| src/math_functions.o| boost::math::bessel_k0_initializer | u | **Third-Party** (Header-only lib) |

#### **Table 4: External Library Dependency Tax (Check 4)**
*Global initialization cost inherited from linked shared objects.*

| External Library | .init_array Size (Hex) | Num. of Pointers |
| :--- | :--- | :--- |
| /usr/local/lib/libLHAPDF.so | 000138 | 39 |
| /lib/x86_64-linux-gnu/libstdc++.so.6 | 000088 | 17 |
| /usr/local/lib/libMinuit2.so.0 | 000048 | 9 |
| /lib/x86_64-linux-gnu/libgomp.so.1 | 000018 | 3 |
| /lib/x86_64-linux-gnu/libgcc_s.so.1 | 000010 | 2 |
| /lib/x86_64-linux-gnu/libc.so.6 | 000010 | 2 |
| /lib/x86_64-linux-gnu/libgsl.so.27 | 000008 | 1 |
| /lib/x86_64-linux-gnu/libm.so.6 | 000008 | 1 |
| /lib/x86_64-linux-gnu/libgslcblas.so.0 | 000008 | 1 |

#### **Point #9: Primitive Shadowing**

| ID | File | Line | Local Symbol | Shadowed Source / Location | Nature of Debt |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **S9-001** | `utilities.cpp` | 173 | `line` (stringstream) | `line` (string) @ L130 | **Type Poisoning**: Re-using a name for a different type in one function. |
| **S9-002** | `utilities.cpp` | 417 | `line` (stringstream) | `line` (string) @ L381 | **Type Poisoning**: Repeated pattern in `ReadVector`. |
| **S9-003** | `utilities.cpp` | 1212 | `i` (int) | `i` (int) @ L1094 | **Iterator Conflict**: Loop counter hiding a functional variable. |
| **S9-004** | `utilities.cpp` | 1243 | `i` (int) | `i` (int) @ L1094 | **Iterator Conflict**: Triple-use of `i` in the same scope. |
| **S9-005** | `utilities.cpp` | 1731 | `eigen_filename` | `eigen_filename` @ L47 | **Global Shadow**: Local variable hiding a file-scope global string. |
| **S9-006** | `data_class.cpp` | 283 | `oor` (exception) | `oor` (exception) @ L269 | **Exception Shadow**: Nested catch block re-using error identifier. |
| **S9-007** | `data_class.cpp` | 295 | `oor` (exception) | `oor` (exception) @ L269 | **Exception Shadow**: Deep nesting risk in error handling. |


---

### **Point #10: Magic Number Hardcoding**  TODO

---

### **Point #11: Linkage Purity & Symbol Visibility**

 **Point 11 Sublist: Linkage & Structural Dependency Mapping**

| Audit Action Item | Linkage Strategy | Primary Phase 1 Relation | Risk Mitigated |
| :--- | :--- | :--- | :--- |
| **Inline Keywords** | Header Purity | **#1**, **#6** | Prevents Multiple Definition linker errors and guard failures. |
| **Meyers Singletons** | Functional Wrapper | **#1**, **#8** | Stops `SIGSEGV` by ensuring init-order and removing raw externs. |
| **Internal Linkage Removal** | Global Stripping | **#2**, **#3** | Eliminates redundant "memory islands" and state fragmentation. |
| **Global Constructor Audit** | Heap Management | **#8** | Removes pre-`main()` bottlenecks and "thundering herd" HPC issues. |
| **ODR Compliance Check** | Symbol Uniqueness | **#1**, **#4** | Prevents linker-bind stage collisions and symbol ambiguity. |
| **Initialization Guarding** | Logic Wrapping | **#7**, **#8** | Breaks dependency deadlocks and manages heap timing. |
| **Namespace Isolation** | Scope Hardening | **#4**, **#9** | Stops collisions with ROOT/LHAPDF and prevents variable shadowing. |
| **Header-Only Constants** | `inline constexpr` | **#3**, **#10** | Prevents magic number drift and constant poisoning. |
| **Template Linkage** | Specialization Fix | **#1**, **#5** | Reduces binary bloat and stops transitive inclusion poisoning. |
| **Library Compatibility** | API Alignment | **#4**, **#5** | Isolates external library symbol leakage into the global scope. |







