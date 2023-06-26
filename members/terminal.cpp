#pragma once

class Terminal : public wxPanel {
    wxString cmd;
    int startCommand = 29;
    wxStyledTextCtrl* shell;
    wxBoxSizer* sizer;
public:
	Terminal(wxWindow* parent, wxWindowID ID) : wxPanel(parent, ID) {
		SetBackgroundColour(wxColor(55, 55, 55));
		sizer = new wxBoxSizer(wxVERTICAL);

		auto separator = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(this->GetSize().GetWidth(), 1), wxLI_HORIZONTAL);
		separator->SetBackgroundColour(wxColor(70, 70, 70));
		sizer->Add(separator, 0, wxEXPAND);

		shell = new wxStyledTextCtrl(this);
		shell->SetBackgroundColour(wxColor(55, 55, 55));

		wxFont font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		font.SetFaceName(wxT("Monospace"));

		shell->StyleSetBackground(wxSTC_STYLE_DEFAULT, wxColor(55, 55, 55));
	    shell->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColor(255, 255, 255));
	    shell->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	    shell->StyleClearAll();

	    shell->SetMarginWidth(0, 0);
	    shell->SetMarginWidth(1, 0);
	    shell->SetMarginWidth(2, 0);
    	shell->SetCaretForeground(wxColour(wxT("WHITE")));
    	shell->SetUseHorizontalScrollBar(false);
		shell->AppendText("[samuel@fedora ThunderCode]$ ");

    	shell->Bind(wxEVT_KEY_UP, &Terminal::OnShellKeyUp, this);

    	shell->SetFocus();

		sizer->Add(shell, 1, wxEXPAND | wxTOP, 5);
		SetSizerAndFit(sizer);
	}
private:
	void Paint(wxPaintEvent& event) {
		wxPaintDC this_dc(this);
	    this_dc.SetBrush(wxColour(70, 70, 70));
	    this_dc.SetPen(wxColour(70, 70, 70));
	}

	void OnShellKeyUp(wxKeyEvent& event) {
	   switch(event.GetKeyCode()) {
	   case 13: {
		   	OnEnterCommand();
		   } break;
		case 8: {
			char previous_char = (char)shell->GetCharAt(shell->GetCurrentPos()-1);
			if(previous_char == '$') {
				shell->AppendText(" ");
				shell->GotoPos(shell->GetCurrentPos()+1);
			}
		} break;
	   }
	}

	void OnEnterCommand() {
		wxArrayString output, errors;
		cmd = shell->GetTextRange(startCommand, shell->GetCurrentPos()-1);

		if(cmd == "clear") {
			shell->SetText("[samuel@fedora ThunderCode]$ ");
		    shell->GotoPos(shell->GetLength());
		    startCommand = shell->GetCurrentPos();
			return;
		} 

        int code = wxExecute(cmd, output, errors, wxEXEC_SYNC);
        ShowOutput(output, errors);
	}

	void ShowOutput(
		const wxArrayString& output,
		const wxArrayString& errors
	) {
	    size_t ouput_size = output.GetCount();
	    size_t erros_size = errors.GetCount();

	    if(erros_size) {
	    	for(size_t n = 0; n < erros_size; n++)
	    	{
	    		shell->AppendText(errors[n]+"\n");
		    }
	    } else {
	    	for(size_t n = 0; n < ouput_size; n++)
	    	{
	    		shell->AppendText(output[n]+"\n");
	    	}	
	    }

	    shell->AppendText("[samuel@fedora ThunderCode]$ ");
	    shell->GotoPos(shell->GetLength());

	    startCommand = shell->GetCurrentPos();
	    shell->SetFocus();
	}
};