#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
    #include "wx/textdlg.h"
#endif

#include "wx/file.h"
#include "wx/filename.h"
#include "wx/settings.h"
#include "code.hpp"

#define MY_FOLDMARGIN 2

wxBEGIN_EVENT_TABLE(CodeContainer, wxStyledTextCtrl)
wxEND_EVENT_TABLE()

enum {
    MARGIN_LINE_NUMBERS
};

CodeContainer::CodeContainer(wxWindow* parent, wxWindowID ID, wxString path) : wxStyledTextCtrl(parent, ID) 
{
    SetLabel(path+"_codeContainer");
    SetName(path);
	SetUseTabs(true);
    SetTabWidth(4);
    SetIndent(4);
    SetTabIndents(true);
    SetBackSpaceUnIndents(true);
    SetIndentationGuides(true);

    //load file content
    LoadNewFile(path);

    //font styling
    StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(70, 70, 70));
    StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
    StyleSetFont(wxSTC_STYLE_DEFAULT, wxFontInfo(10).Family(wxFONTFAMILY_MODERN));

    //clean style for load new font styles
    StyleClearAll();

    //margins styling
    SetMarginWidth(0, TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_9999")));
    SetMarginWidth(MY_FOLDMARGIN, 10);
    SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(45, 120, 210));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(70, 70, 70));

    SetCaretForeground(wxColour(wxT("WHITE")));
    SetLexer(wxSTC_LEX_CPP);

    //  
    StyleSetForeground(wxSTC_C_DEFAULT, wxColour(255, 255, 255));
    StyleSetForeground(wxSTC_C_STRING, wxColour(240, 245, 0));
    StyleSetForeground(wxSTC_C_STRINGEOL, wxColour(240, 245, 0));
    StyleSetForeground(wxSTC_C_STRINGRAW, wxColour(240, 245, 0));
    StyleSetForeground(wxSTC_C_HASHQUOTEDSTRING, wxColour(200, 245, 0));
    StyleSetForeground(wxSTC_C_COMMENT, wxColour(115, 115, 115));
    StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(115, 115, 115));
    StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(115, 115, 115));
    StyleSetForeground(wxSTC_C_COMMENTLINEDOC , wxColour(115, 115, 115));
    StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD , wxColour(115, 115, 115));
    StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR , wxColour(115, 115, 115));
    StyleSetForeground(wxSTC_C_NUMBER, wxColour(230, 230, 0));
    StyleSetForeground(wxSTC_C_WORD, wxColour(230, 200, 0));
    StyleSetForeground(wxSTC_C_WORD2, wxColour(230, 200, 0));
    StyleSetForeground(wxSTC_C_CHARACTER, wxColour(230, 200, 0));
    StyleSetForeground(wxSTC_C_UUID, wxColour(230, 200, 0));
    StyleSetForeground(wxSTC_C_PREPROCESSOR , wxColour(196, 35, 217));
    StyleSetForeground(wxSTC_C_PREPROCESSORCOMMENTDOC , wxColour(200, 80, 220));
    StyleSetForeground(wxSTC_C_OPERATOR , wxColour(16, 230, 190));
    StyleSetForeground(wxSTC_C_REGEX , wxColour(100, 230, 190));
    StyleSetForeground(wxSTC_C_GLOBALCLASS , wxColour(100, 255, 200));
    StyleSetForeground(wxSTC_C_TRIPLEVERBATIM , wxColour(100, 200, 200));
    StyleSetForeground(wxSTC_C_PREPROCESSORCOMMENT , wxColour(200, 200, 200));
    StyleSetForeground(wxSTC_C_USERLITERAL , wxColour(255, 255, 0));
    StyleSetForeground(wxSTC_C_TASKMARKER , wxColour(255, 255, 100));
    StyleSetForeground(wxSTC_C_ESCAPESEQUENCE , wxColour(255, 0, 100));
    //

    SetMarginMask(MY_FOLDMARGIN,wxSTC_MASK_FOLDERS);
    SetFoldMarginColour(true,wxColor(70,70,70));
    SetFoldMarginHiColour(true,wxColor(70,70,70));

    //Set up the markers that will be shown in the fold margin
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND,wxSTC_MARK_BOXPLUSCONNECTED);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND,wxColor(70,70,70));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID,wxSTC_MARK_BOXMINUSCONNECTED);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID,wxColor(70,70,70));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL,wxColor(70,70,70));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL,wxSTC_MARK_LCORNER);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL,wxColor(70,70,70));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB,wxSTC_MARK_VLINE);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB,wxColor(70,70,70));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDER,wxSTC_MARK_BOXPLUS);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDER,wxColor(70,70,70));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDER,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN,wxSTC_MARK_BOXMINUS);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN,wxColor(70,70,70));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN,wxColor(128,128,128));

    MarkerEnableHighlight(true);
    SetMarginSensitive(MY_FOLDMARGIN,true);

    SetFoldLevel(0, 1024);
    SetFoldLevel(1, 1024);
    SetFoldLevel(2, 1024|wxSTC_FOLDLEVELHEADERFLAG);
    SetFoldLevel(3, 1025);
    SetFoldLevel(4, 1025);
    SetFoldLevel(5, 1024);
    SetFoldLevel(6, 1024|wxSTC_FOLDLEVELWHITEFLAG);

    //events 
    Bind(wxEVT_STC_MARGINCLICK, &CodeContainer::onMarginClick, this);
    Bind(wxEVT_STC_STYLENEEDED, &CodeContainer::OnStyleNeeded, this);
    Bind(wxEVT_STC_MODIFIED, &CodeContainer::OnChange, this);
    Bind(wxEVT_STC_CHARADDED, &CodeContainer::CharAdd, this);
    Bind(wxEVT_STC_MARGINCLICK, &CodeContainer::onMarginClick, this);
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

