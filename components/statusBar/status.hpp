#pragma once

class StatusBar : public wxPanel {
	wxBoxSizer* sizer;
	wxStaticText* first_comp;
	wxStaticText* second_comp;
	wxStaticText* third_comp;
public:
	StatusBar(wxFrame* parent, wxWindowID ID);
	void UpdateComps(wxString path, std::string format="text");
	void ClearLabels();
private:
	void ResizeComps();
};