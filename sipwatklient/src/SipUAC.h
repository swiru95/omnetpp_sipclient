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

#ifndef __SIPCLIENTWAT_SIPUAC_H_
#define __SIPCLIENTWAT_SIPUAC_H_

#include "header.h"
#include "func_source\\Functions.h"
#include "func_source\\StatusOfUac.h"
/**
 * TODO - Generated class
 */



typedef map<string, StatusOfUac *> typeS_MapUAC;
typedef map<cMessage *, RequestSIP *> typeRrb_MapUAC;

void *createControlTu(StatusOfUac* ans, int type);

class SipUAC: public inet::ApplicationBase {
public:
    ofstream sipMsgLog;
    string fileName, host; //to PRint


protected:
    virtual int numInitStages() const override {
        return inet::NUM_INIT_STAGES;
    }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void handleSelfMessage(cMessage* msg);
    virtual void setRetransmission(RequestSIP* requestSip, bool isTimerB,
            StatusOfUac * sou);
    virtual void cancelRetransmissions(StatusOfUac * sou);
    virtual int countContentLength(RequestSIP* requestSip);
    virtual void* createIniRequest(ControlTU* controlTu, int type);
    virtual void* createFinRequest(int type, StatusOfUac* sou);
    virtual bool checkAnswer(AnswerSIP *ans, StatusOfUac * sou);
    virtual void finish() override;

private:

    void handleMessageArrivedFromTu(ControlTU * controlTu);
    void handleMessageArrivedFromDtbr(AnswerSIP* answerSip);

    map<string, StatusOfUac *> stateMapUac; //<callId,msg>
    map<cMessage *, RequestSIP *> requestRetransmissionBuffor; //<Timer, Msg>
    map<string, string> callIdMapUac; //<toUser,callId>
    simtime_t T1, T2, T4;

    bool write_logs;
    //statistics
    int createdInv, createdBye, createdCan, createdAck,     //created Req
            sentInv, sentBye, sentCan, sentAck,             //sent Req
            retInv, retBye, retCan, retAck; //canceled retransmission of Req
    int rec180, rec200, rec404, rec406, rec486, rec606;     //received Answers

    simsignal_t stateSignal;


};

#endif
