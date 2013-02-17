#ifndef emptyExample_brainWave_h
#define emptyExample_brainWave_h

#include "ofxJSONElement.h"
#include "ofxNetwork.h"
#include "ofThread.h"

class EegData {
    public :
    EegData() { delta = theta = lowAlpha = highAlpha = lowBeta = highBeta = lowGamma = highGamma = attention = meditation = 0;}
    float delta;
    float theta;
    float lowAlpha;
    float highAlpha;
    float lowBeta;
    float highBeta;
    float lowGamma;
    float highGamma;
    float attention;
    float meditation;
};

class ofxBrainWave : public ofThread {
    private:
        string host;
        int port;
        ofxTCPClient tcpClient;
        ofxJSONElement json;
        EegData eegData;

    protected:
        void threadedFunction();
        void checkStream();
    
    public:
        ofxBrainWave();
        ~ofxBrainWave();
        void setHost(string h);
        void setPort(int p);
        void setup();
        void stop();
        void start();    
    
        EegData getEegData();
};


class ofxBrainWaveScopedLock {
public:
    ofxBrainWaveScopedLock(ofMutex & _mutex): mutex(_mutex){
        mutex.lock();
    };
    ~ofxBrainWaveScopedLock(){
        mutex.unlock();
    };
    ofMutex & mutex;
};

#endif
