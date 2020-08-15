#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

std::string get_stdout_from_cli(std::string cmd){

	std::string data;
	FILE* stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");
	if(stream){
		while(!feof(stream))
			if(fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
	pclose(stream);
	}

	return data;
}

void gradleWriter(const std::unordered_map<std::string, std::string>& inner,
		const std::unordered_map<std::string, std::string>& outer){

	std::ifstream outer_gradle("android/build.gradle");
	if(!outer_gradle.is_open()){
		std::cout<<"Project level gradle file couldn't find under android folder.\n";
		exit(1);
	}
	std::string str = "";
	char c;
	int is_repo = 0;
	while(outer_gradle>> std::noskipws >> c){
		str += c;
	}
	outer_gradle.close();

	std::size_t found = str.find("repositories"); 
	if(found != std::string::npos){
		// put maven.
		for(std::size_t it = found; it<str.length(); ++it)
			if(str[it] == '}') {str.insert(it-1, outer.at("maven")+"\n");break;}
	}
	found = str.rfind("repositories");
	if(found != std::string::npos){
		// put maven.
		for(std::size_t it= found; it<str.length(); ++it)
			if(str[it] == '}') {str.insert(it-1, outer.at("maven")+"\n");break;}

	}

	found = str.find("dependencies");
	if(found != std::string::npos){
		// put dependency
		for(std::size_t it=found; it<str.length(); ++it)
			if(str[it] == '}') {str.insert(it-1, outer.at("agcp")+"\n");break;}
	}

	std::ofstream outer_grad("android/build.gradle");
	outer_grad << str;
	outer_grad.close();

	// //////////////////////////////
	// INNER GRADLE PART 
	/// /////////////////////////////

	std::ifstream inner_gradle("android/app/build.gradle");

	str = "";
	while(inner_gradle >> std::noskipws >> c)
		str += c;

	found = str.find("release");
	std::string put_signing_configs = "\nsigningConfig signingConfigs.release\n";
	std::string put_debug = "\ndebug{\nsigningConfig signingConfigs.release\ndebuggable true\n}\n";
	if(found != std::string::npos)
		for(std::size_t it=found; it<str.length(); ++it)
			if(str[it] == '{')str.insert(it+1, put_signing_configs);
			else if(str[it] == '}') {str.insert(it+1, put_debug); break;}

	found = str.find("defaultConfig");
	if(found != std::string::npos){
		for(std::size_t it = found; it < str.length(); ++it)
			if(str[it] == '}') {str.insert(it+1, "\n"+inner.at("sign_conf")+"\n"); break;}
	}
	found = str.find("dependencies");
	if(found != std::string::npos){
		for(std::size_t it=found; it<str.length(); ++it)
			if(str[it] == '}') {str.insert(it, inner.at("dependency")+"\n"); break;}
	}

	std::ofstream inner_grad("android/app/build.gradle");
	inner_grad << str;
	inner_grad.close();

}

void ionic_zero_to_hero(const std::unordered_map<std::string, std::string>& conf,
		const std::unordered_map<std::string, std::string>& inner,
		const std::unordered_map<std::string, std::string>& outer){

	// from zero to hero.. ionic build
	/*
	 *
	 *
	 * > npm install
	 * > npm install <plugin-path>
	 * > npm install @ionic-native/core --save-dev
	 * > cp node_modules/@hmscore/<plugin-name>/ionic/dist/<plugin-last-name> node_modules/@ionic-native/ -r
	 * > ionic build
	 * > npx cap init [appName] [appId]
	 * > ionic cap add android
	 * > Write gradle files get it from config, inner gradle and outer gradle
	 * > Copy agconnect and jks from desired location.
	 * > npx cap sync
	 * > ionic cap run android
	 *
	 *
 * */

	std::string tillDesktop = "C:/Users/"+conf.at("user")+"/Desktop/";

	std::vector<std::string> steps;
	steps.push_back("npm install ");
	steps.push_back("npm install ../"+conf.at("plugin-path"));
	steps.push_back("npm install @ionic-native/core --save-dev");
	steps.push_back("cp node_modules/@hmscore/"+conf.at("plugin-name")+"/ionic/dist/"+conf.at("plugin-last-name")+" node_modules/@ionic-native/ -r");
	steps.push_back("ionic build");
	steps.push_back("npx cap init " + conf.at("app-name")+ " " + conf.at("app-id"));
	steps.push_back("ionic cap add android");
	steps.push_back("cp ../agconnect-services.json android/app/ -r");
	steps.push_back("cp ../"+conf.at("jks")+ " android/app/ -r");
	steps.push_back("npx cap sync");
	steps.push_back("ionic cap run android");

	std::cout<<"Step 1\n\t"<<steps[0]<<"..... processing\n";
	get_stdout_from_cli(steps[0]);
	std::cout<<"Step 2\n\t"<<steps[1]<<"..... processing\n";
	get_stdout_from_cli(steps[1]);
	std::cout<<"Step 3\n\t"<<steps[2]<<"..... processing\n";
	get_stdout_from_cli(steps[2]);
	std::cout<<"Step 4\n\t"<<steps[3]<<"..... processing\n";
	get_stdout_from_cli(steps[3]);
	std::cout<<"Step 5\n\t"<<steps[4]<<"..... processing\n";
	get_stdout_from_cli(steps[4]);
	std::cout<<"Step 6\n\t"<<steps[5]<<"..... processing\n";
	get_stdout_from_cli(steps[5]);
	std::cout<<"Step 7\n\t"<<steps[6]<<"..... processing\n";
	get_stdout_from_cli(steps[6]);
		
	// run gradler writer here......
	gradleWriter(inner, outer);

	std::cout<<"Step 8\n\t"<<steps[7]<<"..... processing\n";
	get_stdout_from_cli(steps[7]);
	std::cout<<"Step 9\n\t"<<steps[8]<<"..... processing\n";
	get_stdout_from_cli(steps[8]);
	std::cout<<"Step 10\n\t"<<steps[9]<<"..... processing\n";
	get_stdout_from_cli(steps[9]);
	std::cout<<"Step 11\n\t"<<steps[10]<<"..... processing\n";
	get_stdout_from_cli(steps[10]);
	
	std::cout<<"Following steps listed below.\n";
	for(int i=0; i<steps.size(); ++i){
		std::cout<<"Step "<<i+1<<"\n\t- "<<steps[i]<<std::endl;
	}
}

int main(){


	std::unordered_map<std::string, std::string> configurations;
	std::unordered_map<std::string, std::string> gradle_inner_map;
	std::unordered_map<std::string, std::string> gradle_outer_map;


	std::ifstream conf("config");
	if(!conf.is_open()){
		std::cout<<"Config file couldn't found, please check!\n";
		exit(1);
	}
	std::string data;
	bool is_plugin_name= false, is_plugin_path= false, is_app_name= false;
	bool is_app_id= false, is_plugin_last_name= false, is_user= false, is_jks= false;
	while(conf >> data){
		if(data == "plugin-name") is_plugin_name= true;
		else if(data == "plugin-path") is_plugin_path= true;
		else if(data == "plugin-last-name") is_plugin_last_name= true;
		else if(data == "app-id") is_app_id = true;
		else if(data == "app-name") is_app_name= true;
		else if(data == "user") is_user = true;
		else if(data == "jks") is_jks = true;
		else{
			if(is_plugin_path) configurations["plugin-path"]= data;
			else if(is_plugin_name) configurations["plugin-name"]= data;
			else if(is_app_name) configurations["app-name"]= data;
			else if(is_app_id) configurations["app-id"]= data;
			else if(is_plugin_last_name) configurations["plugin-last-name"]= data;
			else if(is_user) configurations["user"]= data;
			else if(is_jks) configurations["jks"]= data;
			is_plugin_path= false;
			is_plugin_name= false;
			is_app_name= false;
			is_app_id= false;
			is_plugin_last_name= false;
			is_jks= false;
			is_user = false;
		}
	}

	
	std::ifstream sign("signingConfigs");
	std::string sign_conf = "";
	if(sign.is_open()){
		char c;
		while(sign >> std::noskipws >>c) sign_conf += c;
	}else{
		std::cout<<"Signing configs file couldn't find, please check it!\n";
		exit(1);
	}
	sign.close();

	gradle_inner_map["sign_conf"] = sign_conf;
	

	std::ifstream gradle("gradle");
	if(gradle.is_open()){
		std::string data;
		bool is_maven = false, is_agcp = false, is_dependency = false;
		while(gradle >> data){
			if(data == "maven") is_maven = true;
			else if(data == "agcp") is_agcp = true;
			else if(data == "dependency") is_dependency = true;
			else{
				// Add to dictionary.
				if(is_maven){
					gradle_outer_map["maven"]= data;
					is_maven = false;
				}else if(is_agcp){
					gradle_outer_map["agcp"]= data;
					is_agcp = false;
				}else if(is_dependency){
					gradle_inner_map["dependency"] += data +"\n";
					is_dependency= false;
				}
			}
		}
	}else{
		std::cout<<"Gradle file couldn't find, please check it!\n";
	}
	gradle.close();

//	std::cout<<gradle_inner_map["sign_conf"]<<"\n";
//	std::cout<<"maven "<<gradle_outer_map["maven"]<<"\n";
//	std::cout<<"agcp "<<gradle_outer_map["agcp"]<<"\n";
//	std::cout<<"dependency "<<gradle_inner_map["dependency"];

	// std::cout<<"Started processing...\n";
	
	
	ionic_zero_to_hero(configurations, gradle_inner_map, gradle_outer_map);

	return 0;
}
