color0 ="#ffb14e"
color1 ="#E2BE0A"
color2 ="#fa8775"
color3 ="#ea5f94"
color4 ="#cd34b5"
color5 ="#9d02d7"
color6 ="#0000ff"
color7 ="#000000"
color8 ="#f5070d"





LW1=3.0
PS1=1.0


###version 2

set dashtype 11 (8,3,8,3)

set dashtype 12 (20,5,20,5)

set dashtype 13 (13,3,3,3)

set dashtype 14 (13,3,3,3,3,3)


set linetype 2  lc rgb color1    ps PS1 lw LW1 pt 9   dt 1
set linetype 3  lc rgb color2    ps PS1 lw LW1 pt 7   dt 2
set linetype 4  lc rgb color3    ps PS1 lw LW1 pt 11  dt 3
set linetype 5  lc rgb color4    ps PS1 lw LW1 pt 13  dt 4
set linetype 6  lc rgb color5    ps PS1 lw LW1 pt 15  dt 11
                                                      
set linetype 9  lc rgb color6    ps PS1 lw LW1 pt 5   dt 12
set linetype 10 lc rgb color7    ps PS1 lw LW1 pt 10  dt 13
set linetype 11 lc rgb color8    ps PS1 lw LW1 pt 12  dt 14




ROWS=1
COLS=1

LMARGIN=0.08
RMARGIN=0.98
BMARGIN=0.12
TMARGIN=0.95




XKEY=0.97

YKEY=0.75

set_left_margin(i)=sprintf(\
"set lmargin screen LMARGIN+(%f-1)*(RMARGIN-LMARGIN)/COLS",i\
)

set_right_margin(i)=sprintf(\
"set rmargin screen LMARGIN+(%f)*(RMARGIN-LMARGIN)/COLS",i\
)

set_bottom_margin(i)=sprintf(\
"set bmargin screen TMARGIN-(%f)*(TMARGIN-BMARGIN)/ROWS",i\
)

set_top_margin(i)=sprintf(\
"set tmargin screen TMARGIN-(%f-1)*(TMARGIN-BMARGIN)/ROWS",i\
)



ylow (meas,err)=meas-err<=0?(meas-err+1E-20):meas-err
yhigh(meas,err)=meas+err

 
 
scale=2.0

width=7.0

height=width/2.0

fontsize=12*scale

set term pdfcairo enhanced color font sprintf("Sans,%i",fontsize) size width*scale,height*scale


