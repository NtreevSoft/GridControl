//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


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
	return new DeviceContext::GrGridRendererDC(pWindowHandle);
#elif __GRAPHIC_DEVICE == _GD_DIRECT2D
	return new GrGridRendererDirect2D(pWindowHandle);
#elif __GRAPHIC_DEVICE == _GD_DIRECT3D
	return new GrGridRendererDirect3D(pWindowHandle);
#else
	throw _Exception("");
#endif
}