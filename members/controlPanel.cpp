class ControlPanel : public wxPanel {
public:
	ControlPanel(wxFrame* parent, wxWindowID ID) : wxPanel(
		parent, ID, wxPoint(parent->GetSize().GetWidth()/2-225, 50), wxSize(450, 250)
	) {
		SetBackgroundColour(wxColor(36, 36, 36));
		SetFocus();

		wxAcceleratorEntry entries[1];
	    entries[0].Set(wxACCEL_NORMAL , WXK_ESCAPE, ID_EXIT_CONTROL_PANEL);
	    wxAcceleratorTable accel(1, entries);
	    SetAcceleratorTable(accel);
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