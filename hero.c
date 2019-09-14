//
// Rodoric H.E.R.O. - Animation test
//
// Based on Roderick H.E.R.O. for Atari VCS 2600
//
// @author retroric <retroric48k@yahoo.fr>  
// @year 2015
//

#include <lib.h>


// VARIABLES (MUST BE DECLARED FIRSTHAND, OR USE FORWARD DECLARATIONS)
unsigned char HIRES_BLANK_SEXTET = 0x40;

// All Sprites are 24 lines x 12 pixels and include an attribute byte for foreground colour 
// => 24 lines x 3 bytes: first byte contains colour attribute, and the next 2 are the pixels (6 pixels per byte)
unsigned char SPRITE_Hero_Hovering_Right[][3] =  {
	{ 0x03, 0x7F, 0x70 },
	{ 0x03, 0x46, 0x40 },
	{ 0x03, 0x46, 0x40 },
	{ 0x01, 0x46, 0x4F },
	{ 0x05, 0x46, 0x70 },
	{ 0x01, 0x46, 0x70 },
	{ 0x01, 0x46, 0x7F },
	{ 0x01, 0x46, 0x4F },
	{ 0x06, 0x5E, 0x5C },
	{ 0x06, 0x5F, 0x7F },
	{ 0x06, 0x5F, 0x4F },
	{ 0x06, 0x5F, 0x4F },
	{ 0x06, 0x4F, 0x4C },
	{ 0x06, 0x4F, 0x43 },
	{ 0x07, 0x43, 0x7C },
	{ 0x07, 0x40, 0x7C },
	{ 0x07, 0x40, 0x7C },
	{ 0x07, 0x40, 0x70 },
	{ 0x07, 0x40, 0x70 },
	{ 0x07, 0x43, 0x70 },
	{ 0x07, 0x43, 0x70 },
	{ 0x07, 0x43, 0x40 },
	{ 0x07, 0x43, 0x40 },
	{ 0x07, 0x43, 0x40 }
};

unsigned char SPRITE_Hero_Hovering_Left[][3] =  {
	{ 0x03, 0x43, 0x7F },
	{ 0x03, 0x40, 0x58 },
	{ 0x03, 0x40, 0x58 },
	{ 0x01, 0x7C, 0x58 },
	{ 0x05, 0x43, 0x58 },
	{ 0x01, 0x43, 0x58 },
	{ 0x01, 0x7F, 0x58 },
	{ 0x01, 0x7C, 0x58 },
	{ 0x06, 0x4E, 0x5E },
	{ 0x06, 0x7F, 0x7E },
	{ 0x06, 0x7C, 0x7E },
	{ 0x06, 0x7C, 0x7E },
	{ 0x06, 0x4C, 0x7C },
	{ 0x06, 0x70, 0x7C },
	{ 0x07, 0x4F, 0x70 },
	{ 0x07, 0x4F, 0x40 },
	{ 0x07, 0x4F, 0x40 },
	{ 0x07, 0x43, 0x40 },
	{ 0x07, 0x43, 0x40 },
	{ 0x07, 0x43, 0x70 },
	{ 0x07, 0x43, 0x70 },
	{ 0x07, 0x40, 0x70 },
	{ 0x07, 0x40, 0x70 },
	{ 0x07, 0x40, 0x70 },
};

// ROTOR animation sprites are only one line high, and contain 3 frames
unsigned char ANIM_Hero_Rotor_Right[][3] =  {
	{ 0x03, 0x7F, 0x70 },
	{ 0x03, 0x4F, 0x40 },
	{ 0x03, 0x46, 0x40 }
};

unsigned char ANIM_Hero_Rotor_Left[][3] =  {
	{ 0x03, 0x43, 0x7F },
	{ 0x03, 0x40, 0x7C },
	{ 0x03, 0x40, 0x58 }
};

/* Start addresses of the vertical rows spanning 6 pixels in HIRES mode: 33 rows (0..32), containing 40 cells (00..39) spanning 6 pixels */
int row_start_addr[] = { 0xA000, 0xA0F0, 0xA1E0, 0xA2D0, 0xA3C0, 0xA4B0, 0xA5A0, 0xA690, 0xA780, 0xA870, 
					     0xA960, 0xAA50, 0xAB40, 0xAC30, 0xAD20, 0xAE10, 0xAF00, 0xAFF0, 0xB0E0, 0xB1D0, 
					     0xB2C0, 0xB3B0, 0xB4A0, 0xB590, 0xB680, 0xB770, 0xB860, 0xB950, 0xBA40, 0xBB30, 
					     0xBC20, 0xBD10, 0xBE00 };


						   
