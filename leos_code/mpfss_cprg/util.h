//ensures header file is only included once
#pragma once

void ocTestUtilTcpOrDie(struct ProtocolDesc* pd,const char* remote_host,
                        const char* port);
double wallClock();

