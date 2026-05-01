#!/bin/bash
# Check some library paths

echo "==============================================================="
echo "--- TOOLCHAIN FRONTEND AUDIT (Compilers & Analysis) ---"
printf "%-18s %-15s %-40s\n" "TOOL" "VERSION" "RESOLVED PATH"
echo "---------------------------------------------------------------"

# tools to check
tools=("gcc" "g++" "gcov" "cc" "c++" "cmake" "ninja" "clang-tidy-18" "include-what-you-use")

for tool in "${tools[@]}"; do
    if command -v "$tool" >/dev/null 2>&1; then
        # Specialized version extraction
        if [[ "$tool" == "include-what-you-use" ]]; then
            version=$($tool --version 2>&1 | grep -oE '[0-9]+\.[0-9]+' | head -n 1)
            display_name="iwyu"
        elif [[ "$tool" == "clang-tidy-18" ]]; then
            version=$($tool --version 2>&1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -n 1)
            display_name="clang-tidy"
        else
            version=$($tool --version 2>&1 | head -n 1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -n 1)
            [[ -z "$version" ]] && version=$($tool --version 2>&1 | head -n 1 | awk '{print $NF}')
            display_name=$tool
        fi

        path=$(readlink -f "$(which "$tool")")
        printf "%-18s %-15s %-40s\n" "$display_name" "$version" "$path"
    else
        printf "%-18s %-15s %-40s\n" "$tool" "NOT INSTALLED" "N/A"
    fi
done

echo ""
echo "--- RUNTIME BACKEND AUDIT (Linkage & ABI) ---"
printf "%-18s %-15s %-40s\n" "LIBRARY" "VERSION" "RESOLVED PATH"
echo "---------------------------------------------------------------"


libs=(
    "libstdc++.so.6"
    "libm.so.6"
    "libgcc_s.so.1"
    "libgsl.so"
    "libboost_math_cplusplus.so"
)

for lib in "${libs[@]}"; do
    # Search dynamic linker cache
    path=$(ldconfig -p | grep "$lib" | awk 'NR==1{print $NF}')

    if [ -n "$path" ]; then
        real_path=$(readlink -f "$path")
        # Extract version if available, otherwise mark as System Base
        version=$(echo "$real_path" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' || echo "Base/ABI")

        printf "%-18s %-15s %-40s\n" "$lib" "$version" "$real_path"
    else
        printf "%-18s %-15s %-40s\n" "$lib" "NOT FOUND" "N/A"
    fi
done

echo "---------------------------------------------------------------"
echo "System Base (GLIBC): $(ldd --version | head -n 1 | awk '{print $NF}')"
echo "==============================================================="
