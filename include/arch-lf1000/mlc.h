/* LF1000 Multi-Layer Controller Driver
 *
 * include/asm/arch/mlc.h -- MLC configuration, driver-side API, and useful 
 * 			     macros.
 *
 * Andrey Yurovsky <andrey@cozybit.com> */
#ifndef LF1000_MLC_H
#define LF1000_MLC_H

#ifdef _LF1000_BOOTLOADER
#include <platform.h>
#else
#include <asm/arch/platform.h>
#include <linux/types.h>
#endif

/* screen resolution (also used to calculate frame buffer size) */
#ifdef CONFIG_MACH_ME_MP2530F
	#define X_RESOLUTION		800
	#define Y_RESOLUTION		480
#else
	#define X_RESOLUTION		320
	#define Y_RESOLUTION		240
#endif

/* MLC "top" layer defaults */
#ifdef CPU_MF2530F
#define DISPLAY_VID_LAYER_PRIORITY	VID_PRIORITY_FOURTH
#elif defined CPU_LF1000
#define DISPLAY_VID_LAYER_PRIORITY	VID_PRIORITY_FIRST
#endif
#define DISPLAY_VID_RGB_FORMAT		VID_RGBFMT_R5G6B5

/* 
 * frame buffer memory
 */
#define FB_BASE_ADDR		LF1000_MLC_FB_BASE
#define BYTES_PP		4
#define FB_SIZE_BASE		(X_RESOLUTION*Y_RESOLUTION*BYTES_PP)
#if (FB_SIZE_BASE % 4096) != 0
#define FB_SIZE		((FB_SIZE_BASE/4096+1)*4096)
#else
#define FB_SIZE		FB_SIZE_BASE
#endif

/* 
 * per-layer framebuffer organization for MagicEyes OpenGL support
 */
/* LF1000 YUV stride must be 4kB */
#define OGL_SIZE		0xD00000 /* 13 MB */
#define RGB_SIZE		FB_SIZE /* 300kB */
/* In reality, the YUV frame buffer must be Y_RESOLUTION*4kB.  We don't have
 * enough memory to make this happen.  Instead, we let the YUV frame buffer
 * encroach on the OGL frame buffer.  One day, we will probably have to solve
 * this problem the right way.  For now, we just make it as big as possible,
 * which is the amount of extra ram in our system minus the OGL and RGB regions.
 * The "extra ram" is 14MB, which is the total (32MB) minus what we pass to
 * linux with the mem= argument (18MB).
 */
#define SZ_1MB			(1024*1024)
#define YUV_SIZE		(14*SZ_1MB - OGL_SIZE - RGB_SIZE) /* 724kB */

#define	RGB_BASE		FB_BASE_ADDR
#define YUV_BASE		(RGB_BASE + RGB_SIZE)
#define OGL_BASE		(YUV_BASE + YUV_SIZE)


/* 
 * devfs-related definitions 
 */
#define MLC_MAJOR		251
#define MLC_LAYER_MAJOR		250

#ifdef CPU_LF1000
/* the LF1000 has three layers:
 * layer 0 -- RGB Layer (3D capable)
 * layer 1 -- RGB Layer (3D capable)
 * layer 2 -- Video Lyaer */
#define MLC_NUM_LAYERS		3
#elif defined(CPU_MF2530F)
/* the MP2530F has four layers:
 * layer 0 -- RGB Layer (3D capable)
 * layer 1 -- RGB Layer (3D capable)
 * layer 2 -- RGB Layer (3D capable)
 * layer 3 -- Video Lyaer */
#define MLC_NUM_LAYERS		4
#else
#error "No CPU variant was set!"
#endif

/* video layer number: typically this is the last layer, change it if that's
 * not the case! */
#define MLC_VIDEO_LAYER		(MLC_NUM_LAYERS-1)

/* hardware-related definitions */

enum {
	PCLKMODE_ONLYWHENCPUACCESS	= 0, /* Operate When CPU Acces */
	PCLKMODE_ALWAYS			= 1, /* Operate Always */
};

enum {   
	BCLKMODE_DISABLE		= 0, /* Bus Clock Disable */
	BCLKMODE_DYNAMIC		= 2, /* Dynamically Controlled by MLC */
	BCLKMODE_ALWAYS			= 3, /* Operate Always */
};

