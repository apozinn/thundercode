struct LanguageInfo {
    const char *name;
    const char *filepattern;
    int lexer;
    struct {
        int type;
        const char *words;
    } styles [STYLE_TYPES_COUNT];
    int folds;
};

extern const LanguageInfo g_LanguagePrefs[];
extern const int g_LanguagePrefsSize;