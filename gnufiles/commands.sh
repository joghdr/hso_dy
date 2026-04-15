#!/bin/bash/


# local  dirin="$1"

# local  flavor="$2"

# local  -a files_in=( "$dir/tmds/*input-[!b]*" )

function parafixorfree {

  local para="$1"
  local file="$2"
  local free="$3"

  sed -i -r "s/$para([[:space:]]+)([[:digit:].-]+)([[:space:]]+)([[:digit:]]+)/$para\1\2\3$free/g" $file


}

function parafree {

  local para="$1"
  local file="$2"
  parafixorfree "$para" "$file" "1"

}

function parafix {

  local para="$1"
  local file="$2"
  parafixorfree "$para" "$file" "0"

}


function paraset {

  local para="$1"
  local val="$2"
  local file="$3"
  sed -i -r "s/$para([[:space:]]+)([[:digit:].-]+)([[:space:]]+)/$para\1$val\3/g" $file



}

function paraget {

  local para="$1"
  local file="$2"
  grep -P "^[[:space:]]*""$para""[[:space:]]+"   $file | sed -E "s/[[:space:]]+/:/g" | cut -d: -f 2

}

function paraprint {
local -a parameters=( $1 ) 
local file="$2"

for para in ${parameters[@]};do

    local val="$(paraget $para $file)"
    
    echo "### para: $para = $val"

done

}

function gnu_paragetlabel {

local -a parameters=( $1 ) 
local file="$2"

local out


for para in ${parameters[@]};do

    local paraname=$(echo $para|sed "s/:.*//g")
    local dim=$(echo $para | sed  "s/$paraname//g") ; dim=$(echo $dim | sed "s/://g")
    local val="$(paraget $paraname $file)"
    local units=" GeV^{$dim}"
    
    
    
#     echo "para=$paraname   dim=$dim   units=$units"
    
    
    out=$out"$paraname = $val $units\n\n"

done


echo $out | sed "s/GeV^{1}/GeV/g;s/GeV^{}//g"


}

## ignore parameters with value 666, used for default val when para missing
function paraprintall {
local file="$1"

cat $file | sed -E "s/[[:space:]]+/:/g" | cut -d: -f1,2 | sed "s/:/ = /g"

}

function parafixall {

  local file="$1"

  local -a PARA=( $(cat $file|grep -v "\#"|sed -r "s/^[[:space:]]+//g;s/[[:space:]]+/:/g"|cut -d: -f1)   )

  for para in ${PARA[@]} ; do

    parafix $para $file

    done

}

function parafreeall {

  local file="$1"

  local -a PARA=( $(cat $file|grep -v "\#"|sed -r "s/^[[:space:]]+//g;s/[[:space:]]+/:/g"|cut -d: -f1)   )

  for para in ${PARA[@]} ; do

    parafree $para $file

  done

}

function paranameprintfree {
local dir="$1"
local file="$( ls $dir/status/*.input )"

local -a names=( $( cat $file |grep -v "\#" | sed -r "s/[[:space:]]+/\t/g" | cut -f 1 ) )

# cat $file |grep -v "\#" | sed -r "s/[[:space:]]+/\t/g" | cut -f 2

local -a free=( $( cat $file |grep -v "\#" | sed -r "s/[[:space:]]+/\t/g" | cut -f 3 ) )

local freenames=""

for i in $( seq 0 $(( ${#free[@]} - 1  )) );  do

    if [[ "${free[$i]}" == "1" ]] ; then 
    
        freenames="$freenames\t${names[$i]}"
        
#         echo $freenames
    
    fi
    
done 

echo -e $freenames | sed -r "s/^[[:space:]]+//g" | sed -r "s/[[:space:]]+/   /g"


}

function exists {

local filename="$1"

if [ -f "$filename" ] ; then

echo 1

else

echo 0

fi

}



function getval {

local var="$1"

local file="$2"

ls $file    | sed "s/.*\///g" |    grep -o -P "\-$var-.*" | sed "s/.dat//g;s/\-$var-//g" | sed -r "s/\-.*//g;s/\/.*//g"

}


function getcolflav {

local flavor="$1"

local flavor_col

case $flavor in
"sb")
 flavor_col=2;;
'ub')
 flavor_col=3;;
'db')
 flavor_col=4;;
'd')
 flavor_col=6;;
'u')
 flavor_col=7;;
's')
 flavor_col=8;;
*)
 flavor_col=7;;
 
esac

echo $flavor_col

}


function  gettitle {

local dir="$1"

echo $dir | cut -d\- -f1 | sed "s/_/ /g"


}


