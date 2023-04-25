#pragma once
#include <wx/graphics.h>

class CustomRectangle : public wxWindow
{
    bool pressedDown;
    int width;
    int height;
    int radius;
    wxColor background;
public:
    CustomRectangle(
        wxPanel* parent, 
        int default_width, 
        int default_height,
        int default_radius,
        wxColor bg
    );
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void render(wxDC& dc);
    DECLARE_EVENT_TABLE () 
}; 

BEGIN_EVENT_TABLE(CustomRectangle, wxPanel) 
    EVT_PAINT(CustomRectangle::paintEvent)
END_EVENT_TABLE()

CustomRectangle::CustomRectangle(
    wxPanel* parent, 
    int default_width, 
    int default_height, 
    int default_radius,
    wxColor bg
) : wxWindow(parent, wxID_ANY)
{
    SetMinSize(wxSize(width, height));
    pressedDown = false;
    width = default_width;
    height = default_height;
    radius = default_radius;
    background = bg;
}

void CustomRectangle::paintEvent(wxPaintEvent & evt)
{
    #define ROUND_RECT_DETAILS (0.0, 0.0, static_cast<double>(width), static_cast<double>(height), radius)
    wxPaintDC dc(this);
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc);
    if(gc) {
        gc->SetPen(background);
        gc->SetBrush(background);
        gc->DrawRoundedRectangle ROUND_RECT_DETAILS;
        delete gc;
    }
}

void CustomRectangle::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void CustomRectangle::render(wxDC&  dc) {}