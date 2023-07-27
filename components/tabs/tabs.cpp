#include "tabs.hpp"

Tabs::Tabs(wxPanel* parent, wxWindowID ID) : wxPanel(parent, ID) {
    SetBackgroundColour(wxColor(50, 50, 50));
    sizer = new wxBoxSizer(wxHORIZONTAL);

    tabs_container = new wxScrolled<wxPanel>(this, ID_TABS_CONTAINER);
    tabs_ctn_sizer = new wxBoxSizer(wxHORIZONTAL);
    tabs_container->SetSizerAndFit(tabs_ctn_sizer);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(icons_dir+"menu_down.png", wxBITMAP_TYPE_PNG));
    menu = new wxStaticBitmap(this, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    menu->Bind(wxEVT_LEFT_UP, &Tabs::OnMenu, this);

    sizer->Add(tabs_container, 1, wxEXPAND);
    sizer->Add(menu, 0, wxALIGN_CENTER | wxRIGHT, 10);
    Add("ThunderCode", "initial_tab");

    SetSizerAndFit(sizer);
    tabs_container->SetScrollRate(20, 20);
    tabs_container->EnableScrolling(true, false);
}

void Tabs::Add(wxString tab_name, wxString path) {    
    if(!IsShown()) Show();
    bool exists = false;
    for(auto& a_tab : tabs_container->GetChildren()) {
        if(a_tab->GetName() == path) {
            a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
            exists = true;
        } else {
            a_tab->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
        }
    }
    if(exists) return;

    wxPanel* new_tab = new wxPanel(tabs_container);
    new_tab->SetName(path);
    new_tab->Bind(wxEVT_LEFT_UP, &Tabs::Select, this);
    wxBoxSizer* new_tab_sizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* tab_infos = new wxPanel(new_tab);
    wxBoxSizer* tab_infos_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* name = new wxStaticText(tab_infos, wxID_ANY, tab_name);
    name->SetName(path);
    name->SetFont(fontWithOtherSize(name, 17));
    name->Bind(wxEVT_LEFT_UP, &Tabs::Select, this);
    tab_infos_sizer->Add(name, 0, wxEXPAND | wxRIGHT, 5);

    wxVector<wxBitmap> bitmaps;
    bitmaps.push_back(wxBitmap(wxBitmap(icons_dir+"close.png", wxBITMAP_TYPE_PNG)));
    wxStaticBitmap* close_icon = new wxStaticBitmap(tab_infos, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps));
    close_icon->Bind(wxEVT_LEFT_UP, &Tabs::OnCloseTab, this);
    tab_infos_sizer->Add(close_icon, 0, wxALIGN_CENTER);

    wxVector<wxBitmap> bitmaps_c;
    bitmaps_c.push_back(wxBitmap(wxBitmap(icons_dir+"white_circle.png", wxBITMAP_TYPE_PNG)));
    wxStaticBitmap* modified_icon = new wxStaticBitmap(tab_infos, wxID_ANY, wxBitmapBundle::FromBitmaps(bitmaps_c));
    modified_icon->Bind(wxEVT_LEFT_UP, &Tabs::OnCloseTab, this);
    modified_icon->Bind(wxEVT_MOTION, &Tabs::OnUnsaveHover, this);
    modified_icon->Hide();
    tab_infos_sizer->Add(modified_icon, 0, wxEXPAND | wxTOP, 2);
    
    tab_infos->SetSizerAndFit(tab_infos_sizer);
    new_tab_sizer->Add(tab_infos, 1, wxEXPAND | wxALL, 8);

    wxPanel* active_bar = new wxPanel(new_tab);
    active_bar->SetBackgroundColour(wxColor(255, 0, 180));
    new_tab_sizer->Add(active_bar, 0, wxEXPAND);

    new_tab->SetSizerAndFit(new_tab_sizer);
    tabs_ctn_sizer->Add(new_tab, 0);
    selected_tab = path;
    tabs_container->FitInside();
    tabs_container->Scroll(1000, 0);

    SetMinSize(wxSize(GetSize().GetWidth(), new_tab->GetSize().GetHeight()));
}

