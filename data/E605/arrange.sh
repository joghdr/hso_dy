#!/bin/bash/

declare fnameid=E605_800_

declare -a files=( $(ls HEPData-ins302822-v1-Table_*) )

for file  in ${files[@]} ;do

declare -a Qrange=( $(grep  -E "\#: M\(P=3 4\) \[GEV\]\,.+ TO .+" $file | cut -d, -f 2 | sed  "s/TO/ /g") )

declare filemod=$fnameid${Qrange[0]}-${Qrange[1]}.dat

cat $file | sed -E "s/^(PT.+)$/#\1\n\n##BEGIN MY KEY\n#\n#\n###MYKEY/g;s/,/  /g" | sed "s/\-15.0%//g;s/15.0%//g;s/\-5.0%//g;s/5.0%//g" |sed "s/###MYKEY/###key:: varavg:qT meas:csec err:stat+  err:stat-/g" | sed "s/##BEGIN MY KEY/##BEGIN MY KEY\n#\n#\n# varavg:E:800\n# varavg:sqrt_s:38.8\n# varbin:xF:0.1:-0.1:0.2\n# varint:Q:${Qrange[0]}:${Qrange[1]}\n#NOTES:\n#\n#err norm = +\/- 15\%\n#err syst unc +\/- 5\% (add in quadrature to stat+)/g" > $filemod

ls $filemod

done
