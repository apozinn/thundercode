#pragma once

class Terminal : public wxPanel {
public:
	Terminal(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) {
		SetBackgroundColour(wxColor(45, 45, 45));
	}
};