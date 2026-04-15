



file="./results/E288_control/./stat//E288//E288_400_11-12.stat"

set title  "E288 400 11-12"
set xlabel "q_T[GeV]"
set ylabel "csec"

set label 'E=400 GeV GeV'  at screen 0.7,screen 0.7
set label '11<Q<12 GeV GeV'  at screen 0.7,screen 0.65
set label '√s=27.4 GeV'  at screen 0.7,screen 0.6
set label 'y=0.03'  at screen 0.7,screen 0.55
set label '{/Symbol c}^2_{pt} (pts)=1 (130)'  at screen 0.8,screen 0.95



stat file u 1:($4+$5) nooutput
ymax=STATS_max_y
xmin=STATS_min_x
xmax=STATS_max_x

stat file u 1:($4-$5) nooutput
ymin=STATS_min_y

xdel=(xmax-xmin)*0.1

ydel=(ymax-ymin)*0.1


plot[xmin-xdel:xmax+xdel][ymin-ydel:ymax+ydel] \
    file u 1:($7*$8)          w l lc "orange" lw 2 t "",\
    file u 1:4 :5 w err pt 13 lc "dark-blue"  t "E288 400 11-12",\
    file u 1:($4-$5):(sprintf("%.3f",$9)) w labels  offset screen 0.0, screen -0.03 not
    


