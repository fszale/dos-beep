PROJ = TSR
PROJFILE = $(PROJ).MAK
DEBUG = 0


PWBRMAKE  = pwbrmake
NMAKE  = nmake
LINKER  = link
ILINK  = ilink
LRF  = echo > NUL
BIND  = bind
RC  = rc
#IMPLIB  = implib
LFLAGS_G  =  /NOI /NOE  /BATCH /STACK :15000
LFLAGS_D  = /CO /FAR 
#/CPARM:1
LFLAGS_R  = /EXE /PACKC
#/CPARM:1
MAPFILE_D  = NUL
MAPFILE_R  = NUL
CC  = cl
CFLAGS_G  = /f- /W4 /AL
#CFLAGS_D  = /qc /Gi$(PROJ).mdt /Zr /Zi /Od
CFLAGS_D  = /Zi /Od /Zr
#CFLAGS_R  = /Ot /Oi /Ol /Oe /Og /Gs
CFLAGS_R  = 
LLIBS_R  = 
LLIBS_D  = 
LIBS = ttsr.lib
ASM  = masm
AFLAGS_G  = /Mx /T
AFLAGS_D  = /Zi

OS  = $(PROJ).obj

all: DBTSR.exe

.SUFFIXES: .cpp .obj

$(PROJ).obj : $(PROJ).CPP 

$(PROJ).bsc : 

DBTSR.exe : $(OS)
!IF $(DEBUG)
        $(LRF) @<<$(PROJ).lrf
$(RT_OS) $(OS: = +^
),$@,$(MAPFILE_D),$(LLIBS_G) $(LLIBS_D) $(LIBS),$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
<<
!ELSE
       $(LRF) @<<$(PROJ).lrf
$(RT_OS) $(OS: = +^
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


run: DBTSR.exe
        DBTSR.exe $(RUNFLAGS)

debug: $(PROJ).exe
        CV $(CVFLAGS) DBTSR.exe $(RUNFLAGS)

clean:
       del $(PROJ).obj
       del $(PROJ).exe

