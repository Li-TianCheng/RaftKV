#include "Modules.h"
#include "KVDatabase.h"

int main() {
	modules::init("../config/config.json");
	KVDatabase database;
	database.serve();
	modules::close();
	return 0;
}
