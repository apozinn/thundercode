#include "tabs.hpp"
#include "../codeContainer/code.hpp"
#include "../../members/emptyWindow.cpp"

Tabs::Tabs(wxPanel* parent, wxWindowID ID) : wxScrolled<wxPanel>(parent, ID) 
{
	this->SetBackgroundColour(wxColor(55, 55, 55));
	sizer = new wxBoxSizer(wxVERTICAL);
    this->AddTab("ThunderCode", "initial_tab");
	this->SetSizerAndFit(sizer);
    this->SetScrollRate(20, 20);
}

void Tabs::AddTab(wxString tab_name, wxString path) {
    path = path.substr(0, path.size()-1);
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
    new_tab->Bind(wxEVT_LEFT_UP, &Tabs::SelectTab, this);
    wxBoxSizer* new_tab_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* tab_infos = new wxPanel(new_tab);
    wxBoxSizer* tab_infos_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* name = new wxStaticText(tab_infos, wxID_ANY, tab_name);
    name->SetName(path);
    name->SetFont(fontWithOtherSize(name, 16));
    name->Bind(wxEVT_LEFT_UP, &Tabs::SelectTab, this);
    tab_infos_sizer->Add(name, 0, wxEXPAND | wxRIGHT, 5);

    wxImagePanel* close_icon = new wxImagePanel(tab_infos, icons_dir+"close.png", wxBITMAP_TYPE_PNG, 8);
    close_icon->Bind(wxEVT_LEFT_UP, &Tabs::CloseTab, this);
    tab_infos_sizer->Add(close_icon, 0, wxEXPAND | wxTOP, 5);

    tab_infos->SetSizerAndFit(tab_infos_sizer);
    new_tab_sizer->Add(tab_infos, 1, wxEXPAND | wxTOP | wxLEFT | wxBOTTOM, 8);

    wxPanel* active_bar = new wxPanel(new_tab);
    active_bar->SetBackgroundColour(wxColor(255, 0, 180));
    new_tab_sizer->Add(active_bar, 0, wxEXPAND);

    new_tab->SetSizerAndFit(new_tab_sizer);
    sizer->Add(new_tab, 0);
    selected_tab = path;
    this->FitInside();
}

void Tabs::ClearTab(wxString tab_path) {
    auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));
    auto tabsContainer = ((Tabs*)FindWindowById(ID_TABS));

    if(codeContainer && tabsContainer) {
        for(auto& a_tab : tabsContainer->GetChildren()) {
            if(a_tab->GetName() == tab_path) {
                if(tab_path == selected_tab) {
                    auto prev_tab = a_tab->GetPrevSibling();
                    auto next_tab = a_tab->GetNextSibling();
                    wxString new_tab_path;

                    if(prev_tab) {
                        auto act_bar = prev_tab->GetChildren()[1];
                        if(act_bar) act_bar->SetBackgroundColour(wxColor(255, 0, 180));
                        new_tab_path = prev_tab->GetName();
                        selected_tab = prev_tab->GetName();
                    }
                    if(next_tab) {
                        auto act_bar = next_tab->GetChildren()[1];
                        if(act_bar) act_bar->SetBackgroundColour(wxColor(255, 0, 180));
                        new_tab_path = next_tab->GetName();
                        selected_tab = next_tab->GetName();
                    }

                    if(new_tab_path.size()) {
                        codeContainer->LoadNewFile(new_tab_path);
                    } else {
                        codeContainer->ClearAll();
                    }

                    if(!prev_tab && !next_tab) {
                        auto main_code = ((wxPanel*)FindWindowById(ID_MAIN_CODE));
                        if(main_code) {
                            main_code->GetChildren()[0]->Hide();
                            main_code->GetChildren()[1]->Hide();

                            EmptyWindow* empty_window = new EmptyWindow(main_code, ID_EMPYT_WINDOW);
                            wxSizer* main_code_sizer = main_code->GetSizer();
                            if(main_code_sizer) {
                                main_code_sizer->Add(empty_window, 1, wxEXPAND);
                                main_code_sizer->Layout();
                            }
                        }
                    }
                }

                a_tab->Destroy();
                tabsContainer->GetSizer()->Layout();
                tabsContainer->Update();
                tabsContainer->FitInside();
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

    this->DestroyChildren();
}

void Tabs::SelectTab(wxMouseEvent& event) {
    wxObject* obj = event.GetEventObject();
    auto this_tab = ((Tabs*)obj);

    if(this_tab) {
        auto codeContainer = ((CodeContainer*)FindWindowById(ID_CODE_CONTAINER));
        auto tabsContainer = ((Tabs*)FindWindowById(ID_TABS));

        wxString tab_path = this_tab->GetName();
        if(!tab_path) return;
        if(tab_path == selected_tab) return;
        selected_tab = tab_path;

        if(tab_path.size()) {
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

        for(auto& a_tab : tabsContainer->GetChildren()) {
            if(a_tab->GetName() == tab_path) {
                a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
            } else {
                a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
            }
        }
    }
}

void Tabs::CloseTab(wxMouseEvent& event) {
	wxObject* obj = event.GetEventObject();
    auto this_tab = ((wxWindow*)obj)->GetGrandParent();
    if(this_tab) Tabs::ClearTab(this_tab->GetName());
}