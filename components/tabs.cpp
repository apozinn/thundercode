#include <vector>
#include "../app.h"
#include "../utils/randoms.hpp"

int select_tab_id;

class Tabs : public wxPanel {
public:
	void selectTab(wxMouseEvent& event);
	void closeTab(wxMouseEvent& event);
	std::vector<std::vector<std::string>> tabs_ids;

	Tabs(wxPanel* parent) : wxPanel(parent, ID_TABS_CONTAINER) {
		wxString dir;
	    if(wxFile::Exists("./icons/settings.png")) dir = "./icons/";
	    else if(wxFile::Exists("../icons/settings.png")) dir = "../icons/";

		this->SetBackgroundColour(wxColor(55, 55, 55));	
		sizer = new wxBoxSizer(wxHORIZONTAL);

		AddTab("ThunderCode");

		this->SetSizerAndFit(sizer);
	}

	void AddTab(wxString tab_name) {
		int tab_id = randomInt(1000, 2000);
		std::string locale = "/";

		tabs_ids.push_back({std::to_string(tab_id), locale});

		wxPanel* tab = new wxPanel(this, tab_id);
		wxBoxSizer* tab_sizer = new wxBoxSizer(wxVERTICAL);

		wxPanel* tab_props = new wxPanel(tab, wxID_ANY);
		tab_props->SetBackgroundColour(wxColor(55, 55, 55));

		wxBoxSizer* props_sizer = new wxBoxSizer(wxHORIZONTAL);

		wxStaticText* name = new wxStaticText(tab_props, wxID_ANY, tab_name);

		name->Connect(wxID_ANY, wxEVT_LEFT_UP,
     		wxMouseEventHandler(Tabs::selectTab));

		wxFont font = name->GetFont(); 
		font.SetPixelSize(wxSize(16, 16));
		name->SetFont(font);

		wxStaticText* close = new wxStaticText(tab_props, wxID_ANY, "x");
		font.SetPixelSize(wxSize(20, 20));
		close->SetFont(font);

		close->Connect(wxID_ANY, wxEVT_LEFT_UP,
     		wxMouseEventHandler(Tabs::closeTab));

		props_sizer->Add(name, 1, wxALIGN_CENTER | wxLEFT, 5);
		props_sizer->Add(close, wxSizerFlags(0).Border(wxTOP, 3));

		tab_props->SetSizerAndFit(props_sizer);
		tab_sizer->Add(tab_props, 30, wxEXPAND | wxRIGHT, 5);

		wxPanel* active_bar = new wxPanel(tab, wxID_ANY);

		for(wxWindowList::iterator it = this->GetChildren().begin(); 
			it != this->GetChildren().end(); it++) 
		{
			wxPanel* a_tab = dynamic_cast<wxPanel *>( *it );
			if(a_tab){
				for(wxWindowList::iterator it = a_tab->GetChildren().begin(); 
					it != a_tab->GetChildren().end(); it++) 
				{
					wxPanel* a_tab_t = dynamic_cast<wxPanel *>( *it );
					if(a_tab_t){
						a_tab_t->SetBackgroundColour(wxColor(55, 55, 55));
				    }
				}
		    }
		}

		active_bar->SetBackgroundColour(wxColor(255, 0, 180));
		tab_sizer->Add(active_bar, 1, wxEXPAND);
		tab->SetSizerAndFit(tab_sizer);	
		sizer->Add(tab, 0, wxEXPAND | wxTOP, 4);
		select_tab_id = tab->GetId();
	}
private:
	wxPanel* tabs_container;
	wxBoxSizer* sizer;
	wxDECLARE_NO_COPY_CLASS(Tabs);
};

void Tabs::selectTab(wxMouseEvent& event)
{
    wxObject* obj = event.GetEventObject();
    auto this_tab = ((Tabs*)obj)->GetParent()->GetParent();

    if(this_tab) {
		auto tabs_ctn = ((Tabs*)obj)->GetParent()->GetParent()->GetParent();

		for(wxWindowList::iterator it = tabs_ctn->GetChildren().begin(); 
			it != tabs_ctn->GetChildren().end(); it++) 
		{
			wxPanel* a_tab = dynamic_cast<wxPanel *>( *it );
			if(a_tab){
				for(wxWindowList::iterator it = a_tab->GetChildren().begin(); 
					it != a_tab->GetChildren().end(); it++) 
				{
					wxPanel* a_tab_t = dynamic_cast<wxPanel *>( *it );
					if(a_tab_t){
						a_tab_t->SetBackgroundColour(wxColor(55, 55, 55));
				    }
				}
		    }
		}

		select_tab_id = this_tab->GetId();
		this_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
	}
}

void Tabs::closeTab(wxMouseEvent& event)
{
    wxObject* obj = event.GetEventObject();
    auto this_tab = ((Tabs*)obj)->GetParent()->GetParent();
    auto tabs_ctn = ((Tabs*)obj)->GetParent()->GetParent()->GetParent();

    if(this_tab) {
    	if(this_tab->GetId() == select_tab_id) {
    		auto prev_tab = this_tab->GetPrevSibling();
    		if(prev_tab) {
				auto act_bar = prev_tab->GetChildren()[1];
				if(act_bar) act_bar->SetBackgroundColour(wxColor(255, 0, 180));
    		}
    	}
		this_tab->Destroy();
	}

	auto this_sizer = tabs_ctn->GetSizer();
	this_sizer->Layout();
}