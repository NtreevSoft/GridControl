#pragma once

#define STRING2(x) #x
#define STRING(x) STRING2(x)

//#pragma message (__FILE__ "(" STRING(__LINE__) "): test")

//#define _WIN32_WINNT 0x0501
//#define WINVER 0x0501

// c++/cli precompiled header
#include "GridDef.h"
#include "GridResource.h"
#include "GridControl.h"
