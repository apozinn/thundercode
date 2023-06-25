#pragma once
#include "../../utils/fileManager.hpp"

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
    void onFileRightClick(wxMouseEvent& event);
    void onDirRightClick(wxMouseEvent& event);
	void Create(std::string path, wxWindow* parent);
	void SetSelectedFile(wxWindow* new_window) {selectedFile = new_window;}
	void FitContainer(wxWindow* window);
	void OnPaint(wxPaintEvent& event);
	wxDECLARE_NO_COPY_CLASS(FilesTree);
};