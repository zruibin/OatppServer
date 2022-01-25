/*
 *
 * platform.cc
 *
 * Created by Ruibin.Chow on 2022/01/25.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#include "platform.h"
#include <pthread.h>

namespace OatppServer {
namespace platform {

int thread_set_name(const char* name)
{
#if defined(__APPLE__)
    return pthread_setname_np(name);
#else
    return pthread_setname_np(pthread_self(), name);
#endif
}

}
}
