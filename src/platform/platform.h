/*
 * platform.h
 *
 * Created by Ruibin.Chow on 2022/01/25.
 * Copyright (c) 2022年 Ruibin.Chow All rights reserved.
 */

#ifndef PLATFORM_H
#define PLATFORM_H

namespace OatppServer {
namespace platform {

int thread_set_name(const char*);
unsigned short thread_get_current_id(void);


}
}

#endif /* !PLATFORM_H */
