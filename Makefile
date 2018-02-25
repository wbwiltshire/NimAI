# Game of Nim AI makefile
# Link: https://docs.microsoft.com/en-us/cpp/build/nmake-reference
CC=cl
#CFLAGS=/c /EHsc
#Compile with debug info
CFLAGS=/c /EHsc /Zi
LINK=link
#LFLAGS=
#Link with debug info
LFLAGS= /DEBUG 
LIBS=
ODIR=bin
NN=NimAI.exe

all: $(ODIR)\$(NN)

.cpp{$(ODIR)}.obj: 
	$(CC) $(CFLAGS) $** /Fo$@

$(ODIR)\$(NN): $(ODIR)\main.obj $(ODIR)\NimBoard.obj $(ODIR)\AlphaBeta.obj
	$(LINK) $**  /out:$@ $(LFLAGS) $(LIBS)

clean:
	@IF EXIST *.pdb (del *.pdb)
	@IF EXIST bin\*.obj (del bin\*.obj)
	@IF EXIST bin\*.pdb (del bin\*.pdb)
	@IF EXIST bin\*.exe (del bin\*.exe)
	@IF EXIST bin\*.asm (del bin\*.asm)
	@IF EXIST bin\*.ilk (del bin\*.ilk)