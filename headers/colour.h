/////////////////////////////////////////////////////////////////////////////
// Name:        wx/colour.h
// Purpose:     wxColourBase definition
// Author:      Julian Smart
// Modified by: Francesco Montorsi
// Created:
// Copyright:   Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COLOUR_H_BASE_
#define _WX_COLOUR_H_BASE_

#include "wx/defs.h"
#include "wx/gdiobj.h"

class WXDLLIMPEXP_FWD_CORE wxColour;

// A macro to define the standard wxColour constructors:
//
// It avoids the need to repeat these lines across all colour.h files, since
// Set() is a virtual function and thus cannot be called by wxColourBase ctors
#def DEFINE_STD_WXCOLOUR_CONSTRUCTORS                                      \
    wxColour() { Init(); }                                                    \
    wxColour(ChannelType red,                                                 \
             ChannelType green,                                               \
             ChannelType blue,                                                \
             ChannelType alpha = wxALPHA_OPAQUE)                              \
        { Init(); Set(red, green, blue, alpha); }                             \
    wxColour(unsigned long colRGB) { Init(); Set(colRGB    ); }               \
    wxColour(const wxString& colourName) { Init(); Set(colourName); }         \
    wxColour(const char *colourName) { Init(); Set(colourName); }             \


// flags for wxColour -> wxString conversion (see wxColour::GetAsString)
enum {
    wxC2S_NAME             = 1,   // return colour name, when possible
    wxC2S_CSS_SYNTAX       = 2,   // return colour in rgb(r,g,b) syntax
    wxC2S_HTML_SYNTAX      = 4    // return colour in #rrggbb syntax
};

const byte wxALPHA_TRANSPARENT = 0;
const byte wxALPHA_OPAQUE = 0xff;

// a valid but fully transparent colour
#define wxTransparentColour() constructWxColour(0, 0, 0, wxALPHA_TRANSPARENT)
#define wxTransparentColor() constructWxColour(0, 0, 0, wxALPHA_TRANSPARENT)


//-----------------------------------------------------------------------------
// wxColourBase: this class has no data members, just some functions to avoid
//               code redundancy in all native wxColour implementations
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxColourBase : public
    wxObject
{
public:
    // type of a single colour component
    typedef byte ChannelType;

    wxColourBase() {}
    virtual ~wxColourBase() {}


    // Set() functions
    // ---------------

    void Set(ChannelType red,
             ChannelType green,
             ChannelType blue,
             ChannelType alpha = wxALPHA_OPAQUE)
        { InitRGBA(red, green, blue, alpha); }

    // implemented in colourcmn.cpp
    bool Set(const wxString &str)
        { return FromString(str); }

    void Set(unsigned long colRGB)
    {
        // we don't need to know sizeof(long) here because we assume that the three
        // least significant bytes contain the R, G and B values
        Set((ChannelType)(0xFF & colRGB),
            (ChannelType)(0xFF & (colRGB >> 8)),
            (ChannelType)(0xFF & (colRGB >> 16)));
    }



    // accessors
    // ---------

    virtual ChannelType Red() const = 0;
    virtual ChannelType Green() const = 0;
    virtual ChannelType Blue() const = 0;
    virtual ChannelType Alpha() const
        { return wxALPHA_OPAQUE ; }

    // implemented in colourcmn.cpp
    virtual wxString GetAsString(long flags = wxC2S_NAME | wxC2S_CSS_SYNTAX) const;

    void SetRGB(wxUint32 colRGB)
    {
        Set((ChannelType)(0xFF & colRGB),
            (ChannelType)(0xFF & (colRGB >> 8)),
            (ChannelType)(0xFF & (colRGB >> 16)));
    }

    void SetRGBA(wxUint32 colRGBA)
    {
        Set((ChannelType)(0xFF & colRGBA),
            (ChannelType)(0xFF & (colRGBA >> 8)),
            (ChannelType)(0xFF & (colRGBA >> 16)),
            (ChannelType)(0xFF & (colRGBA >> 24)));
    }

    wxUint32 GetRGB() const
        { return Red() | (Green() << 8) | (Blue() << 16); }

    wxUint32 GetRGBA() const
        { return Red() | (Green() << 8) | (Blue() << 16) | (Alpha() << 24); }

    virtual bool IsOk() const= 0;

    // older version, for backwards compatibility only (but not deprecated
    // because it's still widely used)
    bool Ok() const { return IsOk(); }

    // manipulation
    // ------------

    // These methods are static because they are mostly used
    // within tight loops (where we don't want to instantiate wxColour's)

    static void          MakeMono    (byte* r, byte* g, byte* b, bool on);
    static void          MakeDisabled(byte* r, byte* g, byte* b, byte brightness = 255);
    static void          MakeGrey    (byte* r, byte* g, byte* b); // integer version
    static void          MakeGrey    (byte* r, byte* g, byte* b,
                                      double weight_r, double weight_g, double weight_b); // floating point version
    static byte AlphaBlend  (byte fg, byte bg, double alpha);
    static void          ChangeLightness(byte* r, byte* g, byte* b, int ialpha);

    wxColour ChangeLightness(int ialpha) const;
    wxColour& MakeDisabled(byte brightness = 255);

    // old, deprecated
    // ---------------


protected:
    // Some ports need Init() and while we don't, provide a stub so that the
    // ports which don't need it are not forced to define it
    void Init() { }

    virtual void
    InitRGBA(ChannelType r, ChannelType g, ChannelType b, ChannelType a) = 0;

    virtual bool FromString(const wxString& s);

};

class WXDLLIMPEXP_CORE wxColour : public wxColourBase
{
public:
    // constructors
    // ------------
    DEFINE_STD_WXCOLOUR_CONSTRUCTORS

    // accessors
    // ---------

    virtual bool IsOk() const { return m_isInit; }

    byte Red() const { return m_red; }
    byte Green() const { return m_green; }
    byte Blue() const { return m_blue; }
    byte Alpha() const { return m_alpha ; }

    // comparison
    bool operator==(const wxColour& colour) const
    {
        return m_isInit == colour.m_isInit
            && m_red == colour.m_red
            && m_green == colour.m_green
            && m_blue == colour.m_blue
            && m_alpha == colour.m_alpha;
    }

    bool operator!=(const wxColour& colour) const { return !(*this == colour); }

protected:
    // Helper function
    void Init();

    virtual void
    InitRGBA(byte r, byte g, byte b, byte a);

private:
    bool          m_isInit;
    byte m_red;
    byte m_blue;
    byte m_green;
    byte m_alpha;

private:
    DECLARE_DYNAMIC_CLASS(wxColour)
};

// wxColour <-> wxString utilities, used by wxConfig, defined in colourcmn.cpp
WXDLLIMPEXP_CORE wxString wxToString(const wxColourBase& col);
WXDLLIMPEXP_CORE bool wxFromString(const wxString& str, wxColourBase* col);

typedef wxColour wxColor;

#endif // _WX_COLOUR_H_BASE_
