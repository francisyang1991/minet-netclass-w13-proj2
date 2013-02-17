#include "myFunction.h"

//mux handle function
void handleCLOSED(const ConnectionToStateMapping<TCPState> & con_state){}
void handleLISTEN(const ConnectionToStateMapping<TCPState> & con_state){
    TEST("BEGIN LISTEN");
}
void handleSYN_RCVD(const ConnectionToStateMapping<TCPState> & con_state){}
void handleSYN_SENT(const ConnectionToStateMapping<TCPState> & con_state){}
void handleSYN_SENT1(const ConnectionToStateMapping<TCPState> & con_state){}
void handleESTABLISHED(const ConnectionToStateMapping<TCPState> & con_state){}
void handleSEND_DATA(const ConnectionToStateMapping<TCPState> & con_state){}
void handleCLOSE_WAIT(const ConnectionToStateMapping<TCPState> & con_state){}
void handleFIN_WAIT1(const ConnectionToStateMapping<TCPState> & con_state){}
void handleCLOSING(const ConnectionToStateMapping<TCPState> & con_state){}
void handleLAST_ACK(const ConnectionToStateMapping<TCPState> & con_state){}
void handleFIN_WAIT2(const ConnectionToStateMapping<TCPState> & con_state){}
void handleTIME_WAIT(const ConnectionToStateMapping<TCPState> & con_state){}

