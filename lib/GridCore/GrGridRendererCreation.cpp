#include "StdAfx.h"

#define _GD_DC			0
#define _GD_DIRECT2D	1
#define _GD_DIRECT3D	2

#define __GRAPHIC_DEVICE _GD_DC

#if __GRAPHIC_DEVICE == _GD_DC
#include "GrGridRendererDC.h"
#elif __GRAPHIC_DEVICE == _GD_DIRECT2D
#include "GrGridRendererDirect2D.h"
#elif __GRAPHIC_DEVICE == _GD_DIRECT3D
#include "GrGridRendererDirect3D.h"
#endif


GrGridRenderer* GrGridRendererCreator(void* pWindowHandle)
{
#if __GRAPHIC_DEVICE == _GD_DC
	return new GrGridRendererDC(pWindowHandle);
#elif __GRAPHIC_DEVICE == _GD_DIRECT2D
	return new GrGridRendererDirect2D(pWindowHandle);
#elif __GRAPHIC_DEVICE == _GD_DIRECT3D
	return new GrGridRendererDirect3D(pWindowHandle);
#else
	throw new std::exception();
#endif
}