#include "ProgressBar.h"

#include <time.h>

#include <climits>

ProgressBar::ProgressBar(unsigned int totalNum, const std::chrono::milliseconds& interval): ncols(80), finishedNum(0), lastNum(0), totalNum(totalNum), colsRatio(0.8), interval(interval) {}

void ProgressBar::start() {
	this->beginTime = std::chrono::steady_clock::now();
	this->lastTime = this->beginTime;
	this->timer.start(this->interval.count(), std::bind(&ProgressBar::show, this));
}

void ProgressBar::finish() {
	this->timer.stop();
	std::cout << std::endl;
}

void ProgressBar::show() {
	std::cout << std::unitbuf;
	std::cout << "\r";
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	unsigned int tmpFinished = this->finishedNum.load();
	auto timeFromStart = now - this->beginTime;
	auto timeFromLast = now - this->lastTime;
	unsigned int gap = tmpFinished - this->lastNum;
	double rate = gap / std::chrono::duration<double>(timeFromLast).count();
	double present = (100.0 * tmpFinished) / this->totalNum;
	std::cout << std::setprecision(1) << std::fixed << present << "%|";
	auto barWidth = (unsigned int)(present * this->colsRatio);
	std::cout << std::setw(barWidth) << std::setfill('=') << "=";
	std::cout << std::setw(this->ncols - barWidth) << std::setfill(' ') << "|";
	std::cout << std::setprecision(1) << std::fixed << rate << "op/s|";
	int timeFromStartCount = std::chrono::duration<double>(timeFromStart).count();

	std::time_t tfs = timeFromStartCount;
	std::tm tmfs;
	gmtime_r(&tfs, &tmfs);
	std::cout << std::put_time(&tmfs, "%X") << "|";

	int timeLast;
	if (rate != 0) {
		timeLast = (int)((this->totalNum - tmpFinished) / rate);
	}
	else {
		timeLast = INT_MAX;
	}

	if ((this->totalNum - tmpFinished) == 0) {
		timeLast = 0;
	}


	std::time_t tl = timeLast;
	std::tm tml;
	gmtime_r(&tl, &tml);
	std::cout << std::put_time(&tml, "%X");
	std::cout << std::nounitbuf;

	this->lastNum = tmpFinished;
	this->lastTime = now;
}

ProgressBar::ProgressBar(const ProgressBar&) {
}
