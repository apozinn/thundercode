#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
    #include "wx/textdlg.h"
#endif

#include "wx/file.h"
#include "wx/filename.h"
#include "wx/settings.h"
#include "code.hpp"

wxBEGIN_EVENT_TABLE(CodeContainer, wxStyledTextCtrl)
wxEND_EVENT_TABLE()

enum {
    MARGIN_LINE_NUMBERS
};

CodeContainer::CodeContainer(wxWindow* parent, wxWindowID ID, wxString path) : wxStyledTextCtrl(parent, ID) 
{
    this->SetLabel(path+"_codeContainer");
    this->SetName(path);
	this->SetUseTabs(true);
    this->SetTabWidth(4);
    this->SetIndent(4);
    this->SetTabIndents(true);
    this->SetBackSpaceUnIndents(true);
    this->SetViewEOL(false);
    this->SetViewWhiteSpace(false);
    this->SetIndentationGuides(true);

    this->SetMarginWidth(0, this->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_9999")));
    this->SetCaretForeground(wxColour(wxT("WHITE")));

    this->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(70, 70, 70));
    this->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));

	wxFont font(wxFontInfo(10).Family(wxFONTFAMILY_MODERN));
    this->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    this->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

    this->StyleClearAll();

    this->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(45, 120, 210));
    this->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(70, 70, 70));

    this->SetLexer(wxSTC_LEX_CONTAINER);
    this->SetMarginMask(0,wxSTC_MASK_FOLDERS);
    this->SetFoldMarginColour(true,wxColor(255,255,255));
    this->SetFoldMarginHiColour(true,wxColor(233,233,233));
    this->MarkerDefine(wxSTC_MARKNUM_FOLDEREND,wxSTC_MARK_BOXPLUSCONNECTED);
    this->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND,wxColor(243,243,243));
    this->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND,wxColor(128,128,128));
    this->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID,wxSTC_MARK_BOXMINUSCONNECTED);
    this->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID,wxColor(243,243,243));
    this->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID,wxColor(128,128,128));
    this->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    this->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL,wxColor(243,243,243));
    this->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL,wxColor(128,128,128));
    this->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL,wxSTC_MARK_LCORNER);
    this->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL,wxColor(243,243,243));
    this->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL,wxColor(128,128,128));
    this->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB,wxSTC_MARK_VLINE);
    this->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB,wxColor(243,243,243));
    this->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB,wxColor(128,128,128));
    this->MarkerDefine(wxSTC_MARKNUM_FOLDER,wxSTC_MARK_BOXPLUS);
    this->MarkerSetForeground(wxSTC_MARKNUM_FOLDER,wxColor(243,243,243));
    this->MarkerSetBackground(wxSTC_MARKNUM_FOLDER,wxColor(128,128,128));
    this->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN,wxSTC_MARK_BOXMINUS);
    this->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN,wxColor(243,243,243));
    this->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN,wxColor(128,128,128));
    this->MarkerEnableHighlight(true);
    this->SetMarginSensitive(0,true);

    this->Bind(wxEVT_STC_MARGINCLICK, &CodeContainer::onMarginClick, this);
    this->Bind(wxEVT_STC_STYLENEEDED, &CodeContainer::OnStyleNeeded, this);
	this->Bind(wxEVT_STC_MODIFIED, &CodeContainer::OnChange, this);
    this->Bind(wxEVT_STC_CHARADDED, &CodeContainer::CharAdd, this);

    this->StyleSetForeground(19,m_GCodecolor);
    wxString text= this->GetText().Upper();
    this->highlightSTCsyntax(0,this->GetTextLength(),text);
    this->setfoldlevels(0,0,text);

    this->LoadNewFile(path);
}

void CodeContainer::OnSave(wxCommandEvent& event) {
    for(auto&& children : FindWindowById(ID_MAIN_CODE)->GetChildren()) {
        if(children->GetLabel().find("_codeContainer") != std::string::npos && children->IsShown()) {
            CodeContainer* children_ct = ((CodeContainer*)children);
            wxString file_path = children->GetName();
            if(file_path.size()) {
                children_ct->SaveFile(file_path);

                for(auto&& tab : FindWindowById(ID_TABS)->GetChildren()) {
                    if(tab->GetName() == file_path) {
                        auto close_ico = tab->GetChildren()[0]->GetChildren()[1];
                        auto unsaved_ico = tab->GetChildren()[0]->GetChildren()[2];

                        if(close_ico) close_ico->Show();
                        if(unsaved_ico) unsaved_ico->Hide();

                        tab->GetSizer()->Layout();
                        tab->Update();
                    }
                }
            } else {
               wxMessageBox(_("Filename or path don't find"), _("Error in save"), wxOK | wxICON_INFORMATION, this);
            }
        }
    }
}

void CodeContainer::onMarginClick(wxStyledTextEvent& event) {}

void CodeContainer::OnStyleNeeded(wxStyledTextEvent& event) {}

void CodeContainer::highlightSTCsyntax(size_t fromPos,size_t toPos, wxString &text) {}

void CodeContainer::setfoldlevels(size_t fromPos, int startfoldlevel, wxString& text) {}

void CodeContainer::OnChange(wxStyledTextEvent& event) {
    wxString key = event.GetText();
    if(this->GetModify()) {
        auto tabs = FindWindowById(ID_TABS);
        if(tabs) {
            for(auto&& tab : tabs->GetChildren()) {
                if(tab->GetName() == this->GetFilename()) {
                    auto close_ico = tab->GetChildren()[0]->GetChildren()[1];
                    auto unsaved_ico = tab->GetChildren()[0]->GetChildren()[2];

                    if(close_ico) close_ico->Hide();
                    if(unsaved_ico) unsaved_ico->Show();

                    tab->GetSizer()->Layout();
                    tab->Update();
                }
            }
        }
    }
}

void CodeContainer::CharAdd(wxStyledTextEvent& event) {}