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
	        while (auto f = readdir(dir)) {
	            if (!f->d_name || f->d_name[0] == '.') continue;
	            if (f->d_type == DT_DIR) {
	            	if(path.substr(path.size()-1) != "/") {
		                cb(path+"/"+f->d_name, "dir", f->d_name);
	            	} else cb(path+f->d_name, "dir", f->d_name);
	            }
	            if (f->d_type == DT_REG) {
	            	if(path.substr(path.size()-1) != "/") {
		                cb(path+"/"+f->d_name, "file", f->d_name);
	            	} else cb(path+f->d_name, "file", f->d_name);
	            }
	        }
	        closedir(dir);
	    }
	}

	bool CreateFile(wxString path) {
		wxFile fname;
		bool created = fname.Create(path);
		return created;
	}

	bool CreateDir(wxString path) {
		bool created = wxFileName::Mkdir(path);
		return created;
	}

	bool DeleteFile(wxString path) {
		int deleted = remove(path);
		if(!deleted) return false;
		return true;
	}

	bool DeleteDir(wxString path) {
		bool deleted = wxFileName::Rmdir(path);
		return deleted;
	}
};