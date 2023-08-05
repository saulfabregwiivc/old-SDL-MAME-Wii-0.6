//============================================================
//  
//  drawgx.c - SDL software and OpenGL implementation
//
//  Copyright (c) 1996-2007, Nicola Salmoria and the MAME Team.
//  Visit http://mamedev.org for licensing and usage restrictions.
//
//  SDLMAME by Olivier Galibert and R. Belmont
//
//============================================================

#include <gccore.h>
#include <ogcsys.h>

// standard C headers
#include <math.h>
#include <stdio.h>
#include <malloc.h>

// MAME headers
#include "osdcomm.h"
#include "render.h"
#include "rendutil.h"
#include "options.h"
#include "driver.h"
#include "ui.h"

// standard SDL headers
#include <SDL/SDL.h>

// OSD headers
#include "osdsdl.h"
#include "window.h"

// libogc's MEM_K0_TO_K1 macro causes compile-time warnings, which MAME treats as errors by default
#undef MEM_K0_TO_K1
#define MEM_K0_TO_K1(x) (void*)((u8 *)(x) + (SYS_BASE_UNCACHED - SYS_BASE_CACHED))

static unsigned char blanktex[2*16] __attribute__((aligned(32))) = {
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

//============================================================
//  DEBUGGING
//============================================================

//============================================================
//  CONSTANTS
//============================================================

//============================================================
//  MACROS
//============================================================

//#define IS_YUV_MODE(_m)				((_m) >= VIDEO_SCALE_MODE_YV12 && (_m) <= VIDEO_SCALE_MODE_YUY2X2)

//============================================================
//  TYPES
//============================================================

typedef struct _sdl_scale_mode sdl_scale_mode;

/* sdl_info is the information about SDL for the current screen */
typedef struct _sdl_info sdl_info;
struct _sdl_info
{
	INT32 				blittimer;
	UINT32				extra_flags;

#if (SDL_VERSION_ATLEAST(1,3,0))
	SDL_TextureID		texture_id;
#else
	// SDL surface
	SDL_Surface 		*sdlsurf;
	SDL_Overlay 		*yuvsurf;
#endif

	// YUV overlay
	UINT32 				*yuv_lookup;
	UINT16				*yuv_bitmap;
	
	// if we leave scaling to SDL and the underlying driver, this 
	// is the render_target_width/height to use
	
	int					hw_scale_width;
	int					hw_scale_height;
	int					last_hofs;
	int					last_vofs;
	int					old_blitwidth;
	int					old_blitheight;
	int					safe_hofs;
	int					safe_vofs;

	// shortcut to scale mode info
	
	const sdl_scale_mode		*scale_mode;
};

struct _sdl_scale_mode
{
	const char *name;
	int		is_scale;			/* Scale mode?           */
	int		is_yuv;				/* Yuv mode?             */
	int		mult_w;				/* Width multiplier      */
	int		mult_h;				/* Height multiplier     */
#if (!SDL_VERSION_ATLEAST(1,3,0))
	int		extra_flags;		/* Texture/surface flags */
#else
	int		sdl_scale_mode;		/* sdl 1.3 scale mode    */
#endif
	int		pixel_format;		/* Pixel/Overlay format  */
	void    (*yuv_blit)(UINT16 *bitmap, sdl_info *sdl, UINT8 *ptr, int pitch);
};

typedef struct _gx_tex gx_tex;
struct _gx_tex
{
	u32 size;
	u8 format;
	gx_tex *next;
	void *addr;
	void *data;
};

static GXRModeObj *vmode;
static GXTexObj texObj;
static GXTexObj blankTex;

static unsigned char *xfb[2];
static int currfb;
static unsigned char *gp_fifo;

static gx_tex *firstTex = NULL;
static gx_tex *lastTex = NULL;
static gx_tex *firstScreenTex = NULL;
static gx_tex *lastScreenTex = NULL;
static int is_inited = 0;

static lwp_t vidthread = LWP_THREAD_NULL;
static sdl_window_info *thread_window;

#define DEFAULT_FIFO_SIZE	(256*1024)

//============================================================
//  INLINES
//============================================================

//============================================================
//  PROTOTYPES
//============================================================

// core functions
static void drawgx_exit(void);
static void drawgx_attach(sdl_draw_info *info, sdl_window_info *window);
static int drawgx_window_create(sdl_window_info *window, int width, int height);
static void drawgx_window_resize(sdl_window_info *window, int width, int height);
static void drawgx_window_destroy(sdl_window_info *window);
static const render_primitive_list *drawgx_window_get_primitives(sdl_window_info *window);
static int drawgx_window_draw(sdl_window_info *window, UINT32 dc, int update);
static void drawgx_destroy_all_textures(sdl_window_info *window);
static void drawgx_window_clear(sdl_window_info *window);
static int drawgx_xy_to_render_target(sdl_window_info *window, int x, int y, int *xt, int *yt);

#if (SDL_VERSION_ATLEAST(1,3,0))
static void setup_texture(sdl_window_info *window, int tempwidth, int tempheight);
#endif

// YUV overlays

/*static void drawgx_yuv_init(sdl_info *sdl);
static void yuv_RGB_to_YV12(UINT16 *bitmap, sdl_info *sdl, UINT8 *ptr, int pitch);
static void yuv_RGB_to_YV12X2(UINT16 *bitmap, sdl_info *sdl, UINT8 *ptr, int pitch);
static void yuv_RGB_to_YUY2(UINT16 *bitmap, sdl_info *sdl, UINT8 *ptr, int pitch);
static void yuv_RGB_to_YUY2X2(UINT16 *bitmap, sdl_info *sdl, UINT8 *ptr, int pitch);*/

// Static declarations

#if (!SDL_VERSION_ATLEAST(1,3,0))
//static int shown_video_info = 0;

static const sdl_scale_mode scale_modes[] =
{
		{ "none",    0, 0, 0, 0, SDL_DOUBLEBUF, 0, 0 },
		{ "async",   0, 0, 0, 0, SDL_DOUBLEBUF | SDL_ASYNCBLIT, 0, 0 },
		{ "yv12",    1, 1, 1, 1, 0,              SDL_YV12_OVERLAY, 0 },
		{ "yv12x2",  1, 1, 2, 2, 0,              SDL_YV12_OVERLAY, 0 },
		{ "yuy2",    1, 1, 1, 1, 0,              SDL_YUY2_OVERLAY, 0 },
		{ "yuy2x2",  1, 1, 2, 1, 0,              SDL_YUY2_OVERLAY, 0 },
		{ NULL }
};
#else
static const sdl_scale_mode scale_modes[] =
{
		{ "none",    0, 0, 0, 0, SDL_TEXTURESCALEMODE_NONE, 0, 0 },
		{ "hwblit",  1, 0, 1, 1, SDL_TEXTURESCALEMODE_FAST, 0, 0 },
		{ "hwbest",  1, 0, 1, 1, SDL_TEXTURESCALEMODE_BEST, 0, 0 },
		{ "yv12",    1, 1, 1, 1, SDL_TEXTURESCALEMODE_NONE, SDL_PIXELFORMAT_YV12, yuv_RGB_to_YV12 },
		{ "yv12x2",  1, 1, 2, 2, SDL_TEXTURESCALEMODE_NONE, SDL_PIXELFORMAT_YV12, yuv_RGB_to_YV12X2 },
		{ "yuy2",    1, 1, 1, 1, SDL_TEXTURESCALEMODE_NONE, SDL_PIXELFORMAT_YUY2, yuv_RGB_to_YUY2 },
		{ "yuy2x2",  1, 1, 2, 1, SDL_TEXTURESCALEMODE_NONE, SDL_PIXELFORMAT_YUY2, yuv_RGB_to_YUY2X2 },
		{ NULL }
};
#endif

//============================================================
//  drawgx_scale_mode
//============================================================

const char *drawgx_scale_mode_str(int index)
{
	const sdl_scale_mode *sm = scale_modes;
	
	while (index>0)
	{
		if (sm->name == NULL)
			return NULL;
		index--;
		sm++;
	}
	return sm->name;
};

int drawgx_scale_mode(const char *s)
{
	const sdl_scale_mode *sm = scale_modes;
	int index;
	
	index = 0;
	while (sm->name != NULL)
	{
		if (strcmp(sm->name, s) == 0)
			return index;
		index++;
		sm++;
	}
	return -1;
}

//============================================================
//  drawgx_init
//============================================================

int drawgx_init(sdl_draw_info *callbacks)
{
	// fill in the callbacks
	callbacks->exit = drawgx_exit;
	callbacks->attach = drawgx_attach;

	if (SDL_VERSION_ATLEAST(1,3,0))
		mame_printf_verbose("Using SDL multi-window soft driver (SDL 1.3+)\n");
	else
		mame_printf_verbose("Using SDL single-window soft driver (SDL 1.2)\n");

	return 0;
}

//============================================================
//  drawgx_exit
//============================================================

static void drawgx_exit(void)
{
}

//============================================================
//  drawgx_attach
//============================================================

static void drawgx_attach(sdl_draw_info *info, sdl_window_info *window)
{
	// fill in the callbacks
	window->create = drawgx_window_create;
	window->resize = drawgx_window_resize;
	window->get_primitives = drawgx_window_get_primitives;
	window->draw = drawgx_window_draw;
	window->destroy = drawgx_window_destroy;
	window->destroy_all_textures = drawgx_destroy_all_textures;
	window->clear = drawgx_window_clear;
	window->xy_to_render_target = drawgx_xy_to_render_target;
}

//============================================================
//  drawgx_destroy_all_textures
//============================================================

static void drawgx_destroy_all_textures(sdl_window_info *window)
{
	// the video thread does this on exit
}

static void drawgx_shutdown(void)
{
	GX_AbortFrame();
	GX_Flush();

	VIDEO_Flush();
	VIDEO_WaitVSync();
	VIDEO_SetBlack(true);
}

//============================================================
//  drawgx_window_create
//============================================================

static int drawgx_window_create(sdl_window_info *window, int width, int height)
{
	sdl_info *sdl = window->dxdata;
	u32 xfbHeight;
	f32 yscale;
	Mtx44 perspective;
	Mtx GXmodelView2D;
	GXColor background = {0, 0, 0, 0xff};
	currfb = 0;
	// allocate memory for our structures
	sdl = malloc(sizeof(*sdl));
	memset(sdl, 0, sizeof(*sdl));

	window->dxdata = sdl;
	
	sdl->scale_mode = &scale_modes[window->scale_mode];

	sdl->extra_flags = (window->fullscreen ?  SDL_FULLSCREEN : SDL_RESIZABLE);

	sdl->extra_flags |= sdl->scale_mode->extra_flags;

	/*sdl->sdlsurf = SDL_SetVideoMode(width, height, 
				   0, SDL_SWSURFACE | SDL_ANYFORMAT | sdl->extra_flags);*/
	//sdl->sdlsurf = SDL_SetVideoMode(640, 480, 32, SDL_DOUBLEBUF);

	//if (!sdl->sdlsurf)
	//	return 1;

	window->width = gx_screenWidth();//sdl->sdlsurf->w;
	window->height = 480;//sdl->sdlsurf->h;

	sdl->safe_hofs = (window->width - window->width * options_get_float(mame_options(), SDLOPTVAL_SAFEAREA)) / 2;
	sdl->safe_vofs = (window->height - window->height * options_get_float(mame_options(), SDLOPTVAL_SAFEAREA)) / 2;

	/*if (sdl->scale_mode->is_yuv)
		yuv_overlay_init(window);*/

	sdl->yuv_lookup = NULL;
	sdl->blittimer = 0;

	//if (is_inited) return 0;
	//is_inited = 1;
	//drawgx_yuv_init(sdl);
	//SDL_QuitSubSystem(SDL_INIT_VIDEO);
	if (is_inited) return 0;

	is_inited = 1;

	VIDEO_Init();
	VIDEO_SetBlack(true);
	vmode = VIDEO_GetPreferredMode(NULL);

	switch (vmode->viTVMode >> 2)
	{
		case VI_PAL:
			vmode = &TVPal574IntDfScale;
			vmode->xfbHeight = 480;
			vmode->viYOrigin = (VI_MAX_HEIGHT_PAL - 480)/2;
			vmode->viHeight = 480;
			break;

		case VI_NTSC:
			break;

		default:
			break;
	}

	VIDEO_Configure(vmode);

	xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmode));
	xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmode));

	VIDEO_ClearFrameBuffer(vmode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer(vmode, xfb[1], COLOR_BLACK);

	VIDEO_SetNextFramebuffer(xfb[currfb]);

	VIDEO_Flush();
	VIDEO_WaitVSync();
	if (vmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();
	else while (VIDEO_GetNextField()) VIDEO_WaitVSync();

	gp_fifo = memalign(32, DEFAULT_FIFO_SIZE);
	memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);
	GX_Init(gp_fifo, DEFAULT_FIFO_SIZE);
	atexit(drawgx_shutdown);

	GX_SetCopyClear(background, 0x00ffffff);
 
	// other gx setup
	GX_SetViewport(0,0,vmode->fbWidth,vmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(vmode->efbHeight,vmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopySrc(0,0,vmode->fbWidth,vmode->efbHeight);
	GX_SetDispCopyDst(vmode->fbWidth,xfbHeight);
	GX_SetCopyFilter(vmode->aa,vmode->sample_pattern,GX_TRUE,vmode->vfilter);
	GX_SetFieldMode(vmode->field_rendering,((vmode->viHeight==2*vmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	if (vmode->aa)
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(xfb[currfb],GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);

	GX_SetNumChans(1);
	GX_SetNumTexGens(1);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);

	guOrtho(perspective,0,479,0,gx_screenWidth()-1,0,300);
	GX_LoadProjectionMtx(perspective, GX_ORTHOGRAPHIC);

	guMtxIdentity(GXmodelView2D);
	guMtxTransApply (GXmodelView2D, GXmodelView2D, 0.0F, 0.0F, -5.0F);
	GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);

	GX_SetViewport(0,0,vmode->fbWidth,vmode->efbHeight,0,1);
	GX_InvVtxCache();
	GX_ClearVtxDesc();
	GX_InvalidateTexAll();

	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

	GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

	VIDEO_SetBlack(false);

	GX_InitTexObj(&blankTex, blanktex, 1, 1, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);

	return 0;
}

//============================================================
//  drawgx_window_resize
//============================================================

static void drawgx_window_resize(sdl_window_info *window, int width, int height)
{
}


//============================================================
//  drawgx_window_destroy
//============================================================

static void drawgx_window_destroy(sdl_window_info *window)
{
	thread_window = NULL;
	LWP_JoinThread(vidthread, NULL);
	vidthread = LWP_THREAD_NULL;
}

//============================================================
//  drawgx_window_clear
//============================================================

static void drawgx_window_clear(sdl_window_info *window)
{
	sdl_info *sdl = window->dxdata;
	
	sdl->blittimer = 3;
}

//============================================================
//  drawgx_xy_to_render_target
//============================================================

static int drawgx_xy_to_render_target(sdl_window_info *window, int x, int y, int *xt, int *yt)
{
	sdl_info *sdl = window->dxdata;

	*xt = x - sdl->last_hofs;
	*yt = y - sdl->last_vofs;
	if (*xt<0 || *xt >= window->blitwidth)
		return 0;
	if (*yt<0 || *xt >= window->blitheight)
		return 0;
	if (!sdl->scale_mode->is_scale)
	{
		return 1;
	}
	/* Rescale */
	*xt = (*xt * sdl->hw_scale_width) / window->blitwidth;
	*yt = (*yt * sdl->hw_scale_height) / window->blitheight;
	return 1;
}

//============================================================
//  drawgx_window_get_primitives
//============================================================

static const render_primitive_list *drawgx_window_get_primitives(sdl_window_info *window)
{
	sdl_info *sdl = window->dxdata;
	
	if ((!window->fullscreen) || (video_config.switchres))
	{
		sdlwindow_blit_surface_size(window, window->width, window->height);
	}
	else
	{
		sdlwindow_blit_surface_size(window, window->monitor->center_width, window->monitor->center_height);
	}
	if (!sdl->scale_mode->is_scale)
		render_target_set_bounds(window->target, window->blitwidth, window->blitheight, sdlvideo_monitor_get_aspect(window->monitor));
	else
		render_target_set_bounds(window->target, sdl->hw_scale_width, sdl->hw_scale_height, 0);
	return render_target_get_primitives(window->target);
}

/* adapted from rendersw.c, might not work because as far as I can tell, only 
   laserdisc uses YCbCr textures, and we don't support that be default */

static u32 yuy_rgb = 0;

inline u8 clamp16_shift8(u32 x)
{
	return (((s32) x < 0) ? 0 : (x > 65535 ? 255: x >> 8));
}

inline u16 GXGetRGBA5551_YUY16(u32 *src, u32 x, u8 i)
{
	if (!(i & 1))
	{
		u32 ycc = src[x];
		u8 y = ycc;
		u8 cb = ycc >> 8;
		u8 cr = ycc >> 16;
		u32 r, g, b, common;

		common = 298 * y - 56992;
		r = (common +            409 * cr);
		g = (common - 100 * cb - 208 * cr + 91776);
		b = (common + 516 * cb - 13696);

		yuy_rgb = MAKE_RGB(clamp16_shift8(r), clamp16_shift8(g), clamp16_shift8(b)) | 0xFF;

		return (yuy_rgb >> 16) & 0x0000FFFF;
	}
	else
	{
		return yuy_rgb & 0x0000FFFF;
	}
}

/* heavily adapted from Wii64 */

inline u16 GXGetRGBA5551_RGB5A3(u16 *src, u32 x)
{
	u16 c = src[x];
	if ((c&1) != 0)		c = 0x8000|(((c>>11)&0x1F)<<10)|(((c>>6)&0x1F)<<5)|((c>>1)&0x1F);   //opaque texel
	else				c = 0x0000|(((c>>12)&0xF)<<8)|(((c>>7)&0xF)<<4)|((c>>2)&0xF);   //transparent texel
	return (u32) c;
}

inline u16 GXGetRGBA8888_RGBA8(u32 *src, u32 x, u8 i)
{
	u32 c = src[x];
	u32 color = (i & 1) ? /* GGBB */ c & 0x0000FFFF : /* AARR */ (c >> 16) & 0x0000FFFF;
	return (u16) color;
}

inline u16 GXGetRGBA5551_PALETTE16(u16 *src, u32 x, int i, const rgb_t *palette)
{
	u16 c = src[x];
	u32 rgb = palette[c];
	if (i == TEXFORMAT_PALETTE16) return rgb_to_rgb15(rgb) | (1 << 15);
	else return (u32)(((RGB_RED(rgb) >> 4) << 8) | ((RGB_GREEN(rgb) >> 4) << 4) | ((RGB_BLUE(rgb) >> 4) << 0) | ((RGB_ALPHA(rgb) >> 5) << 12));
}

static gx_tex *create_texture(render_primitive *prim)
{
	int j, k, l, x, y, tx, ty, bpp;
	int flag = PRIMFLAG_GET_TEXFORMAT(prim->flags);
	int rawwidth = prim->texture.width;
	int rawheight = prim->texture.height;
	int width = ((rawwidth + 3) & (~3));
	int height = ((rawheight + 3) & (~3));
	u8 *data = prim->texture.base;
	u8 *src;
	u16 *fixed;
	gx_tex *newTex = malloc(sizeof(*newTex));

	memset(newTex, 0, sizeof(*newTex));

	j = 0;

	switch(flag)
	{
	case TEXFORMAT_ARGB32:
	case TEXFORMAT_RGB32:
		newTex->format = GX_TF_RGBA8;
		bpp = 4;
		fixed = (newTex->data) ? newTex->data : memalign(32, height * width * bpp);
		for (y = 0; y < height; y+=4)
		{
			for (x = 0; x < width; x+=4)
			{
				for (k = 0; k < 4; k++)
				{
					ty = y + k;
					src = &data[bpp * prim->texture.rowpixels * ty];
					for (l = 0; l < 4; l++)
					{
						tx = x + l;
						if (ty >= rawheight || tx >= rawwidth)
						{
							fixed[j] = 0x0000;
							fixed[j+16] = 0x0000;
						}
						else
						{
							fixed[j] =    GXGetRGBA8888_RGBA8((u32*) src, tx, 0);
							fixed[j+16] = GXGetRGBA8888_RGBA8((u32*) src, tx, 1);
						}
						j++;
					}
				}
				j += 16;
			}
		}
		break;
	case TEXFORMAT_PALETTE16:
	case TEXFORMAT_PALETTEA16:
		newTex->format = GX_TF_RGB5A3;
		bpp = 2;
		fixed = (newTex->data) ? newTex->data : memalign(32, height * width * bpp);
		for (y = 0; y < height; y+=4)
		{
			for (x = 0; x < width; x+=4)
			{
				for (k = 0; k < 4; k++)
				{
					ty = y + k;
					src = &data[bpp * prim->texture.rowpixels * ty];
					for (l = 0; l < 4; l++)
					{
						tx = x + l;
						if (ty >= rawheight || tx >= rawwidth)
							fixed[j++] = 0x0000;
						else
							fixed[j++] = GXGetRGBA5551_PALETTE16((u16*) src, tx, flag, prim->texture.palette);
					}
				}
			}
		}
		break;
	case TEXFORMAT_RGB15:
		newTex->format = GX_TF_RGB5A3;
		bpp = 2;
		fixed = (newTex->data) ? newTex->data : memalign(32, height * width * bpp);
		for (y = 0; y < height; y+=4)
		{
			for (x = 0; x < width; x+=4)
			{
				for (k = 0; k < 4; k++)
				{
					ty = y + k;
					src = &data[bpp * prim->texture.rowpixels * ty];
					for (l = 0; l < 4; l++)
					{
						tx = x + l;
						if (ty >= rawheight || tx >= rawwidth)
							fixed[j++] = 0x0000;
						else
							fixed[j++] = GXGetRGBA5551_RGB5A3((u16*) src, tx);
					}
				}
			}
		}
		break;
	case TEXFORMAT_YUY16:
		newTex->format = GX_TF_RGBA8;
		bpp = 4;
		fixed = (newTex->data) ? newTex->data : memalign(32, height * width * bpp);
		for (y = 0; y < height; y+=4)
		{
			for (x = 0; x < width; x+=4)
			{
				for (k = 0; k < 4; k++)
				{
					ty = y + k;
					src = &data[bpp * prim->texture.rowpixels * ty];
					for (l = 0; l < 4; l++)
					{
						tx = x + l;
						if (ty >= rawheight || tx >= rawwidth)
						{
							fixed[j] = 0x0000;
							fixed[j+16] = 0x0000;
						}
						else
						{
							fixed[j] =    GXGetRGBA5551_YUY16((u32*) src, tx, 0);
							fixed[j+16] = GXGetRGBA5551_YUY16((u32*) src, tx, 1);
						}
						j++;
					}
				}
				j += 16;
			}
		}
		break;
	default:
		return NULL;
	}

	newTex->size = height * width * bpp;
	newTex->data = fixed;
	newTex->addr = &(*data);

	if (PRIMFLAG_GET_SCREENTEX(prim->flags))
	{
		if (firstScreenTex == NULL)
			firstScreenTex = newTex;
		else
			lastScreenTex->next = newTex;

		lastScreenTex = newTex;
	}
	else
	{
		if (firstTex == NULL)
			firstTex = newTex;
		else
			lastTex->next = newTex;

		lastTex = newTex;
	}

	return newTex;
}

static gx_tex *get_texture(render_primitive *prim)
{
	gx_tex *t = firstTex;
	
	if (PRIMFLAG_GET_SCREENTEX(prim->flags))
		return create_texture(prim);

	while (t != NULL)
		if (t->addr == prim->texture.base)
			return t;
		else
			t = t->next;

	return create_texture(prim);
}

static void prep_texture(render_primitive *prim)
{
	gx_tex *newTex = get_texture(prim);

	if (newTex == NULL)
		return;

	DCFlushRange(newTex->data, newTex->size);
	GX_InitTexObj(&texObj, newTex->data, prim->texture.width, prim->texture.height, newTex->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
	GX_LoadTexObj(&texObj, GX_TEXMAP0);
}

static void clearTexs()
{
	gx_tex *t = firstTex;
	gx_tex *n;
	
	while (t != NULL)
	{
		n = t->next;
		free(t->data);
		free(t);
		t = n;
	}
	
	firstTex = NULL;
	lastTex = NULL;
}

static void clearScreenTexs()
{
	gx_tex *t = firstScreenTex;
	gx_tex *n;
	
	while (t != NULL)
	{
		n = t->next;
		free(t->data);
		free(t);
		t = n;
	}
	
	firstScreenTex = NULL;
	lastScreenTex = NULL;
}

//============================================================
//  drawgx_window_draw
//============================================================

static void *draw_thread()
{
	sdl_info *sdl;
	render_primitive *prim;
	INT32 vofs, hofs, blitwidth, blitheight, ch, cw;

	while (1)
	{
		if (thread_window == NULL) break;

		sdl = thread_window->dxdata;

		if (video_config.novideo)
		{
			return NULL;
		}

		// if we haven't been created, just punt
		if (sdl == NULL)
			return NULL;

		vofs = hofs = 0;
		blitwidth = thread_window->blitwidth;
		blitheight = thread_window->blitheight;

		// figure out what coordinate system to use for centering - in window mode it's always the
		// SDL surface size.  in fullscreen the surface covers all monitors, so center according to
		// the first one only
		if ((thread_window->fullscreen) && (!video_config.switchres))
		{
			ch = thread_window->monitor->center_height;
			cw = thread_window->monitor->center_width;
		}
		else
		{
			ch = thread_window->height;
			cw = thread_window->width;
		}

		// do not crash if the window's smaller than the blit area
		if (blitheight > ch)
		{
				blitheight = ch;
		}
		else if (video_config.centerv)
		{
			vofs = (ch - thread_window->blitheight) / 2;
		}

		if (blitwidth > cw)
		{
				blitwidth = cw;
		}
		else if (video_config.centerh)
		{
			hofs = (cw - thread_window->blitwidth) / 2;
		}

		sdl->last_hofs = hofs;
		sdl->last_vofs = vofs;

		hofs += sdl->safe_hofs;
		vofs += sdl->safe_vofs;

		osd_lock_acquire(thread_window->primlist->lock);

		for (prim = thread_window->primlist->head; prim != NULL; prim = prim->next)
		{
			u8 r, g, b, a;
			r = (u8)(255.0f * prim->color.r);
			g = (u8)(255.0f * prim->color.g);
			b = (u8)(255.0f * prim->color.b);
			a = (u8)(255.0f * prim->color.a);

			switch (PRIMFLAG_GET_BLENDMODE(prim->flags))
			{
				case BLENDMODE_NONE:
					GX_SetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
					break;
				case BLENDMODE_ALPHA:
					GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
					break;
				case BLENDMODE_RGB_MULTIPLY:
					GX_SetBlendMode(GX_BM_SUBTRACT, GX_BL_SRCCLR, GX_BL_ZERO, GX_LO_CLEAR);
					break;
				case BLENDMODE_ADD:
					GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
					break;
			}

			switch (prim->type)
			{
				case RENDER_PRIMITIVE_LINE:
					GX_LoadTexObj(&blankTex, GX_TEXMAP0);
					GX_SetLineWidth((u8)(prim->width * 16.0f), GX_TO_ZERO);
					GX_Begin(GX_LINES, GX_VTXFMT0, 2);
						GX_Position2f32(prim->bounds.x0+hofs, prim->bounds.y0+vofs);
						GX_Color4u8(r, g, b, a);
						GX_TexCoord2f32(0, 0);
						GX_Position2f32(prim->bounds.x1+hofs, prim->bounds.y1+vofs);
						GX_Color4u8(r, g, b, a);
						GX_TexCoord2f32(0, 0);
					GX_End();
					break;
				case RENDER_PRIMITIVE_QUAD:
					if (prim->texture.base != NULL)
					{
						prep_texture(prim);
						GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
							GX_Position2f32(prim->bounds.x0+hofs, prim->bounds.y0+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(prim->texcoords.tl.u, prim->texcoords.tl.v);
							GX_Position2f32(prim->bounds.x0+hofs, prim->bounds.y1+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(prim->texcoords.bl.u, prim->texcoords.bl.v);
							GX_Position2f32(prim->bounds.x1+hofs, prim->bounds.y1+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(prim->texcoords.br.u, prim->texcoords.br.v);
							GX_Position2f32(prim->bounds.x1+hofs, prim->bounds.y0+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(prim->texcoords.tr.u, prim->texcoords.tr.v);
						GX_End();
					}
					else
					{
						GX_LoadTexObj(&blankTex, GX_TEXMAP0);
						GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
							GX_Position2f32(prim->bounds.x0+hofs, prim->bounds.y0+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(0, 0);
							GX_Position2f32(prim->bounds.x0+hofs, prim->bounds.y1+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(0, 0);
							GX_Position2f32(prim->bounds.x1+hofs, prim->bounds.y1+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(0, 0);
							GX_Position2f32(prim->bounds.x1+hofs, prim->bounds.y0+vofs);
							GX_Color4u8(r, g, b, a);
							GX_TexCoord2f32(0, 0);
						GX_End();
					}
					break;
			}
		}

		osd_lock_release(thread_window->primlist->lock);

		currfb ^= 1;

		GX_DrawDone();

		GX_CopyDisp(xfb[currfb],GX_TRUE);

		VIDEO_SetNextFramebuffer(xfb[currfb]);

		VIDEO_Flush();
		VIDEO_WaitVSync();

		clearScreenTexs();
	}

	clearTexs();

	return NULL;
}

static int drawgx_window_draw(sdl_window_info *window, UINT32 dc, int update)
{
	if (vidthread == LWP_THREAD_NULL)
		LWP_CreateThread(&vidthread, draw_thread, NULL, NULL, 0, 68);

	thread_window = window;
	
	return 0;
}
