#include <stdio.h>
#include <dos.h>
#include "menu.h"
extern "C"
{
#include "cbtree.h"
}
#include "convert.h"
#include "wapi.h"
#include "dosbeep.h"

void mouse(int none,int ntwo,int nthree,int nfour);

void mouse(int none,int ntwo,int nthree,int nfour)
{
   Mouse_Regs.x.ax = none;
   Mouse_Regs.x.bx = ntwo;
   Mouse_Regs.x.cx = nthree;
   Mouse_Regs.x.dx = nfour;

   if(MouseExists == TRUE)
      int86(0x33, &Mouse_Regs, &Mouse_Regs);

   one = Mouse_Regs.x.ax;
   two = Mouse_Regs.x.bx;
   three = Mouse_Regs.x.cx;
   four = Mouse_Regs.x.dx;
}

