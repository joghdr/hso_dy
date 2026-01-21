#!/bin/bash/

declare fnameid=E772_800_

declare -a files=( $(ls HEPData-ins372414-v1-Table_*) )

rm $fnameid*

for file  in ${files[@]} ;do

    declare -a Qcentral=( $(grep "\#: M \[GeV\]\," $file | cut -d, -f2) )

    for index in $(seq 0 $(( ${#Qcentral[@]} - 2 )) ) ; do 
    
        declare begin=${Qcentral[$index]}
        
        declare   end=${Qcentral[$((index + 1))]}
        
        declare Qmin=$( bc -l <<< "${Qcentral[$index]}-0.5"  )
        
        declare Qmax=$( bc -l <<< "${Qcentral[$index]}+0.5"  )
        
        declare filemod=$fnameid$Qmin-$Qmax.dat
        
        cat $file | sed -n "/#: table_doi:/,/#: keyword phrases:/p" > $filemod
        
        cat $file | sed -n "/\#: M \[GeV\]\,$begin/,/\#: M \[GeV\]\,$end/p"  | sed "s/\#: M \[GeV\]\,$end//g" >> $filemod
        
        
        
        sed -i -E "s/^(PT.+)$/#\1\n\n##BEGIN MY KEY\n#\n#\n###MYKEY/g;s/,/  /g" $filemod
        sed -i "s/\-15.0%//g;s/15.0%//g;s/\-5.0%//g;s/5.0%//g" $filemod
        sed -i "s/###MYKEY/###key:: varavg:qT meas:csec err:stat+  err:stat-/g" $filemod
        sed -i "s/##BEGIN MY KEY/##BEGIN MY KEY\n#\n#\n#varavg:E:800\n#varavg:sqrt_s:38.8\n#varint:xF:0.1:0.3\n#varint:Q:$Qmin:$Qmax\n#NOTES:\n#\n#err acceptance 8%\n#err luminosity 5%\n#err trigger efficiency 3% \n#err reconstruction efficiency 1%\n#err syst total = 10%/g" $filemod
        
        ls $filemod
                
        
    done
    
    
    declare begin=${Qcentral[$(( ${#Qcentral[@]} - 1 ))]}
        
    declare Qmin=$( bc -l <<< "$begin-0.5"  )
        
    declare Qmax=$( bc -l <<< "$begin+0.5"  )
        
    declare filemod=$fnameid$Qmin-$Qmax.dat
    
    cat $file | sed -n "/#: table_doi:/,/#: keyword phrases:/p" > $filemod
        
    cat $file | sed -n "/\#: M \[GeV\]\,$begin/,\$p"   >> $filemod
    
    sed -i -E "s/^(PT.+)$/#\1\n\n##BEGIN MY KEY\n#\n#\n###MYKEY/g;s/,/  /g" $filemod
    sed -i "s/\-15.0%//g;s/15.0%//g;s/\-5.0%//g;s/5.0%//g" $filemod
    sed -i "s/###MYKEY/###key:: varavg:qT meas:csec err:stat+  err:stat-/g" $filemod
    sed -i "s/##BEGIN MY KEY/##BEGIN MY KEY\n#\n#\n#varavg:E:800\n#varavg:sqrt_s:38.8\n#varint:xF:0.1:0.3\n#varint:Q:$Qmin:$Qmax\n#NOTES:\n#\n#err acceptance 8%\n#err luminosity 5%\n#err trigger efficiency 3% \n#err reconstruction efficiency 1%\n#err syst total = 10%/g" $filemod
        
    ls $filemod
    

#     echo ${Qcentral[@]}

done



# for filemod in $fnameid* ; do 



# ls $filemod

# done




