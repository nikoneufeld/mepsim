/* 
 * mepreqsim
 * simple-minded stand-alone simulation of the MEP request dataflow in LHCb
 * niko.neufeld@cern.ch 2012
 */

/* clock is 25 ns */

/* sink
   sink has as eventrx process, buffer and eventconsumer process
*/
struct Event {
    int nbyte; 
    int nfrag;
    int tcreate;
};

struct Request {
    int credit;
    int tcreate;
};
/* Farmnode
 * A farmnode has a buffer and workers
 * A farmnode can request an event if it has buffer-space and slots available
 * (function getevent() reduces slots and space)
 * A farmnode can be in two states: waiting for an event (when it has buffer-space) or waiting for buffer
 */ 
enum Nodestate = { WEVT, WBUF }; 
struct Farmnode {
    int m_bslots;
    int m_bused;
    int m_bsize;
    int m_nworker;      
    int m_chunksize;
    int m_address;
    Nodestate m_state;
    struct Worker {
	int tstart; // the time when the worker was launched
	int nbyte; // the number of bytes it occupies in the buffer
	int twork; // the time it takes to free the buffer
	int getevent(const & Event e, int t) { // add an event at time t
	    nbyte = e.nbyte;
	    bused += nbyte;
	    tstart = t;
	    twork = e.nfrag * p_tproc;
	    return 0;
	}
	bool clearevent(int t} {
	    if (tstart + twork <= t) {
		bused -= nbyte;
		bslots++;
		nbyte = twork = tstart = 0;
		return true;
	    } 
	    return false;
	}
    };
    struct Worker worker[64];
    Farmnode(int nworker, int bsize, int bslots, int address, int chunksize) 
    m_bslots(bslots), m_nworker(nworker), m_bslots(bslots), m_address(address),
	m_chunksize(chunksize) {
	m_bused = 0;
	m_state = WEVT;
    }
    ~Farmnode() {}
    bool getevent(const Event & e, int t) {
	if (m_state != WEVT) return false; // event dropped
	
    }
    bool sendreq() {
	if (m_bslots && m_bused > m_chunksize) {
	    m_bslots--;
	    m_bused += m_chunksize;
	    return true;
	}
	return false;
    }
    bool tick(int t) { // just check what happened until now
	for (int i = 0; i < m_nworker; ++i) 
	    worker[i].clearevent(t);
	if (m_state == WEVT) return;
	// we've been waiting for buffer space
	if (sendreq()) {
	    m_state = WEVT;
	    return true;
	}
	return false;
    }
};
    
  
