#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#include <iostream>

#include "Minet.h"

#include "tcpstate.h"

#include "myFunction.h"


using std::cout;
using std::endl;
using std::cerr;
using std::string;

int main(int argc, char *argv[])
{
  MinetHandle mux, sock;

  ConnectionList<TCPState> clist;

  MinetInit(MINET_TCP_MODULE);

  mux=MinetIsModuleInConfig(MINET_IP_MUX) ? MinetConnect(MINET_IP_MUX) : MINET_NOHANDLE;
  sock=MinetIsModuleInConfig(MINET_SOCK_MODULE) ? MinetAccept(MINET_SOCK_MODULE) : MINET_NOHANDLE;

  if (MinetIsModuleInConfig(MINET_IP_MUX) && mux==MINET_NOHANDLE) {
    MinetSendToMonitor(MinetMonitoringEvent("Can't connect to mux"));
    return -1;
  }

  if (MinetIsModuleInConfig(MINET_SOCK_MODULE) && sock==MINET_NOHANDLE) {
    MinetSendToMonitor(MinetMonitoringEvent("Can't accept from sock module"));
    return -1;
  }

  MinetSendToMonitor(MinetMonitoringEvent("tcp_module handling TCP traffic"));

  MinetEvent event;

cerr<<"BEGIN"<<endl;


  while (MinetGetNextEvent(event)==0) {
    // if we received an unexpected type of event, print error
    if (event.eventtype!=MinetEvent::Dataflow 
	|| event.direction!=MinetEvent::IN) {
      MinetSendToMonitor(MinetMonitoringEvent("Unknown event ignored."));
    // if we received a valid event from Minet, do processing
    } else {
      //  Data from the IP layer below  //
      if (event.handle==mux) {
	Packet p;
	MinetReceive(mux,p);

	bool checksumok;
	unsigned tcphlen=TCPHeader::EstimateTCPHeaderLength(p);
	cerr << "estimated header len="<<tcphlen<<"\n";
	p.ExtractHeaderFromPayload<TCPHeader>(tcphlen);
	IPHeader ipl=p.FindHeader(Headers::IPHeader);
	TCPHeader tcph=p.FindHeader(Headers::TCPHeader);

	cerr << "TCP Packet: IP Header is "<<ipl<<" and ";
	cerr << "TCP Header is "<<tcph << " and ";

	cerr << "Checksum is " << (tcph.IsCorrectChecksum(p) ? "VALID" : "INVALID");

	//connection, tuple
	Connection c;
	ipl.GetDestIP(c.src);
	ipl.GetSourceIP(c.dest);
	ipl.GetProtocol(c.protocol);
	tcph.GetDestPort(c.srcport);
	tcph.GetSourcePort(c.destport);


	//flag
	unsigned char flag = 0;

	//SYN
	tcph.GetFlags(flag);

	TEST((int)flag);

	//FOR TEST
	if(IS_SYN(flag)) {
	    TEST("add LISTEN state connection");
	    TCPState ts(100, LISTEN, 3);
	    ConnectionToStateMapping<TCPState> m(c, Time(3), ts, TIMER_START);
	    clist.push_back(m);
	}

	//first check whether there's already a connection
	ConnectionList<TCPState>::iterator cs = clist.FindMatching(c);
	
	if(cs!=clist.end()) {
	//if there is an opening socket matching

	    // packet corrupt
	    if(!tcph.IsCorrectChecksum()) {
		unsigned char flag = 0;
		SET_ACK(flag);
		MinetSend(makePacket(*cs, NULL, flag), pcorrupt);
	    } else {
	    
		//if-else or switch for each STATE
		//app layer should tell trans layer to create a connection with LISTEN state
		switch(cs->state.GetState()) {
		    case CLOSED: 
		    case LISTEN: TEST("BEGIN LISTEN");
				 if(!IS_SYN(flag)) {}
				 break;
		    case SYN_RCVD:
		    case SYN_SENT:
		    case SYN_SENT1:
		    case ESTABLISHED:
		    case SEND_DATA:
		    case CLOSE_WAIT:
		    case FIN_WAIT1:
		    case CLOSING:
		    case LAST_ACK:
		    case FIN_WAIT2:
		    case TIME_WAIT:
		    default: cerr<<"Wrong Connection State."<<endl;
		} 
	    }

	} else {
	    cerr<<"No such connection."<<endl;
	}
	
      }
      //  Data from the Sockets layer above  //
      if (event.handle==sock) {
	SockRequestResponse s;
	MinetReceive(sock,s);
	cerr << "Received Socket Request:" << s << endl;
	SockRequestResponse repl;
	repl.type = STATUS;
	repl.error=EWHAT;
	MinetSend(sock, repl);
      }
    }
    cerr<<"Cycle End"<<endl;
  }
  return 0;
}


