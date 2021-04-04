//
// Created by baoyixing on 3/29/21.
//

#ifndef CODE_PROGRESSBAR_H
#define CODE_PROGRESSBAR_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "Timer.h"


class ProgressBar {
public:
	ProgressBar(unsigned int totalNum, const std::chrono::milliseconds& interval);
	ProgressBar(const ProgressBar&);
	void start();
	void finish();
	void update(unsigned int num) { finishedNum += num; }
	void update() { return update(1); }
	unsigned int getCols() const { return ncols; }
	void setCols(unsigned int n_cols) {
		this->ncols = n_cols;
		this->colsRatio = (double)n_cols / 100;
	}
	void show();

protected:
	unsigned int ncols {100};							// 进度条长度(不包含前后缀)
	std::atomic<unsigned int> finishedNum {0};			// 已完成的数量
	unsigned int lastNum {0};							// 上次完成的数量
	unsigned int totalNum;								// 总数
	double colsRatio {1.0};								// 进度条长度与百分比之间的系数
	std::chrono::steady_clock::time_point beginTime;	// 开始时间
	std::chrono::steady_clock::time_point lastTime;		// 上次重绘的时间
	std::chrono::milliseconds interval;					// 重绘周期
	Timer timer;
};


#endif	  //CODE_PROGRESSBAR_H
