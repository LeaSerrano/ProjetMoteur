#!/bin/sh
bindir=$(pwd)
cd /home/lea/M1-IMAGINE/S2/MoteurJeux/projet-moteurdejeux/App/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/lea/M1-IMAGINE/S2/MoteurJeux/projet-moteurdejeux/build/App 
	else
		"/home/lea/M1-IMAGINE/S2/MoteurJeux/projet-moteurdejeux/build/App"  
	fi
else
	"/home/lea/M1-IMAGINE/S2/MoteurJeux/projet-moteurdejeux/build/App"  
fi
