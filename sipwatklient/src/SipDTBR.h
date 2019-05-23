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

#ifndef __SIPCLIENTWAT_SIPDTBR_H_
#define __SIPCLIENTWAT_SIPDTBR_H_

#include "header.h"

using namespace omnetpp;
using namespace inet;

/**
 * TODO - Generated class
 */
class SipDTBR: public inet::ApplicationBase {
public:
    ofstream sipMsgLog;
    ofstream uacStatusLog;
    ofstream uasStatusLog;
protected:
    virtual int numInitStages() const override {
        return inet::NUM_INIT_STAGES;
    }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
private:
    int localPort, destPort;
    L3Address destAddress;
    UDPSocket socket;

    //statistics
    int recPac, sentPac,
        decReq,capReq,
        decAns,capAns;


};

#endif
