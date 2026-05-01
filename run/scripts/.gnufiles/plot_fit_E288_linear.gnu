load 'GNUFILES_DIR/.format_pdf.gnu'



DIR=""

ID=""

subDIR="stat"

key1=""

key2=""

file(E,Q)=sprintf("%s/%s/E288/E288_%.0f_%.0f-%.0f.stat",DIR,subDIR,E,Q,Q+1)

Label(key1,key2)=sprintf("%s %s",key1,key2)

OUTPUT=sprintf("%s/plots/E288-%s.pdf",DIR,ID)

 set output OUTPUT

 set multiplot


 
ROWS=3
COLS=8

LMARGIN=0.12
RMARGIN=0.98
BMARGIN=0.25
TMARGIN=0.95



margins(row,col)=sprintf("\
colint=%i;rowint=%i;\
\
eval set_left_margin(colint);eval set_right_margin(colint);eval set_bottom_margin(rowint);eval set_top_margin(rowint)"\
,row,col)



YMIN=0;YMAX=0

Plot(E,Q)=\
sprintf("Ein=%i;Qin=%i;plot[:][YMIN:YMAX] \
 \
 file(Ein,Qin) u 1:(($7+$10)*$8):(($7-$11)*$8) w filledcu lt LTline not,\
 file(Ein,Qin) u 1:($4):($5) w err lt LTdata dt 1 not,\
 file(Ein,Qin) u 1:($7*$8) w l lt LTband t keyQ(Qin)"\
 \
 ,E,Q)

 
PlotKey(key1,key2)=\
sprintf("Ein=400;Qin=13;\
\
set key reverse Left spacing 1.5 samplen 1.5  at screen 1, screen BMARGIN+dely/2*(1.5);\
plot[0:1][0:1] \
 \
 file(Ein,Qin) u 1:(-1):(0) w err lt LTdata dt 1  t 'E288',\
 file(Ein,Qin) u 1:(-1) w l lt LTband t Label(\"%s\",\"%s\") ,\
 file(Ein,Qin) u 1:(-1):(-1) w filledcu lt LTline t 'Hessian band'"\
 \
 ,key1,key2)
 
 xmin(Q)=\
 Q==4?0:\
 Q==5?0:\
 Q==6?0:\
 Q==7?0:\
 Q==8?0:\
 Q==11?0:\
 Q==12?0:\
 Q==13?0:0

 
 xmax(Q)=\
 Q==4?1.3:\
 Q==5?1.5:\
 Q==6?1.5:\
 Q==7?1.6:\
 Q==8?1.7:\
 Q==11?2.5:\
 Q==12?2.8:\
 Q==13?2.8:0
 
 
 setxrange(Q)=sprintf("Qin=%i;set xrange [xmin(Qin):xmax(Qin)] ",Q)

setyrange(E,Q)=sprintf("\
Ein=%i;Qin=%i;\
stat file(Ein,Qin) u 1:($4+$5) nooutput;\
ymax_temp=STATS_max_y;\
stat file(Ein,Qin) u 1:($4-$5) nooutput;\
ymin_temp=STATS_min_y;\
YMIN=ymin_temp - 0.05*(ymax_temp-ymin_temp);\
YMAX=ymax_temp + 0.05*(ymax_temp-ymin_temp);\
",E,Q)
 
 
 keyQ(Q)=sprintf("%.0f<Q<%.0f",Q,Q+1)
 
 
 set style fill transparent solid 0.35 noborder
 
 set xtics 0.,0.4,2.8 format "%.1f"
 

 
 
set xlabel "q_T [GeV]" offset screen 0.0,screen 0.01
 
 
 
unset key
 


