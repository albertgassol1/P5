#include <iostream>
#include <math.h>
#include "instrumentFM.h"
#include "keyvalue.h"
#include <errno.h>
#include <string.h>

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentFM::InstrumentFM(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  
  
  /////////////////////////
  //     TAULA NORMAL    //
  /////////////////////////

  if (!kv.to_int("N",N))
    N = 40; //default value

  if (!kv.to_float("N1",N1))
    N1 = 5; //default value
  if (!kv.to_float("N2",N2))
    N2 = 6; //default value
  if (!kv.to_float("I",I))
    I = 1; //default value
  I = 1. - pow(2, -I / 12.);
  
  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  float fase = 0;
  phase = 0;
  index = 0;
  step1 = 2 * M_PI /(float) N;
  //index = 0;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(fase);
    fase += step1;
  }
  
  /*
  /////////////////////////
  //     TAULA FITXER    //
  /////////////////////////
  std::string file_name = "period2.wav";
  static string kv_null;
  /*
  if ((file_name = kv("guitar.wav")) == kv_null) {
    cerr << "Error: no se ha encontrado el campo con el fichero de la señal para un instrumento FicTabla" << endl;
    throw -1;
  }
  
  unsigned int fm;

  if (readwav_mono(file_name, fm, tbl) < 0) {
    
    cerr << "Error: no se puede leer el fichero " << file_name << " para un instrumento FicTabla" << " (" << strerror(errno) << ")"<< endl;
    throw -1;
  }


  N = tbl.size();
  cout<<N<<endl;
  phase = 0;
  index = 0;
  step1 = 2 * M_PI /(float) N;

  */
}


void InstrumentFM::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    float fc = 440.0 * pow(2 ,((float)note-69.0)/12.0);
    //cout<<fc<<endl;
    nota = fc/SamplingRate;
    fm = (N2/N1) * fc;
	A = vel / 127.;
    count = 0;
    step2 = 2 * M_PI * nota;
    //cout<<nota<<", "<<step2/step1<<endl;
    phase = 0;
    index = 0;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentFM::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    
    if(phase == (int)phase){
      
      x[i] = A*tbl[index]; 
   
    }else{

      int index1 = (int)ceil(phase);
      int index2 = (int)floor(phase);
      float alpha1 = (float) index2 - phase;
      float alpha2 = (float) phase - index1;

      if(index2 == N){

        index2 = 0;
      }

      x[i] = 0.8*(tbl[index1]*(alpha1) + tbl[index2]*(alpha2));

    }
    count++;
    step2 = 2*M_PI*(nota + I*(fm/SamplingRate)*cos(2*M_PI*(fm/SamplingRate)*count));
    phase += step2/step1;
    index = phase;
    //cout<<x[i]<<","<<phase<<endl;

    if(index >= N){
      phase = step2/step1;
      index = 0;
      //phase -= resta*(step2/step1);
    }
    //while (phase > 2*M_PI) phase -= 2*M_PI; 
  } 
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
