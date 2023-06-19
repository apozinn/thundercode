class NotifyPanel : public wxPanel {
	wxBoxSizer* sizer;
	wxPanel* content_box;
	wxPanel* interactions_box;
	wxStaticText* notify_label;
public:
	NotifyPanel(
		wxWindow* parent, 
		std::string notify_text,
		std::vector<NotifyInteractions> ntf_interactions
	) : wxPanel(
		parent, 
		ID_NOTIFY_PANEL, 
		wxPoint(parent->GetSize().GetWidth()/2-150, 10), 
		wxSize(330, 70)
	) {
		sizer = new wxBoxSizer(wxVERTICAL);

		content_box = new wxPanel(this);
		wxBoxSizer* content_box_sizer = new wxBoxSizer(wxHORIZONTAL);
		notify_label = new wxStaticText(content_box, wxID_ANY, notify_text);
		notify_label->SetFont(fontWithOtherSize(notify_label, 18));
		content_box_sizer->Add(notify_label, 1, wxEXPAND | wxALL, 5);
		content_box->SetSizerAndFit(content_box_sizer);

		interactions_box = new wxPanel(this);
		wxBoxSizer* interactions_box_sizer = new wxBoxSizer(wxHORIZONTAL);

		if(ntf_interactions.size()) {
			for(auto&& interaction : ntf_interactions) {
				wxStaticText* interaction_label = new wxStaticText(interactions_box, wxID_ANY, interaction.label);
				interaction_label->SetFont(fontWithOtherSize(interaction_label, 17));
				interaction_label->SetForegroundColour(wxColor(45, 120, 210));
				interactions_box_sizer->Add(interaction_label, 0, wxEXPAND | wxLEFT, 1);
				interaction_label->Bind(wxEVT_LEFT_UP, &NotifyPanel::InteractionSelected, this);
			}
		}
		interactions_box->SetSizerAndFit(interactions_box_sizer);

		sizer->Add(content_box, 1, wxEXPAND);
		sizer->Add(interactions_box, 0, wxALIGN_RIGHT | wxALL, 5);
		SetSizerAndFit(sizer);

		SetMinSize(wxSize(330, 70));
		SetSize(wxSize(330, 70));
		
		Bind(wxEVT_PAINT, &NotifyPanel::Paint, this);
	}
private:
	void Paint(wxPaintEvent& event) {
		wxClientDC dc(this);
		wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

		if(gc) {
			gc->SetPen(wxColor(46, 46, 46));
	        gc->SetBrush(wxColor(46, 46, 46));

	        gc->DrawRoundedRectangle(0.0, 0.0, static_cast<double>(this->GetSize().GetWidth()), static_cast<double>(this->GetSize().GetHeight()), 10);
	        delete gc;
		}
	}
	void InteractionSelected(wxMouseEvent& event) {
		Destroy();
	}
};