function plottmdinput {

local dir="$1"

local flavor="$2"

if [ "$flavor" == "" ] ; then flavor="u" ; fi

local -a files=( "$dir/tmds/*input-[!b]*.dat"  )

for file in ${files[@]}; do 


gnufile=$(echo  $file | sed "s/.dat//g;s/\/tmds\//\/gnufiles\//g")_$flavor.gnu

if [ -r $gnufile ] ;then

gnuplot -persist $gnufile

else 

echo "writing '$gnufile'"

pdffile=$(echo  $file | sed "s/.dat//g")_$flavor.pdf


x=$(getval "x"  $file)

muQ0=$(getval "muQ0" $file)

Q0=$(getval "Q0" $file)

E=$(getval "E" $file)



filepert=$(ls $dir/tmds/*pert*muQ-$muQ0*Q-$Q0*x-$x*.dat)

col=$(getcolflav $flavor)

label_flavor=$flavor

title=$(gettitle $dir)



GNUSCRIPT="
\n
\n
file=\"$file\"\n
filepert=\"$filepert\"\n
\n
\n
\n
\n
\n
load \"./gnufiles/.format2.gnu\"\n
\n
set title \"$title\"\n
\n
set xlabel \"k_T [GeV]\"\n
\n
set ylabel \"f ( x , k_T ; {/Symbol m}_{Q_0} , Q_0^2)\"\n
\n
set ytics format \"$LINENO0^{%L}\"\n

set label 1 sprintf(\"x=%.3f\",$x) at screen 0.3, screen 0.75\n

set label 2 \"Q_0=$Q0 GeV\" at screen 0.3, screen 0.70\n

set label 3 \"{/Symbol m}_{Q_0}=$muQ0 GeV\" at screen 0.3, screen 0.65\n

set label 4 \"$label_flavor\" at screen 0.55, screen 0.3 font \"Sans,18\"\n

#set label 5 \"E = $E GeV\" at screen 0.6, screen 0.92 \n

set key reverse Left\n

set logscale y\n

plot[:1.5*$Q0][] \\\\\n
\\\\\n
file     u 1:(abs(\$$col)) w l ls 6 dt 4 lw 1.5 t \"f\",\\\\\n
\\\\\n
filepert u 1:(abs(\$$col)) w l ls 7 dt 2 lw 1.5 t \"f_{pert}\"\n
\n
\n
set output \"$pdffile\"\n
\n
\n
load \"./gnufiles/.format_pdf.gnu\"\n
\n
replot
\n
\n
print \"$pdffile\"
\n
"


echo  -e $GNUSCRIPT > $gnufile

gnuplot -persist $gnufile

fi

done 

}


function plottmdinput_rho {

local dir="$1"

local flavor="$2"

if [ "$flavor" == "" ] ; then flavor="u" ; fi

local -a files=( "$dir/tmds/*input-[!b]*.dat"  )

for file in ${files[@]}; do 


gnufile=$(echo  $file | sed "s/.dat//g;s/\/tmds\//\/gnufiles\//g")_"$flavor"_"rho".gnu

if [ -r $gnufile ] ;then

gnuplot -persist $gnufile

else 

echo "writing '$gnufile'"

pdffile=$(echo  $file | sed "s/.dat//g")_"$flavor"_"rho".pdf


x=$(getval "x"  $file)

muQ0=$(getval "muQ0" $file)

Q0=$(getval "Q0" $file)

E=$(getval "E" $file)



filepert=$(ls $dir/tmds/*pert*muQ-$muQ0*Q-$Q0*x-$x*.dat)

col=$(getcolflav $flavor)

label_flavor=$flavor

title=$(gettitle $dir)



GNUSCRIPT="
\n
\n
file=\"$file\"\n
filepert=\"$filepert\"\n
\n
\n
\n
\n
\n
load \"./gnufiles/.format2.gnu\"\n
\n
set title \"$title\"\n
\n
set xlabel \"k_T/Q_0 \"\n
\n
set ylabel \"f ( x , k_T ; {/Symbol m}_{Q_0} , Q_0^2)\"\n
\n
set xtics 0,0.2,1.0 format \"%.1f\"\n
\n
set ytics format \"$LINENO0^{%L}\"\n
\n


set label 1 sprintf(\"x=%.3f\",$x) at screen 0.5, screen 0.75\n

set label 2 \"Q_0=$Q0 GeV\" at screen 0.5, screen 0.70\n

set label 3 \"{/Symbol m}_{Q_0}=$muQ0 GeV\" at screen 0.5, screen 0.65\n

set label 4 \"$label_flavor\" at screen 0.3, screen 0.3 font \"Sans,22\"\n

#set label 5 \"E = $E GeV\" at screen 0.6, screen 0.92 \n

set key reverse Left\n

set logscale y\n

plot[:1.0][] \\\\\n
\\\\\n
file     u (\$1/$Q0):(abs(\$$col)) w l ls 6 dt 4 lw 1.5 t \"f\",\\\\\n
\\\\\n
filepert u (\$1/$Q0):(abs(\$$col)) w l ls 7 dt 2 lw 1.5 t \"f_{pert}\"\n
\n
\n
set output \"$pdffile\"\n
\n
\n
load \"./gnufiles/.format_pdf.gnu\"\n
\n
replot
\n
\n
print \"$pdffile\"
\n
"


echo  -e $GNUSCRIPT > $gnufile

gnuplot -persist $gnufile

fi

done 

}


function plotE288 {

local dir="$1"

local dirID="$(echo $dir | sed 's/results//g')"


for E in 200 300 400 ; do

local gnuscript="$dir/gnufiles/E288-E-$E.gnu"

local pdffile="$dir/stat/E288/E288-E-$E.pdf"


if [ -f $gnuscript ] ; then 

gnuplot -persist $gnuscript

else


local SCRIPT="\n
\n
\n
load \"gnufiles/.format3.gnu\"\n
\n
\n
XMIN=-0.4 ; XMAX=3.0\n
\n
\n
TC(chi2pt)=chi2pt<1?14:chi2pt<2?9:chi2pt<3?13:1
\n
\n
file(E,Q)=sprintf(\"$dir/stat/E288/E288_%.0f_%.0f-%.0f.stat\",E,Q,Q+1)\n
\n
keyQ(Q)=sprintf(\"%.0f<Q<%.0f\",Q,Q+1)\n
\n
set title \"<<TITLE>> \"\n
\n
\n
set logscale y\n
\n
set xtics 0.,0.4,2.8 format \"%.1f\"\n
\n
set ytics format \"10^{%L}\"\n
\n
set xlabel \"q_T [GeV]\"\n
\n
set ylabel \"csec\"\n
\n
set key reverse Left samplen 1.5 at screen 0.90, screen 0.85 \n
\n\n\n
<<CHI2LABEL>>\n
\n\n\n
plot[XMIN:XMAX][] \\\\\n
<<PLOT>>
\n
\n
\n
load 'gnufiles/.format2_pdf.gnu'\n
set output '$pdffile'\n
replot\n
\n
print '$pdffile'\n
\n
"



local counter="0"

for Q in 4 5 6 7 8 9 10 11 12 13 ; do 

local file="$dir/stat/E288/E288_"$E"_"$Q"-"$(( Q + 1 ))".stat"

local file_escape=$(echo $file | sed -r "s/([_./-])/\\\\\1/g")



if [ -f $file ] ; then

local chi2_label_ypos=$(grep -E -m 1  "^[[:digit:]].+" $file | cut -f4)

local chi2_pt=$(grep "#chi^2/pts"  $file | cut -d= -f2)

line1=" '$file_escape'  u 1:4:5     w err  lt $(( Q - 2 ))   t keyQ($Q),\\\\\\\\\\\n"


line2=" '$file_escape'  u 1:(\$7\/\$8) w l    lt $(( Q - 2 ))  not,\\\\\\\\\\\n"


line3=" '$file_escape'  u 1:(\$4-\$5):(sprintf(\"%.1f\",\$9)):(sprintf(\"%.1f\",\$9)):(TC(\$9)) w labels font \"Courier-Bold,10\" textcolor variable  offset screen 0.0,screen -0.02 not"


if [ ! $counter -eq 0 ] ; then
# echo "counter=$counter"
    SCRIPT=$(echo $SCRIPT | sed "s/<<PLOT>>/,\\\\\\\\\\\n<<PLOT>>/g")

fi

counter=$(( $counter + 1 ))

# echo "out: counter=$counter"

SCRIPT=$( echo $SCRIPT |sed "s/<<PLOT>>/\\\\\\\\\\\n<<PLOT>>/g" | sed -r "s/<<PLOT>>/$line1<<PLOT>>/g"| sed -r "s/<<PLOT>>/$line2<<PLOT>>/g" | sed -r "s/<<PLOT>>/$line3<<PLOT>>/g")


SCRIPT=$( echo $SCRIPT  | sed -r "s/<<CHI2LABEL>>/set label sprintf('%.2f',$chi2_pt) at XMIN+0.1,$chi2_label_ypos font ',13' \\\n<<CHI2LABEL>>/g")



fi

done

local title=$(echo $dirID | sed -r "s/[_-]/ /g;s/\///g;s/\.//g")

title="'$title':  E288: E = $E GeV"


SCRIPT=$( echo $SCRIPT |sed "s/<<PLOT>>/\\\\\\\\\\\n/g" | sed "s/<<CHI2LABEL>>//g" | sed -r "s/<<TITLE>>/$title/g")


echo "writing $gnuscript"

echo -e $SCRIPT > $gnuscript

gnuplot -persist $gnuscript 

fi

done 

}

function plotE605 {

local dir="$1"


for E in 800 ; do

local gnuscript="$dir/gnufiles/E605-E-$E.gnu"

local pdffile="$dir/stat/E605/E605-E-$E.pdf"


if [ -f $gnuscript ] ; then 

gnuplot -persist $gnuscript

else


local SCRIPT="\n
\n
\n
load \"gnufiles/.format3.gnu\"\n
\n
\n
XMIN=-0.4 ; XMAX=3.0\n
\n
\n
TC(chi2pt)=chi2pt<1?14:chi2pt<2?9:chi2pt<3?13:1
\n
\n
file(E,Q)=sprintf(\"$dir/stat/E605/E605_%.0f_%.0f-%.0f.stat\",E,Q,Q+1)\n
\n
keyQ(Qmin,Qmax)=sprintf(\"%.1f<Q<%.1f\",Qmin,Qmax)\n
\n
set title \"E605:  <<TITLE>>\"\n
\n
\n
set logscale y\n
\n
set xtics 0.,0.4,2.8 format \"%.1f\"\n
\n
set ytics format \"10^{%L}\"\n
\n
set xlabel \"q_T [GeV]\"\n
\n
set ylabel \"csec\"\n
\n
set key reverse Left samplen 1.5 at screen 0.90, screen 0.85 \n
\n\n\n
<<CHI2LABEL>>\n
\n\n\n
plot[XMIN:XMAX][] \\\\\n
<<PLOT>>
\n
\n
\n
load 'gnufiles/.format2_pdf.gnu'\n
set output '$pdffile'\n
replot\n
\n
print '$pdffile'\n
\n
"



local counter="0"

local -a Qmin=( 7 8  10.5 11.5 13.5 )

local -a Qmax=( 8 9  11.5 13.5 18.0 )

for i in  $(seq 0 4)  ; do 

local Q=${Qmin[$i]}

local Qend=${Qmax[$i]}

local file="$dir/stat/E605/E605_"$E"_"$Q"-"$Qend".stat"

local file_escape=$(echo $file | sed -r "s/([_-./])/\\\\\1/g")



if [ -f $file ] ; then

local chi2_label_ypos=$(grep -E -m 1  "^[[:digit:]].+" $file | cut -f4)

local chi2_pt=$(grep "#chi^2/pts"  $file | cut -d= -f2)

line1=" '$file_escape'  u 1:4:5     w err  lt $(( $(echo $Q|sed -E "s/\..+//g") - 2 ))   t keyQ($Q,$Qend),\\\\\\\\\\\n"


line2=" '$file_escape'  u 1:(\$7\/\$8) w l    lt $(( $(echo $Q|sed -E "s/\..+//g") - 2 ))  not,\\\\\\\\\\\n"


line3=" '$file_escape'  u 1:(\$4-\$5):(sprintf(\"%.1f\",\$9)):(sprintf(\"%.1f\",\$9)):(TC(\$9)) w labels font \"Courier-Bold,10\" textcolor variable  offset screen 0.0,screen -0.02 not"



if [ ! $counter -eq 0 ] ; then
# echo "counter=$counter"

    SCRIPT=$(echo $SCRIPT | sed "s/<<PLOT>>/,\\\\\\\\\\\n<<PLOT>>/g")

fi

counter=$(( $counter + 1 ))

# echo "out: counter=$counter"

SCRIPT=$( echo $SCRIPT |sed "s/<<PLOT>>/\\\\\\\\\\\n<<PLOT>>/g" | sed -r "s/<<PLOT>>/$line1<<PLOT>>/g"| sed -r "s/<<PLOT>>/$line2<<PLOT>>/g" | sed -r "s/<<PLOT>>/$line3<<PLOT>>/g")


SCRIPT=$( echo $SCRIPT  | sed -r "s/<<CHI2LABEL>>/set label sprintf('%.2f',$chi2_pt) at XMIN+0.1,$chi2_label_ypos font ',13' \\\n<<CHI2LABEL>>/g")



fi

done

local title=$(echo $dir | sed -r "s/[_-]/ /g")

title="$title. E = $E GeV" 

SCRIPT=$( echo $SCRIPT |sed "s/<<PLOT>>/\\\\\\\\\\\n/g" | sed "s/<<CHI2LABEL>>//g" | sed -r "s/<<TITLE>>/$title/g")


echo "writing $gnuscript"

echo -e $SCRIPT > $gnuscript


gnuplot -persist $gnuscript 

fi

done 

}

#######NOTE: COMPARISONS BETWEEN TWO FITS 
##% diff E288,E605 compared to exp. rel. err.
function compare-E288 {


local dir1=$1/stat/E288
local dir2=$2/stat/E288

local res1="$(echo $1 | sed -E "s/[_-/]/ /g")"
local res2="$(echo $2 | sed -E "s/[_-/]/ /g")"

local id1="$(echo $1 | sed -E "s/(.+)\//\1/g")"
local id2="$(echo $2 | sed -E "s/(.+)\//\1/g")"



local E288diff="E288diff-$id1-vs-$id2.gnu"

local colorerr="red"

local colorband="#6206B2"

local colorpt="black"

local pointtype=7

local pointsize=0.4

local line="plot [0:3][] '<paste $dir1/FNAME   $dir2/FNAME   ' u 1:(-100*\$5/\$4):(100*\$5/\$4) w filledcu fs transparent solid 0.15 noborder lc rgb '$colorerr'  not, '' u 1:(100*(2.0*(\$7-\$16)/(\$7+\$16))) lc rgb '$colorpt' ps $pointsize pt $pointtype lw 1.3 not"

local INIT="
load 'gnufiles/.gnu_script_diffE288.gnu'\\n\\n
set term qt size 600,600 font 'Courier,13'\\n
LMARGIN=0.15\\n
RMARGIN=0.75\\n
BMARGIN=0.10\\n
TMARGIN=0.90\\n\\n
set multiplot\\n\\n\\n\\n
set xtics 0,1,2 format ''\\n
set ytics ('-5%%' -5, '5%%' 5)\\n
eval setplot(200,4) ; plot[0:3][-10:10] -100 not\\n
eval setplot(200,5) ; plot[0:3][-10:10] -100 not\\n
eval setplot(200,6) ; plot[0:3][-10:10] -100 not\\n
eval setplot(200,7) ; plot[0:3][-10:10] -100 not\\n
set xlabel 'q_T [GeV]' offset 0,screen (TMARGIN-BMARGIN)/8*0.15\\n
set xtics format '%.0f'\\n
eval setplot(200,8) ; plot[0:3][-10:10] -100 not\\n\\n
unset xlabel\\n
set xtics format ''\\n
set ytics ('' -5, '' 5)\\n
eval setplot(300,4) ; plot[0:3][-10:10] -100 not\\n
eval setplot(300,5) ; plot[0:3][-10:10] -100 not\\n
eval setplot(300,6) ; plot[0:3][-10:10] -100 not\\n
eval setplot(300,7) ; plot[0:3][-10:10] -100 not\\n
eval setplot(300,8) ; plot[0:3][-10:10] -100 not\\n
set xlabel 'q_T [GeV]' offset 0,screen (TMARGIN-BMARGIN)/8*0.15\\n
set xtics format '%.0f'\\n
eval setplot(300,11); plot[0:3][-10:10] -100 not\\n\\n
unset xlabel\\n
set xtics format ''\\n
eval setplot(400,5) ; plot[0:3][-10:10] -100 not\\n
eval setplot(400,6) ; plot[0:3][-10:10] -100 not\\n
eval setplot(400,7) ; plot[0:3][-10:10] -100 not\\n
eval setplot(400,8) ; plot[0:3][-10:10] -100 not\\n
eval setplot(400,11); plot[0:3][-10:10] -100 not\\n
eval setplot(400,12); plot[0:3][-10:10] -100 not\\n
set xlabel 'q_T [GeV]' offset 0,screen (TMARGIN-BMARGIN)/8*0.15\\n
set xtics format '%.0f'\\n
eval setplot(400,13); plot[0:3][-10:10] -100 not\\n\\n
"

local TEMPLATE="
unset xtics; unset ytics;unset border;unset xlabel;unset ylabel\\n\\n
colorband1='#6206B2'\\n
colorband2='#FAE6AC'\\n
colorband3='dark-blue'\\n
colorerr=colorband1\\n
set yrange [-10:10]\\n
set object 1 rectangle from  0,-5 to  3,5  fc rgb colorerr fs transparent solid 0.2 noborder\\n
set object 2 rectangle from  0,-1 to  3,1  fc rgb colorerr fs transparent solid 0.2 noborder\\n
NEWLINE
"

local LABELS="
\n\n
###\n
\n
set label 1 'E=200 GeV' at screen LMARGIN+(0)*(RMARGIN-LMARGIN)/3.,0.94\\n
set label 2 'E=300 GeV' at screen LMARGIN+(1)*(RMARGIN-LMARGIN)/3.,0.94\\n
set label 3 'E=400 GeV' at screen LMARGIN+(2)*(RMARGIN-LMARGIN)/3.,0.94\\n
\n                           
set label 4  ' 4<Q<5  [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(1-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
set label 5  ' 5<Q<6  [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(2-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
set label 6  ' 6<Q<7  [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(3-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
set label 7  ' 7<Q<8  [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(4-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
set label 8  ' 8<Q<11 [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(5-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
set label 11 '11<Q<12 [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(6-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
set label 12 '12<Q<13 [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(7-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
set label 13 '13<Q<14 [GeV]' at screen RMARGIN+0.02, screen TMARGIN-(8-1)*(TMARGIN-BMARGIN)/8. - (TMARGIN-BMARGIN)/8/2\n
\n
set label 14 '( $res1 ) vs ( $res2 ))' at screen LMARGIN+0.1,0.98\\n \n 
unset border; unset xtics;unset ytics;unset object\n
set lmargin  0\n
set rmargin  1\n
set bmargin  0\n
set tmargin  1\n
set key  reverse Left spacing 1.5 at screen LMARGIN+0.33,screen BMARGIN+0.15  \n
plot -100 w filledcu lc rgb colorerr fs transparent solid 0.4 noborder t '±1%',-100 w filledcu lc rgb colorerr fs transparent solid 0.2 noborder t '±5%',-100 w filledcu lc rgb 'red' fs transparent solid 0.15 noborder t 'exp. rel. err.',-100 w p lc rgb 'black' pt $pointtype lw 1.5 t '% diff.'
"

local GNUSCRIPT=$INIT$TEMPLATE

for E in 200 300 400 ; do
    

for Q in 4 5 6 7 8 11 12 13  ; do

    local fname=E288_"$E"_"$Q"-"$(( Q+1 ))".stat
    
    local newline=$(echo $line | sed "s/FNAME/$fname/g" | sed -E "s/([_-./])/\\\\\1/g") 
    
    
    if [ -f $dir1/$fname ] && [ -f $dir2/$fname ] ; then 
    
       GNUSCRIPT=$(echo $GNUSCRIPT | sed "s/ NEWLINE/eval setplot($E,$Q)\\\\n\\\\n NEWLINE/g"| sed "s/LT/$(( Q-2 ))/g")
    
       GNUSCRIPT=$(echo $GNUSCRIPT | sed "s/ NEWLINE/$newline/g"| sed "s/LT/$(( Q-2 ))/g")
       
       GNUSCRIPT=$GNUSCRIPT"BREAK NEWLINE"
        
    fi

done

done


GNUSCRIPT=$(echo  $GNUSCRIPT | sed "s/BREAK NEWLINE//g;s/BREAK/\\\\n\\\\n/g")

echo  -e $GNUSCRIPT > $E288diff

echo  -e $LABELS >> $E288diff

ls $E288diff

gnuplot -p $E288diff

}

function compare-E605 {


local dir1=$1/stat/E605
local dir2=$2/stat/E605

local res1="$(echo $1 | sed -E "s/[_-/]/ /g")"
local res2="$(echo $2 | sed -E "s/[_-/]/ /g")"

local id1="$(echo $1 | sed -E "s/(.+)\//\1/g")"
local id2="$(echo $2 | sed -E "s/(.+)\//\1/g")"



local E605diff="E605diff-$id1-vs-$id2.gnu"

local colorerr="red"

local colorband="#6206B2"

local colorpt="black"

local pointtype=7

local pointsize=0.4

local line="plot [0:3][] '<paste $dir1/FNAME   $dir2/FNAME   ' u 1:(-100*\$5/\$4):(100*\$5/\$4) w filledcu fs transparent solid 0.15 noborder lc rgb '$colorerr'  not, '' u 1:(100*(2.0*(\$7-\$16)/(\$7+\$16))) lc rgb '$colorpt' ps $pointsize pt $pointtype lw 1.3 not"

local INIT="
load 'gnufiles/.gnu_script_diffE605.gnu'\\n\\n
set term qt size 400,430 font 'Courier,12'\\n
LMARGIN=0.02\\n
RMARGIN=0.95\\n
BMARGIN=0.12\\n
TMARGIN=0.77\\n\\n
set multiplot\\n\\n\\n\\n
set xtics 0,1,3 format ''\\n
set ytics ('-5%%' -5, '5%%' 5)\\n
eval setplot(7   ) ; plot[0:3][-10:10] -100 not\\n
eval setplot(8   ) ; plot[0:3][-10:10] -100 not\\n
eval setplot(10.5) ; plot[0:3][-10:10] -100 not\\n
eval setplot(11.5) ; plot[0:3][-10:10] -100 not\\n
set xlabel 'q_T [GeV]' offset 0,screen (TMARGIN-BMARGIN)/5*0.15\\n
set xtics format '%.0f'\\n
eval setplot(13.5) ; plot[0:3][-10:10] -100 not\\n
"

local TEMPLATE="
unset xtics; unset ytics;unset border;unset xlabel;unset ylabel\\n\\n
colorband1='#6206B2'\\n
colorband2='#FAE6AC'\\n
colorband3='dark-blue'\\n
colorerr=colorband1\\n
set yrange [-10:10]\\n
set object 1 rectangle from  0,-5 to  3,5  fc rgb colorerr fs transparent solid 0.2 noborder\\n
set object 2 rectangle from  0,-1 to  3,1  fc rgb colorerr fs transparent solid 0.2 noborder\\n
NEWLINE
"

local LABELS="
\n\n
###\n
\n
set label 1 'E=800 GeV' at screen LMARGIN+(1)*(RMARGIN-LMARGIN)/4,TMARGIN+0.02\\n
\n                           
set label 7  '   7<Q<8[GeV]' at screen LMARGIN+(1)*(RMARGIN-LMARGIN)/2.+0.12, screen TMARGIN-(1-1)*(TMARGIN-BMARGIN)/5. - (TMARGIN-BMARGIN)/5*2/3\n
set label 8  '   8<Q<9[GeV]' at screen LMARGIN+(1)*(RMARGIN-LMARGIN)/2.+0.12, screen TMARGIN-(2-1)*(TMARGIN-BMARGIN)/5. - (TMARGIN-BMARGIN)/5*2/3\n
set label 10 '10.5<Q<11.5[GeV]' at screen LMARGIN+(1)*(RMARGIN-LMARGIN)/2.+0.12, screen TMARGIN-(3-1)*(TMARGIN-BMARGIN)/5. - (TMARGIN-BMARGIN)/5*2/3\n
set label 11 '11.5<Q<13.5[GeV]' at screen LMARGIN+(1)*(RMARGIN-LMARGIN)/2.+0.12, screen TMARGIN-(4-1)*(TMARGIN-BMARGIN)/5. - (TMARGIN-BMARGIN)/5*2/3\n
set label 13 '13.5<Q<18.0[GeV]' at screen LMARGIN+(1)*(RMARGIN-LMARGIN)/2.+0.12, screen TMARGIN-(5-1)*(TMARGIN-BMARGIN)/5. - (TMARGIN-BMARGIN)/5*2/3\n
\n
set label 14 '$res1' at screen LMARGIN,0.98\\n \n
set label 15 'vs' at screen LMARGIN+0.1,0.93\\n \n
set label 16 '$res2' at screen LMARGIN,0.88\\n \n
unset border; unset xtics;unset ytics;unset object\n
set lmargin  0\n
set rmargin  1\n
set bmargin  0\n
set tmargin  1\n
set key reverse  Left spacing 1.2 at screen RMARGIN,screen 1  \n
plot -100 w filledcu lc rgb colorerr fs transparent solid 0.4 noborder t '±1%',-100 w filledcu lc rgb colorerr fs transparent solid 0.2 noborder t '±5%',-100 w filledcu lc rgb 'red' fs transparent solid 0.15 noborder t 'exp. err.',-100 w p lc rgb 'black' pt $pointtype lw 1.5 t '% diff.'
"

local GNUSCRIPT=$INIT$TEMPLATE

local -a QMIN=( 7 8 10.5 11.5 13.5  )
local -a QMAX=( 8 9 11.5 13.5 18.0  )

E=800

    

for i in 0 1 2 3 4  ; do

local Qmin=${QMIN[$i]}
local Qmax=${QMAX[$i]}

    local fname=E605_"$E"_"$Qmin"-"$Qmax".stat
    
    local newline=$(echo $line | sed "s/FNAME/$fname/g" | sed -E "s/([_-./])/\\\\\1/g") 
    
    
    if [ -f $dir1/$fname ] && [ -f $dir2/$fname ] ; then 
    
       GNUSCRIPT=$(echo $GNUSCRIPT | sed "s/ NEWLINE/eval setplot($Qmin)\\\\n\\\\n NEWLINE/g")
    
       GNUSCRIPT=$(echo $GNUSCRIPT | sed "s/ NEWLINE/$newline/g")
       
       GNUSCRIPT=$GNUSCRIPT"BREAK NEWLINE"
        
    fi

done


GNUSCRIPT=$(echo  $GNUSCRIPT | sed "s/BREAK NEWLINE//g;s/BREAK/\\\\n\\\\n/g")

echo  -e $GNUSCRIPT > $E605diff

echo  -e $LABELS >> $E605diff

ls $E605diff

gnuplot -p $E605diff

}



#####plot tmds

function plottmd {


#### input variables
local  dir="$1"

local  flavor="$2"
#kt or bt. Use kt as default
local  space="$3"

local  xlog="$4"

local  ylog="$5"

local  parameters="$6"

local  XSIZE_QT="450"

local  YSIZE_QT="450"


local  XSIZE_PDF="3.4"

local  YSIZE_PDF="3.4"

local  MARGINS

local SHOWPARA



### check input, use default if needed
if [ "$flavor" == "" ] ; then flavor="u" ; fi

if [ "$space" != "kt" ] && [ "$space" != "bt" ] ; then space="kt" ; fi

if [ "$xlog" != "0" ] && [ "$xlog" != "1" ] ; then xlog="0" ; fi

if [ "$ylog" != "0" ] && [ "$ylog" != "1" ] ; then ylog="1" ; fi

if [ "$parameters" == "" ] ; then

SHOWPARA=""

else

# echo " --- showing parameters ----"

local  parafile=$dir/input/para_*.input

# gnu_paragetlabel "$parameters" "$parafile"

# exit 0
 
SHOWPARA="set label 11 \"$(gnu_paragetlabel "$parameters" "$parafile"  )\" at graph 1.02,screen 0.85"

XSIZE_QT="1.2*$XSIZE_QT"
XSIZE_PDF="1.2*$XSIZE_PDF"

MARGINS="set lmargin screen 0.1"

fi


### get para values 


###get all tmd_hso files in dir
local  command="ls $dir/tmds/tmd-hso-$space-*.dat"

local  -a  hso_files=( $($command 2> /dev/null | grep -v \*) )

### find corresponding pert and inpt files
local  -a pert_files
local  -a inpt_files
local  -a pdf_files

local  -a    avals
local  -a muQ0vals
local  -a   Q0vals
local  -a  muQvals
local  -a    Qvals
local  -a    xvals

local  counter=0

for f in ${hso_files[@]};do 

      a=$(getval    a $f);    avals[$counter]=$a
   muQ0=$(getval muQ0 $f); muQ0vals[$counter]=$muQ0
     Q0=$(getval   Q0 $f);   Q0vals[$counter]=$Q0
    muQ=$(getval  muQ $f);  muQvals[$counter]=$muQ
      Q=$(getval    Q $f);    Qvals[$counter]=$Q
      x=$(getval    x $f);    xvals[$counter]=$x

    pert="$(echo $f | sed -E "s/hso/pert/g;s/(.*\/.*)-a-$a-muQ0-$muQ0-Q0-$Q0-/\1-/g")"
    
    inpt="$(echo $f | sed -E "s/hso/input/g;s/(.*\/.*)-a-$a-/\1-/g")"
    
    pdffile="$(echo $f | sed "s/\.dat/.pdf/g")"
    
    
    
    
    pert_files[$counter]=$pert
    inpt_files[$counter]=$inpt
    
    pdf_files[$counter]=$pdffile
    
    counter=$(( counter + 1 ))

done




##### some plot settings
local  col=$(getcolflav $flavor)
local  xlabel="$(echo $space|sed "s/bt/b_T [GeV^{-1}]/g;s/kt/k_T [GeV]/g")"


local  xpos1; local  ypos1  
local  xpos2; local  ypos2
local  xpos3; local  ypos3
local  xpos4; local  ypos4
local  xpos5; local  ypos5
local  xpos6; local  ypos6

local  SETLOGSCALEX
local  SETLOGSCALEY
local  FUNCTION
local  YLABEL

local  YMIN
local  YMAX
local  KEY

    
    xpos1=0.34 ;  ypos1=0.75
    xpos2=0.30 ;  ypos2=0.85
    xpos3=0.30 ;  ypos3=0.80
    xpos4=0.65 ;  ypos4=0.65
    xpos5=0.65 ;  ypos5=0.60
    xpos6=0.70 ;  ypos6=0.55
    
    SETLOGSCALEX=""
    SETLOGSCALEY="set logscale y; set ytics format '10^{%L}'"
    FUNCTION="function(x,y)=abs(y)"
    YLABEL="TMD pdf"
    
    YMIN="ymin=STATS_min_y/5.0"
    YMAX="ymax=STATS_max_y*3.0 "
    
    
    
    
    
    


if [ $xlog == "1" ] && [ $ylog == "1" ] ; then
    
    echo "------ setting log-log plot. ------------"
    
    xpos1=0.24 ;  ypos1=0.62
    xpos2=0.20 ;  ypos2=0.72
    xpos3=0.20 ;  ypos3=0.67
    xpos4=0.20 ;  ypos4=0.45
    xpos5=0.225;  ypos5=0.40
    xpos6=0.24 ;  ypos6=0.35
    
    SETLOGSCALEX="set logscale x; set xtics format '10^{%L}'"
    SETLOGSCALEY="set logscale y; set ytics format '10^{%L}'"
    FUNCTION="function(x,y)=abs(y)"
    
fi

if [ $xlog == "1" ] && [ $ylog == "0" ] ; then
    
    echo "------ setting log-lin plot. ------------"
    
    xpos1=0.67 ;  ypos1=0.62
    xpos2=0.63 ;  ypos2=0.72
    xpos3=0.63 ;  ypos3=0.67
    xpos4=0.63 ;  ypos4=0.50
    xpos5=0.655;  ypos5=0.45
    xpos6=0.67 ;  ypos6=0.40
    
    SETLOGSCALEX="set logscale x; set xtics format '10^{%L}'"
    SETLOGSCALEY=""
    
    if [ $space == "kt" ] ; then  FUNCTION="function(x,y)=x**2*abs(y)" ; fi
    
    if [ $space == "kt" ] ; then
    
        YLABEL="k_T^2 * TMD pdf"
        
        YMIN="ymin=STATS_min_y*0"
        
        YMAX="ymax=STATS_max_y*1.15"
        
    else
    
    
    
        xpos1=0.10 ;  ypos1=0.20
        xpos2=0.05 ;  ypos2=0.30
        xpos3=0.05 ;  ypos3=0.25
        xpos4=0.10 ;  ypos4=0.80
        xpos5=0.05 ;  ypos5=0.75
        xpos6=0.05 ;  ypos6=0.70
    
        YLABEL="        TMD pdf"  
        
        FUNCTION="function(x,y)=y"
        
        YMIN="ymin=-STATS_max_y"
        
        YMAX="ymax=STATS_max_y*1.15"
        
        KEY="set key left"
        
    fi
    
    
    
    
    
fi


if [ $xlog == "0" ] && [ $ylog == "0" ] ; then
    
    
    echo "------ setting lin-lin plot. ------------"
    
    xpos1=0.32 ;  ypos1=0.80
    xpos2=0.28 ;  ypos2=0.90
    xpos3=0.28 ;  ypos3=0.85
    xpos4=0.60 ;  ypos4=0.65
    xpos5=0.625;  ypos5=0.60
    xpos6=0.64 ;  ypos6=0.55
    
    SETLOGSCALEX=""
    SETLOGSCALEY=""
    FUNCTION="function(x,y)=x**2*abs(y)"
    
    if [ $space == "kt" ] ; then
    
        YLABEL="k_T^2 * TMD pdf"
    else
    
        YLABEL="b_T^2 * TMD pdf"    
    fi
    
    
    YMIN="ymin=STATS_min_y*0"
    YMAX="ymax=STATS_max_y*1.2"
    
    
fi


for index in $(seq 0 $(( ${#hso_files[@]} - 1 )) ); do 


    pert=${pert_files[$index]}
    inpt=${inpt_files[$index]}
     hso=${hso_files[$index]}
     
     pdffile=${pdf_files[$index]}
     
    a=${avals[$index]}
    muQ0=${muQ0vals[$index]}
    Q0=${Q0vals[$index]}
    muQ=${muQvals[$index]}
    Q=${Qvals[$index]}
    x=${xvals[$index]}
     
     
     
     
     
     
gnuplot -persist << EOF

    $FUNCTION

    set dashtype 2 (25,15,25,15)
    
    set dashtype 3 (15,6,6,6,6,6)

    
    color1="red"
    color2="black"
    color3="#3C7FFA"
    color4="#3A6AFA"
    
    
    XSIZE_QT=$XSIZE_QT
    
    YSIZE_QT=$YSIZE_QT
    
    set term qt size XSIZE_QT,YSIZE_QT font "Sans,11"
    
    LW=1.25
    
    $QTTERM
    
    $MARGINS
    
    $KEY
    
    set size ratio 1
    
    $SETLOGSCALEX
    
    $SETLOGSCALEY
    
    set xlabel "$xlabel" 
    set ylabel "$YLABEL" 
    
    
    set label 1 "a= $a GeV"                     at graph $xpos1, graph  $ypos1
    set label 2 "{/Symbol m}_{Q_0 }= $muQ0 GeV" at graph $xpos2, graph  $ypos2
    set label 3 "  Q_0= $Q0 GeV"                at graph $xpos3, graph  $ypos3
    
    set label 4 "{/Symbol m}_{Q   }= $muQ GeV"  at graph $xpos4, graph  $ypos4
    set label 5 " Q= $Q GeV"                    at graph $xpos5, graph  $ypos5
    set label 6 "x= $x"                         at graph $xpos6, graph  $ypos6
    
    
    $SHOWPARA
    
    
    stats [][]\
    "$hso" u 1:(function(\$1,\$$col)) nooutput
    
    $YMIN    
    
    $YMAX
    
    set arrow from $Q,ymin to $Q,ymax dt 2 nohead

    #print STATS_min_y,ymin  
    #print STATS_max_y,ymax  
      
    
    plot [:1.5*$Q][ymin:ymax]\
    '$pert' u 1:(function(\$1,\$$col)) w l lw LW lc rgb color3  dt 1  t 'pert.',\
    '$inpt' u 1:(function(\$1,\$$col)) w l lw LW lc rgb color2  dt 2  t 'inpt.',\
     '$hso' u 1:(function(\$1,\$$col)) w l lw LW lc rgb color1  dt 3  t 'RG imprv.'
      
    scale=2
    
    XSIZE_PDF=$XSIZE_PDF
    
    YSIZE_PDF=$YSIZE_PDF
     
    LW=LW*scale
    
    set term pdfcairo enhanced size XSIZE_PDF*scale,YSIZE_PDF*scale font sprintf("Sans,%i",12*scale)
    
    set output "$pdffile"
    
    replot
    
    print "$pdffile"
    
#     print "SHOWPARA=$SHOWPARA"
EOF
    
    
     

done 





}

###plots in pdf format for paper 
function plotE288-band {

local dir="$1"
local key1="$2"
local key2="$3"

local gnufname="plot_fit_E288_linear.gnu"

if [ ! -d "$dir/plots/" ] ; then 
    mkdir "$dir/plots/"
fi

cat gnufiles/$gnufname | \
\
sed -E "s/^([[:space:]]*DIR[[:space:]]*=[[:space:]]*).+/\1\"$dir\"/g" | \
\
sed -E "s/^([[:space:]]*key1[[:space:]]*=[[:space:]]*).+/\1\"$key1\"/g" | \
\
sed -E "s/^([[:space:]]*key2[[:space:]]*=[[:space:]]*).+/\1\"$key2\"/g"  \
\
> "$dir/plots/$gnufname"

gnuplot "$dir/plots/$gnufname"

}

function plotE605-band {

local dir="$1"
local key1="$2"
local key2="$3"

local gnufname="plot_fit_E605_linear.gnu"

if [ ! -d "$dir/plots/" ] ; then 
    mkdir "$dir/plots/"
fi

cat gnufiles/$gnufname | \
\
sed -E "s/^([[:space:]]*DIR[[:space:]]*=[[:space:]]*).+/\1\"$dir\"/g" | \
\
sed -E "s/^([[:space:]]*key1[[:space:]]*=[[:space:]]*).+/\1\"$key1\"/g" | \
\
sed -E "s/^([[:space:]]*key2[[:space:]]*=[[:space:]]*).+/\1\"$key2\"/g"  \
\
> "$dir/plots/$gnufname"

gnuplot "$dir/plots/$gnufname"

}

function plotD0Iepem {

local dir="$1"
local key1="$2"
local key2="$3"

local gnufname="plot_postdc_D0I_Z0_1999_band.gnu"

if [ ! -d "$dir/plots/" ] ; then 
    mkdir "$dir/plots/"
fi

if [ -f "$dir/plots/$gnufname" ] ; then

gnuplot "$dir/plots/$gnufname"

else


cat gnufiles/$gnufname | \
\
sed -E "s/^([[:space:]]*DIR[[:space:]]*=[[:space:]]*).+/\1\"$dir\"/g" | \
\
sed -E "s/^([[:space:]]*key1[[:space:]]*=[[:space:]]*).+/\1\"$key1\"/g" | \
\
sed -E "s/^([[:space:]]*key2[[:space:]]*=[[:space:]]*).+/\1\"$key2\"/g"  \
\
> "$dir/plots/$gnufname"

gnuplot "$dir/plots/$gnufname"

fi

}


function plotCDFIepem {

local dir="$1"
local key1="$2"
local key2="$3"

local gnufname="plot_postdc_CDFI_Z0_1999_band.gnu"

if [ ! -d "$dir/plots/" ] ; then 
    mkdir "$dir/plots/"
fi

if [ -f "$dir/plots/$gnufname" ] ; then

gnuplot "$dir/plots/$gnufname"

else



cat gnufiles/$gnufname | \
\
sed -E "s/^([[:space:]]*DIR[[:space:]]*=[[:space:]]*).+/\1\"$dir\"/g" | \
\
sed -E "s/^([[:space:]]*key1[[:space:]]*=[[:space:]]*).+/\1\"$key1\"/g" | \
\
sed -E "s/^([[:space:]]*key2[[:space:]]*=[[:space:]]*).+/\1\"$key2\"/g"  \
\
> "$dir/plots/$gnufname"

gnuplot "$dir/plots/$gnufname"

fi

}


function plotCDFIIepem {

local dir="$1"
local key1="$2"
local key2="$3"

local gnufname="plot_postdc_CDFII_Z0_2012_band.gnu"

if [ ! -d "$dir/plots/" ] ; then 
    mkdir "$dir/plots/"
fi

if [ -f "$dir/plots/$gnufname" ] ; then

gnuplot "$dir/plots/$gnufname"

else


cat gnufiles/$gnufname | \
\
sed -E "s/^([[:space:]]*DIR[[:space:]]*=[[:space:]]*).+/\1\"$dir\"/g" | \
\
sed -E "s/^([[:space:]]*key1[[:space:]]*=[[:space:]]*).+/\1\"$key1\"/g" | \
\
sed -E "s/^([[:space:]]*key2[[:space:]]*=[[:space:]]*).+/\1\"$key2\"/g"  \
\
> "$dir/plots/$gnufname"

gnuplot "$dir/plots/$gnufname"

fi

}



##### operations on matrices given by main code (cov,cho,etc)

function print_matrix_nb {


local infile="$1"

local outfile="$2"


local para_number="$(grep -m 1  ":" $infile | sed "s/:/:\n/g" | grep -c ":")"

local max=$(( $para_number - 1 ))


local  -a para_values=( $(grep   ":" $infile | sed "s/\#cov://g;s/:/\t/g" | cut -f3) )

local  re index=0

local  matrix=""

for i in $(seq 0 $max) ; do 

    matrix="$matrix{"
    
    for j in $(seq 0 $max) ; do 
    

        matrix="$matrix,${para_values[$index]}"
        
        index=$(( $index + 1 ))

    done 
    
    matrix="$matrix},"
    

done 

matrix="{$(echo $matrix | sed 's/{\,/{/g'  )}"
# echo -e $matrix > $outfile

echo $matrix | sed 's/\,}/}/g' | sed -r 's/e([[:digit:]]+)/*10^(\1)/g'| sed -r 's/e\-([[:digit:]]+)/*10^(-\1)/g'

}


function print_matrix {


local infile="$1"

local outfile="$2"


local para_number="$(grep -m 1  ":" $infile | sed "s/:/:\n/g" | grep -c ":")"

local max=$(( $para_number - 1 ))


local  -a para_values=( $(grep   ":" $infile | sed "s/\#cov://g;s/:/\t/g" | cut -f3) )

local  re index=0

local  matrix=""

for i in $(seq 0 $max) ; do 

    for j in $(seq 0 $max) ; do 
    

        matrix="$matrix\t${para_values[$index]}"
        
        index=$(( $index + 1 ))

    done 
    
    matrix="$matrix\n"
    

done 


echo -e $matrix > $outfile

echo $outfile

}


function print_cov {

local dir="$1"

local infile="$dir/cov/covariance.dat"

local outfile="$dir/cov/covariance.matrix"

print_matrix $infile $outfile


}

function plot_cov {

local dir="$1"

local dirname="$( echo "$dir" | sed  -r 's/([/.-])/\\\1/g' )"

local filematrixname=$( echo "$dir/cov/covariance.matrix" | sed  -r 's/([/.-])/\\\1/g' )

# echo $filematrixname

local gnufilein="gnufiles/cov.gnu"

local gnufileout="$dir/cov/cov.gnu"

cp $gnufilein $gnufileout

local paranames="$(paranameprintfree $dir)"


sed -i "s/<<DIR>>/$dir/g"             $gnufileout 

sed -i "s/<<FILE>>/$filematrixname/g" $gnufileout 

sed -i "s/<<PARA>>/$paranames/g"      $gnufileout 

gnuplot -persist $gnufileout

echo $gnufileout


}

function plot_eigen {

local dir="$1"

local dirname="$( echo "$dir" | sed  -r 's/([/.-])/\\\1/g' )"

local filematrixname=$( echo "$dir/min/eigensets.dat" | sed  -r 's/([/.-])/\\\1/g' )

# echo $filematrixname

local gnufilein="gnufiles/eigen.gnu"

local gnufileout="$dir/cov/eigen.gnu"

cp $gnufilein $gnufileout

local paranames="$( paranameprintfree $dir)"

local paranamesall="$( grep -m1 "." "$dir/min/eigensets.dat" )"


sed -i "s/<<DIR>>/$dir/g"               $gnufileout 

sed -i "s/<<FILE>>/$filematrixname/g"   $gnufileout 

sed -i "s/<<PARA>>/$paranames/g"        $gnufileout

sed -i "s/<<PARAALL>>/$paranamesall/g"  $gnufileout


gnuplot -persist $gnufileout


echo $gnufileout



}

# function plot_eigen_compare {
# 
# 
# local dir1="$1"
# 
# local dir1="$2"
# 
# local eigenfile1="$dir1/min/eigensets.dat"
# 
# local eigenfile2="$dir2/min/eigensets.dat"
# 
# local dirname1="$( echo "$dir1" | sed  -r 's/([/.-])/\\\1/g' )"
# 
# local dirname2="$( echo "$dir2" | sed  -r 's/([/.-])/\\\1/g' )"
# 
# local eigenfilename1="$( echo "$eigenfile1" | sed  -r 's/([/.-])/\\\1/g' )"
# 
# local eigenfilename2="$( echo "$eigenfile2" | sed  -r 's/([/.-])/\\\1/g' )"
# 
# local -a paranames=( $( paranameprintfree $dir1 ) )
# 
# local para_number=${#paranames[@]}
# 
# local script="
# 
# plot \"referee_Q17_fit_E288_spect-RG/min/eigensets.dat\" u \"Lambda\":\"bK\" pt 8 ps 2 lw 1.2,
#         \"referee_Q17_fit_E288_spect/min/eigensets.dat\" u \"Lambda\":\"bK\" pt 6 ps 2 lw 1.2  
# 
# "
# # local gnufilein="gnufiles/eigen.gnu"
# 
# # local gnufileout="$dir/min/eigen.gnu"
# 
# # cp $gnufilein $gnufileout
# echo "$script" > trash666.gnu
# 
# 
# }

