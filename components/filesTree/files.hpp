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
#include "../statusBar/status.hpp"

class FilesTree : public wxPanel {
	wxPanel* files_tree;
    wxSizer* sizer;
    wxScrolled<wxPanel>* files_container;
    wxBoxSizer* file_ctn_sizer;
    wxScrolled<wxPanel>* project_files_ctn;
public:
    wxWindow* selectedFile;
	FileManager* fileManager = new FileManager();
	FilesTree(wxWindow* parent, wxWindowID ID);
	void Update();
	void CreateFile(wxWindow* parent, wxString name, wxString path);
	void CreateDir(wxWindow* parent, wxString name, wxString path);
	void OnFileSelect(wxMouseEvent& event);
	void ToggleDir(wxMouseEvent& event);
	void OpenFile(wxString path);
	void onTopMenuClick(wxMouseEvent& event);
	void Create(std::string path, wxWindow* parent) {
		fileManager->ListChildrens(
        	path, [&](const std::string &path, const std::string &type, const std::string &name
    	) {
	        if(type == "dir") {
	            this->CreateDir(parent, name, path);
	        }
    	});

    	fileManager->ListChildrens(
        	path, [&](const std::string &path, const std::string &type, const std::string &name
    	) {
	        if(type == "file") {
	            this->CreateFile(parent, name, path);
	        }
    	});
	}
	void SetSelectedFile(wxWindow* new_window) {
		selectedFile = new_window;
	}
	wxDECLARE_NO_COPY_CLASS(FilesTree);
};