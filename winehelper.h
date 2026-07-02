
using WINE_ORIG_ID2D1Factory          = ID2D1Factory;
using WINE_ORIG_ID2D1RenderTarget     = ID2D1RenderTarget;
using WINE_ORIG_ID2D1HwndRenderTarget = ID2D1HwndRenderTarget;

class ID2D1HwndRenderTarget_WineCompat;

class ID2D1Factory_WineCompat : public WINE_ORIG_ID2D1Factory {
public:
    HRESULT CreateHwndRenderTarget(
        D2D1_RENDER_TARGET_PROPERTIES rtProps,
        D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps,
        ID2D1HwndRenderTarget_WineCompat **rt
    ) {
        return reinterpret_cast<WINE_ORIG_ID2D1Factory*>(this)->CreateHwndRenderTarget(
            &rtProps,
            &hwndProps,
            reinterpret_cast<WINE_ORIG_ID2D1HwndRenderTarget **>(rt)
        );
    }
};

class ID2D1HwndRenderTarget_WineCompat : public WINE_ORIG_ID2D1HwndRenderTarget {
public:
    HRESULT Resize(D2D1_SIZE_U size) {
        return reinterpret_cast<WINE_ORIG_ID2D1HwndRenderTarget*>(this)->Resize(&size);
    }

    HRESULT CreateSolidColorBrush(
        D2D1_COLOR_F color,
        ID2D1SolidColorBrush **brush
    ) {
        return reinterpret_cast<WINE_ORIG_ID2D1RenderTarget*>(this)->CreateSolidColorBrush(
            &color,
            NULL,
            brush
        );
    }

    void SetTransform(D2D1_MATRIX_3X2_F transform) {
        reinterpret_cast<WINE_ORIG_ID2D1RenderTarget*>(this)->SetTransform(&transform);
    }

    void Clear(D2D1_COLOR_F color) {
        reinterpret_cast<WINE_ORIG_ID2D1RenderTarget*>(this)->Clear(&color);
    }

    void DrawRectangle(
        D2D1_RECT_F rect,
        ID2D1Brush *brush,
        FLOAT strokeWidth = 1.0f
    ) {
        reinterpret_cast<WINE_ORIG_ID2D1RenderTarget*>(this)->DrawRectangle(
            &rect,
            brush,
            strokeWidth,
            NULL
        );
    }

    HRESULT EndDraw() {
        return reinterpret_cast<WINE_ORIG_ID2D1RenderTarget*>(this)->EndDraw(NULL, NULL);
    }

    void DrawText(
        const WCHAR *text,
        UINT32 len,
        IDWriteTextFormat *fmt,
        D2D1_RECT_F rect,
        ID2D1Brush *brush
    ) {
        reinterpret_cast<WINE_ORIG_ID2D1RenderTarget*>(this)->DrawText(
            text,
            len,
            fmt,
            &rect,
            brush,
            D2D1_DRAW_TEXT_OPTIONS_NONE,
            DWRITE_MEASURING_MODE_NATURAL
        );
    }
};

#define ID2D1Factory          ID2D1Factory_WineCompat
#define ID2D1HwndRenderTarget ID2D1HwndRenderTarget_WineCompat

inline HRESULT D2D1CreateFactory(
    D2D1_FACTORY_TYPE factoryType,
    ID2D1Factory **factory
) {
    return ::D2D1CreateFactory(
        factoryType,
        __uuidof(WINE_ORIG_ID2D1Factory),
        NULL,
        reinterpret_cast<void **>(factory)
    );
}
