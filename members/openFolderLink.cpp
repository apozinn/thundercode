#include <wx/statline.h>

class OpenFolderLink : public wxPanel {
public:
	wxBoxSizer* sizer;
	OpenFolderLink(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) {
		sizer = new wxBoxSizer(wxVERTICAL);

		sizer->AddStretchSpacer(1);

		wxStaticText* label = new wxStaticText(this, wxID_ANY, "Open Folder");
		label->SetFont(fontWithOtherSize(label, 15));
		label->SetForegroundColour(wxColor(45, 120, 210));
		sizer->Add(label, 0, wxALIGN_CENTER);

		wxStaticLine* under_line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(67, 1), wxLI_HORIZONTAL);
		under_line->SetBackgroundColour(wxColor(45, 120, 210));
		sizer->Add(under_line, 0, wxALIGN_CENTER | wxTOP, 3);

		sizer->AddStretchSpacer(1);
		SetSizerAndFit(sizer);
	}
};