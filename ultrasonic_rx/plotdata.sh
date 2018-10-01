gnuplot <<EOF
#set terminal x11
plot "$1" using 1:2 with lines, \
	"$1" using 1:3 with lines, \
	"$1" using 1:4 with lines, \
	"$1" using 1:(atan2(\$4,\$3)*400) lw 2 with lines
pause 30000
EOF
