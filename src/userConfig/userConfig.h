#include <nlohmann/json.hpp>
#include <fstream>
#include <wx/stdpaths.h>
using json = nlohmann::json;

class UserConfig {
	std::string config_path = 
	wxStandardPaths::Get()
	.GetUserConfigDir()
	.ToStdString()+"/.thundercode/userconfig.json";
public:
	json Get();
	bool Update(json new_data);
};