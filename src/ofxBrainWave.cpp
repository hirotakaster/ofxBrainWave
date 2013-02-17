#include <iostream>
#include "ofxBrainWave.h"

ofxBrainWave::ofxBrainWave() {
    host = "127.0.0.1";
    port = 13854;
}

ofxBrainWave::~ofxBrainWave() {
    if (this->isThreadRunning())
        stop();
    tcpClient.close();
}

void ofxBrainWave::setHost(string h) {
    host = h;
}

void ofxBrainWave::setPort(int p) {
    port = p;
}

void ofxBrainWave::setup() {
    tcpClient.setup(host, port);
    tcpClient.setMessageDelimiter("\r");
    tcpClient.send("{""enableRawOutput"": true, ""format"": ""Json""}");
}

void ofxBrainWave::stop() {
    stopThread();
}

void ofxBrainWave::start() {
    startThread();
}

void ofxBrainWave::threadedFunction() {
    while (this->isThreadRunning()) {
        this->checkStream();
    } 
}

void ofxBrainWave::checkStream() {
    ofxBrainWaveScopedLock scopedLock(mutex);

    if (tcpClient.isConnected()) {
        string val = tcpClient.receive();
        if (val.length() > 0 && json.parse(val)) {
            if (json.isMember("eegPower")) {
                eegData.delta = json["eegPower"]["delta"].asInt();
                eegData.theta = json["eegPower"]["theta"].asInt();
                eegData.lowAlpha = json["eegPower"]["lowAlpha"].asInt();
                eegData.highAlpha = json["eegPower"]["highAlpha"].asInt();
                eegData.lowBeta = json["eegPower"]["lowBeta"].asInt();
                eegData.highBeta = json["eegPower"]["highBeta"].asInt();
                eegData.lowGamma = json["eegPower"]["lowGamma"].asInt();
                eegData.highGamma = json["eegPower"]["highGamma"].asInt();
            }
            
            if (json.isMember("eSense")) {
                eegData.attention = json["eSense"]["attention"].asInt();
                eegData.meditation = json["eSense"]["meditation"].asInt();
            }
        }
    }
}

EegData ofxBrainWave::getEegData() {
    ofxBrainWaveScopedLock scopedLock(mutex);
    return eegData;
}
