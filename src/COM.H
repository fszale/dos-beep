//-------------------------------------------------------------------------
//
//  com.h
//  Copyright 1993 Four Corner Enterprises
//
//  This file contains all the definitions and data structures for
//  the com ISR's. 
//
//  By:                     Comment:
//  ---                     --------
//  M. Shutes               created
//  01-15-93
//
//
//-------------------------------------------------------------------------

#ifndef __COM_H__
#define __COM_H__


//  This structure deines the buffer, size BUFFSIZE, used for I/O buffers
//  by the COM routines.  By using a buffer size of 256 or 32257, updating
//  the indices is simplified.
//------------------------------------------------------------------------

#ifdef BUFFSIZE
    #undef BUFFSIZE
    #define BUFFSIZE 256
#else
    #define BUFFSIZE 256
#endif

/*
#ifndef BYTE
    #define BYTE    char
#else
    #undef  BYTE
    #define BYTE    char
#endif


#ifndef BOOL
    #define BOOL    char
#else
    #undef  BOOL    char
    #define BOOL    char
#endif


#ifndef TRUE
    #define TRUE    1
    #define FALSE   0
#endif
*/

typedef struct
{
    char                Buffer[BUFFSIZE];   // Buffer area
    unsigned char       WriteIndex;         // Current write position in buffer
    unsigned char       ReadIndex;          // Current read position in buffer
    int                 High;                // High water mark in % of buffer size
    long                HMark;              // High water mark in char's
    int                 Low;                 // Low water mark in % of buffer size
    long                LMark;              // Low water mark in char's
    BOOL                Flow;                // Flow control flag for the buffer
} BUFFER;





//  This structure defines a COM port.  It is initialized when the port is
//  opened with comPortOpen().
//------------------------------------------------------------------------

typedef struct
{
    void (__interrupt __far __cdecl *oldVector)(void );
    int                 UartBase;           // base uart address
    int                 IrqMask;            // IRQ mask for com port
    int                 InterruptNumber;    // interrupt number
    BUFFER              In;                 // receive buffer
    BUFFER              Out;                // send buffer
} PORT;





//  The ifdef M_186 is checking for Microsoft C/Quick C.  Borland and
//  Microsoft differ slightly on the names of some of the DOS specific
//  procedure names, and the fixing up is done here.
//---------------------------------------------------------------------

#ifdef M_I86
    #define inport inp
    #define outport outp
    #define getvect _dos_getvect
    #define setvect _dos_setvect
    #define enable  _enable
#endif




//  The fully qualified function prototypes.  All of these routines actually
//  reside in com.c, and are part of the interrupt driven communications
//  package.
//--------------------------------------------------------------------------

PORT *comPortOpen( int Address, int InterruptNumber );
void comSetWaterMarks( PORT *Port, int High, int Low, int Buffer );
void comPortSet( PORT *Port, long Speed, unsigned char Parity, int Data, int Stopbits );
void comPortClose( PORT *Port );
int  comPutCh( unsigned char OutByte, PORT *Port );
int  comGetCh( PORT *Port);
void DTR_High();
void DTR_Low();



//  Prototypes for the polled communications package.
//---------------------------------------------------

char pollReadCh( int );
void pollSendCh( int, char );
int  pollGetStatus( int );
int  pollSetupPort( int, long, BYTE, BYTE, BYTE );
void pollClosePort( int );



//  These are the standard UART address and interrupt numbers for the two
//  IBM compatable COM ports.  Additional port information (com 3, com 4,
//  ect.. should be placed here).
//-----------------------------------------------------------------------

#define COM1_UART       0x3F8
#define COM3_UART       0x3E8
#define COM2_UART       0x2F8
#define COM4_UART       0x2E8

#define COM1_INTERRUPT  12 // the same for comm 3
#define COM2_INTERRUPT  11 // the same for comm 4




