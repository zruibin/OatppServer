/*
 * util.h
 *
 * Created by Ruibin.Chow on 2022/01/27.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace OatppServer {
namespace util {

std::string get_current_time_string(void);
long get_current_time_seconds(void);
long get_current_time_milliseconds(void);

}
}

#endif /* !UTIL_H */