unset xtics; unset ytics; unset xlabel ; unset ylabel

 
 
 LTdata=8
 LTline=9
 LTband=9
 
 set xtics 0.2,0.5,1.2 format ""
 eval setxrange( 5); eval setyrange(400, 5); eval margins(2,1); eval Plot(400, 5)
 eval setxrange( 6); eval setyrange(400, 6); eval margins(3,1); eval Plot(400, 6)
 eval setxrange( 7); eval setyrange(400, 7); eval margins(4,1); eval Plot(400, 7)
 set xtics 0.3,0.6,1.5 format ""
 eval setxrange( 8); eval setyrange(400, 8); eval margins(5,1); eval Plot(400, 8)
 set xtics 0.3,1.0,2.8 format ""
 eval setxrange(11); eval setyrange(400,11); eval margins(6,1); eval Plot(400,11)
 set xtics 0.3,1.0,2.8 format "%.1f"
 set xlabel "q_T [GeV]" offset screen 0.0,screen 0.01
 eval setxrange(12); eval setyrange(400,12); eval margins(7,1); eval Plot(400,12)
 eval setxrange(13); eval setyrange(400,13); eval margins(8,1); eval Plot(400,13)
 
 set xtics 0.2,0.4,1.2 format "";unset xlabel
 set ylabel "q^0 d^3{/Symbol s} / d^3q [ fb * GeV^{-2} ]" offset screen 0.0,screen 0
 eval setxrange( 4); eval setyrange(300, 4); eval margins(1,2); eval Plot(300, 4)
 set xtics 0.2,0.5,1.2 format "";unset ylabel
 eval setxrange( 5); eval setyrange(300, 5); eval margins(2,2); eval Plot(300, 5)
 eval setxrange( 6); eval setyrange(300, 6); eval margins(3,2); eval Plot(300, 6)
 eval setxrange( 7); eval setyrange(300, 7); eval margins(4,2); eval Plot(300, 7)
 set xtics 0.3,0.6,1.5 format ""
 eval setxrange( 8); eval setyrange(300, 8); eval margins(5,2); eval Plot(300, 8)
 set xtics 0.3,1.0,2.8 format "%.1f"
 set xlabel "q_T [GeV]" offset screen 0.0,screen 0.01
 eval setxrange(11); eval setyrange(300,11); eval margins(6,2); eval Plot(300,11)
 
 set xtics 0.2,0.4,1.2 format "%.1f"
 eval setxrange( 4); eval setyrange(200, 4); eval margins(1,3); eval Plot(200, 4)
 set xtics 0.2,0.5,1.2 format "%.1f"
 eval setxrange( 5); eval setyrange(200, 5); eval margins(2,3); eval Plot(200, 5)
 eval setxrange( 6); eval setyrange(200, 6); eval margins(3,3); eval Plot(200, 6)
 eval setxrange( 7); eval setyrange(200, 7); eval margins(4,3); eval Plot(200, 7)
 set xtics 0.3,0.6,1.5 format "%.1f"
 eval setxrange( 8); eval setyrange(200, 8); eval margins(5,3); eval Plot(200, 8)

 
 
 LMARGIN_BIG=LMARGIN*2/3
 BMARGIN_BIG=BMARGIN/2
 
 set lmargin screen LMARGIN_BIG; set rmargin screen RMARGIN; set bmargin screen BMARGIN_BIG; set tmargin screen TMARGIN
 
 set xlabel "⟨Q⟩ [GeV]"
 
 set ylabel "E [GeV]"
 
 x0= (LMARGIN-LMARGIN_BIG)/(RMARGIN-LMARGIN_BIG)
 
 delx=(RMARGIN-LMARGIN)/COLS/2.0/(RMARGIN-LMARGIN_BIG)
 
 y0= (BMARGIN-BMARGIN_BIG)/(TMARGIN-BMARGIN_BIG)
 
 dely=(TMARGIN-BMARGIN)/ROWS/2.0/(TMARGIN-BMARGIN_BIG)
 
 set xtics ("4.5"  x0+1*delx,"5.5"  x0+3*delx,"6.5"  x0+5*delx,"7.5"  x0+7*delx,"8.5"  x0+9*delx,"11.5"  x0+11*delx,"12.5"  x0+13*delx,"13.5" x0+15*delx) nomirror
 
 set ytics ("200"  y0+1*dely,"300"  y0+3*dely,"400"  y0+5*dely) nomirror
 
 eval PlotKey(key1,key2)
 
 unset multiplot
 
 
 
 print OUTPUT
