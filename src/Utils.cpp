#include "Utils.hpp"

std::string ToString(const XEvent &e)
{
    static const char *const X_EVENT_TYPE_NAMES[] = {
        "",
        "",
        "KeyPress",
        "KeyRelease",
        "ButtonPress",
        "ButtonRelease",
        "MotionNotify",
        "EnterNotify",
        "LeaveNotify",
        "FocusIn",
        "FocusOut",
        "KeymapNotify",
        "Expose",
        "GraphicsExpose",
        "NoExpose",
        "VisibilityNotify",
        "CreateNotify",
        "DestroyNotify",
        "UnmapNotify",
        "MapNotify",
        "MapRequest",
        "ReparentNotify",
        "ConfigureNotify",
        "ConfigureRequest",
        "GravityNotify",
        "ResizeRequest",
        "CirculateNotify",
        "CirculateRequest",
        "PropertyNotify",
        "SelectionClear",
        "SelectionRequest",
        "SelectionNotify",
        "ColormapNotify",
        "ClientMessage",
        "MappingNotify",
        "GeneralEvent",
    };

    if (e.type < 2 || e.type >= LASTEvent)
    {
        std::ostringstream out;
        out << "Unknown (" << e.type << ")";
        return out.str();
    }

    // 1. Compile properties we care about.
    std::vector<std::pair<std::string, std::string>> properties;
    switch (e.type)
    {
    case CreateNotify:
        properties.emplace_back(
            "window", ToString(e.xcreatewindow.window));
        properties.emplace_back(
            "parent", ToString(e.xcreatewindow.parent));
        properties.emplace_back(
            "size",
            Size<int>(e.xcreatewindow.width, e.xcreatewindow.height).ToString());
        properties.emplace_back(
            "position",
            Position<int>(e.xcreatewindow.x, e.xcreatewindow.y).ToString());
        properties.emplace_back(
            "border_width",
            ToString(e.xcreatewindow.border_width));
        properties.emplace_back(
            "override_redirect",
            ToString(static_cast<bool>(e.xcreatewindow.override_redirect)));
        break;
    case DestroyNotify:
        properties.emplace_back(
            "window", ToString(e.xdestroywindow.window));
        break;
    case MapNotify:
        properties.emplace_back(
            "window", ToString(e.xmap.window));
        properties.emplace_back(
            "event", ToString(e.xmap.event));
        properties.emplace_back(
            "override_redirect",
            ToString(static_cast<bool>(e.xmap.override_redirect)));
        break;
    case UnmapNotify:
        properties.emplace_back(
            "window", ToString(e.xunmap.window));
        properties.emplace_back(
            "event", ToString(e.xunmap.event));
        properties.emplace_back(
            "from_configure",
            ToString(static_cast<bool>(e.xunmap.from_configure)));
        break;
    case ConfigureNotify:
        properties.emplace_back(
            "window", ToString(e.xconfigure.window));
        properties.emplace_back(
            "size",
            Size<int>(e.xconfigure.width, e.xconfigure.height).ToString());
        properties.emplace_back(
            "position",
            Position<int>(e.xconfigure.x, e.xconfigure.y).ToString());
        properties.emplace_back(
            "border_width",
            ToString(e.xconfigure.border_width));
        properties.emplace_back(
            "override_redirect",
            ToString(static_cast<bool>(e.xconfigure.override_redirect)));
        break;
    case ReparentNotify:
        properties.emplace_back(
            "window", ToString(e.xreparent.window));
        properties.emplace_back(
            "parent", ToString(e.xreparent.parent));
        properties.emplace_back(
            "position",
            Position<int>(e.xreparent.x, e.xreparent.y).ToString());
        properties.emplace_back(
            "override_redirect",
            ToString(static_cast<bool>(e.xreparent.override_redirect)));
        break;
    case MapRequest:
        properties.emplace_back(
            "window", ToString(e.xmaprequest.window));
        break;
    case ConfigureRequest:
        properties.emplace_back(
            "window", ToString(e.xconfigurerequest.window));
        properties.emplace_back(
            "parent", ToString(e.xconfigurerequest.parent));
        properties.emplace_back(
            "valueMask",
            XConfigureWindowValueMaskToString(e.xconfigurerequest.value_mask));
        properties.emplace_back(
            "position",
            Position<int>(e.xconfigurerequest.x,
                          e.xconfigurerequest.y)
                .ToString());
        properties.emplace_back(
            "size",
            Size<int>(e.xconfigurerequest.width,
                      e.xconfigurerequest.height)
                .ToString());
        properties.emplace_back(
            "border_width",
            ToString(e.xconfigurerequest.border_width));
        break;
    case ButtonPress:
    case ButtonRelease:
        properties.emplace_back(
            "window", ToString(e.xbutton.window));
        properties.emplace_back(
            "button", ToString(e.xbutton.button));
        properties.emplace_back(
            "position_root",
            Position<int>(e.xbutton.x_root, e.xbutton.y_root).ToString());
        break;
    case MotionNotify:
        properties.emplace_back(
            "window", ToString(e.xmotion.window));
        properties.emplace_back(
            "position_root",
            Position<int>(e.xmotion.x_root, e.xmotion.y_root).ToString());
        properties.emplace_back(
            "state", ToString(e.xmotion.state));
        properties.emplace_back(
            "time", ToString(e.xmotion.time));
        break;
    case KeyPress:
    case KeyRelease:
        properties.emplace_back(
            "window", ToString(e.xkey.window));
        properties.emplace_back(
            "state", ToString(e.xkey.state));
        properties.emplace_back(
            "keycode", ToString(e.xkey.keycode));
        break;
    default:
        // No properties are printed for unused events.
        break;
    }

    // 2. Build final string.
    const std::string propertiesString = Join(
        properties, ", ", [](const std::pair<std::string, std::string> &pair)
        { return pair.first + ": " + pair.second; });
    std::ostringstream out;
    out << X_EVENT_TYPE_NAMES[e.type] << " { " << propertiesString << " }";
    return out.str();
}

