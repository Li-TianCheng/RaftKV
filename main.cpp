#include "Modules.h"
#include "KVDatabase.h"

int main(int argc, char *argv[]) {
	int opt;
	string path;
	const char *optString = "c:";
	while ((opt = getopt(argc, argv, optString)) != -1) {
		if (opt == 'c') {
			path = optarg;
		}
	}
	modules::init(path);
	KVDatabase::serve();
	modules::close();
	return 0;
}
