#pragma once

#include <vector>
#include <wx/filename.h>
#include <wx/scrolwin.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/statbmp.h>

#include "../../utils/randoms.hpp"
#include "../../utils/fileManager.hpp"
#include "../../members/imagePanel.cpp"

class FilesTree : public wxPanel {
	wxPanel* files_tree;
    wxSizer* sizer;
    wxScrolled<wxPanel>* files_container;
    wxBoxSizer* file_ctn_sizer;
    wxScrolled<wxPanel>* project_files_ctn;
public:
	FileManager* fileManager = new FileManager();
	FilesTree(wxWindow* parent, wxWindowID ID);
	void Update();
	void CreateFile(wxWindow* parent, wxString name, wxString path);
	void CreateDir(wxWindow* parent, wxString name, wxString path);
	void OnFileSelect(wxMouseEvent& event);
	void ToggleDir(wxMouseEvent& event);
	wxDECLARE_NO_COPY_CLASS(FilesTree);
};