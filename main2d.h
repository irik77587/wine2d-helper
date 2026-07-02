#ifndef MAIN2D_H
#define MAIN2D_H

#include <windows.h>
#include <wchar.h>
#include <d2d1.h>

/*
 * Wine's d2d1helper.h may miss D2D1FORCEINLINE.
 * Define it before including d2d1helper.h if needed.
 */
#ifndef D2D1FORCEINLINE
#ifdef FORCEINLINE
#define D2D1FORCEINLINE FORCEINLINE
#else
#define D2D1FORCEINLINE inline __attribute__((always_inline))
#endif
#endif

#include <d2d1helper.h>
#include <dwrite.h>

#if defined(__WINE__)

#include "winehelper.h"

#endif /* __WINE__ */

#endif /* MAIN2D_H */
