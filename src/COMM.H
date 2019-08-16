#define TIMER_ON   1
#define TIMER_OFF  0

#define CBR_110	   0xFF10
#define CBR_300      0xFF11
#define CBR_600      0xFF12
#define CBR_1200     0xFF13
#define CBR_2400     0xFF14
#define CBR_4800     0xFF15
#define CBR_9600     0xFF16

#define NOPARITY	       0
#define ODDPARITY	       1
#define EVENPARITY	    2
#define MARKPARITY	    3
#define SPACEPARITY	    4

#define ONESTOPBIT	    0
#define TWOSTOPBITS	    2

#define MESSAGE          1
#define NOT_A_MESSAGE    2

BOOL OpenPort( int port, long speed, 
			      char parity, int stopbits, 
			      int data_len);
BOOL ClosePort();
BOOL WriteCommChar(char ch);
BOOL WriteCommString(LPSTR string, BOOL bType);
int  ReadCommChar();
void Delay(WORD seconds);
int  ProcessComPort(BYTE byCOMMPort);
long ProcessBaudRate(DWORD dwBaud);
char ProcessParity(BYTE byParity);
int  ProcessStopBits(BYTE byStopBits);
int  ProcessInterrupt(int port);
void Drop_Dtr(unsigned char set_bit);
void Reset_Port();