//  This group of defines creates all the definitions used to access
//  registers and bit fields inthe 8250 UART.  While the names may not
//  be the, they are the ones used on the 8250 data sheets, so they are
//  generally no changed.  Since the definitions are only used in COM.C,
//  they are not include in the com.h header file where they might
//  normally be expected.
//-----------------------------------------------------------------------

#define RBR                 0       // recieve buffer rigister
#define THR                 0       // transmit buffer register
#define IER                 1       // interrupt enable register
#define IER_RX_DATA         1       // enable RX interrupt bit
#define IER_THRE            2       // enable TX interrupt bit
#define IIR                 2       // interrupt ID register
#define IIR_MODEM_STATUS    0       // modem status interrupt id
#define IIR_TRANSMIT        2       // transmit interrupt id
#define IIR_RECEIVE         4       // receive interrupt id
#define IIR_LINE_STATUS     6       // line status interrupt id
#define LCR                 3       // line control register
#define LCR_DLAB            0x80    // divisor access bit
#define LCR_EVEN_PARITY     0x8     // set parity 'E' bits
#define LCR_ODD_PARITY      0x18    // set parity 'O' bits
#define LCR_NO_PARITY       0       // set parity 'N' bits
#define LCR_1_STOP_BIT      0       // bits to set 1 stop bit
#define LCR_2_STOP_BITS     4       // bits to set 2 stop bits
#define LCR_5_DATA_BITS     0       // bits to set 5 data bits
#define LCR_6_DATA_BITS     1       // bits to set 6 data bits
#define LCR_7_DATA_BITS     2       // bits to set 7 data bits
#define LCR_8_DATA_BITS     3       // bits to set 8 data bits
#define MCR                 4       // modem control register
#define MCR_DTR             1       // bit to turn on DTR
#define MCR_RTS             2       // bit to turn on RTS
#define MCR_OUT1            4       // bit to turn on OUT1
#define MCR_OUT2            8       // bit to turn on OUT2
#define LSR                 5       // line status register
#define MSR                 6       // modem status register
#define DLL                 0       // divisor latch LSB
#define DLM                 1       // divisor latch MSB



// Various constrants used only in the program
//--------------------------------------------

#define INT_CONTROLLER      0x20    // the address of the 8259
#define EOI                 0x20    // the end of int command
#define BREAK_VECTOR        0x23    // the CTRL-BREAK vector



//  These defines the speed, parity, stop, and databits in the polled
//  communications drivers.  Please note: values are use in conjunction
//  with int 14 calls
//---------------------------------------------------------------------

#define B_300   0x40
#define B_1200  0x80
#define B_2400  0xA0
#define B_4800  0xC0
#define B_9600  0xE0

#define P_NONE  0x00        // no parity
#define P_ODD   0x08        // odd parity
#define P_EVEN  0x10        // even parity

#define S_ONE   0x00        // one stop bit
#define S_TWO   0x04        // two stop bits

// needs hex values for 5 and 6 data bits settings

#define D_5     0x00        // 5 data bits
#define D_6     0x01        // 6 data bits
#define D_7     0x02        // 7 data bits
#define D_8     0x03        // 8 data bits



//  These are the error defines that may returned by functions in the
//  communications library.
//-------------------------------------------------------------------

#define SUCCESS     0           // Everything Okay
#define FAILURE     1           // general failure
#define RECV        2           // okay to recv byte
#define OVERRUN     3           // data overrun error
#define PARITY      4           // data parity error
#define FRAMING     5           // data framing error
#define TRANS       6           // okay to ransmit bytes
#define FIFOFULL    7           // FIFO bufer is full
#define BREAK       8           // data break recieved
#define NOPORT      9           // port address is not defined


//  These are used to define with buffer structure is be access in
//  certain non-specefic function (eg. comSetWaterMarks )
//----------------------------------------------------------------

#define SEND        50
#define RECEIVE     100

#endif
