#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include <iomanip>

void swapbin_27c801(const char* src, char* dst) {
	memcpy(dst, src, 1 << 20);
	memcpy(dst + 262144, src + 65536, 1 << 16);
	memcpy(dst + 524288, src + 131072, 1 << 16);
	memcpy(dst + 65536, src + 262144, 1 << 16);
	memcpy(dst + 131072, src + 524288, 1 << 16);
	memcpy(dst + 589824, src + 393216, 1 << 16);
	memcpy(dst + 786432, src + 196608, 1 << 16);
	memcpy(dst + 196608, src + 786432, 1 << 16);
	memcpy(dst + 393216, src + 589824, 1 << 16);
	memcpy(dst + 851968, src + 458752, 1 << 16);
	memcpy(dst + 458752, src + 851968, 1 << 16);
	memcpy(dst + 917504, src + 720896, 1 << 16);
	memcpy(dst + 720896, src + 917504, 1 << 16);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "No (valid) input filename given, aborting" << std::endl;
		return 1;
	}
	std::ifstream in(argv[1], std::ifstream::ate | std::ifstream::binary);
	unsigned long fs = in.tellg();
	if (fs & 0xFFFFF) {
		std::cout << "File size not 1MB aligned, aborting" << std::endl;
		return 1;
	}

	std::string filename(argv[1]);
	size_t lastdot = filename.find_last_of(".");
	auto fn = lastdot == std::string::npos
		? std::make_tuple(filename, "")
		: std::make_tuple(filename.substr(0, lastdot), filename.substr(lastdot));

	in.seekg(0, std::ios::beg);
	unsigned long p;
	char* bin = new char[1 << 20];
	char* bout = new char[1 << 20];
	for (p = 0; p < fs >> 20; p++) {
		in.read(bin, 1 << 20);
		swapbin_27c801(bin, bout);
		std::string fnout = std::get<0>(fn) + "_27C801_part" + (char)('0' + p) + std::get<1>(fn);
		std::ofstream out(fnout, std::ios::out | std::ios::trunc | std::ios::binary);
		out.write(bout, 1 << 20);
		out.close();
	}
	delete[] bin;
	delete[] bout;
	std::cout << "Finished, " << p << " output files written" << std::endl;
	return 0;
}