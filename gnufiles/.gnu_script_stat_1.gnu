#<<LOADFORMAT>>


file="<<FNAME>>"

set title  "<<TITLE>>"
set xlabel "<<XLABEL>>"
set ylabel "<<YLABEL>>"

#<<LABELS>>


stat file u <<XCOL>>:($<<YCOL>>+$<<ECOL>>) nooutput
ymax=STATS_max_y
xmin=STATS_min_x
xmax=STATS_max_x

stat file u <<XCOL>>:($<<YCOL>>-$<<ECOL>>) nooutput
ymin=STATS_min_y

xdel=(xmax-xmin)*0.1

ydel=(ymax-ymin)*0.1


plot[xmin-xdel:xmax+xdel][ymin-ydel:ymax+ydel] \
    file u <<XCOL>>:<<YCOLT>>          w l lc "orange" lw 2 t "<<KEYT>>",\
    file u <<XCOL>>:<<YCOL>> :<<ECOL>> w err pt 13 lc "dark-blue"  t "<<KEY>>",\
    file u <<XCOL>>:($<<YCOL>>-$<<ECOL>>):(sprintf("%.3f",$<<LCOL>>)) w labels  offset screen 0.0, screen -0.03 not
    


