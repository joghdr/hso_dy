OUTPUT='<<DIR>>/cov/plot_cov.pdf'

PARA='<<PARA>>'


set size ratio 1
set palette gray negative

# grid lines
set x2tics 1 format '' scale 0,0.001
set y2tics 1 format '' scale 0,0.001`
set mx2tics 2
set my2tics 2

# labeling
set xtics 1 out nomirror
set ytics 1 out nomirror

set autoscale fix
set link x
set link y

set grid front mx2tics my2tics lw 4.0 lt -1 lc rgb 'white'


set palette defined ( -1 "#380356"  , -0.001 "white" , 0.001 "white" , 1 "#003b2e"  ) 

set cbrange [-.1:.1];


set for [i=1:words(PARA)] xtics ( word(PARA,i) i-1 )
set for [i=1:words(PARA)] ytics ( word(PARA,i) i-1 )


plot '<<FILE>>' matrix with image not

load "gnufiles/.format_pdf.gnu"

set lmargin screen 0.1
set rmargin screen 0.9

set output OUTPUT

replot

print OUTPUT
     
