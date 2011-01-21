/*
 * drivers/lf1000/mlc/mlc_hal.h
 *
 * LF1000 Multi-Layer Controller (MLC) Driver
 *
 * Andrey Yurovsky <andrey@cozybit.com> */

#ifndef MLC_HAL_H
#define MLC_HAL_H

/* MLC Registers as offsets */

#define MLCCONTROLT		0x00
#define MLCSCREENSIZE		0x04
#define MLCBGCOLOR		0x08
#ifdef CPU_LF1000
	#define MLCLEFTRIGHT0	0x0C
	#define MLCTOPBOTTOM0	0x10
	#define MLCLEFTRIGHT0_0	0x14
	#define MLCTOPBOTTOM0_0	0x18
	#define MLCLEFTRIGHT0_1	0x1C
	#define MLCTOPBOTTOM0_1 0x20
	#define MLCCONTROL0	0x24
	#define MLCHSTRIDE0	0x28
	#define MLCVSTRIDE0	0x2C
	#define MLCTPCOLOR0	0x30
	#define MLCINVCOLOR0	0x34
	#define MLCADDRESS0	0x38
	#define MLCPALETTE0	0x3C
	#define MLCLEFTRIGHT1	0x40
	#define MLCTOPBOTTOM1	0x44
	#define MLCLEFTRIGHT1_0	0x48
	#define MLCTOPBOTTOM1_0	0x4C
	#define MLCLEFTRIGHT1_1	0x50
	#define MLCTOPBOTTOM1_1	0x54
	#define MLCCONTROL1	0x58
	#define MLCHSTRIDE1	0x5C
	#define MLCVSTRIDE1	0x60
	#define MLCTPCOLOR1	0x64
	#define MLCINVCOLOR1	0x68
	#define MLCADDRESS1	0x6C
	#define MLCPALETTE1	0x70
	#define MLCLEFTRIGHT2	0x74
	#define MLCTOPBOTTOM2	0x78
	#define MLCCONTROL2	0x7C
	#define MLCVSTRIDE3	0x80	/* this is actually for Layer 2 */
	#define MLCTPCOLOR3	0x84	/* this is actually for Layer 2 */
	/*reserved		0x88*/
	#define MLCADDRESS3	0x8C	/* this is actually for Layer 2 */
	#define MLCADDRESSCB	0x90
	#define MLCADDRESSCR	0x94
	#define MLCSTRIDECB	0x98
	#define MLCSTRIDECR	0x9C
	#define MLCHSCALE	0xA0
	#define MLCVSCALE	0xA4
	#define MLCLUENH	0xA8
	#define MLCCHENH0	0xAC
	#define MLCCHENH1	0xB0
	#define MLCCHENH2	0xB4
	#define MLCCHENH3	0xB8
	/* (reserved) */
	#define MLCCLKENB	0x3C0
#elif defined(CPU_MF2530F)
	#define MLCLEFTOP0	0x0C
	#define MLCRIGHTBOTTOM0	0x10
	#define MLCCONTROL0	0x14
	#define MLCHSTRIDE0	0x18
	#define MLCVSTRIDE0	0x1C
	#define MLCTPCOLOR0	0x20
	#define MLCINVCOLOR0	0x24
	#define MLCADDRESS0	0x28
	#define MLCLEFTOP1	0x2C
	#define MLCRIGHTBOTTOM1	0x30
	#define MLCCONTROL1	0x34
	#define MLCHSTRIDE1	0x38
	#define MLCVSTRIDE1	0x3C
	#define MLCTPCOLOR1	0x40
	#define MLCINVCOLOR1	0x44
	#define MLCADDRESS1	0x48
	#define MLCLEFTOP2	0x4C
	#define MLCRIGHTBOTTOM2	0x50
	#define MLCCONTROL2	0x54
	#define MLCHSTRIDE2	0x58
	#define MLCVSTRIDE2	0x5C
	#define MLCTPCOLOR2	0x60
	#define MLCINVCOLOR2	0x64
	#define MLCADDRESS2	0x68
	#define MLCLEFTOP3	0x6C
	#define MLCRIGHTBOTTOM3	0x70
	#define MLCCONTROL3	0x74
	#define MLCHSTRIDE3	0x78
	#define MLCVSTRIDE3	0x7C
	#define MLCTPCOLOR3	0x80
	#define MLCADDRESS3	0x88
	#define MLCADDRESSCB	0x8C
	#define MLCADDRESSCR	0x90
	#define MLCVSTRIDECB	0x94
	#define MLCVSTRIDECR	0x98
	#define MLCHSCALE	0x9C
	#define MLCVSCALE	0xA0
	#define MLCLUENH	0xA4
	#define MLCCHENH0	0xA8
	#define MLCCHENH1	0xAC
	#define MLCCHENH2	0xB0
	#define MLCCHENH3	0xB4 
	/* (reserved) */
	#define MLCCLKENB	0x1C0
