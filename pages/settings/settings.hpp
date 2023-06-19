#pragma once

class SettingsPage : public wxPanel {
	wxPanel* title_panel;
public:
	SettingsPage(wxWindow* parent, wxWindowID ID);
private:
	void Paint(wxPaintEvent& event);
};