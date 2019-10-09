__attribute__((visibility("default")))
extern "C" int call_me () {
	return 888;
}

int web_impl () {
	return 777;
}