enum {
	VID_PRIORITY_FIRST	= 0,   /* video  > Cursor > Window > 3D */
	VID_PRIORITY_SECOND	= 1,   /* Cursor > video  > Window > 3D */
	VID_PRIORITY_THIRD	= 2,   /* Cursor > Window > video  > 3D */
#ifdef CPU_LF1000
	VID_PRIORITY_INVALID	= 3,
#else
	VID_PRIORITY_FOURTH	= 3,   /* Cursor > Window > 3D	 > video*/
	VID_PRIORITY_INVALID	= 4,
#endif
};

enum RGBFMT
{
	VID_RGBFMT_R5G6B5	= 0x4432,   /* 16bpp { R5, G6, B5 }. */
	VID_RGBFMT_B5G6R5	= 0xC432,   /* 16bpp { B5, G6, R5 }. */

	VID_RGBFMT_X1R5G5B5	= 0x4342,   /* 16bpp { X1, R5, G5, B5 }. */
	VID_RGBFMT_X1B5G5R5	= 0xC342,   /* 16bpp { X1, B5, G5, R5 }. */
	VID_RGBFMT_X4R4G4B4	= 0x4211,   /* 16bpp { X4, R4, G4, B4 }. */
	VID_RGBFMT_X4B4G4R4	= 0xC211,   /* 16bpp { X4, B4, G4, R4 }. */
	VID_RGBFMT_X8R3G3B2	= 0x4120,   /* 16bpp { X8, R3, G3, B2 }. */
	VID_RGBFMT_X8B3G3R2	= 0xC120,   /* 16bpp { X8, B3, G3, R2 }. */

	VID_RGBFMT_A1R5G5B5	= 0x3342,   /* 16bpp { A1, R5, G5, B5 }. */
	VID_RGBFMT_A1B5G5R5	= 0xB342,   /* 16bpp { A1, B5, G5, R5 }. */
	VID_RGBFMT_A4R4G4B4	= 0x2211,   /* 16bpp { A4, R4, G4, B4 }. */
	VID_RGBFMT_A4B4G4R4	= 0xA211,   /* 16bpp { A4, B4, G4, R4 }. */
	VID_RGBFMT_A8R3G3B2	= 0x1120,   /* 16bpp { A8, R3, G3, B2 }. */
	VID_RGBFMT_A8B3G3R2	= 0x9120,   /* 16bpp { A8, B3, G3, R2 }. */

	VID_RGBFMT_G8R8_G8B8	= 0x4ED3,   /* 16bpp { G8, R8, G8, B8 }. */
	VID_RGBFMT_R8G8_B8G8	= 0x4F84,   /* 16bpp { R8, G8, B8, G8 }. */
	VID_RGBFMT_G8B8_G8R8	= 0xCED3,   /* 16bpp { G8, B8, G8, R8 }. */
	VID_RGBFMT_B8G8_R8G8	= 0xCF84,   /* 16bpp { B8, G8, R8, G8 }. */
	VID_RGBFMT_X8L8		= 0x4003,   /* 16bpp { X8, L8 }. */
	VID_RGBFMT_A8L8		= 0x1003,   /* 16bpp { A8, L8 }. */
	VID_RGBFMT_L16		= 0x4554,   /* 16bpp { L16 }. */

	VID_RGBFMT_R8G8B8	= 0x4653,   /* 24bpp { R8, G8, B8 }. */
	VID_RGBFMT_B8G8R8	= 0xC653,   /* 24bpp { B8, G8, R8 }. */

	VID_RGBFMT_X8R8G8B8	= 0x4653,   /* 32bpp { X8, R8, G8, B8 }. */
	VID_RGBFMT_X8B8G8R8	= 0xC653,   /* 32bpp { X8, B8, G8, R8 }. */
	VID_RGBFMT_A8R8G8B8	= 0x0653,   /* 32bpp { A8, R8, G8, B8 }. */
	VID_RGBFMT_A8B8G8R8	= 0x8653,   /* 32bpp { A8, B8, G8, R8 }. */
};


#ifndef _LF1000_BOOTLOADER

struct mlc_layer_position {
	u32 top;
	u32 left;
	u32 right;
	u32 bottom;
};

struct mlc_screen_size {
	u32 width;
	u32 height;
};

struct mlc_overlay_size {
	u32 srcwidth;
	u32 srcheight;
	u32 dstwidth;
	u32 dstheight;
};

/** Turn the MLC on or off.
 *
 *  \param[in] en 0 to disable, not 0 to enable. */
void mlc_SetMLCEnable(u8 en);

/** Set screen resolution.
 *
 *  The screen width and height cannot exceed 4095.
 *
 *  \param[in] width Screen horizontal resolution.
 *  \param[in] height Screen vertical resolution. 
 *
 *  \retval int 0 on success, -EINVAL if width or height were not valid. */
