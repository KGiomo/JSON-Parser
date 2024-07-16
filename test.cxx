#include "json.cxx"

int main(int argc, char **argv)
{
	std::string json_filename("Nasa.json");
	json j;
	std::fstream input_stream(json_filename.c_str());
	// check if file has been opened correctly...
	input_stream >> j;
	
	//std::cout << j << "\n";
	
	json x;
	x.set_string("-123.4");
	std::cout << (*(++j.begin_list()));
	(*(++j.begin_list()))["moid_au"] = x;
	std::cout << (*(++j.begin_list()));
	return 0;
}

