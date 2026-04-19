#!/bin/bash
# HSO Mapping Generator for IWYU (Full STL + Namespace + Self-Header Persistence)

OUTPUT_FILE="hso.imp"
INCLUDE_DIR="include/hso"

# 1. Detect IWYU system mapping path (Standard for Ubuntu/Debian/RHEL)
IWYU_SYS_PATH=$(dirname $(find /usr -name "gcc.libc.imp" | head -n 1))

if [ -z "$IWYU_SYS_PATH" ]; then
    echo "Warning: System mapping files not found in /usr. Falling back to relative refs."
    IWYU_SYS_PATH="."
fi

echo "[" > "$OUTPUT_FILE"

# 2. FORCE C++ HEADERS OVER C/INTERNAL HEADERS
# This silences the "please use math.h" regressions
cat <<EOF >> "$OUTPUT_FILE"
  { "include": ["<math.h>", "public", "<cmath>", "public"] },
  { "include": ["<stdio.h>", "public", "<cstdio>", "public"] },
  { "include": ["<stdlib.h>", "public", "<cstdlib>", "public"] },
  { "include": ["<stddef.h>", "public", "<cstddef>", "public"] },
  { "include": ["<bits/std_abs.h>", "private", "<cmath>", "public"] },
  { "include": ["<sys/stat.h>", "public", "<filesystem>", "public"] },
EOF

# 3. EXPLICIT SYMBOL-TO-HEADER MAPPINGS
# Hard-binds core C++ symbols to their modern headers
cat <<EOF >> "$OUTPUT_FILE"
  { "symbol": ["std::abs", "private", "<cmath>", "public"] },
  { "symbol": ["std::string", "private", "<string>", "public"] },
  { "symbol": ["std::vector", "private", "<vector>", "public"] },
  { "symbol": ["std::map", "private", "<map>", "public"] },
  { "symbol": ["std::filesystem", "private", "<filesystem>", "public"] },
EOF

# 4. STANDARD LIBRARY REFERENCES (System Mappings)
if [ -d "$IWYU_SYS_PATH" ]; then
    echo "  { \"ref\": \"$IWYU_SYS_PATH/gcc.libc.imp\" }," >> "$OUTPUT_FILE"
    echo "  { \"ref\": \"$IWYU_SYS_PATH/gcc.symbols.imp\" }," >> "$OUTPUT_FILE"
    echo "  { \"ref\": \"$IWYU_SYS_PATH/gcc.stl.headers.imp\" }," >> "$OUTPUT_FILE"
fi

# 5. HSO NAMESPACE REGEX
# Handles all headers inside include/hso/ automatically
echo '  { "include": ["@\"hso/(.*)\"", "private", "<hso/\\1>", "public"] },' >> "$OUTPUT_FILE"

# 6. SELF-HEADER FORCE (Systemic fix for "remove #include <hso/X.h>")
# This loop identifies every header and marks it as a mandatory public provider
if [ -d "$INCLUDE_DIR" ]; then
    HEADERS=$(ls "$INCLUDE_DIR" | grep '\.h$')
    for HEADER in $HEADERS; do
        # Mapping both the quoted form and the bracketed form to ensure persistence
        echo "  { \"include\": [\"\\\"$HEADER\\\"\", \"private\", \"<hso/$HEADER>\", \"public\"] }," >> "$OUTPUT_FILE"
        echo "  { \"include\": [\"<hso/$HEADER>\", \"public\", \"<hso/$HEADER>\", \"public\"] }," >> "$OUTPUT_FILE"
    done
fi

# Finalize JSON (remove trailing comma from the very last entry)
sed -i '$ s/,$//' "$OUTPUT_FILE"
echo "]" >> "$OUTPUT_FILE"

echo "*** [HSO] Generated $OUTPUT_FILE with STL overrides and Self-Header protection."
