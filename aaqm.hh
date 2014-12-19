#ifndef AAQM_HH
#define AAQM_HH
#include <click/element.hh>
#include <stdio.h>
#define MQOS1 100
#define MQOS2 40
#define MQOS3 30
#define MQOS4 10
#define MQOS5 5
#define BUFFER_SIZE 200

CLICK_DECLS

/*
 * =c
 * AAQM()
 * =s debugging
 * demonstrates how to write a package
 * =d
 *
 * This is the only element in the `sample' package. It demonstrates how to
 * write an element that will be placed in a package. It does nothing except
 * report that the package was successfully loaded when it initializes. */

class AAQM : public Element { public:

    AAQM();		// SEE sample.cc FOR CONSTRUCTOR
    ~AAQM();		// SEE sample.cc FOR DESTRUCTOR

    const char *class_name() const	{ return "AAQM"; }

    //exactly 5 input and 5 output ports
     const char *port_count() const { return "5/5"; }

     //push input+pull output INPUT:5 inputs from the IP classifier. OUTPUT: 5 outputs to the Round Robin scheduler
     const char *processing() const     { return "h/l"; }


    void push(int port, Packet *p);
    Packet* pull(int port);

    int initialize(ErrorHandler *errh);
    void add_handlers();
    static String read_handler(Element *, void *) CLICK_COLD;
    //FILE *log_file;

    typedef struct PacketHolder{
    	Packet *p;
    	struct PacketHolder *next;
    	struct PacketHolder *pre;
    }PacketHolder;

    typedef struct packetHolderList{
    	PacketHolder *head;
    	PacketHolder *tail;
#ifdef HAVE_INT64_TYPES
        unsigned long counter;
	unsigned long dropNum;
	unsigned long total;
#else
        unsigned long counter;
	unsigned long dropNum;
	unsigned long total;
#endif
    }packetHolderList;

    packetHolderList *Queue1;
    packetHolderList *Queue2;
    packetHolderList *Queue3;
    packetHolderList *Queue4;
    packetHolderList *Queue5;

    
    //minimal qos of each queue
    unsigned long temp_count = 0;
    unsigned long pull_counter = 0;
    unsigned long delete_counter = 0;
    int test_counter=0;


    void insertIntoQueue(packetHolderList **, Packet **);
    void deleteTail(packetHolderList **Queue);
    Packet* dequeuePacket(packetHolderList **Queue);
    
    void checkq1(Packet* p);
    void checkq2(Packet* p);
    void checkq3(Packet* p);
    void checkq4(Packet* p);
    void checkq5(Packet* p);


};

CLICK_ENDDECLS
#endif
