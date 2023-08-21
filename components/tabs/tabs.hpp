#pragma once

#include "../../utils/randoms.hpp"
#include "../codeContainer/code.hpp"
#include "../filesTree/files.hpp"
#include "../../members/emptyWindow.cpp"

class Tabs : public wxPanel {
	wxSizer* sizer;
	wxString selected_tab;
	wxStaticBitmap* menu;
	wxScrolled<wxPanel>* tabs_container;
	wxBoxSizer* tabs_ctn_sizer;
public:
	Tabs(wxPanel* parent, wxWindowID ID);
	void Add(wxString tab_name, wxString path);
	void Close(wxString tab_path);
	void CloseAll();
	void Select(wxMouseEvent& event);
	void OnCloseTab(wxMouseEvent& event);
	void OnMenu(wxMouseEvent& event);
	void OnUnsaveHover(wxMouseEvent& event);
private:
	void OnEnterComp(wxMouseEvent& event);
	void OnLeaveComp(wxMouseEvent& event);
	wxDECLARE_NO_COPY_CLASS(Tabs);
};