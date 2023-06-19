#include "./settings.hpp"

SettingsPage::SettingsPage(
	wxWindow* parent, wxWindowID ID
) : wxPanel(parent, ID) {
	SetBackgroundColour(wxColor(55, 55, 55));
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	title_panel = new wxPanel(this);
	title_panel->Bind(wxEVT_PAINT, &SettingsPage::Paint, this);
	wxBoxSizer* title_panel_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxImagePanel* title_icon = new wxImagePanel(title_panel, icons_dir+"settings.png", wxBITMAP_TYPE_PNG, 26);
	title_panel_sizer->Add(title_icon, 0, wxALL | wxALIGN_CENTER, 5);
	wxStaticText* title = new wxStaticText(title_panel, wxID_ANY, "Settings");
	title->SetForegroundColour(wxColor(100, 100, 100));
	title_panel_sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 5);
	title_panel->SetSizerAndFit(title_panel_sizer);
	sizer->Add(title_panel, 0, wxALL, 5);

	SetSizerAndFit(sizer);
}

void SettingsPage::Paint(wxPaintEvent& event) {
	wxClientDC dc(title_panel);
	wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

	if(gc) {
		gc->SetPen(wxColor(255, 255, 255));
        gc->SetBrush(wxColor(255, 255, 255));
        gc->DrawRoundedRectangle(0.0, 0.0, static_cast<double>(title_panel->GetSize().GetWidth()), static_cast<double>(title_panel->GetSize().GetHeight()), 10);
        delete gc;
	}
}