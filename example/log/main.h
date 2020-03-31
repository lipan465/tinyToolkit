#ifndef __EXAMPLE__LOG__MAIN__H__
#define __EXAMPLE__LOG__MAIN__H__


/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include <iostream>

#include "log/fileSink.h"
#include "log/syslogSink.h"
#include "log/syncLogger.h"
#include "log/asyncLogger.h"
#include "log/consoleSink.h"
#include "log/regexFilter.h"
#include "log/configurator.h"
#include "log/simpleLayout.h"
#include "log/dailyFileSink.h"
#include "log/patternLayout.h"
#include "log/priorityFilter.h"
#include "log/rotatingFileSink.h"
#include "log/priorityRangeFilter.h"


using namespace tinyToolkit;


#endif // __EXAMPLE__LOG__MAIN__H__
