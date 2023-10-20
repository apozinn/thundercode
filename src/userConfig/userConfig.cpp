#include "userConfig.h"
#include <fstream>
#include <wx/stdpaths.h>

json UserConfig::Get() {
	wxString appDataDir = wxStandardPaths::Get().GetUserConfigDir()+"/.thundercode";
	wxFileName appDataDir_op(appDataDir);
	std::string config_locale = appDataDir.ToStdString()+"/userconfig.json";

	if(appDataDir_op.GetFullPath() == appDataDir) {
		if(!appDataDir_op.Exists()) {
			bool dir_created = wxFileName::Mkdir(appDataDir);
			if(dir_created) {
				wxFile newConfigFile;
				bool created = newConfigFile.Create(appDataDir+"/userconfig.json");
				if(created) {
					std::ofstream newConfigFile_locale(config_locale);
					json new_json_obj = {
					  {"show_minimap", true},
					};
					newConfigFile_locale << std::setw(4) << new_json_obj << std::endl;
				}
			}
		} 
	} 

	
	std::ifstream config_file(config_locale);
	config_file >> data;
	return data;
}