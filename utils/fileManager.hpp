#pragma once

#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <functional>
#include <bits/stdc++.h>

namespace fs = std::filesystem;

class FileManager {
public:
	void ListAllFiles(const std::string &path, std::function<void(const std::string&, const std::string&, const std::string&)> cb) {
		if (auto dir = opendir(path.c_str())) {
	        while (auto f = readdir(dir)) {
	            if (!f->d_name || f->d_name[0] == '.') continue;
	            if (f->d_type == DT_DIR) {
	                cb(path + f->d_name, "dir", f->d_name);
	                ListAllFiles(path + f->d_name + "/", cb);
	            }
	            if (f->d_type == DT_REG)
	                cb(path + f->d_name, "file", f->d_name);
	        }
	        closedir(dir);
	    }
	}

	void ListChildrens(const std::string &path, std::function<void(const std::string&, const std::string&, const std::string&)> cb) {
	    if (auto dir = opendir(path.c_str())) {
	    	std::vector<std::string> dir_content;
	        while (auto f = readdir(dir)) {
	            if (!f->d_name || f->d_name[0] == '.') continue;
	            if (f->d_type == DT_DIR) {
		            dir_content.push_back(f->d_name);
	            }
	            if (f->d_type == DT_REG) {
		            dir_content.push_back(f->d_name);
	            }
	        }

			std::sort(dir_content.begin(), dir_content.end());


	        for(auto dc : dir_content) {
	        	std::cout << dc << " dc\n";
	        }

	        closedir(dir);
	    }
	}

	int GetChildrensCount(wxString path) {
		int childrens_count = 0;
		if (auto dir = opendir(path.c_str())) {
	        while (auto f = readdir(dir)) {
	            if (!f->d_name || f->d_name[0] == '.') continue;
	            childrens_count+1;
	        }
	        closedir(dir);
	    }
	    return childrens_count;
	}

	std::string GetParentPath(std::string path) {
		wxFileName path_props = wxFileName::DirName(path);
		std::cout << path_props.GetFullName() << "\n";
		
		std::string parent_path = path.substr(0, path.find(path_props.GetName())-1);
		return parent_path;
	}
};