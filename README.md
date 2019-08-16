# dos-beep
## Why?
Tonight I was forced to contemplate larger meaning of life (family member has dire health issues) and our purpose here on this planet, such pondering drove me to feel nostalgic. I kept thinking about past projects and the wonderful people I have met along the way, the more I thought about this the further back in time I went. That's when the idea struck me to find my very first code I ever wrote professionally and document some of its highlights here to become part of the permafrost of the Internet (a.k.a. old code on github).

## How?
Finding this code took some digging, after all it was all written back in 1993/94 time frame.  If I recall correctly, Windows 3.11 with Lanman and OS/2 were the pinnacle of technology.   So, I started to review old backups online and lo and behold I found a zip file, EUREKA! :fireworks: This is it, it was my very first commercial application I coded from scratch back in 1993/94 while I was still in college.

## What?
So, here it is in all of its glory.  Well, sort of.  The original company I worked for where I wrote the code is long gone, however, just in case someone wants to give me grief I decide to only include the manual and a subset of the actual code base.  Its old, C++ based code with MAKE files, nobody is going to use this now but back then it was something else.  

The application was a dos based utility to manage a database of subscribers and communicate with modems to send out messages to alphanumeric pagers.  Back in the day, these types of applications were used by subscribers of PageNet and Skytel (major beeper carriers of the day).  I think we even made the cover of PC Magazine back in the day.  We also had a Windows based version of this application but there I only helped to write some support libraries, DosBeep was mine from the core.  This is where I learned to program low level code, communicating with DSP chips, controlling modems (dial tone, Hello?), creating visual interface and adding mouse support in DOS and interacting with PCMCIA drivers, and so much more, all good stuff.

Let's not forget the great people that were part of this company, it was a small team but people were fantastic.  There were some characters there too that gave us lots of gossip fodder and stories to remember, many left Michigan for greener pasture (California, Texas, Chicago) and some of us remained in Michigan.  Still, it was a blast at the time, I have learned a lot from all those people and sharpened my own skills under their watch.

## Contents


#### Code Highlights
- [MAINPROC.CPP](/src/MAINPROC.CPP)
- [DOSBEEP.CPP](/src/DOSBEEP.CPP)
- More in the /src folder

#### Manual
- [BEEP03.DOC](/manual/BEEP03.DOC)
- [BEEPCONT.DOC](/manual/BEEPCONT.DOC)
- More in the /manual folder
