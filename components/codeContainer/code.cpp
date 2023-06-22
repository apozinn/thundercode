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
    #ifdef __WXGTK__
        wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        font.SetFaceName(wxT("Monospace"));
    #else
        wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    #endif

    StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(55, 55, 55));
    StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
    StyleSetFont(wxSTC_STYLE_DEFAULT, font);

    //clean style for load new font styles
    StyleClearAll();

    //margins styling
    SetMarginWidth(0, TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_99999")));
    SetMarginWidth(MY_FOLDMARGIN, 10);
    SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
    StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColor(128, 128, 128));
    StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColor(55, 55, 55));

    SetMarginType(1, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(1, wxSTC_MASK_FOLDERS);
    SetMarginWidth(1, 16);
    SetMarginSensitive(1, true);

    //fold
    SetProperty(wxT("fold"), wxT("1"));
    SetProperty(wxT("fold.comment"), wxT("1"));
    SetProperty(wxT("fold.compact"), wxT("1"));
    SetProperty(wxT("fold.preprocessor"), wxT("1"));
    SetProperty(wxT("fold.html"), wxT("1"));
    SetProperty(wxT("fold.html.preprocessor"), wxT("1"));
    SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    SetMarginWidth(2, 0);
    SetCaretForeground(wxColour(wxT("WHITE")));
    SetLexer(wxSTC_LEX_CPP);
    SetKeyWords(0, wxT("asm auto bool break case catch char class const const_cast \
                              continue default delete do double dynamic_cast else enum explicit \
                              export extern false float for friend goto if inline int long \
                              mutable namespace new operator private protected public register \
                              reinterpret_cast return short signed sizeof static static_cast \
                              struct switch template throw true try typedef typeid \
                              typename union unsigned using virtual void volatile wchar_t \
                              while"));
    SetKeyWords(1, wxT("this"));

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
    StyleSetForeground(wxSTC_C_NUMBER, wxColour(255, 155, 0));

    StyleSetForeground(wxSTC_C_WORD, wxColour(206, 42, 235));
    StyleSetForeground(wxSTC_C_WORD2, wxColour(17, 118, 250));
    StyleSetForeground(wxSTC_C_CHARACTER, wxColour(230, 200, 0));
    StyleSetForeground(wxSTC_C_UUID, wxColour(230, 200, 0));
    StyleSetForeground(wxSTC_C_PREPROCESSOR , wxColour(117, 66, 245));
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

    SetMarginMask(MY_FOLDMARGIN, wxSTC_MASK_FOLDERS);
    SetFoldMarginColour(true,wxColor(55,55,55));
    SetFoldMarginHiColour(true,wxColor(55,55,55));

    //Set up the markers that will be shown in the fold margin
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND,wxSTC_MARK_BOXPLUSCONNECTED);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND,wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID,wxSTC_MARK_BOXMINUSCONNECTED);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID,wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL,wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL,wxSTC_MARK_LCORNER);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL,wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB,wxSTC_MARK_VLINE);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB,wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDER,wxSTC_MARK_BOXPLUS);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDER,wxColor(55,55,55));
    MarkerSetBackground(wxSTC_MARKNUM_FOLDER,wxColor(128,128,128));

    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN,wxSTC_MARK_BOXMINUS);
    MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN,wxColor(55,55,55));
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

    SetScrollWidthTracking(true);

    //events 
    Bind(wxEVT_STC_MARGINCLICK, &CodeContainer::onMarginClick, this);
    Bind(wxEVT_STC_STYLENEEDED, &CodeContainer::OnStyleNeeded, this);
    Bind(wxEVT_STC_MODIFIED, &CodeContainer::OnChange, this);
    Bind(wxEVT_STC_CHARADDED, &CodeContainer::CharAdd, this);
    Bind(wxEVT_STC_MARGINCLICK, &CodeContainer::onMarginClick, this);
    Bind(wxEVT_LEFT_UP, &CodeContainer::onClick, this);
    Bind(wxEVT_KEY_UP, &CodeContainer::OnArrowsPress, this);
}

void CodeContainer::OnSave(wxCommandEvent& event) {
    for(auto&& children : FindWindowById(ID_MAIN_CODE)->GetChildren()) {
        if(children->GetLabel().find("_codeContainer") != std::string::npos && children->IsShown()) {
            CodeContainer* children_ct = ((CodeContainer*)children);
            wxString file_path = children->GetName();
            if(file_path.size()) {
                children_ct->SaveFile(file_path);

                for(auto&& tab : FindWindowById(ID_TABS_CONTAINER)->GetChildren()) {
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
    if (event.GetMargin() == 1) {
        int lineClick = LineFromPosition(event.GetPosition());
        int levelClick = GetFoldLevel(lineClick);

        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
            ToggleFold(lineClick);
        }
    }
}

void CodeContainer::OnStyleNeeded(wxStyledTextEvent& event) {}
void CodeContainer::highlightSTCsyntax(size_t fromPos,size_t toPos, wxString &text) {}
void CodeContainer::setfoldlevels(size_t fromPos, int startfoldlevel, wxString& text) {}

void CodeContainer::OnChange(wxStyledTextEvent& event) {
    wxString key = event.GetText();
    if(GetModify()) {
        auto tabs = FindWindowById(ID_TABS_CONTAINER);
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

    SetStatus(1);

    if(FindWindowById(ID_STTSBAR_CODELOCALE)) {
        ((wxStaticText*)FindWindowById(ID_STTSBAR_CODELOCALE))->SetLabel(
            "Line "+std::to_string(GetCurrentLine())+", Column "+std::to_string(GetColumn(GetCurrentPos()))
        );
    }
}

void CodeContainer::OnArrowsPress(wxKeyEvent& event) {
    if(FindWindowById(ID_STTSBAR_CODELOCALE)) {
        ((wxStaticText*)FindWindowById(ID_STTSBAR_CODELOCALE))->SetLabel(
            "Line "+std::to_string(GetCurrentLine())+", Column "+std::to_string(GetColumn(GetCurrentPos()))
        );
    }
}

void CodeContainer::CharAdd(wxStyledTextEvent& event) {
    char chr = (char)event.GetKey();
    int currentLine = GetCurrentLine();
	char previous_char = (char)GetCharAt(GetCurrentPos()-2);

    if (chr == '\n') {
        int currentLineInd;
        int nextLineInd;

        if(currentLine > 0) {
            currentLineInd = GetLineIndentation(currentLine-1);
            nextLineInd = GetLineIndentation(currentLine+1);
        }

        if(previous_char == '{' && !nextLineInd) {
            SetLineIndentation(currentLine, currentLineInd+GetIndent());
            GotoPos(GetCurrentPos() + currentLineInd+GetIndent()-3);
            InsertText(GetCurrentPos(), "\n");
        } else {
            if(currentLineInd && nextLineInd) {
                if(currentLineInd == nextLineInd) {
                    SetLineIndentation(currentLine, currentLineInd);
                    GotoPos(PositionFromLine(currentLine) + currentLineInd-3);
                } else if(nextLineInd > currentLineInd) {
                    SetLineIndentation(currentLine, nextLineInd);
                    GotoPos(PositionFromLine(currentLine) + nextLineInd-3);
                }
            }
        }

    } 

    if (chr == '#') {
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

void CodeContainer::onClick(wxMouseEvent& event) {
    auto code_locale = ((wxStaticText*)FindWindowById(ID_STTSBAR_CODELOCALE));
    if(code_locale) {
        code_locale->SetLabel("Line "+std::to_string(GetCurrentLine()+1)+", Column "+std::to_string(GetColumn(GetCurrentPos())));
    }
    event.Skip();
}