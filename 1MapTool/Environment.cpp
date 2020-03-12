#include "Parser.h"
#include "Environment.h"

Environment::Environment()
{
	Parser parser	{};
	parser.load_file("config.ini");

	parser.get_bool( "debug_mode", this->debug_mode );
}

Environment::~Environment()
{
}
