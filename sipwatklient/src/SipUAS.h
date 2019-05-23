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

#ifndef __SIPCLIENTWAT_SIPUAS_H_
#define __SIPCLIENTWAT_SIPUAS_H_

#include "header.h"
#include "func_source\\Functions.h"
#include "func_source\\StatusOfUas.h"
using namespace omnetpp;

/**
 * TODO - Generated class
 */

typedef map<cMessage *, AnswerSIP *> typeRrb_MapUAS;
typedef map<string, StatusOfUas *> typeS_MapUAS;


void *createControlTu(StatusOfUas* ans, int type);
//SipUas
class SipUAS: public inet::ApplicationBase {
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
    virtual void setRetransmission(AnswerSIP* answerSip,
            StatusOfUas * sou);
    virtual void cancelRetransmissions(StatusOfUas * sou);
    virtual void* createAnswer(int type, StatusOfUas *sou);
    virtual bool checkRequest(RequestSIP * req, StatusOfUas *sou);
    virtual void finish() override;

private:

    void handleMessageArrivedFromTu(ControlTU* controlTu);
    void handleMessageArrivedFromDtbr(RequestSIP* answerSip);

    map<string, StatusOfUas *> stateMapUas; //<callId,msg>
    map<cMessage *, AnswerSIP *> requestRetransmissionBuffor; //<timer,msg>
    map<string, string> callIdMap; //<toUser,callId>
    simtime_t T1, T2, T4;

    bool write_logs;
    //statistics
    int create180, create200, create404, create406, create486, create606,
        sent180, sent200, sent404, sent406, sent486, sent606,
        ret180, ret200, ret404, ret406, ret486, ret606;
    int recInv, recBye, recCan, recAck;

    simsignal_t stateSignal;
};

#endif
