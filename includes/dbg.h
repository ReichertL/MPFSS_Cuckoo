// Code adapted from http://www.c.learncodethehardway.org/book/ex20.html
// Original Author: Zed Shaw
#pragma once

#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>



#ifdef INFO
#define log_info(M, ...) fprintf(stdout, "[INFO] (%s:%s:L%d) " M, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define debug(M, ...)

#elif DEBUG
#define debug(M, ...) fprintf(stdout, "DEBUG %s:%s:L%d: " M "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stdout, "[INFO] (%s:%s:L%d) " M, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else
#define debug(M, ...)
#define log_info(M, ...)

#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))



#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%s:L%d: errno: %s) " M, __FILE__, __FUNCTION__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define test_fail(M, ...) fprintf(stdout, "[TEST FAIL] (%s:%s:L%d: ) " M, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)




#endif
