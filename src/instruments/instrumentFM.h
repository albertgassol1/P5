#ifndef INSTRUMENTFM
#define INSTRUMENTFM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"
#include "wavfile_mono.h"

namespace upc {
  class InstrumentFM: public upc::Instrument {
    EnvelopeADSR adsr;
    float phase;
    float step1, step2;
    float nota;
    int N;
    float N1, N2;
    float fm;
    int count;
    float I;
    unsigned int index;
	float A;
    std::vector<float> tbl;
  public:
    InstrumentFM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif