struct StyleInfo {
    const wxString name;
    const wxColour foreground;
    const wxString fontname;
    int fontsize;
    int fontstyle;
    int lettercase;
};

extern const StyleInfo global_lexer_styles[];