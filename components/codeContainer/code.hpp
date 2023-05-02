#pragma once
#include <wx/stc/stc.h>

class CodeContainer : public wxStyledTextCtrl {
    wxStyledTextCtrl* m_activeSTC;
public:
    CodeContainer(wxWindow* parent, wxWindowID ID, wxString path);
    void OnSave(wxCommandEvent& event);
    void LoadNewFile(wxString file_path) {
        this->LoadFile(file_path);
        this->SelectNone();
        this->SetLexer(wxSTC_LEX_CPP);
        SetFilename(file_path);
        if(!this->IsShown()) this->Show();

        auto tab_size_comp = ((wxStaticText*)FindWindowById(ID_STTSBAR_TAB_SIZE));
        auto file_ext_comp = ((wxStaticText*)FindWindowById(ID_STTSBAR_FILE_EXT));

        if(tab_size_comp) {
            tab_size_comp->SetLabel("Tab Size: 4");
        }

        if(file_ext_comp) {
            size_t last_dot;
            last_dot = file_path.find_last_of(".");
            if(last_dot) {
                std::string file_ext = file_path.ToStdString().substr(last_dot+1);
                if(file_ext.size()) {
                    file_ext_comp->SetLabel(file_ext);
                }
            }
        }

        auto parent = file_ext_comp->GetParent();
        auto parent_sizer = parent->GetSizer();

        parent_sizer->Layout();
        parent_sizer->Fit(parent);

        auto code_locale = ((wxStaticText*)FindWindowById(ID_STTSBAR_CODELOCALE));
        if(code_locale) {
            code_locale->SetLabel("Line "+std::to_string(GetCurrentLine())+", Column "+std::to_string(GetColumn(GetCurrentPos())));
        }
    }
    void SetFilename(wxString n_filename) {filename = n_filename;};
    wxString GetFilename() {return filename;};
    void onMarginClick(wxStyledTextEvent& event);
    void OnStyleNeeded(wxStyledTextEvent& event);
    void highlightSTCsyntax(size_t fromPos,size_t toPos,wxString &text);
    void setfoldlevels(size_t fromPos,int startfoldlevel,wxString &text);
	void OnChange(wxStyledTextEvent& event);
    void CharAdd(wxStyledTextEvent& event);
    void onClick(wxMouseEvent& event);
private:
    wxString filename;
    wxDECLARE_NO_COPY_CLASS(CodeContainer);
    wxDECLARE_EVENT_TABLE();
};