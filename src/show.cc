#include "show.h"

void show(size_t origin_sz, size_t after_sz, double time) {
	double perc = (double)after_sz / (double)origin_sz;
	std::cout << "\n";
	std::cout << std::setw(120) << std::setfill('-') << "\n";
	std::cout << std::setfill(' ') << std::setiosflags(std::ios::left) << std::setw(30) << "origin size" << std::setw(30) << "after size" << std::setw(30) << "percentage" << std::setw(30) << "time" << std::resetiosflags(std::ios::left) << std::endl;

	std::cout << std::setfill('-') << std::setw(120) << "\n";

	std::cout << std::setfill(' ') << std::setiosflags(std::ios::left) << std::setw(30) << origin_sz << std::setw(30) << after_sz << std::setw(30) << std::setprecision(4) << std::fixed << perc << std::setw(30) << std::setprecision(6) << time << std::resetiosflags(std::ios::left) << "\n\n";

	std::cout << std::setw(120) << std::setfill('-') << "\n";
	std::cout << std::setfill(' ');
	return;
}
