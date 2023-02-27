class wxImagePanel : public wxPanel {
	wxBitmap image;
    public:
        wxImagePanel(wxPanel* parent, wxString file, wxBitmapType format);
        
        void paintEvent(wxPaintEvent & evt);
        void paintNow();
        void render(wxDC& dc);
        
        DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
EVT_PAINT(wxImagePanel::paintEvent)
END_EVENT_TABLE()

wxImagePanel::wxImagePanel(
	wxFrame* parent, wxString file, wxBitmapType format
) : wxPanel(parent) {
    image.LoadFile(file, format);
}

void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void wxImagePanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void wxImagePanel::render(wxDC&  dc)
{
    dc.DrawBitmap( image, 0, 0, false );
}