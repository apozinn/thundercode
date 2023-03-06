#include "tabs.hpp"
#include "../codeContainer/code.hpp"

Tabs::Tabs(wxPanel* parent, wxWindowID ID) : wxScrolled<wxPanel>(parent, ID) 
{
	this->SetBackgroundColour(wxColor(55, 55, 55));
	sizer = new wxBoxSizer(wxVERTICAL);

	this->AddTab("ThunderCode", "initial_tab");
	this->SetSizerAndFit(sizer);
	tabs_ctn = this;
}

void Tabs::AddTab(wxString tab_name, wxString path) {
	if(this) {
            if(!this->IsShown()) {
                this->Show();
            }

            bool alredy_exists = false;
            for(wxWindowList::iterator it = this->GetChildren().begin(); 
                it != this->GetChildren().end(); it++) 
            {
                wxPanel* a_tab = dynamic_cast<wxPanel *>( *it );
                if(a_tab){
                    if(a_tab->GetName() == path) {
                        a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
                        alredy_exists = true;
                    } else {
                        a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
                    }
                }
            }

            if(alredy_exists) return;

            wxPanel* tab = new wxPanel(this);
            tab->SetName(path);
            wxBoxSizer* tab_sizer = new wxBoxSizer(wxVERTICAL);
            selected_tab = path;

            wxPanel* tab_props = new wxPanel(tab, wxID_ANY);
            tab_props->SetBackgroundColour(wxColor(55, 55, 55));

            wxBoxSizer* props_sizer = new wxBoxSizer(wxHORIZONTAL);

            wxStaticText* name = new wxStaticText(tab_props, wxID_ANY, tab_name);

            name->Connect(wxID_ANY, wxEVT_LEFT_UP,
                wxMouseEventHandler(Tabs::SelectTab));

            wxFont font = name->GetFont(); 
            font.SetPixelSize(wxSize(16, 16));
            name->SetFont(font);

            wxStaticText* close = new wxStaticText(tab_props, wxID_ANY, "x");
            font.SetPixelSize(wxSize(20, 20));
            close->SetFont(font);

            close->Connect(wxID_ANY, wxEVT_LEFT_UP,
                wxMouseEventHandler(Tabs::CloseTab));

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
            sizer->Add(tab, 0, wxTOP, 4);
        } 
}

void Tabs::ClearTab(wxString tab_path) {
	 auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));
        auto t_tab = this->GetGrandParent()->GetParent();
        if(t_tab) {
            for(wxWindowList::iterator it = t_tab->GetChildren().begin(); 
                it != t_tab->GetChildren().end(); it++) 
            {
                wxPanel* a_tab = dynamic_cast<wxPanel *>( *it );
                if(a_tab){
                    if(a_tab->GetName() == tab_path) {
                        if(a_tab->GetName() == selected_tab) {
                            auto prev_tab = a_tab->GetPrevSibling();
                            auto next_tab = a_tab->GetNextSibling();
                            wxString new_tab_path;

                            if(prev_tab) {
                                auto act_bar = prev_tab->GetChildren()[1];
                                if(act_bar) act_bar->SetBackgroundColour(wxColor(255, 0, 180));
                                new_tab_path = prev_tab->GetName();
                                selected_tab = prev_tab->GetName();
                            } else if(next_tab) {
                                auto act_bar = next_tab->GetChildren()[1];
                                if(act_bar) act_bar->SetBackgroundColour(wxColor(255, 0, 180));
                                new_tab_path = next_tab->GetName();
                                selected_tab = next_tab->GetName();
                            }

                            if(new_tab_path.size() && new_tab_path != "initial_tab") {
                                if(codeContainer) codeContainer->LoadNewFile(new_tab_path);
                            } else {
                                if(codeContainer) codeContainer->ClearAll();
                            }

                            a_tab->Destroy();
                            auto t_sizer = t_tab->GetSizer();
                            if(t_sizer) t_sizer->Layout();

                            if(!prev_tab && !next_tab || !t_tab->GetChildren().size()) {
                                auto code_block = ((wxPanel*)FindWindowById(ID_CODE_BLOCK));
                                if(code_block) {
                                    code_block->GetChildren()[0]->Hide();
                                    code_block->GetChildren()[1]->Hide();

                                    wxPanel* empty_window = new wxPanel(code_block, ID_EMPYT_WINDOW);
                                    empty_window->SetBackgroundColour(wxColor(55, 55, 55));

                                    wxSizer* code_block_sizer = code_block->GetSizer();
                                    if(code_block_sizer) {
                                        code_block_sizer->Add(empty_window, 1, wxEXPAND);
                                        code_block_sizer->Layout();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
}

void Tabs::ClearAllTabs() {
	auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));
	if(codeContainer) codeContainer->ClearAll();

    if(this->IsShown()) {
        this->DestroyChildren();
    } else {
        this->Show();
	}
}

void Tabs::SelectTab(wxMouseEvent& event) {
	 wxObject* obj = event.GetEventObject();
    auto this_tab = ((Tabs*)obj)->GetParent()->GetParent();

    if(this_tab) {
        auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));

        auto tabs_ctn = ((Tabs*)obj)->GetParent()->GetParent()->GetParent();
        wxString tab_path = this_tab->GetName();
        selected_tab = tab_path;

        if(tab_path.size() && tab_path != "initial_tab") {
            if(codeContainer) {
                if(!codeContainer->IsShown()) {
                    FindWindowById(ID_EMPYT_WINDOW)->Destroy();
                    codeContainer->Show();
                }
                codeContainer->LoadNewFile(tab_path);
            }
        } else {
            if(codeContainer) codeContainer->ClearAll();
        }

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

        selected_tab = this_tab->GetName();
        this_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
    }
}

void Tabs::CloseTab(wxMouseEvent& event) {
	wxObject* obj = event.GetEventObject();
    auto this_tab = ((Tabs*)obj)->GetGrandParent();
    if(this_tab) Tabs::ClearTab(this_tab->GetName());
}