int mlc_SetScreenSize(u32 width, u32 height);

void mlc_GetScreenSize(struct mlc_screen_size *size);

/** Set the screen background color.
 *
 *  The background color is displayed in any part of the screen that is not
 *  covered by a layer.  It is specififed in 24-bit RGB format, ie: 0xRRGGBB.
 *  This function will 'clip' the color parameter at 24 bits.  The color change
 *  is not applied until the Top Dirty Bit is set by calling 
 *  mlc_SetTopDirtyBit().
 *
 *  \param[in] color Color to set for the screen background. */
void mlc_SetBackground(u32 color);

u32 mlc_GetBackground(void);

void mlc_SetClockMode(u8 pclk, u8 bclk);
void mlc_SetFieldEnable(u8 en);
int mlc_SetLayerPriority(u32 priority);
u32 mlc_GetLayerPriority(void);
void mlc_SetTopDirtyFlag(void);

int mlc_SetLayerEnable(u8 layer, u8 en);
int mlc_SetAddress(u8 layer, u32 addr);
int mlc_SetAddressCb(u8 layer, u32 addr);
int mlc_SetAddressCr(u8 layer, u32 addr);
int mlc_GetAddress(u8 layer, int *addr);
int mlc_SetLockSize(u8 layer, u32 locksize);
int mlc_GetLockSize(u8 layer, int *locksize);

/** Set the horizonal stride for a layer.
 *
 *  The horizonal stride determines how many bytes are read by the MLC as a
 *  pixel.  Thus for 24-bit color, the horizonal stride should be set to 3 and
 *  for 8-bit color it should be set to 2.
 *
 *  \note The horizonal stride is not applicable for the video layer.
 *
 *  \param[in] layer Layer number, 0 through 2.
 *  \param[in] hstride Horizonal stride.
 *  \retval int 0 on success, -EINVAL on invalid layer number. */
int mlc_SetHStride(u8 layer, u32 hstride);

int mlc_GetHStride(u8 layer);

/** Set the vertical stride for a layer.
 *
 *  The vertical stride for an RGB layer should be set to the number of bytes
 *  per pixel (horizonal stride) multiplied by the layer's horizonal 
 *  resolution.  For the video and 3D mode layers, the vertical stride should
 *  instead be set as 4096.
 *
 *  \param[in] layer Layer number, 0 through 3.
 *  \param[in] vstride Vertical stride.
 *  \retval int 0 on success, -EINVAL on invalid layer number. */
int mlc_SetVStride(u8 layer, u32 vstride);

int mlc_GetVStride(u8 layer);

int mlc_Set3DEnable(u8 layer, u8 en);
int mlc_Get3DEnable(u8 layer, int *en);
int mlc_SetFormat(u8 layer, enum RGBFMT format);
int mlc_GetFormat(u8 layer, int *format);
int mlc_SetPosition(u8 layer, s32 top, s32 left, s32 right, s32 bottom);
int mlc_GetPosition(u8 layer, struct mlc_layer_position *p);
int mlc_SetTransparencyColor(u8 layer, u32 color);
int mlc_GetTransparencyColor(u8 layer, int *color);
int mlc_SetTransparencyAlpha(u8 layer, u8 alpha);
int mlc_GetTransparencyAlpha(u8 layer);
int mlc_SetBlendEnable(u8 layer, u8 en);
int mlc_GetBlendEnable(u8 layer, int *en);
int mlc_SetTransparencyEnable(u8 layer, u8 en);
int mlc_GetTransparencyEnable(u8 layer, int *en);
int mlc_SetInvertEnable(u8 layer, u8 en);
int mlc_GetInvertEnable(u8 layer, int *en);
int mlc_SetInvertColor(u8 layer, u32 color);
int mlc_GetInvertColor(u8 layer, int *color);
int mlc_SetDirtyFlag(u8 layer);
int mlc_GetDirtyFlag(u8 layer);

int mlc_SetOverlaySize(u8 layer, u32 srcwidth, u32 srcheight, u32 dstwidth, u32 dstheight);
int mlc_GetOverlaySize(u8 layer, struct mlc_overlay_size *psize);

#ifdef CPU_LF1000
int mlc_SetLayerInvisibleAreaEnable(u8 layer, u8 en);
int mlc_GetLayerInvisibleAreaEnable(u8 layer);
int mlc_SetLayerInvisibleArea(u8 layer, s32 top, s32 left, s32 right, s32 bottom);
int mlc_GetLayerInvisibleArea(u8 layer, struct mlc_layer_position *p);
#endif /* CPU_LF1000 */

#endif

#endif
