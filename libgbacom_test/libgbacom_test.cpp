// libgbacom_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <VBA/VBAIOManager.h>

void TestJoyboot() {
	GBACom::VBA::VBAIOManager test_manager;

	char status = 0;

	std::ifstream infile("D:\\Downloads\\sonic_adventure_2_-_tiny_chao_garden_(amy)_(multiboot)\\TCG_GC.mb", std::ios::in | std::ios::binary | std::ios::ate);
	uint32_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	char* file_data = new char[0x40000];
	infile.read(file_data, length);

	test_manager.JoyBoot(file_data, length, 0x47534245, status);

	delete[] file_data;

	GBACom::VBA::VBAConnectionWaiter_Shutdown();
}

int main()
{
	TestJoyboot();
}