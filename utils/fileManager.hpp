#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <dirent.h>

namespace fs = std::filesystem;

class FileManager {
public:
	void listFiles(const std::string &path, std::function<void(const std::string &)> cb) {
	    if (auto dir = opendir(path.c_str())) {
	        while (auto f = readdir(dir)) {
	            if (!f->d_name || f->d_name[0] == '.') continue;
	            if (f->d_type == DT_DIR) 
	                listFiles(path + f->d_name + "/", cb);
	            if (f->d_type == DT_REG)
	                cb(path + f->d_name);
	        }
	        closedir(dir);
	    }
	}
};