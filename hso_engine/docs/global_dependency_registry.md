# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan
## Current Global State Staged for Refactor
The engine was develop to produce an analysis of Drell-Yan data prepared for publication. However, there are significant improvements to be made
before the engine can be scalable, which is a must for more comprehensive analysis like using more data sets, running Montecarlo simulations.

The main source of technical debt is the proliferation of global state, which prevents the engine from being truly modular and thread safe.
The followiing is registry of the global state symbols currently in the engine. Currently, an incremental refactor of the code will aim at elmininating
these globals.
### **Global Dependency Registry**
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



