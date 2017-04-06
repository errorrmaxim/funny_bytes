LIBS	= ./libprocps.a -lncurses
all:	sys_monitor

sys_monitor:	main.o printer_foo.o 
	g++ main.o printer_foo.o -o sys_monitor

main.o:	main.cpp
	g++ -c main.cpp

printer_foo.o:	printer_foo.cpp
	g++ -c printer_foo.cpp
