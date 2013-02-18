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

Packet & makePacket(const ConnectionToStateMapping<TCPState> & con2state, const Buffer & buf, unsigned char flag) {

    Packet & p = * new Packet();

    //IP
    IPHeader hip;
    hip.SetProtocol(IP_PROTO_TCP);
    hip.SetSourceIP(con2state.connection.src);
    hip.SetDestIP(con2state.connection.dest);

    //TCP
    TCPHeader htcp;
    htcp.SetSourcePort(con2state.connection.srcport, p);
    htcp.SetDestPort(con2state.connection.destport, p);
    htcp.SetSeqNum(con2state.state.GetLastRecvd(), p);
    htcp.SetAckNum();

    //flag
    htcp.SetFlags(flag, pcorrupt);

    //Data

    //length
    unsigned tcp_header_length = htcp.EstimateTCPHeaderLength(p);

    htcp.SetHeaderLen(tcp_header_length, p);

    hip.SetTotalLength(tcp_header_length+IP_HEADER_BASE_LENGTH, pc);

    p.pushFrontHeader(hip);
    p.PushBackHeader(htcp);

    return p;
}