void CodeContainer::onMarginClick(wxStyledTextEvent& event) {
    int margin = event.GetMargin();
    int position = event.GetPosition();
    int line = LineFromPosition(position);
    int foldLevel = GetFoldLevel(line);
    bool headerFlag = (foldLevel & wxSTC_FOLDLEVELHEADERFLAG)!=0;

    if(margin == MY_FOLDMARGIN && headerFlag) ToggleFold(line);
}

void CodeContainer::OnStyleNeeded(wxStyledTextEvent& event) {}
void CodeContainer::highlightSTCsyntax(size_t fromPos,size_t toPos, wxString &text) {}
void CodeContainer::setfoldlevels(size_t fromPos, int startfoldlevel, wxString& text) {}

void CodeContainer::OnChange(wxStyledTextEvent& event) {
    wxString key = event.GetText();
    if(GetModify()) {
        auto tabs = FindWindowById(ID_TABS);
        if(tabs) {
            for(auto&& tab : tabs->GetChildren()) {
                if(tab->GetName() == GetFilename()) {
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

void CodeContainer::CharAdd(wxStyledTextEvent& event) {
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();

    if (chr == '\n') {
        int lineInd = 0;
        if (currentLine > 0) {
            lineInd = GetLineIndentation(currentLine - 1);
        }
        if (lineInd == 0) return;
        SetLineIndentation (currentLine, lineInd);
        GotoPos(PositionFromLine (currentLine) + lineInd);
    } else if (chr == '#') {
        wxString s = "define?0 elif?0 else?0 endif?0 error?0 if?0 ifdef?0 "
                     "ifndef?0 include?0 line?0 pragma?0 undef?0";
        AutoCompShow(0,s);
    }

    if(chr == 'i') {
        wxString s = "import?0 if?0";
            AutoCompShow(0,s);
    }

    if(chr == '(') InsertText(GetCurrentPos(), ")");
    if(chr == '{') InsertText(GetCurrentPos(), "}");
    if(chr == '"') InsertText(GetCurrentPos(), "\"");
    if(chr == '`') InsertText(GetCurrentPos(), "`");
    if(chr == '[') InsertText(GetCurrentPos(), "]");
    if(event.GetKey() == 39) InsertText(GetCurrentPos(), "'");
}