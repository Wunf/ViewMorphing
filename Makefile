CFLAGS = -c -g
INCPATH = -IC:\Include 
LIB = $(LIBS)GL\glut32.lib -lopengl32

ViewMorphing : main.o
	g++ $^ -o ViewMorphing $(LIB) 
main.o : main.cpp main.h
	g++ $(CFLAGS) $(INCPATH) $< 
clean : 
	del *.exe *.o