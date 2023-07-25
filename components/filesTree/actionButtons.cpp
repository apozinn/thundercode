class ActionButtons : public wxPanel {
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
public:
	ActionButtons(wxWindow* parent) : wxPanel(parent, ID_ACTION_BUTTONS) {
		SetBackgroundColour(wxColor(45, 45, 45));

	    wxStaticBitmap* pjt_arrow = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"play.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(pjt_arrow, 1, wxEXPAND);

	    wxStaticBitmap* pjt_arrow2 = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"bug.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(pjt_arrow2, 1, wxEXPAND);

	    wxStaticBitmap* pjt_arrow3 = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"layout.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(pjt_arrow3, 1, wxEXPAND);

	    wxStaticBitmap* pjt_arrow4 = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"actionBtn_code.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(pjt_arrow4, 1, wxEXPAND);

	    wxStaticBitmap* pjt_arrow5 = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"terminal.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(pjt_arrow5, 1, wxEXPAND);

	    wxStaticBitmap* pjt_arrow6 = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"collapse.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(pjt_arrow6, 1, wxEXPAND);

	    wxStaticBitmap* pjt_arrow7 = new wxStaticBitmap(
	    	this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(icons_dir+"action_icon.png", wxBITMAP_TYPE_PNG))
	    );
	    sizer->Add(pjt_arrow7, 1, wxEXPAND);

		SetSizerAndFit(sizer);
	}
};