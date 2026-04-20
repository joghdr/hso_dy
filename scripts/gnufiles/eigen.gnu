


dir="<<DIR>>"

file =sprintf("%s/min/eigensets.dat",dir)

eval sprintf("key1=system   (' echo %s | sed \"s/[./_-]/ /g\"') ",dir)



PARA='<<PARA>>'

PARAALL='<<PARAALL>>'

size=words(PARAALL)

parafree=words(PARA)

compare(s1,s2)=(s1 eq s2? 1 : 0)

array FREE[size] ; do for [i=1:size] { FREE[i]=0}


do for [i=1:size]    {

    

    do for [j=1:parafree]  {
    
        FREE[i]=FREE[i]+compare(word(PARAALL,i),word(PARA,j))
    
    }

}


set size ratio 1

set key above 

counter=1

do for [i=1:size-1]    {


    do for [j=i+1:size]  {
    
        
            if((FREE[i]==1)&&(FREE[j]==1)) {
        
                set term qt counter font ",14"
                set ylabel word(PARAALL,i)
                set xlabel word(PARAALL,j)
        
                plot file u word(PARAALL, j):word(PARAALL, i) pt 7 lw 2 lc 7 ps 2 t key1
                
                print i,j
                
                counter=counter+1
            }
        
        
    
    }

}

print key1

