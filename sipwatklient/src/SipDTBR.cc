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

#include "SipDTBR.h"

Define_Module(SipDTBR);

void SipDTBR::initialize(int stage) {
    ApplicationBase::initialize(stage);
    if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        destPort = par("destPort");
        localPort = par("localPort");
        if (destPort != 5060 and destPort != 5061) {
            error("Bad numbers of UDP ports.");
        }
        if (localPort != 5060 and localPort != 5061) {
            error("Bad numbers of UDP ports.");
        }

        bool x=getParentModule()->par("write_logs");

        if(x){
        string hostName = getParentModule()->getParentModule()->getName();
        string fileName = "simlogs//" + hostName + "_SipMsgLog.txt";
        sipMsgLog.open(fileName);
        if (sipMsgLog.is_open()) {
            sipMsgLog << hostName << " created this file: " << fileName
                    << " at " << simTime() << "\n#\n#";
            sipMsgLog.close();
        } else {
            error("Error in Dtbr: file has errors!");
        }
        fileName = "simlogs//" + hostName + "_StatusOfUac.txt";
        uacStatusLog.open(fileName);
        if (uacStatusLog.is_open()) {
            uacStatusLog << hostName << " created this file: " << fileName
                    << " at " << simTime() << "\n#\n#";
            uacStatusLog.close();
        } else {
            error("Error in Dtbr: file has errors!");
        }
        fileName = "simlogs//" + hostName + "_StatusOfUas.txt";
        uacStatusLog.open(fileName);
        if (uacStatusLog.is_open()) {
            uacStatusLog << hostName << " created this file: " << fileName
                    << " at " << simTime() << "\n#\n#";
            uacStatusLog.close();
        } else {
            error("Error in Dtbr: file has errors!");
        }
        }
        //statistics
        recPac = 0;
        sentPac = 0;
        decReq = 0;
        capReq = 0;
        decAns = 0;
        capAns = 0;

        //Creating socket
        socket.setOutputGate(gate("outSipClient"));
        socket.bind(localPort);
    }
}

void SipDTBR::handleMessageWhenUp(cMessage *msg) {
    // TODO - Generated method body
    if (msg->arrivedOn("inSipClient"))
            {
        UDPPacket *udpPacket = dynamic_cast<UDPPacket *>(msg);
        if (nullptr == udpPacket)
                {
            delete msg;
            return;
        }
        recPac++;
        if (udpPacket->hasEncapsulatedPacket()) {
            cPacket *payload = udpPacket->decapsulate();
            RequestSIP *requestSip = dynamic_cast<RequestSIP *>(payload);
            if (requestSip == nullptr) {
                delete requestSip;
            } else {
                decReq++;
                send(requestSip, gate("outUasSipEngine"));
                delete msg;
                return;
            }
            AnswerSIP *answerSip = dynamic_cast<AnswerSIP *>(payload);
            if (answerSip == nullptr) {
                delete answerSip;
                return;
            } else {
                decAns++;
                send(answerSip, gate("outUacSipEngine"));
                delete msg;
                return;
            }
        }
        else {
            delete msg;
            return;
        }
    }
    else if ((msg->arrivedOn("inUacSipEngine"))
            or (msg->arrivedOn("inUasSipEngine"))) {
        UDPPacket *packetUdp = new UDPPacket();
        RequestSIP *requestSip = dynamic_cast<RequestSIP *>(msg);
        if (requestSip == nullptr)
                {
            delete requestSip;
        } else {
            RequestSIPControlInfo * controlInfo =
                    dynamic_cast<RequestSIPControlInfo*>(requestSip->removeControlInfo());
            if (controlInfo) {
                capReq++;
                packetUdp->encapsulate(requestSip);
                sentPac++;
                socket.sendTo(packetUdp, controlInfo->getDestAddress(),
                        destPort);
            } else {
                error("No ControlInfo in requestSip.");
            }
        }
        AnswerSIP *answerSip = dynamic_cast<AnswerSIP *>(msg);
        if (answerSip == nullptr)
                {
            delete answerSip;
        } else {
            AnswerSIPControlInfo * controlInfo =
                    dynamic_cast<AnswerSIPControlInfo*>(answerSip->removeControlInfo());
            if (controlInfo) {
                capAns++;
                packetUdp->encapsulate(answerSip);
                sentPac++;
                socket.sendTo(packetUdp, controlInfo->getDestAddress(),
                        destPort);
            } else {
                error("No ControlInfo in answerSip.");
            }
        }
        if (false == packetUdp->hasEncapsulatedPacket()) {
            delete msg;
            return;
        }
    }
}
void SipDTBR::finish() {
    recordScalar("Received Packets", recPac);
    recordScalar("Sent Packets", sentPac);
    recordScalar("Decapsulated Requests", decReq);
    recordScalar("Decapsulated Answer", decAns);
    recordScalar("Capsulated Requests", capReq);
    recordScalar("Capsulated Requests", capAns);
}
