#pragma once

class StatusBar : public wxPanel {
	wxBoxSizer* sizer;
public:
	StatusBar(wxFrame* parent, wxWindowID ID);
};