#else
	#error "No CPU variant defined!"
#endif


/* MLC TOP CONTROL REGISTER (MLCCONTROLT) */
#ifdef CPU_LF1000
#define GRP3DADDRCHAN		12	/* primary or secondary address block*/
#define GEALLOWEDSEL		12	/* select (two namings) */
#define PIXELBUFFER_PWD		11	/* pixel buffer power on/off */
#define PIXELBUFFER_SLD		10	/* pixel buffer sleep mode */
#endif
#define PRIORITY		8
#define DITTYFLAG		3
#define MLCENB			1
#define FIELDENB		0

/* MLC SCREEN SIZE REGISTER (MLCSCREENSIZE) */
#define SCREENHEIGHT		16
#define SCREENWIDTH		0

/* MLC RGB Layer n Control Register (MLCCONTROLn) */
#define FORMAT			16	/* see table 21-5 */
#ifdef CPU_LF1000
#define PALETTEPWD		15	/* layer n palette table on/off */
#define PALETTESLD		14	/* layer n palette table sleep mode */
#endif
#define LOCKSIZE		12	/* memory read size */
#define GRP3DENB		8	/* set layer as output of 3D core */
#define LAYERENB		5	/* enable the layer */
#define DIRTYFLAG		4
#define BLENDENB		2
#define INVENB			1
#define TPENB			0

#ifdef CPU_MF2530F
/* MLC RGB LAYER n LEFT TOP REGISTER (MLCLEFTOPn) */
#define TOP			16
#define LEFT			0
#endif

#ifdef CPU_LF1000
/* MLC RGB Layer n Left Right Register (MLCLEFTRIGHTn) */
#define LEFT			16
#define RIGHT			0
#endif

#ifdef CPU_MF2530F
/* MLC RGB LAYER n RIGHT BOTTOM REGISTER (MLCRIGHTBOTTOMn) */
#define RIGHT			16
#define BOTTOM			0
#endif

#ifdef CPU_LF1000
/* MLC RGB Layer n Top Bottom Register (MLCTOPBOTTOMn) */
#define TOP			16
#define BOTTOM			0
#endif

/* MLC RGB Layer n Unvalid Area 0 left right register (MLCLEFTRIGHT0_0) */
#define UNVALIDENB		28
#define UNVALIDLEFT		16
#define UNVALIDRIGHT		0

/* MLC RGB Layer n Unvalid Area 0 Top Bottom Register */
#define UNVALIDTOP		16
#define UNVALIDBOTTOM		0

/* MLC Pallete Address Data (MLCPALETTEn) Register */
#ifdef CPU_LF1000
#define PALETTEADDR		24
#define PALETTEDATA		0
#endif

/* MLC RGB LAYER n TRANSPARENCY COLOR REGISTER (MLCTPCOLORn) */
#define ALPHA			28
#define TPCOLOR			0

/* MLC RGB LAYER n INVERSION COLOR REGISTER (MLCINVCOLORn) */
#define INVCOLOR		0

/* MLC CLOCK GENERATION ENABLE REGISTER (MLCCLKENB) */
#define _PCLKMODE		3 /* FIXME: cleaner way to fix namespace? */
#define BCLKMODE		0

/* MLC Video layer Horizontal Scale (MLCHSCALE) Register */
#ifdef CPU_LF1000
#define HFILTERENB		28 /* bilinear filter enable */
#endif
#define HSCALE			0  /* horizonal scale ratio */

/* MLC Video layer Vertical Scale (MLCVSCALE) Register */
#ifdef CPU_LF1000
#define VFILTERENB		28 /* bilinear filter enable */
#endif
#define VSCALE			0  /* vertical scale ratio */

#endif

