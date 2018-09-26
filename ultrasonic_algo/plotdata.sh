gnuplot <<EOF
#set terminal x11
plot "plot.txt" using 1:2 with lines, \
	"plot.txt" using 1:3 with lines, \
	"plot.txt" using 1:4 with lines, \
	"plot.txt" using 1:5  with lines, \
	"plot.txt" using 1:6 lw 2 with lines, \
	"plot.txt" using 1:7 with lines, \
	"plot.txt" using 1:8 with lines
pause 30000
EOF
