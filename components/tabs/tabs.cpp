#include "tabs.hpp"
#include "../codeContainer/code.hpp"

Tabs::Tabs(wxPanel* parent, wxWindowID ID) : wxScrolled<wxPanel>(parent, ID) 
{
	this->SetBackgroundColour(wxColor(55, 55, 55));
	sizer = new wxBoxSizer(wxVERTICAL);
    this->AddTab("ThunderCode", "initial_tab");
	this->SetSizerAndFit(sizer);
    this->SetScrollRate(20, 20);
}

void Tabs::AddTab(wxString tab_name, wxString path) {
    sizer = this->GetSizer();
    if(!sizer) {
        wxBoxSizer* n_s = new wxBoxSizer(wxHORIZONTAL);
        this->SetSizerAndFit(n_s);
        sizer = n_s;
    }
    
    if(!this->IsShown()) this->Show();

    bool exists = false;
    for(auto& a_tab : this->GetChildren()) {
        if(a_tab->GetName() == path) {
            a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
            exists = true;
        } else {
            a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
        }
    }
    if(exists) return;

    wxPanel* new_tab = new wxPanel(this);
    new_tab->SetName(path);
    wxBoxSizer* new_tab_sizer = new wxBoxSizer(wxVERTICAL);

    //tab infos
    wxPanel* tab_infos = new wxPanel(new_tab);
    wxBoxSizer* tab_infos_sizer = new wxBoxSizer(wxHORIZONTAL);

    //name
    wxStaticText* name = new wxStaticText(tab_infos, wxID_ANY, tab_name);
    name->SetFont(fontWithOtherSize(name, 16));
    tab_infos_sizer->Add(name, 0, wxEXPAND | wxRIGHT, 5);

    tab_infos_sizer->Layout();

    //close icon
    wxImagePanel* close_icon = new wxImagePanel(tab_infos, icons_dir+"close.png", wxBITMAP_TYPE_PNG, 8);
    tab_infos_sizer->Add(close_icon, 0, wxEXPAND | wxTOP, 5);

    tab_infos->SetSizerAndFit(tab_infos_sizer);
    new_tab_sizer->Add(tab_infos, 1, wxEXPAND | wxTOP | wxLEFT | wxBOTTOM, 8);
    //

    //tab active bar
    wxPanel* active_bar = new wxPanel(new_tab);
    active_bar->SetBackgroundColour(wxColor(255, 0, 180));
    new_tab_sizer->Add(active_bar, 0, wxEXPAND);
    //

    new_tab->SetSizerAndFit(new_tab_sizer);
    sizer->Add(new_tab, 0);
    selected_tab = path;
    this->FitInside();
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
	if(codeContainer) {
        codeContainer->ClearAll();
        codeContainer->Hide();
    }

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

        auto this_ctn = ((Tabs*)obj)->GetParent()->GetParent()->GetParent();
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

        for(wxWindowList::iterator it = this_ctn->GetChildren().begin(); 
            it != this_ctn->GetChildren().end(); it++) 
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