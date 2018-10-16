gnuplot <<EOF
#set terminal x11
mag(x,y)=sqrt(x*x+y*y)
plot "$1" using 1:2 with lines, \
	"$1" using 1:3 with lines, \
	"$1" using 1:4 with lines, \
	"$1" using 1:5 lw 2 with lines, \
	"$1" using 1:(mag(\$3,\$4)) with lines, \
	"$1" using 1:6 lw 2 with lines
pause 30000
EOF

#	"$1" using 1:(atan2(\$4,\$3)*800) with lines
