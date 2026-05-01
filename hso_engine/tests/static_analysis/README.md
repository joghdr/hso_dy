

# Architectural Standard: External Linkage Protocol (Extern)

The use of the `extern` keyword in a high-integrity physics engine is categorized as a structural deficiency. While the C++ language permits `extern` to facilitate the sharing of mutable global state across translation units, a **Safe and Sound** architecture rejects this practice to ensure thread-safety, modularity, and deterministic execution.

---

### **1. The Header Logic (`include/*.h`)**
A header file must function as a **Stateless Contract**. The presence of `extern` in a header indicates that a module is coupled to a global variable defined in a separate translation unit, which is a violation of **Tier I: Structural Integrity**.

* **Global Variables:** **STRICTLY FORBIDDEN**. The state of an `extern` variable cannot be synchronized across multiple threads without significant overhead and risk of race conditions. This leads to non-deterministic simulation results.
* **Mandatory Alternative (Constants):** For read-only values (e.g., physical constants, fixed tolerances), the `inline constexpr` specifier must be used. This provides external linkage for immutable values, ensuring every translation unit sees the same bit-identical constant without requiring a dedicated definition in a source file.
* **Mandatory Alternative (Mutable State):** For values that must change during runtime, data must be encapsulated within a **Context Object** or **Configuration Struct**. This object is then passed to functions via reference or pointer (Dependency Injection), ensuring the state is explicit and traceable.

---

### **2. The Source Logic (`src/*.cpp`)**
In a source file, `extern` is frequently used to manually reference a symbol defined in another module. This practice is identified as a **Modular Leak**.

* **Inter-Module Coupling:** Direct access to another module's variables via `extern` bypasses the public API. If a translation unit requires data from another, it should request it through a formal function call or getter, not by reaching into the external file's global scope.
* **Initialization Risk:** `extern` variables are susceptible to the **Static Initialization Order Fiasco**. If a simulation relies on these variables being initialized in a specific sequence, the program may crash or utilize uninitialized memory during the startup phase.

---

### **3. The Linkage Enforcement Strategy**
To transition from a monolithic state to a modular one, the `extern` keyword must be systematically eliminated from the codebase.

* **Detection:** Automated tools (e.g., grep-based sentinels) must be utilized to maintain a trajectory toward zero `extern` occurrences in the `include/` directory.
* **Refactoring Path:** `extern` symbols are either converted to `inline constexpr` (if they are logically constants) or refactored into function parameters (if they represent runtime state).

---

### **External Linkage Strategy Summary**

| Context | Keyword | Standard C++ Usage | Safe and Sound Requirement |
| :--- | :--- | :--- | :--- |
| **Header (`.h`)** | `extern` | Global State (Mutable) | **FORBIDDEN** (Pass by Reference) |
| **Header (`.h`)** | `inline constexpr` | Shared Constant | **MANDATORY** (For all Constants) |
| **Source (`.cpp`)** | `extern` | External Reference | **FORBIDDEN** (Use Public API) |


# Architectural Standard: Linkage and Encapsulation Protocol

In **"Mean-Mode,"** the distinction between how `static` behaves in headers versus source files is the boundary between a **Leaky Abstraction** and a **Professional Architecture**. Following the **Safe and Sound** strategy ensures that your engine is physically impossible to break with multithreading race conditions or initialization order fiascos.

---

### **The Header Logic (`include/*.h`)**
A header is a **Public Contract**. In this architecture, the header is strictly for **Interface Declarations** and is entirely **Stateless**.

