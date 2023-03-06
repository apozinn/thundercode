#pragma once

class MenuBar : public wxMenuBar {
public:
	MenuBar(wxWindowID ID) : wxMenuBar() {
		wxMenu *menuFile = new wxMenu;
	    menuFile->Append(ID_NEW_FILE, _("&New File\tCtrl-N"));
	    menuFile->Append(ID_OPEN_FOLDER, _("&Open folder\tCtrl-K"));
	    menuFile->Append(wxID_OPEN, _("&Open file"));
	    menuFile->Append(wxID_SAVE, _("&Save"));
	    menuFile->AppendSeparator();
	    menuFile->Append(wxID_EXIT, _("&Exit"));

	    wxMenu *menuEdit = new wxMenu;
	    menuEdit->Append(wxID_ANY, _("&Edit"));

	    wxMenu *menuSelection = new wxMenu;
	    menuSelection->Append(wxID_ANY, _("&Selection"));

	    wxMenu *menuFind = new wxMenu;
	    menuFind->Append(wxID_ANY, _("&Find"));

	    wxMenu *menuView = new wxMenu;
	    menuView->Append(ID_HIDDE_FILES_TREE, _("&Hidde files-tree\tCtrl-H"));
	    menuView->Append(ID_HIDDE_SIDE_NAV, _("&Hidde Side-Nav\tCtrl-H"));
	    menuView->Append(ID_HIDDE_MENU_BAR, _("&Hidde Menu\tCtrl-H"));

	    wxMenu *menuTools = new wxMenu;
	    menuTools->Append(wxID_ANY, _("&Tools"));

	    wxMenu *menuPreference = new wxMenu;
	    menuPreference->Append(wxID_ANY, _("&Preferences"));

	    wxMenu *menuTerminal = new wxMenu;
	    menuTerminal->Append(wxID_ANY, _("&Terminal"));

	    wxMenu *menuHelp = new wxMenu;
	    menuHelp->Append(wxID_ABOUT, _("&About"));

	    this->Append(menuFile, _("&File"));
	    this->Append(menuEdit, _("&Edit"));
	    this->Append(menuSelection, _("&Selection"));
	    this->Append(menuFind, _("&Find"));
	    this->Append(menuView, _("&View"));
	    this->Append(menuTools, _("&Tools"));
	    this->Append(menuTerminal, _("&Terminal"));
	    this->Append(menuPreference, _("&Preferences"));
	    this->Append(menuHelp, _("&Help"));
	}
	wxDECLARE_NO_COPY_CLASS(MenuBar);
};