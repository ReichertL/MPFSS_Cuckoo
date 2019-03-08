//ensures header file is only included once
//#pragma once

#ifndef UTIL_H
#define UTIL_H
#endif

void ocTestUtilTcpOrDie(struct ProtocolDesc* pd,const char* remote_host,
                        const char* port);
double wallClock();
void benchmark(double runtime, size_t size, int t, int cp, char type[]);


