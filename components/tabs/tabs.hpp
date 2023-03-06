#pragma once

#include <vector>
#include "../../utils/randoms.hpp"

class Tabs : public wxScrolled<wxPanel> {
	wxScrolled<wxPanel>* tabs_ctn;
	wxBoxSizer* sizer;
	wxString selected_tab;
public:
	Tabs(wxPanel* parent, wxWindowID ID);
	void AddTab(wxString tab_name, wxString path);
	void ClearTab(wxString tab_path);
	void ClearAllTabs();
	void SelectTab(wxMouseEvent& event);
	void CloseTab(wxMouseEvent& event);
	wxDECLARE_NO_COPY_CLASS(Tabs);
};