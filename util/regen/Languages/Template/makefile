CC	=	gcc
RC	=	windres
LDFLAGS	=	
FLAGS	=	-I. -O2
LIBS	=	Regen.o

OBJ	=       stub.o

OUT	=	Satellite.dll

all	:	$(OUT)

$(OUT)	:	$(OBJ)
		$(RC) -DDEBUGGERS Regen.rc -o Regen.o
		$(CC) -shared -o $(OUT) $(OBJ) $(LIBS) $(LDFLAGS)
	        
%.o :		%.cpp
		$(CC) -c $< -o $@ $(FLAGS)
