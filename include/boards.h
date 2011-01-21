#ifndef LF1000_BOARDS_H
#define LF1000_BOARDS_H

/* This is a list of board types that can be detected at runtime to deal with
 * hardware quirks. See gpio_get_board_config() for more information. */

/* LF1000 Development boards and original Form Factor (alpha) board. */
#define LF1000_BOARD_ALPHA	0x00

/* EP1 Form Factor Board */
#define LF1000_BOARD_EP1	0x03

#endif
