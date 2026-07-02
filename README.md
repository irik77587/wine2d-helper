# wine2d-helper

A small compatibility helper for building Microsoft-style Direct2D sample code with **Wine / winegcc**.

This project is aimed at cases where Direct2D sample code written for the Windows SDK does not compile cleanly under Wine because Wine’s headers expose lower-level COM signatures instead of the convenience overloads used by many Microsoft examples.

## Overview

`wine2d-helper` provides a thin compatibility layer that lets sample-style Direct2D code compile under Wine with little or no change to the actual sample source.

It is currently focused on the common patterns used in simple Direct2D “Hello World” style examples based on:

* `ID2D1Factory`
* `ID2D1HwndRenderTarget`
* `IDWriteFactory`
* `IDWriteTextFormat`

## File layout

The project is split into two headers:

### `main2d.h`

Public include wrapper for the sample source.

It:

* includes the normal Windows / Direct2D / DirectWrite headers
* applies small compatibility fixes needed before `d2d1helper.h`
* includes `winehelper.h` only when building under Wine

In other words, your sample source should include **only** `main2d.h`.

### `winehelper.h`

Wine-specific compatibility layer.

It provides the overload-based compatibility helpers used to make Microsoft-style sample calls work under Wine, such as forwarding value-style arguments to Wine’s pointer-based Direct2D COM methods.

This file is only used when `__WINE__` is defined.

## What problem it solves

Microsoft Direct2D samples often use convenience-style calls like:

```cpp id="3w0t0j"
D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
```

```cpp id="l9ghr8"
pFactory->CreateHwndRenderTarget(rtProps, hwndProps, &pRT);
```

```cpp id="z6qf7j"
pRT->Resize(D2D1::SizeU(width, height));
pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
pRT->DrawRectangle(D2D1::RectF(...), pBrush, 1.0f);
pRT->EndDraw();
pRT->DrawText(text, len, pTextFormat, layoutRect, pBrush);
```

Under Wine, the same interfaces may expect lower-level forms such as:

* pointers instead of by-value structs
* explicit optional parameters
* full argument lists instead of convenience overloads

`wine2d-helper` adds a compatibility layer for those sample-style calls.

## How it works

When building under Wine, `winehelper.h`:

* preserves the original Wine Direct2D interface types
* defines compatibility classes that inherit from those interfaces
* adds extra overloads matching the style used by Microsoft samples
* forwards those overloads to the underlying Wine COM methods

This allows sample code to keep using the familiar Direct2D helper style while still calling Wine’s underlying interfaces correctly.

## Usage

In your Direct2D sample source, include:

```cpp id="obm8s1"
#include "main2d.h"
```

instead of manually including the Direct2D / DirectWrite headers yourself.

Then build with `winegcc`.

Example:

```bash id="rv8u4e"
winegcc -I. -o Hello2d main2d.cpp -ld2d1 -ldwrite
```

If your headers are under a separate include directory, use the appropriate include path instead.

## Intended scope

This project is meant for **small Direct2D sample programs**, especially code based on Microsoft’s introductory Direct2D examples.

It is **not** intended to be:

* a full Direct2D portability layer
* a general-purpose replacement for the Windows SDK
* a guarantee that arbitrary Direct2D code will compile unchanged under Wine

If a sample uses additional Direct2D interfaces or more advanced APIs, more compatibility overloads may need to be added.

## Notes

* This project is a compatibility shim, not a replacement for Wine’s Direct2D implementation.
* The focus is on making sample-style code easier to compile under Wine.
* The helper is intentionally small and targeted rather than comprehensive.
* Future Wine header changes may require updates to the compatibility layer.

## License

Add your preferred license here (for example, MIT).
