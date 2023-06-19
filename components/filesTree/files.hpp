#pragma once

#include <vector>
#include <wx/filename.h>
#include <wx/scrolwin.h>
#include <wx/wfstream.h>
#include <wx/fswatcher.h>
#include <wx/txtstrm.h>
#include <wx/statbmp.h>
#include "wx/listctrl.h"
#include "wx/cmdline.h"

#include "../../utils/randoms.hpp"
#include "../../utils/fileManager.hpp"
#include "../../members/imagePanel.cpp"
#include "../statusBar/status.hpp"
#include "../codeContainer/code.hpp"
#include "../tabs/tabs.hpp"
#include <wx/graphics.h>

class FilesTree : public wxPanel {
	wxPanel* files_tree;
    wxSizer* sizer;
    wxScrolled<wxPanel>* files_container;
    wxBoxSizer* file_ctn_sizer;
    wxScrolled<wxPanel>* project_files_ctn;
    wxFileSystemWatcher* m_watcher;
    wxString m_dirToWatch;

    wxTextCtrl *m_evtConsole;         // events console
    wxListView *m_filesList;          // list of watched paths

     virtual void OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop))
    {
        if ( CreateWatcherIfNecessary() )
        {
            if ( !m_dirToWatch.empty() )
                AddDirectory(m_dirToWatch);
        }
    }

    virtual void OnInitCmdLine(wxCmdLineParser& parser)
    {
        OnInitCmdLine(parser);
        parser.AddParam("directory to watch",
                        wxCMD_LINE_VAL_STRING,
                        wxCMD_LINE_PARAM_OPTIONAL);
    }

    virtual bool OnCmdLineParsed(wxCmdLineParser& parser)
    {
        if ( !OnCmdLineParsed(parser) )
            return false;

        if ( parser.GetParamCount() )
            m_dirToWatch = parser.GetParam();

        return true;
    }

     void AddDirectory(const wxString& dir);

    bool CreateWatcherIfNecessary();

    const wxString LOG_FORMAT = " %-12s %-36s    %-36s";

    static wxString GetFSWEventChangeTypeName(int changeType)
{
    switch (changeType)
    {
    case wxFSW_EVENT_CREATE:
        return "CREATE";
    case wxFSW_EVENT_DELETE:
        return "DELETE";
    case wxFSW_EVENT_RENAME:
        return "RENAME";
    case wxFSW_EVENT_MODIFY:
        return "MODIFY";
    case wxFSW_EVENT_ACCESS:
        return "ACCESS";
    }

    return "INVALID_TYPE";
}
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
	void Create(std::string path, wxWindow* parent) {
		std::vector<std::string> folders_buffer;
		std::vector<std::string> files_buffer;

		fileManager->ListChildrens(path, [&](const std::string &path, const std::string &type, const std::string &name) {
	        if(type == "dir") {
	        	folders_buffer.push_back(name);
	        } else files_buffer.push_back(name);
    	});

    	std::sort(folders_buffer.begin(), folders_buffer.end());
    	std::sort(files_buffer.begin(), files_buffer.end());

    	for(auto&& f_ : folders_buffer) {
    		fileManager->ListChildrens(path, [&](const std::string &path, const std::string &type, const std::string &name) {
		        if(type == "dir") {
		        	if(name == f_) this->CreateDir(parent, name, path);
		        }
    		});
    	}

    	for(auto&& f_ : files_buffer) {
    		fileManager->ListChildrens(path, [&](const std::string &path, const std::string &type, const std::string &name) {
		        if(type == "file") {
		        	if(name == f_) this->CreateFile(parent, name, path);
		        }
    		});
    	}
	}

	void SetSelectedFile(wxWindow* new_window) {selectedFile = new_window;}
	void FitContainer(wxWindow* window) {
		auto next_parent = window;
        bool has_next_parent = true;
        while(has_next_parent) {
        	int height = 0;
        	for(auto&& children : next_parent->GetChildren()) {
        		if(children->IsShown()) height = height+children->GetSize().GetHeight();
        	}

            next_parent->SetSize(next_parent->GetSize().GetWidth(), height);
            next_parent->SetMinSize(wxSize(next_parent->GetSize().GetWidth(), height));

             if(next_parent->GetParent() && next_parent->GetId() != ID_PROJECT_FILES_CTN) {
                next_parent = next_parent->GetParent();
            } else has_next_parent = false;
        }
	}
	void OnPaint(wxPaintEvent& event);
	void UpdateTree(wxFileSystemWatcherEvent& event);

	void OnFileSystemEvent(wxFileSystemWatcherEvent& event);
    void LogEvent(const wxFileSystemWatcherEvent& event);
    void CreateWatcher();

	wxDECLARE_NO_COPY_CLASS(FilesTree);
};