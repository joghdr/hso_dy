#!/bin/bash
# Generate HSO mappings

OUTPUT_FILE="devtools/hso_iwyu_mappings.imp"
INCLUDE_DIR="include/hso"


echo "[" > "$OUTPUT_FILE"

# Avoid some specific errors incolving c libs
cat <<EOF >> "$OUTPUT_FILE"
  { "include": ["<bits/std_abs.h>", "private", "<cmath>", "public"] },
  { "include": ["<sys/stat.h>", "public", "<filesystem>", "public"] },
EOF

# Handles all headers inside include/hso/ automatically
echo '  { "include": ["@\"hso/(.*)\"", "private", "<hso/\\1>", "public"] },' >> "$OUTPUT_FILE"

if [ -d "$INCLUDE_DIR" ]; then
    HEADERS=$(ls "$INCLUDE_DIR" | grep '\.h$')
    for HEADER in $HEADERS; do
        # Mapping both the quoted form and the bracketed form to ensure persistence
        echo "  { \"include\": [\"\\\"$HEADER\\\"\", \"private\", \"<hso/$HEADER>\", \"public\"] }," >> "$OUTPUT_FILE"
        echo "  { \"include\": [\"<hso/$HEADER>\", \"public\", \"<hso/$HEADER>\", \"public\"] }," >> "$OUTPUT_FILE"
    done
fi

# Finalize .json fila
sed -i '$ s/,$//' "$OUTPUT_FILE"
echo "]" >> "$OUTPUT_FILE"

echo "*** [HSO] Generated $OUTPUT_FILE with STL overrides and Self-Header protection."