std::string XConfigureWindowValueMaskToString(u_long valueMask)
{
    std::vector<std::string> masks;
    if (valueMask & CWX)
    {
        masks.emplace_back("X");
    }
    if (valueMask & CWY)
    {
        masks.emplace_back("Y");
    }
    if (valueMask & CWWidth)
    {
        masks.emplace_back("Width");
    }
    if (valueMask & CWHeight)
    {
        masks.emplace_back("Height");
    }
    if (valueMask & CWBorderWidth)
    {
        masks.emplace_back("BorderWidth");
    }
    if (valueMask & CWSibling)
    {
        masks.emplace_back("Sibling");
    }
    if (valueMask & CWStackMode)
    {
        masks.emplace_back("StackMode");
    }
    return Join(masks, "|");
}

std::string XRequestCodeToString(u_char requestCode)
{
    static const char *const X_REQUEST_CODE_NAMES[] = {
        "",
        "CreateWindow",
        "ChangeWindowAttributes",
        "GetWindowAttributes",
        "DestroyWindow",
        "DestroySubwindows",
        "ChangeSaveSet",
        "ReparentWindow",
        "MapWindow",
        "MapSubwindows",
        "UnmapWindow",
        "UnmapSubwindows",
        "ConfigureWindow",
        "CirculateWindow",
        "GetGeometry",
        "QueryTree",
        "InternAtom",
        "GetAtomName",
        "ChangeProperty",
        "DeleteProperty",
        "GetProperty",
        "ListProperties",
        "SetSelectionOwner",
        "GetSelectionOwner",
        "ConvertSelection",
        "SendEvent",
        "GrabPointer",
        "UngrabPointer",
        "GrabButton",
        "UngrabButton",
        "ChangeActivePointerGrab",
        "GrabKeyboard",
        "UngrabKeyboard",
        "GrabKey",
        "UngrabKey",
        "AllowEvents",
        "GrabServer",
        "UngrabServer",
        "QueryPointer",
        "GetMotionEvents",
        "TranslateCoords",
        "WarpPointer",
        "SetInputFocus",
        "GetInputFocus",
        "QueryKeymap",
        "OpenFont",
        "CloseFont",
        "QueryFont",
        "QueryTextExtents",
        "ListFonts",
        "ListFontsWithInfo",
        "SetFontPath",
        "GetFontPath",
        "CreatePixmap",
        "FreePixmap",
        "CreateGC",
        "ChangeGC",
        "CopyGC",
        "SetDashes",
        "SetClipRectangles",
        "FreeGC",
        "ClearArea",
        "CopyArea",
        "CopyPlane",
        "PolyPoint",
        "PolyLine",
        "PolySegment",
        "PolyRectangle",
        "PolyArc",
        "FillPoly",
        "PolyFillRectangle",
        "PolyFillArc",
        "PutImage",
        "GetImage",
        "PolyText8",
        "PolyText16",
        "ImageText8",
        "ImageText16",
        "CreateColormap",
        "FreeColormap",
        "CopyColormapAndFree",
        "InstallColormap",
        "UninstallColormap",
        "ListInstalledColormaps",
        "AllocColor",
        "AllocNamedColor",
        "AllocColorCells",
        "AllocColorPlanes",
        "FreeColors",
        "StoreColors",
        "StoreNamedColor",
        "QueryColors",
        "LookupColor",
        "CreateCursor",
        "CreateGlyphCursor",
        "FreeCursor",
        "RecolorCursor",
        "QueryBestSize",
        "QueryExtension",
        "ListExtensions",
        "ChangeKeyboardMapping",
        "GetKeyboardMapping",
        "ChangeKeyboardControl",
        "GetKeyboardControl",
        "Bell",
        "ChangePointerControl",
        "GetPointerControl",
        "SetScreenSaver",
        "GetScreenSaver",
        "ChangeHosts",
        "ListHosts",
        "SetAccessControl",
        "SetCloseDownMode",
        "KillClient",
        "RotateProperties",
        "ForceScreenSaver",
        "SetPointerMapping",
        "GetPointerMapping",
        "SetModifierMapping",
        "GetModifierMapping",
        "NoOperation",
    };
    
    return X_REQUEST_CODE_NAMES[requestCode];
}