// FUNCTION PROTOTYPES (FORWARD DECLARATIONS)
void draw_hero_hovering_right(char xstart, char ystart, char skip_rotor);
void draw_hero_hovering_left(char xstart, char ystart, char skip_rotor);
void draw_hero_hovering_rotor(char xstart, char ystart, char xdir, char frame);
void erase_1x4cell(char xstart, char ystart);
void erase_3x4cell(char xstart, char ystart);
void erase_3x1cell(char xstart, char ystart);
void animate_hero();
void test_keys();
void play_hero();

// Declare the assembly code functions
//void cli(); // enable interrupts
//void sei(); // disable interrupts

// ============ MAIN ==============

// ========== Gestion du clavier: [OricANu pp. 338-340]
//  voir aussi p. 55, mieux expliqué !
#define NO_KEY_PRESS 0x38
#define KEYB_NORM_KEYS_LINE_COL_PATTERN		0x208		// Motif ligne/colonne dernière touche pressée
#define KEYB_SPECIAL_KEYS_LINE_COL_PATTERN	0x209		// Idem pour touches de contrôle (Shift, Ctrl, Funct)
#define KEYB_NORM_KEYS_COL_PATTERN			0x20A		// Motif colonne dernière touche pressée	
#define KEYB_NORM_KEYS_PATTERN_FOR_COL4		0x20B		// Idem pour touches de contrôle (Shift, Ctrl, Funct)
#define is_shifted() ( ((unsigned char) *0x20C) & 128)	// #FF = Majuscule pressée, #7F = minuscule pressée (bit 7 set or cleared)
#define KEYB_REPEAT_COUNTER					0x20E		// Compteur de répétition clavier



// Scan codes (source: V:\EMULATION\ORIC\OSDK\OSDK_1_5\sample\assembly\game_4kkong\vars.s)
#define KEY_LEFT  172
#define KEY_RIGHT 188
#define KEY_DOWN  180
#define KEY_UP    156
#define KEY_SPACE 132

#define KEY_RETURN 175

#define KEY_Z 170
#define KEY_X 176

#define NO_KEY 0x38 // NO KEY PRESSED


#define XDIR_RIGHT  1   // Going RIGHT (doubles as increment value for going right)
#define XDIR_LEFT  -1   // Going LEFT  (doubles as increment value for going left)
#define XMIN 0          // Min X starting cell for Sprite = 0
#define XMAX 37	        // Max X starting cell for Srite = 37 (sprite will spawn cells 37..39)
#define YMIN 0          // Min Y starting cell for Sprite = 0
#define YMAX 29	        // Max Y starting cell for Srite = 29 (sprite will spawn cells 29..32)

#define ROTOR_ANIM_FRAMES 3 // Animation frames for H.E.R.O. backpack rotor


void main() {
	//asm("brk"); // attempt to debug "crash to monitor at PC=00E5" with OSDK1.15 and OSDKCOMP=-O3
	//test_keys();
	play_hero();
}

// ======================================================================
// Test Keys: display scan code of pressed key (press SPACE to exit test)
// ======================================================================
void test_keys() {

 unsigned char *keyb_norm_key_press_addr = (unsigned char *) 0x208;
 unsigned char key      = NO_KEY; // 0x38 = NO KEY PRESSED
 unsigned char prev_key = NO_KEY; // 0x38 = NO KEY PRESSED
 
 for(; key != KEY_SPACE;) { // loop until SPACE is pressed to quit
	key = *keyb_norm_key_press_addr;
	if(key != NO_KEY) {	// 0x38 = NO KEY PRESSED
		if(key != prev_key) {
			// Do not log repeat key presses
			printf("Key pressed: %d\n", key);
			prev_key = key;
		}
	}
  }  
}

