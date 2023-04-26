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
    name->SetFont(fontWithOtherSize(name, 17));
    name->Bind(wxEVT_LEFT_UP, &Tabs::SelectTab, this);
    tab_infos_sizer->Add(name, 0, wxEXPAND | wxRIGHT, 5);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(wxBitmap(icons_dir+"close.png", wxBITMAP_TYPE_PNG)));
    wxStaticBitmap* close_icon = new wxStaticBitmap(tab_infos, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    close_icon->Bind(wxEVT_LEFT_UP, &Tabs::CloseTab, this);
    tab_infos_sizer->Add(close_icon, 0, wxALIGN_CENTER);

    wxVector<wxBitmap> bitmaps_c;
    bitmaps_c.push_back(wxBitmap(wxBitmap(icons_dir+"white_circle.png", wxBITMAP_TYPE_PNG)));
    wxStaticBitmap* modified_icon = new wxStaticBitmap(tab_infos, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_c));
    modified_icon->Hide();
    tab_infos_sizer->Add(modified_icon, 0, wxEXPAND | wxTOP, 2);
    
    tab_infos->SetSizerAndFit(tab_infos_sizer);
    new_tab_sizer->Add(tab_infos, 1, wxEXPAND | wxALL, 8);

    wxPanel* active_bar = new wxPanel(new_tab);
    active_bar->SetBackgroundColour(wxColor(255, 0, 180));
    new_tab_sizer->Add(active_bar, 0, wxEXPAND);

    new_tab->SetSizerAndFit(new_tab_sizer);
    sizer->Add(new_tab, 0);
    selected_tab = path;
    this->FitInside();
}

void Tabs::ClearTab(wxString tab_path) {
    auto codeContainer = ((CodeContainer*)FindWindowByLabel(tab_path+"_codeContainer"));
    auto imgContainer = ((wxPanel*)FindWindowByLabel(tab_path+"_imgContainer"));

    if(codeContainer) {
        codeContainer->Destroy();
    }

    if(imgContainer) {
        imgContainer->Destroy();
    }

    if(codeContainer || imgContainer && this) {
        for(auto& tab : this->GetChildren()) {
            if(tab->GetName() == tab_path) {
                auto main_code = FindWindowById(ID_MAIN_CODE);
                if(tab_path == selected_tab) {
                    auto prev_tab = tab->GetPrevSibling();
                    auto next_tab = tab->GetNextSibling();

                    if(prev_tab) {
                        auto act_bar = prev_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
                        selected_tab = prev_tab->GetName();
                    } else if(next_tab) {
                        auto act_bar = next_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
                        selected_tab = next_tab->GetName();
                    }

                    auto new_ct = FindWindowByLabel(selected_tab+"_codeContainer");
                    auto new_it = FindWindowByLabel(selected_tab+"_imgContainer");

                    if(new_ct) {
                        new_ct->Show();
                    } else if(new_it) {
                        new_it->Show();
                    }

                    if(!prev_tab && !next_tab) {
                        this->Hide();
                        FindWindowById(ID_EMPYT_WINDOW)->Show();

                        auto tab_size_comp = ((wxStaticText*)FindWindowById(ID_STTSBAR_TAB_SIZE));
                        if(tab_size_comp) {
                            tab_size_comp->SetLabel("");
                        }

                        auto file_ext_comp = ((wxStaticText*)FindWindowById(ID_STTSBAR_FILE_EXT));
                        if(file_ext_comp) {
                            file_ext_comp->SetLabel("");
                        }
                    }
                }

                tab->Destroy();
                this->GetSizer()->Layout();
                this->Update();
                this->FitInside();
                main_code->GetSizer()->Layout();
                main_code->Update();
            }
        }
    }
}

void Tabs::ClearAllTabs() {
    this->DestroyChildren();
}

void Tabs::SelectTab(wxMouseEvent& event) {
    auto this_tab = ((Tabs*)event.GetEventObject());
    wxString tab_path = this_tab->GetName();
    auto main_code = FindWindowById(ID_MAIN_CODE);

    if(tab_path == selected_tab) return;
    selected_tab = tab_path;

    for(auto& children : this->GetChildren()) {
        if(children->GetName() == tab_path) {
            children->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
        } else {
            children->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
        }
    }

    auto codeContainer = ((CodeContainer*)FindWindowByLabel(tab_path+"_codeContainer"));
    for(auto&& other_ct : main_code->GetChildren()) {
        if(other_ct->GetId() != ID_TABS) other_ct->Hide();
    }

    if(codeContainer) {
        if(!codeContainer->IsShown()) {
            codeContainer->Show();
        }
        main_code->GetSizer()->Layout();
        main_code->Update();
    }
}

void Tabs::CloseTab(wxMouseEvent& event) {
	wxObject* obj = event.GetEventObject();
    auto this_tab = ((wxWindow*)obj)->GetGrandParent();
    if(this_tab) Tabs::ClearTab(this_tab->GetName());
}