void Tabs::Close(wxString tab_path) {
    auto codeContainer = ((CodeContainer*)FindWindowByName(tab_path+"_codeContainer"));
    auto imgContainer = ((wxPanel*)FindWindowByLabel(tab_path+"_imgContainer"));

    if(codeContainer) codeContainer->Destroy();
    if(imgContainer) imgContainer->Destroy();

    if(codeContainer || imgContainer && tabs_container) {
        for(auto& tab : tabs_container->GetChildren()) {
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

                    auto file_ctn = FindWindowByLabel(selected_tab+"_file_container");
                    if(file_ctn) {
                        auto fileContainer = ((FilesTree*)FindWindowById(ID_FILES_TREE));
                        if(fileContainer) {
                            if(!prev_tab && !next_tab) {
                                fileContainer->selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
                            } else {
                                fileContainer->selectedFile->SetBackgroundColour(wxColor(45, 45, 45));
                                fileContainer->SetSelectedFile(file_ctn);
                                file_ctn->SetBackgroundColour(wxColor(60, 60, 60));
                            }
                        }
                    }

                    auto other_codeContainer = ((CodeContainer*)FindWindowByName(selected_tab+"_codeContainer"));
                    if(other_codeContainer) other_codeContainer->Show();

                    auto new_imageContainer = FindWindowByLabel(selected_tab+"_imgContainer");
                    if(new_imageContainer) new_imageContainer->Show();
                    
                    if(!prev_tab && !next_tab) {
                        this->Hide();
                        FindWindowById(ID_EMPYT_WINDOW)->Show();
                        ((StatusBar*)FindWindowById(ID_STATUS_BAR))->ClearLabels();
                    }
                }

                tab->Destroy();
                tabs_container->GetSizer()->Layout();
                tabs_container->FitInside();
                tabs_container->Update();
                main_code->GetSizer()->Layout();
                main_code->Update();
            }
        }
    }
}

void Tabs::CloseAll() {
    auto main_code = FindWindowById(ID_MAIN_CODE);
    tabs_container->DestroyChildren();
    Hide();
    for(auto&& other_ct : main_code->GetChildren()) {
        if(other_ct->GetId() != ID_TABS &&
            other_ct->GetId() != ID_EMPYT_WINDOW) other_ct->Destroy();
    }
    FindWindowById(ID_EMPYT_WINDOW)->Show();
}

void Tabs::Select(wxMouseEvent& event) {
    auto this_tab = ((Tabs*)event.GetEventObject());
    wxString tab_path = this_tab->GetName();
    auto main_code = FindWindowById(ID_MAIN_CODE);

    if(tab_path == selected_tab) return;
    selected_tab = tab_path;

    for(auto& children : tabs_container->GetChildren()) {
        if(children->GetName() == tab_path) {
            children->GetChildren()[1]->SetBackgroundColour(wxColor(255, 0, 180));
        } else {
            children->GetChildren()[1]->SetBackgroundColour(wxColor(55, 55, 55));
        }
    }

    auto codeContainer = ((CodeContainer*)FindWindowByName(tab_path+"_codeContainer"));
    auto imageContainer = ((wxStaticBitmap*)FindWindowByLabel(tab_path+"_imageContainer"));
    auto status_bar = ((StatusBar*)FindWindowById(ID_STATUS_BAR));

    for(auto&& other_ct : main_code->GetChildren()) {
        if(other_ct->GetId() != ID_TABS) other_ct->Hide();
    }

    if(codeContainer) {
        codeContainer->Show();
        status_bar->UpdateComps(tab_path, "text", codeContainer->current_lang->name);
    }

    if(imageContainer) {
        imageContainer->Show();
        status_bar->UpdateComps(tab_path, "image", "img");
    }

    main_code->GetSizer()->Layout();
    main_code->Update();
}

void Tabs::OnCloseTab(wxMouseEvent& event) {
    wxObject* obj = event.GetEventObject();
    auto this_tab = ((wxWindow*)obj)->GetGrandParent();
    if(this_tab) Tabs::Close(this_tab->GetName());
}

void Tabs::OnMenu(wxMouseEvent& event) {
    wxMenu* tabsMenu = new wxMenu;
    tabsMenu->Append(ID_CLOSE_ALL_FILES, _("&Close All"));
    tabsMenu->Append(wxID_ANY, _("&First Tab"));
    tabsMenu->Append(wxID_ANY, _("&Last Tab"));
    tabsMenu->Append(wxID_ANY, _("&Close Saved"));
    PopupMenu(tabsMenu);
}

void Tabs::OnUnsaveHover(wxMouseEvent& event) {
    wxStaticBitmap* unsave_icon = ((wxStaticBitmap*)event.GetEventObject());
    if(unsave_icon) {
        wxVector<wxBitmap> bitmaps;
        if(event.GetPosition().x <= 2 || event.GetPosition().y <= 2) {
            bitmaps.push_back(wxBitmap(wxBitmap(icons_dir+"close.png", wxBITMAP_TYPE_PNG)));
        } else {
            bitmaps.push_back(wxBitmap(wxBitmap(icons_dir+"white_circle.png", wxBITMAP_TYPE_PNG)));
        }
        unsave_icon->SetBitmap(wxBitmapBundle::FromBitmaps(bitmaps));
    }
}