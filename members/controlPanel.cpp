class ControlPanel : public wxPanel {
public:
	ControlPanel(wxFrame* parent, wxWindowID ID) : wxPanel(
		parent, ID, wxPoint(parent->GetSize().GetWidth()/2-225, 50), wxSize(450, 250)
	) {
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		SetMinSize(wxSize(450, 250));
		SetSize(wxSize(450, 250));
		SetBackgroundColour(wxColor(36, 36, 36));
		SetFocus();

		wxPanel* top_container = new wxPanel(this);
		wxBoxSizer* top_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);

		wxVector<wxBitmap> bitmaps;
	    bitmaps.push_back(wxBitmap(icons_dir+"search.png", wxBITMAP_TYPE_PNG));
	    wxStaticBitmap* search_icon = new wxStaticBitmap(top_container, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
	    top_ctn_sizer->Add(search_icon, 0, wxEXPAND | wxALL, 5);

	    wxTextCtrl* search_input = new wxTextCtrl(top_container, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	   	search_input->SetBackgroundColour(wxColor(36, 36, 36));
	   	search_input->SetFocus();
	    top_ctn_sizer->Add(search_input, 1, wxEXPAND);

		top_container->SetSizerAndFit(top_ctn_sizer);
		sizer->Add(top_container, 0, wxEXPAND);

		wxAcceleratorEntry entries[1];
	    entries[0].Set(wxACCEL_NORMAL , WXK_ESCAPE, ID_EXIT_CONTROL_PANEL);
	    wxAcceleratorTable accel(1, entries);
	    SetAcceleratorTable(accel);

	    SetSizerAndFit(sizer);

	    SetMinSize(wxSize(450, 250));
		SetSize(wxSize(450, 250));
	}
	void Close(wxCommandEvent& event) {
		FindWindowById(ID_STATUS_BAR)->SetFocus();
		Destroy();
	}
private:
	wxDECLARE_NO_COPY_CLASS(ControlPanel);
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(ControlPanel, wxPanel)
    EVT_MENU(ID_EXIT_CONTROL_PANEL, ControlPanel::Close)
wxEND_EVENT_TABLE()