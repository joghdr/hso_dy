load 'gnufiles/.format4_pdf.gnu'

height=width/3.0

set term pdfcairo enhanced color font sprintf("Sans,%i",fontsize) size width*scale,height*scale

DIR=""

subDIR="stat"

key1=""

key2=""

Label(key1,key2)=sprintf("%s %s",key1,key2)


 
file(Qmin,Qmax)=sprintf("%s/%s/E605/E605_800_%s-%s.stat",DIR,subDIR,Qmin,Qmax)

OUTPUT=sprintf("%s/plots/E605-%s.pdf",DIR,DIR)
 
set output OUTPUT
 
set multiplot 

 
ROWS=1
COLS=5

LMARGIN=0.05
RMARGIN=0.98
BMARGIN=0.37
TMARGIN=0.95



margins(row,col)=sprintf("\
rowint=%i;colint=%i;\
\
eval set_left_margin(colint);eval set_right_margin(colint);eval set_bottom_margin(rowint);eval set_top_margin(rowint)"\
,row,col)


YMIN=0;YMAX=0

Plot(Qmin,Qmax)=\
sprintf("plot[:][YMIN:YMAX] \
 \
 file(\"%s\",\"%s\") u 1:(($7+$10)*$8):(($7-$11)*$8) w filledcu lt LTline not,\
 file(\"%s\",\"%s\") u 1:($4):($5) w err lt LTdata dt 1 not,\
 file(\"%s\",\"%s\") u 1:($7*$8) w l lt LTband not"\
 \
 ,Qmin,Qmax,Qmin,Qmax,Qmin,Qmax)
 
PlotKey(key1,key2)=\
sprintf("Qminin='7';Qmaxin='8';\
\
set key  spacing 2.0 samplen 1.5  at screen x0+2.15*delx, y0+1.8*dely;\
plot[0:1][0:1] \
 \
 file(Qminin,Qmaxin) u 1:(-1):(0) w err lt LTdata dt 1  t 'E605\n(csec/nucl.)',\
 file(Qminin,Qmaxin) u 1:(-1) w l lt LTband t Label(\"%s\",\"%s\") ,\
 file(Qminin,Qmaxin) u 1:(-1):(-1) w filledcu lt LTline t 'Hessian'"\
 \
 ,key1,key2)
 
 xmin(Qmin)=\
 Qmin=="7"?0:\
 Qmin=="8"?0:\
 Qmin=="10.5"?0:\
 Qmin=="11.5"?0:\
 Qmin=="13.5"?0:0

 
 xmax(Qmin)=\
 Qmin=="7"?3:\
 Qmin=="8"?3:\
 Qmin=="10.5"?3:\
 Qmin=="11.5"?3:\
 Qmin=="13.5"?3:0
 
 
 setxrange(Qmin)=sprintf( "set xrange [xmin('%s'):xmax('%s')] ",Qmin,Qmin)

setyrange(Qmin,Qmax)=sprintf("\
\
stat file('%s','%s') u 1:($4+$5) nooutput;\
ymax_temp=STATS_max_y;\
stat file('%s','%s') u 1:($4-$5) nooutput;\
ymin_temp=STATS_min_y;\
YMIN=ymin_temp - 0.05*(ymax_temp-ymin_temp);\
YMAX=ymax_temp + 0.05*(ymax_temp-ymin_temp);\
",Qmin,Qmax,Qmin,Qmax)
 
 
 keyQ(Q)=sprintf("%.0f<Q<%.0f",Q,Q+1)
 
 
 set style fill transparent solid 0.35 noborder
 
 set xtics 0.,0.4,2.8 format "%.1f"
 

 
 
 set xlabel "q_T [GeV]" offset screen 0.0,screen 0.01
 
 set ylabel "q^0 d^3{/Symbol s} / d^3q [fb * GeV^{-2}]" offset screen 0.0,screen 0
 
 unset key
 


 set ytics format "" nomirror

 
 
 LTdata=8
 LTline=9
 LTband=9
 
 set xtics 0,1.2,3
 
 eval setxrange(   "7"); eval setyrange(   "7",   "8"); eval margins(1,1); eval Plot(   "7",   "8")
 unset ylabel
 eval setxrange(   "8"); eval setyrange(   "8",   "9"); eval margins(1,2); eval Plot(   "8",   "9")
 eval setxrange("10.5"); eval setyrange("10.5","11.5"); eval margins(1,3); eval Plot("10.5","11.5")
 eval setxrange("11.5"); eval setyrange("11.5","13.5"); eval margins(1,4); eval Plot("11.5","13.5")
 eval setxrange("13.5"); eval setyrange("13.5","18.0"); eval margins(1,5); eval Plot("13.5","18.0")
                                                                                 

 
 
 LMARGIN_BIG=LMARGIN*1/2
 BMARGIN_BIG=BMARGIN*4/9
 
 unset border;unset ytics
 
 set arrow from screen LMARGIN_BIG,screen BMARGIN_BIG to screen RMARGIN, screen BMARGIN_BIG nohead
 
 set lmargin screen LMARGIN_BIG; set rmargin screen RMARGIN; set bmargin screen BMARGIN_BIG; set tmargin screen TMARGIN
 
 set xlabel "⟨Q⟩ [GeV]"
 
 #set  label "E = 800 GeV" at screen LMARGIN*(1.35),screen TMARGIN*(0.92)
 
 x0= (LMARGIN-LMARGIN_BIG)/(RMARGIN-LMARGIN_BIG)
 
 delx=(RMARGIN-LMARGIN)/COLS/2.0/(RMARGIN-LMARGIN_BIG)
 
 y0= (BMARGIN-BMARGIN_BIG)/(TMARGIN-BMARGIN_BIG)
 
 dely=(TMARGIN-BMARGIN)/ROWS/2.0/(TMARGIN-BMARGIN_BIG)
 
 set xtics ("7.5"  x0+1*delx,"8.5"  x0+3*delx,"11.0"  x0+5*delx,"12.5"  x0+7*delx,"15.75"  x0+9*delx) nomirror
 
 #set ytics ("800"  y0+1*dely) nomirror
 
 eval PlotKey(key1,key2)
 
 unset multiplot
 
 
 
 print OUTPUT
