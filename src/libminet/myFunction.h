#ifndef MYFUN_H
#define MYFUN_H

#include<iostream>

#include "Minet.h"

#include "constate.h"
#include "tcpstate.h"

#define TIMER_START true

#define TEST(str) cout<<"\n======================\nTEST: "<<(#str)<<" "<<str<<"\n========================="<<endl;

using std::cout;
using std::endl;

//mux handle function
extern void handleCLOSED(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleLISTEN(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleSYN_RCVD(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleSYN_SENT(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleSYN_SENT1(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleESTABLISHED(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleSEND_DATA(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleCLOSE_WAIT(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleFIN_WAIT1(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleCLOSING(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleLAST_ACK(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleFIN_WAIT2(const ConnectionToStateMapping<TCPState> & con_state);
extern void handleTIME_WAIT(const ConnectionToStateMapping<TCPState> & con_state);

#endif
