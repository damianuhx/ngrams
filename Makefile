CPP = g++
INCDIRS = 
LIBS = 
LIBDIRS = 
LFLAGS = 
CFLAGS = $(INCDIRS) -O3
DBG = -g3


#insert additional object-files here...
OBJS = main.o


EXECUTABLE = Exercise
COMMAND = nice time ./${EXECUTABLE}


program: ${EXECUTABLE}

run: program
	./${EXECUTABLE}

${EXECUTABLE}: ${OBJS}
	$(CPP) $(LFLAGS) $(DBG)  $(OBJS) $(LIBDIRS) $(LIBS) -o $@


%.o : %.cpp %.h
	$(CPP) $(CFLAGS) $(DBG) -c $< -o $@ 

clean:
	- rm *.o *~ 
	- rm ${EXECUTABLE}

depend: *cpp 
	- rm depend
	$(CPP) $(CFLAGS) $(DBG) -MM *.cpp > depend

include depend
