#pragma once

class EmptyWindow : public wxPanel {
	wxBoxSizer* sizer;
public:
	EmptyWindow(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) {
		this->SetBackgroundColour(wxColor(31, 31, 31));
		sizer = new wxBoxSizer(wxVERTICAL);
		wxString logo_dir;

		if(wxFile::Exists("./assets/logo_emptyWindow.png")) logo_dir = "./assets/logo_emptyWindow.png";
	    else if(wxFile::Exists("../assets/logo_emptyWindow.png")) logo_dir = "../assets/logo_emptyWindow.png";

		wxImagePanel* logo = new wxImagePanel(this, logo_dir, wxBITMAP_TYPE_PNG, 200);
		sizer->AddStretchSpacer();
		sizer->Add(logo, 0, wxALL | wxALIGN_CENTER, 10);
		sizer->AddStretchSpacer();

		this->SetSizerAndFit(sizer);

		Bind(wxEVT_PAINT, &EmptyWindow::OnPaint, this);
	}
private:
	void OnPaint(wxPaintEvent& event) {
		auto target = ((wxWindow*)event.GetEventObject());
	    if(target) {
	        wxClientDC dc(target);
	        if(!dc.IsOk()) return;
	        dc.SetPen(wxPen(wxColor(65, 65, 65), 0.20));
	        dc.DrawLine(0 , 0, 0, target->GetSize().GetHeight());
	    }
	}
};