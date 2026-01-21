set lmargin  screen 0.15
set rmargin  screen 0.97
set bmargin  screen 0.15
set tmargin  screen 0.90



color0 ="#ffb14e"
color1 ="#E2BE0A"
color2 ="#fa8775"
color3 ="#ea5f94"
color4 ="#cd34b5"
color5 ="#9d02d7"
color6 ="#0000ff"
color7 ="#000000"
color8 ="#f5070d"



LW1=2.0
PS1=0.6

###version 1
set linetype 1  lc "red"                  ps PS1 lw LW1 pt 5
set linetype 2  lc "steelblue"            ps PS1 lw LW1 pt 9
set linetype 3  lc "dark-spring-green"    ps PS1 lw LW1 pt 7
set linetype 4  lc "black"                ps PS1 lw LW1 pt 11
set linetype 5  lc "dark-violet"          ps PS1 lw LW1 pt 13
set linetype 6  lc "orange-red"           ps PS1 lw LW1 pt 15
set linetype 7  lc "royalblue"            ps PS1 lw LW1 pt 4
set linetype 8  lc "dark-blue"            ps PS1 lw LW1 pt 8
set linetype 9  lc "orange"               ps PS1 lw LW1 pt 6
set linetype 10 lc "green"                ps PS1 lw LW1 pt 10
set linetype 11 lc "cyan"                 ps PS1 lw LW1 pt 12
set linetype 12 lc "blue"                 ps PS1 lw LW1 pt 14

###version 2
set linetype 2  lc rgb color1    ps PS1 lw LW1 pt 9   dt 1
set linetype 3  lc rgb color2    ps PS1 lw LW1 pt 7   dt 2
set linetype 4  lc rgb color3    ps PS1 lw LW1 pt 11  dt 3
set linetype 5  lc rgb color4    ps PS1 lw LW1 pt 13  dt 4
set linetype 6  lc rgb color5    ps PS1 lw LW1 pt 15  dt 11
                                                      
set linetype 9  lc rgb color6    ps PS1 lw LW1 pt 5   dt 12
set linetype 10 lc rgb color7    ps PS1 lw LW1 pt 10  dt 13
set linetype 11 lc rgb color8    ps PS1 lw LW1 pt 12  dt 14




set dashtype 11 (8,3,8,3)

set dashtype 12 (20,5,20,5)

set dashtype 13 (13,3,3,3)

set dashtype 14 (13,3,3,3,3,3)

scale=1.0

fontsize=12*scale

set term pdfcairo enhanced color font sprintf("Sans,%i",fontsize) size 3.4*scale,3.4*scale/0.7


