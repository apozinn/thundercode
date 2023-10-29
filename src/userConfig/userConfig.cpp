#include "userConfig.hpp"

json UserConfig::Get() {
	wxString appDataDir = wxStandardPaths::Get().GetUserConfigDir()+"/.thundercode";
	wxFileName appDataDir_op(appDataDir);
	config_path = appDataDir.ToStdString()+"/userconfig.json";

	if(appDataDir_op.GetFullPath() == appDataDir) {
		if(!appDataDir_op.Exists()) {
			bool dir_created = wxFileName::Mkdir(appDataDir);
			if(dir_created) {
				wxFile newConfigFile;
				bool created = newConfigFile.Create(appDataDir+"/userconfig.json");
				if(created) {
					std::ofstream newConfigFile_locale(config_path);
					json new_json_obj = {
					  {"show_minimap", true},
					  {"show_menu", true},
					};
					newConfigFile_locale << std::setw(4) << new_json_obj << std::endl;
				}
			}
		} 
	} 

	json data;

	try
    {
        std::ifstream config_file(config_path);
		if(config_file) {
		data = json::parse(config_file);
	}
    }
    catch (const json::exception& e)
    {
        std::cout << "message: " << e.what() << '\n' << "exception id: " << e.id << std::endl;
    }

	return data;
}

bool UserConfig::Update(json new_data) {
	std::ofstream config_file(config_path);
	if(config_file) {
		config_file << std::setw(4) << new_data << std::endl;
		return true;
	} else return false;
}