// =======================================================
// Play HERO using arrow keys, press SPACE to end game
// =======================================================
void play_hero() {
	
	char x = 10, 
		 y = 10;
	char xdir = XDIR_RIGHT; // going RIGHT
	char rotor_anim_frame = 0; // start animation frame for rotor
	
	unsigned char *keyb_norm_key_press_addr = (unsigned char *) 0x208;
	unsigned char key      = NO_KEY; // 0x38 = NO KEY PRESSED

	hires();
	
	draw_hero_hovering_right(x, y, 1);  // DRAW SPRITE at initial position

	 for(; key != KEY_SPACE;) {
		key = *keyb_norm_key_press_addr;
		if(key == KEY_RIGHT) {
			if((xdir == XDIR_RIGHT) && (x < XMAX)) { // ALREADY going RIGHT => MOVE right 1 cell IF WE CAN
				// Here we need to FIRST move the sprite before ERASING old portion to AVOID causing attribute blink
				x++;                             // MOVE: increase X position
				draw_hero_hovering_right(x, y, 1);  // REDRAW SPRITE at new position
				erase_1x4cell(x-1, y);           // ERASE SPRITE'S OLD POSITION: We ONLY need to erase the FIRST PREVIOUS column of the sprite
				//draw_hero_hovering_rotor(x, y, xdir, rotor_anim_frame); // LAST: REDRAW rotor with current animation frame
			}
			else { 	// EITHER We were previously going LEFT  => Just REVERSE current direction and redraw new sprite
					// OR we reached maximum X position, so we stay at current position, still facing right (and redrawing the sprite is not really necessary)
				xdir = XDIR_RIGHT;
				draw_hero_hovering_right(x, y, 1);
			}
		}
		else if(key == KEY_LEFT) {
			if((xdir == XDIR_LEFT) && (x > XMIN)) { // ALREADY going LEFT  => MOVE left 1 cell IF WE CAN
				// Here we can ERASE the required portion of the sprite FIRST without causing attribute blink
				erase_1x4cell(x+2, y);          // ERASE SPRITE: We ONLY need to erase the LAST column of the sprite 
				x--; 							// MOVE: decrease X position
				draw_hero_hovering_left(x, y, 1);	// REDRAW SPRITE at new position
				//draw_hero_hovering_rotor(x, y, xdir, rotor_anim_frame); // LAST: REDRAW rotor with current animation frame
			}
			else { 	// EITHER we were previously going RIGHT => Just REVERSE current direction and redraw new sprite
					// OR we reached minimum X position, so we stay at current position, still facing left (and redrawing the sprite is not really necessary)
				xdir = XDIR_LEFT;
				draw_hero_hovering_left(x, y, 1);
			}
		}
		else if( (key == KEY_UP) && (y > YMIN) ) {
			erase_3x1cell(x, y+3);             // ERASE SPRITE: We ONLY need to erase the LAST ROW of the sprite 
			y--; 							   // MOVE: decrease y position
			if(xdir == XDIR_RIGHT) {
				draw_hero_hovering_right(x, y, 1);  // REDRAW SPRITE at new position		
			}
			else {
				draw_hero_hovering_left(x, y, 1);  // REDRAW SPRITE at new position		
			}
		}
		else if( (key == KEY_DOWN) && (y < YMAX) ) {
			erase_3x1cell(x, y);             // ERASE SPRITE FIRST to avoid disgracious ghost effect. We ONLY need to erase the FIRST ROW of the sprite 
			y++; 							   // MOVE: increase y position
			if(xdir == XDIR_RIGHT) {
				draw_hero_hovering_right(x, y, 1);  // REDRAW SPRITE at new position		
			}
			else {
				draw_hero_hovering_left(x, y, 1);  // REDRAW SPRITE at new position		
			}
		}
		else {
			// IDLING: draw rotor animation frames
			//draw_hero_hovering_rotor(x, y, xdir, rotor_anim_frame);
			draw_hero_hovering_rotor(x, y, xdir, 0);
			draw_hero_hovering_rotor(x, y, xdir, 1);
			draw_hero_hovering_rotor(x, y, xdir, 2);
		}
	  // CYCLE ROTOR ANIMATION
	  rotor_anim_frame++;
	  // MUCH quicker than using the modulo operator of course
	  if(rotor_anim_frame == ROTOR_ANIM_FRAMES) rotor_anim_frame = 0; 
	}
}

// ======================================================================
// Draw HERO hovering rotor
// ======================================================================
void draw_hero_hovering_rotor(char xstart, char ystart, char xdir, char frame) {
	unsigned char* curr_addr = (unsigned char *) row_start_addr[ystart] + xstart;
	unsigned char *curr_sprite_addr;

	if(xdir == XDIR_RIGHT) {
		curr_sprite_addr = &ANIM_Hero_Rotor_Right[frame][0];
	}
	else { // XDIR_LEFT
		curr_sprite_addr = &ANIM_Hero_Rotor_Left[frame][0];
	}
	// draw rotor sprite animation frame
	*curr_addr++ = *curr_sprite_addr++;
	*curr_addr++ = *curr_sprite_addr++;
	*curr_addr =   *curr_sprite_addr;
}

