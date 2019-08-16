/* Header file for ACK-3D Map Editor */

#define CURRENT_SQUAREX	    161
#define CURRENT_SQUAREY	    4

#define PASS_X		    243
#define PASS_Y		    2

typedef struct {
	int	x;
	int	y;
	int	x1;
	int	y1;
} RECT;

typedef struct {
	int	BoxX;
	int	BoxY;
	char	*FileName;
	char	DoBeep;
	int	NumButtons;
	RECT	ButtonCoords[4];
} BOXES;

#define BOX_ALREADY_1_OBJECT	0
#define BOX_NEW_WARNING		1
#define BOX_MODIFIED_WARNING	2
#define BOX_SAVED		3
#define BOX_ALREADY_START_CODE	4
#define BOX_MAX_SPECIAL_CODE	5

/* Prototypes */
void SetVmode(int);
void SetPalette(unsigned char *);
char *AddExtent(char *,char *);
int mouse_installed(void);
void mouse_show_cursor(void);
void mouse_hide_cursor(void);
void mouse_read_cursor(int *,int *,int *);
void mouse_released(void);
unsigned int inkey(void);

