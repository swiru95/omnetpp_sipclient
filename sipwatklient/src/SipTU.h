//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __SIPCLIENTWAT_SIPTU_H_
#define __SIPCLIENTWAT_SIPTU_H_

#include "header.h"
#include "func_source\\Functions.h"
/**
 * TODO - Generated class
 */
enum TuState {
    BUSY = 21, STANDBY = 22, READY2CALL=23
};
class CallUser {
public:
    string name;
    double startTime, stopTime;
    L3Address address;

    CallUser() {
    }
    bool operator<(const CallUser &u) const {
        return startTime < u.startTime;
    }
    void printUser() const {
        EV << "Call2User: " << name << "@" << address.str() << endl
                  << "Start at: " << startTime << endl << "End at: " << stopTime
                  << endl;
    }
};

class SipTU: public inet::ApplicationBase {

public:
    bool isTalking;
    int tuState;
    string localUser, myVoiceCodec;
    double delayTime;
    set<CallUser> usersList;
    cMessage* timerStart, *timerStop, *delayTimer;

protected:
    virtual int numInitStages() const override {
        return inet::NUM_INIT_STAGES;
    }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual ControlTU* createControlTu(set<CallUser>::iterator it,int infoC);
    virtual ControlTU* createControlTu(int infoC, const char* call2user);
    virtual void handleSelfMessage(cMessage *msg);
    virtual void handleMessageArrivedFromUas(ControlTU* controlTu);
    virtual void handleMessageArrivedFromUac(ControlTU* controlTu);
    virtual void finish() override;

    int recCALL, recTERM, recCANT,
        sentCALL, sentTERM, sentCANT,
        rec200, rec404, rec406, rec486, rec606,
        sent200, sent404, sent406, sent486, sent606;

    simsignal_t stateSignal;

};

#endif
