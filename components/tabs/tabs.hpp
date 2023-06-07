#pragma once

#include "../../utils/randoms.hpp"
#include "../codeContainer/code.hpp"
#include "../filesTree/files.hpp"
#include "../../members/emptyWindow.cpp"

class Tabs : public wxScrolled<wxPanel> {
	wxSizer* sizer;
	wxString selected_tab;
public:
	Tabs(wxPanel* parent, wxWindowID ID);
	void Add(wxString tab_name, wxString path);
	void Close(wxString tab_path);
	void CloseAll();
	void Select(wxMouseEvent& event);
	void OnCloseTab(wxMouseEvent& event);
	wxDECLARE_NO_COPY_CLASS(Tabs);
};