* **File-Level `static`:** **FORBIDDEN**. It creates "Internal Linkage," giving every translation unit its own "ghost" copy of a variable. This leads to silent numerical divergence in physics results.
* **Class-Level `static`:** **FORBIDDEN**. Even though "legal" in C++, it creates hidden global state. We enforce **Dependency Injection** or **Context Objects** instead.
* **Sentinel Enforcement:** Any occurrence of the `static` keyword in a header triggers an immediate build failure (Point #2 of the Technical Debt Taxonomy).

---

### **The Source Logic (`src/*.cpp`)**
In a source file, `static` (or anonymous namespaces) is a **Mandatory Tool for Encapsulation**. It is the only place the keyword is permitted.

* **Internal Linkage:** All helper functions and local variables must be marked `static`. This hides your implementation "Guts" from the other 40+ translation units.
* **Optimization:** This allows the compiler to perform maximum optimizations (like aggressive inlining) because it knows the code cannot be accessed from outside that specific file.
* **Namespace Hygiene:** Prevents symbol collisions. You can have the same helper name in multiple files without the linker complaining.

---

### **The Role of Includes**
Including a header (e.g., `f1.h`) inside its corresponding source file (`f1.cpp`) is a **Self-Consistency Check**.

* **Signature Validation:** It forces the compiler to verify that your implementation matches your public promise.
* **Lean Headers:** Internal helpers are **never** declared in the header just for "forward declaration" convenience. They are defined locally in the `.cpp` file, keeping the header clean and preventing "Compile-Time Bloat" across the project.

---

### **Static Usage & Architecture Strategy (Safe and Sound)**

| Context | Keyword | Standard C++ Use | "Safe and Sound" Choice |
| :--- | :--- | :--- | :--- |
| **Header (`.h`)** | `static` (File) | Internal Linkage (Dangerous) | **FORBIDDEN** (Zero Tolerance) |
| **Header (`.h`)** | `static` (Class) | Shared Class Member | **FORBIDDEN** (Use Context Objects) |
| **Header (`.h`)** | `inline` | External Linkage (Shared) | **PREFER** (For Constants/Small Logic) |
| **Source (`.cpp`)** | `static` | Internal (Private to File) | **MANDATORY** (For all Helpers) |
| **Source (`.cpp`)** | `none` | Global (Public to All) | **FORBIDDEN** (Except Public API) |



### **Architectural Standard: Management of Persistent Global State (Point #3)**

#### **1. Purpose**
This standard defines the requirements for the lifecycle and ownership of data within the system. It seeks to eliminate **Persistent Global State** to ensure that the software remains deterministic, thread-safe, and modular. While Point #1 (Linkage) and Point #2 (Scope) govern the visibility and connection of symbols, Point #3 governs the physical storage and duration of data in memory.

#### **2. Technical Definition**
**Persistent Global State** is defined as any mutable data structure that exists for the duration of the process and is stored in the binary’s **Data** or **BSS** segments.

* **Data Segment:** Memory for initialized global or static variables.
* **BSS Segment:** Memory for uninitialized global or static variables, zero-filled by the Operating System at startup.
* **Guard Variables:** Compiler-generated flags used to synchronize the initialization of global objects with non-trivial constructors.



#### **3. Identification Criteria**
A component is in violation of this standard if it contains symbols that meet any of the following criteria in a binary audit (`nm`):
1.  **Mutable Persistence:** Any symbol marked with type `B`, `b`, `D`, or `d` that is not explicitly qualified as a constant.
2.  **Implicit Synchronization:** The presence of `guard variables`, which indicate "on-demand" global initialization.
3.  **Static Internal State:** Variables declared with the `static` keyword within function or file scopes. **Deleting the `static` keyword resolves Point #3 for local function variables by moving the variable from the BSS/Data segments to the Stack.**

#### **4. Prohibited Architectural Patterns**
The following patterns are strictly forbidden under **Tier I: Structural Integrity**:
* **Global Accumulators:** Using shared global variables to sum results (e.g., `chi2_total`). This creates a mandatory race condition in parallel environments.
* **Global Control Toggles:** Using global Boolean flags or Enums to alter logic flow across translation units.
* **Shared Pointers/Resource Handles:** Storing handles to external resources (e.g., PDF pointers, file streams) in global memory, which leads to "Pointer Poisoning" if one module reassigns the shared resource.

#### **5. Mandated Resolution Patterns**
To comply with this standard, all persistent state must be migrated to **Explicit Data Flow** models:

* **Removal of `static` (Local Scope):** Deleting the `static` keyword from local function variables immediately resolves the Point #3 violation by moving the variable to the **Stack**. This ensures the variable is re-initialized upon every function call, enabling re-entrancy.
* **Context Object Pattern:** Group related state into a structured object (`Context`). This object must be instantiated on the **Stack** within a controlled scope (e.g., `main`).
* **Dependency Injection:** Functions must receive their required state explicitly via arguments. Functions are prohibited from "reaching out" to the global scope to retrieve data.
* **Immutable Metadata:** Data that is truly constant must be declared `constexpr`. This informs the compiler that the data is **Fixed Metadata**, moving it out of the mutable Data segment.



#### **6. Compliance Verification**
Architectural compliance is verified through a **Zero-Tolerance Binary Audit**:
1.  **Extraction:** Perform `nm -C --defined-only` on the final executable.
2.  **Filtration:** Remove system-level infrastructure (e.g., `vtable`, `typeinfo`).
3.  **Validation:** The audit is successful only if the remaining list contains **zero** mutable symbols (`B`, `D`, `b`, `d`) within application-specific namespaces.

---

### **Symbol Type Reference Table**

The following table outlines the symbol types encountered during an `nm` audit and their correspondence to the memory and architectural state of the program.

| Symbol Type | Memory Segment | Scope / Linkage | Correspondence | Audit Status |
| :--- | :--- | :--- | :--- | :--- |
| **B** | BSS | Global (External) | Uninitialized Public Global variable. | **Violation (Point #3)** |
| **b** | BSS | Local (Internal) | Uninitialized `static` variable. | **Violation (Point #3)** |
| **D** | Data | Global (External) | Initialized Public Global variable. | **Violation (Point #3)** |
| **d** | Data | Local (Internal) | Initialized `static` variable. | **Violation (Point #3)** |
| **R** | Read-Only Data | Global (External) | Constant (`const`/`constexpr`) global. | **Safe Infrastructure** |
| **r** | Read-Only Data | Local (Internal) | Constant `static` variable. | **Safe Infrastructure** |
| **T** | Text (Code) | Global (External) | Public Function / Method. | **Standard Execution** |
| **t** | Text (Code) | Local (Internal) | Private/`static` Function. | **Standard Execution** |
| **U** | Undefined | External | Reference to a symbol in another file. | **Point #1 (Check Linkage)** |


