#pragma once
#ifdef GLOG_OUTPUT
#include <glog/logging.h>
#define  OUTPUT LOG(INFO)
#else
#include <iostream>
#define OUTPUT std::cout
#endif