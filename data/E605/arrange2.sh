#!/bin/bash/

declare fnameid=E605_800_

declare -a files=( $(ls HEPData-ins302822-v1-Table_*) )


for file  in ${files[@]} ;do

declare -a Qrange=( $(grep  -E "\#: M\(P=3 4\) \[GEV\]\,.+ TO .+" $file | cut -d, -f 2 | sed  "s/TO/ /g") )

declare filetemp=$fnameid${Qrange[0]}-${Qrange[1]}.temp

cat $file | sed -E "s/^(PT.+)$/#\1\n\n##BEGIN MY KEY\n#\n#\n###MYKEY/g;s/,/  /g" | sed "s/\-15.0%//g;s/15.0%//g;s/\-5.0%//g;s/5.0%//g" |sed "s/###MYKEY/###key:: varbin:qT:avg varbin:qT:min varbin:qT:max meas:csec err:stat+  err:stat-/g" | sed "s/##BEGIN MY KEY/##BEGIN MY KEY\n#\n#\n# varavg:E:800\n# varavg:sqrt_s:38.8\n# varbin:xF:0.1:-0.1:0.2\n# varint:Q:${Qrange[0]}:${Qrange[1]}\n#NOTES:\n#\n#err norm = +\/- 15\%\n#err syst unc +\/- 5\% (add in quadrature to stat+)\n# source files report only avg qT. Here, bins are assumed to be the same as for E288./g" > $filetemp

# ls $filetemp

done


###### second step tp add bin values (as in E288)

for file in $fnameid*.temp; do

    declare file_final=$(echo $file | sed "s/.temp/.dat/g")
##separate header from data
    grep "\#" $file > header.temp

    grep -v "\#" $file | sed -E "s/[[:space:]]+/:/g"  > data.temp
##separate qT values from data
    cat data.temp | cut -d: -f2- > data_noqT.temp

    declare -a qTvals=( $( cat data.temp  | cut -d: -f1 )  )
##write avg qT with bin extrema as in E288
    echo "" > qTbin.temp

    for qT in ${qTvals[@]};do 

        declare qTmin=$( echo "$qT-0.1" |  bc -l | sed -E "s/^(\.[[:digit:]]+)/0\1/g" )

        declare qTmax=$( echo "$qT+0.1" |  bc -l | sed -E "s/^(\.[[:digit:]]+)/0\1/g" )

        echo "$qT $qTmin $qTmax" >> qTbin.temp

    done
##print header to final file
    cat   header.temp > $file_final
##print data now with qT bin
    paste qTbin.temp   data_noqT.temp | sed -E "s/[[:space:]]+/:/g;s/:/  /g" >> $file_final

ls $file_final

done

rm *.temp




