#include <nlohmann/json.hpp>
using json = nlohmann::json;

class UserConfig {
	json data;
public:
	json Get();
};