// ======================================================================
// Draw HERO hovering right
// ======================================================================
void draw_hero_hovering_right(char xstart, char ystart, char skip_rotor) {
	// ATTENTION,  **toutes** les déclarations de variables
	// doivent être effectuées AVANT d'entrer la MOINDRE instruction !!!!
	
	unsigned char *curr_addr = (unsigned char *) row_start_addr[ystart] + xstart;
	unsigned char *curr_sprite_addr;
	char line = 0;

//	if(skip_rotor) {
//		// Skip displaying first line for rotor, as this will be animated separately
//		line++;
//		curr_addr += 40;
//	}
	
	// draw 3x4 cell sprite: 1 column for colour attribute, 2 columns for sprite shape, times 4 rows = 4x6 = 24 lines
	curr_sprite_addr = &SPRITE_Hero_Hovering_Right[line][0];
	for(; line < 24; line++) {
		*curr_addr++ = *curr_sprite_addr++;
		*curr_addr++ = *curr_sprite_addr++;
		*curr_addr   = *curr_sprite_addr++;
		curr_addr += 38; /* 38 = 40-2 (already incremented pointer twice): offset for start of xcell on next line */
	}
	
	// Animate rotor (play 2 last frames)
	draw_hero_hovering_rotor(xstart, ystart, XDIR_RIGHT, 1);
	draw_hero_hovering_rotor(xstart, ystart, XDIR_RIGHT, 2);
}

// ======================================================================
// Draw HERO hovering left
// ======================================================================
void draw_hero_hovering_left(char xstart, char ystart, char skip_rotor) {
	// ATTENTION, **toutes** les déclarations de variables
	// doivent être effectuées AVANT d'entrer la mOINDRE instruction !!!!
	
	unsigned char* curr_addr = (unsigned char *) row_start_addr[ystart] + xstart;
	unsigned char *curr_sprite_addr;
	char line = 0;

//	if(skip_rotor) {
//		// Skip displaying first line for rotor, as this will be animated separately
//		line++;
//		curr_addr += 40;
//	}
	
	// draw 3x4 cell sprite: 1 column for colour attribute, 2 columns for sprite shape, times 4 rows = 4x6 = 24 lines
	curr_sprite_addr = &SPRITE_Hero_Hovering_Left[line][0];
	for(; line < 24; line++) {
		*curr_addr++ = *curr_sprite_addr++;
		*curr_addr++ = *curr_sprite_addr++;
		*curr_addr   = *curr_sprite_addr++;
		curr_addr += 38; /* 38 = 40-2 (already incremented pointer twice): offset for start of xcell on next line */
	}
	// Animate rotor (play 2 last frames)
	draw_hero_hovering_rotor(xstart, ystart, XDIR_LEFT, 1);
	draw_hero_hovering_rotor(xstart, ystart, XDIR_LEFT, 2);
}

// ======================================================================
// Erase a 1x4 group of cells (6x24 pixels)
// ======================================================================
void erase_1x4cell(char xstart, char ystart) {
	unsigned char* curr_addr = (unsigned char *) row_start_addr[ystart] + xstart;
	int line = 0;
	for(; line < 24; line++) {
		*curr_addr = HIRES_BLANK_SEXTET;
		curr_addr += 40; /* offset for start of xcell on next line */
	}
}

// ======================================================================
// Erase a 3x4 group of cells (18x24 pixels)
// ======================================================================
void erase_3x4cell(char xstart, char ystart) {
	unsigned char* curr_addr = (unsigned char *) row_start_addr[ystart] + xstart;
	int line = 0;
	for(; line < 24; line++) {
		*curr_addr++ = HIRES_BLANK_SEXTET;
		*curr_addr++ = HIRES_BLANK_SEXTET;
		*curr_addr = HIRES_BLANK_SEXTET;
		curr_addr += 38; /* 38 = 40-2 (already incremented pointer twice): offset for start of xcell on next line */
	}
}

// ======================================================================
// Erase a 3x1 group of cells (18x6 pixels)
// ======================================================================
void erase_3x1cell(char xstart, char ystart) {
	unsigned char* curr_addr = (unsigned char *) row_start_addr[ystart] + xstart;
	int line = 0;
	for(; line < 6; line++) {
		*curr_addr++ = HIRES_BLANK_SEXTET;
		*curr_addr++ = HIRES_BLANK_SEXTET;
		*curr_addr = HIRES_BLANK_SEXTET;
		curr_addr += 38; /* 38 = 40-2 (already incremented pointer twice): offset for start of xcell on next line */
	}
}
