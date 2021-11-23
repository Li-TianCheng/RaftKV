#include "Modules.h"
#include "KVDatabase.h"

int main() {
	modules::init("../config/config.json");
	KVDatabase::serve();
	modules::close();
	return 0;
}
