PROJ = SETUP
PROJFILE = $(PROJ).MAK
OBJ = BEEP
OBJ1 = DELAY
DEBUG = 0


PWBRMAKE  = pwbrmake
NMAKE  = nmake
LINKER  = link
ILINK  = ilink
LRF  = echo > NUL
BIND  = bind
RC  = rc
#IMPLIB  = implib
LFLAGS_G  =  /NOI /NOE  /BATCH /STACK :10000
LFLAGS_D  = /CO  /PACKC
LFLAGS_R  = /EXE /PACKC
MAPFILE_D  = NUL
MAPFILE_R  = NUL
CC  = cl
#CFLAGS_G  = /W4 /AL
CFLAGS_G  =  /f- /W4 /AL /Ox
#CFLAGS_D  = /qc /Gi$(PROJ).mdt /Zr /Zi /Od
CFLAGS_D  = /Zi /Od /Zr
#CFLAGS_R  = /Ot /Oi /Ol /Oe /Og /Gs
CFLAGS_R  = 
LLIBS_R  = 
LLIBS_D  = 
LIBS = 
ASM  = masm
AFLAGS_G  = /Mx /T
AFLAGS_D  = /Zi

OBJS  = $(PROJ).obj $(OBJ).obj $(OBJ1).obj

all: $(PROJ).exe

.SUFFIXES: .cpp .obj

$(PROJ).obj : $(PROJ).CPP 

$(OBJ).obj : $(OBJ).CPP 

$(OBJ1).obj : $(OBJ1).CPP 

$(PROJ).bsc : 

$(PROJ).exe : $(OBJS)
!IF $(DEBUG)
        $(LRF) @<<$(PROJ).lrf
$(RT_OBJS) $(OBJS: = +^
),$@,$(MAPFILE_D),$(LLIBS_G) $(LLIBS_D) $(LIBS),$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
<<
!ELSE
       $(LRF) @<<$(PROJ).lrf
$(RT_OBJS) $(OBJS: = +^
),$@,$(MAPFILE_R),$(LLIBS_G) $(LLIBS_R) $(LIBS),$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
<<
!ENDIF
!IF $(DEBUG)
        $(LINKER) @$(PROJ).lrf
#$(ILINK) -a -e "$(LINKER) @$(PROJ).lrf" $@
!ELSE
        $(LINKER) @$(PROJ).lrf
!ENDIF


.cpp.obj :
!IF $(DEBUG)
        $(CC) /c $(CFLAGS_G) $(CFLAGS_D) /Fo$@ $<
!ELSE
        $(CC) /c $(CFLAGS_G) $(CFLAGS_R) /Fo$@ $<
!ENDIF


run: $(PROJ).exe
        $(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
        CV $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)

clean:
       del *.obj
       del dosbeep.exe

