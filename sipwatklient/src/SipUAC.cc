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

#include "SipUAC.h"
Define_Module(SipUAC);
void SipUAC::initialize(int stage) {
    // TODO - Generated method body
    ApplicationBase::initialize(stage);
    if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        T1 = par("T1");
        T2 = par("T2");
        T4 = par("T4");
        if (T1 <= 0 or T2 <= 0 or T4 <= 0) {
            error("Bad times!");
        }
        if (T1 <= 0 or T1 > 5) {
            error("Bad T1");
        }
        if (T2 < 4 or T2 > 20) {
            error("Bad T2");
        }
        if (T4 <= 0 or T4 > 20) {
            error("Bad T4");
        }

        write_logs=getParentModule()->getParentModule()->par("write_logs");
        //stats
        stateSignal = registerSignal("StateS");

        createdAck = 0;
        createdInv = 0;
        createdCan = 0;
        createdBye = 0;

        sentAck = 0;
        sentInv = 0;
        sentCan = 0;
        sentBye = 0;

        retAck = 0;
        retBye = 0;
        retCan = 0;
        retInv = 0;

        rec180 = 0;
        rec200 = 0;
        rec404 = 0;
        rec406 = 0;
        rec486 = 0;
        rec606 = 0;
        //forFile
        if(write_logs){
        host =
                getParentModule()->getParentModule()->getParentModule()->getName();
        fileName += host + "_SipMsgLog.txt";
        }
    }
}
void SipUAC::handleMessageWhenUp(cMessage *msg) {
    // TODO - Generated method body
    if (msg->arrivedOn("inTu")) {
        ControlTU* controlTu = dynamic_cast<ControlTU*>(msg);
        if (controlTu == nullptr) {
            delete msg;
            return;
        }
        if (controlTu->getDestOfMsg() != Uac) {
            delete msg;
            return;
        }
        handleMessageArrivedFromTu(controlTu);
        delete controlTu;
        controlTu = nullptr;
    } else if (msg->arrivedOn("inDtbr")) {
        AnswerSIP *answerSip = dynamic_cast<AnswerSIP *>(msg);
        if (answerSip == nullptr) {
            delete msg;
            return;
        }
        handleMessageArrivedFromDtbr(answerSip);
        delete answerSip;
        answerSip = nullptr;
    } else if (msg->isSelfMessage()) {
        handleSelfMessage(msg);
    } else {
        delete msg;
        error("Error in 68UAC");
    }
}
void SipUAC::handleMessageArrivedFromTu(ControlTU* controlTu) {
    int infoCarried = controlTu->getInfoCarried();
    RequestSIP* requestSip;
    TypeCid_Map::iterator itCId = callIdMapUac.find(controlTu->getCall2User());
    if (infoCarried == CALL) {
        if (callIdMapUac.end() != itCId) {
            return;
        }
        createdInv++;
        requestSip = (RequestSIP *) createIniRequest(controlTu, INVITE);
        stateMapUac[requestSip->getCall_ID()] = new StatusOfUac(TRYING,
        TRUE, requestSip);
        itCId = callIdMapUac.begin();
        callIdMapUac[requestSip->getToUser()] = requestSip->getCall_ID();
        emit(stateSignal, TRYING);
        if(write_logs){
        stateMapUac[requestSip->getCall_ID()]->print(host.c_str());
        }
    } else if (infoCarried == TERMINATE) {
        typeS_MapUAC::iterator itSM = stateMapUac.find(itCId->second);
        if (itSM == stateMapUac.end()) { //HERE
            createdBye++;
            requestSip = (RequestSIP*) createIniRequest(controlTu, BYE);
            stateMapUac[requestSip->getCall_ID()] = new StatusOfUac(TRYING,
            FALSE, requestSip);
            callIdMapUac[controlTu->getCall2User()] = requestSip->getCall_ID();
            emit(stateSignal, TRYING);
        } else if (itSM->second->isFlagInvite() == TRUE
                and itSM->second != nullptr) {
            itSM->second->setStateUac(TRYING);
            emit(stateSignal, TRYING);
            createdCan++;
            requestSip = (RequestSIP *) createFinRequest(CANCEL, itSM->second);
            if(write_logs){
            itSM->second->print(host.c_str());
            }
        } else {
            error("Error");
        }
    } else {
        delete controlTu;
        error("Error in 100UAC.");
        return;
    }
    if (requestSip != nullptr) {
        typeS_MapUAC::iterator itSM = stateMapUac.find(
                requestSip->getCall_ID());
        if (itSM == stateMapUac.end()) {
            error("Error in 106UAC");
            return;
        }
        if(write_logs){
        printRequest(requestSip, fileName.c_str(), host.c_str());
        }
        setRetransmission(requestSip, true, itSM->second);
        switch (requestSip->getRequestType()) {
        case INVITE:
            sentInv++;
            break;
        case BYE:
            sentBye++;
            break;
        case CANCEL:
            sentCan++;
            break;
        }
        send(requestSip, gate("outDtbr"));
    } else {
        error("Error2 in handle message from TU.");
        return;
    }
}
void SipUAC::handleMessageArrivedFromDtbr(AnswerSIP* answerSip) {
    ControlTU * controlTu = nullptr;
    typeS_MapUAC::iterator itSM = stateMapUac.find(answerSip->getCall_ID());
    if (itSM == stateMapUac.end()) {
        return;
    }
    if ((itSM->second->getStateUac() == TRYING)
            and (checkAnswer(answerSip, itSM->second))) { //TODO
        if (answerSip->getAnswerType() == OK) {
            rec200++;
            cancelRetransmissions(itSM->second);
            if (itSM->second->isFlagInvite() == TRUE) {
                createdAck++;
                RequestSIP *requestSip = (RequestSIP *) createFinRequest(ACK,
                        itSM->second);
                if (requestSip != nullptr) {
                    if(write_logs){
                    printRequest(requestSip, fileName.c_str(), host.c_str());
                    }
                    send(requestSip, gate("outDtbr"));
                    sentAck++;
                } else {
                    error("Error with request");
                }
            }
            delete itSM->second;
            itSM->second = nullptr;
            stateMapUac.erase(itSM);
            controlTu = (ControlTU *) createControlTu(answerSip, OKansw);
            auto itCId = callIdMapUac.find(answerSip->getToUser());
            callIdMapUac.erase(itCId);
            itCId = callIdMapUac.begin();
            if (controlTu != nullptr) {
                send(controlTu, gate("outTu"));
            } else {
                error("Error with controlTU");
            }
            return;
        } else if (answerSip->getAnswerType() == Ringing) {
            rec180++;
            itSM->second->setStateUac(PROCESS);
            emit(stateSignal, PROCESS);
            itSM->second->setToTag(answerSip->getToTag());
            if(write_logs){
            itSM->second->print(host.c_str());
            }
            cancelRetransmissions(itSM->second);
            controlTu = (ControlTU *) createControlTu(answerSip, RINGINGansw);
            if (controlTu != nullptr) {
                send(controlTu, gate("outTu"));
            } else {
                error("Error with controlTU");
            }
            return;
        } else if (answerSip->getAnswerType() == Not_Acceptable6) {
            cancelRetransmissions(itSM->second);
            delete itSM->second;
            itSM->second = nullptr;
            stateMapUac.erase(itSM);
            auto itCId = callIdMapUac.find(answerSip->getToUser());
            callIdMapUac.erase(itCId);
            itCId = callIdMapUac.begin();
            return;
        } else {
            EV_WARN
                           << "Something Happens, ive received response without ringing and ok!"
                           << endl;
        }
    } else if ((itSM->second->getStateUac() == PROCESS
            or itSM->second->getStateUac() == COMPLETE)
            and (checkAnswer(answerSip, itSM->second))) {
        if (answerSip->getAnswerType() == OK
                and itSM->second->getStateUac() == PROCESS) {
            rec200++;
            cancelRetransmissions(itSM->second);
            if (itSM->second->isFlagInvite() == TRUE) {
                createdAck++;
                RequestSIP *requestSip = (RequestSIP *) createFinRequest(ACK,
                        itSM->second);
                if (requestSip != nullptr) {
                    sentAck++;
                    if(write_logs){
                    printRequest(requestSip, fileName.c_str(), host.c_str());
                    }
                    send(requestSip, gate("outDtbr"));
                } else {
                    error("Error with request");
                }
            }
            if (strcmp(answerSip->getMethod(), "CANCEL")) {
                delete itSM->second;
                stateMapUac.erase(itSM);
            }
            controlTu = (ControlTU *) createControlTu(answerSip, OKansw);
            if (controlTu != nullptr) {
                send(controlTu, gate("outTu"));
            } else {
                error("Error with controlTU");
            }

            return;
        } else if (answerSip->getAnswerType() == Busy_Here
                or answerSip->getAnswerType() == Not_Acceptable4
                or answerSip->getAnswerType() == Not_Acceptable6
                or answerSip->getAnswerType() == Not_Found) {
            int kind = answerSip->getAnswerType();
            switch (kind) {
            case Busy_Here:
                rec486++;
                break;
            case Not_Acceptable4:
                rec406++;
                break;
            case Not_Acceptable6:
                rec606++;
                break;
            case Not_Found:
                rec486++;
                break;
            }
            itSM->second->setStateUac(COMPLETE);
            emit(stateSignal, COMPLETE);
            cancelRetransmissions(itSM->second);
            if(write_logs){
            itSM->second->print(host.c_str());
            }
            if (itSM->second->getTimerK() == nullptr) {
                string s = itSM->second->getCallId();
                itSM->second->setTimerK(new cMessage(s.c_str()));
                scheduleAt(simTime() + T4, itSM->second->getTimerK());
            }
            if (itSM->second->isFlagInvite() == TRUE) {
                createdAck++;
                RequestSIP *requestSip = (RequestSIP *) createFinRequest(ACK,
                        itSM->second);
                if (requestSip != nullptr) {
                    sentAck++;
                    if(write_logs){
                    printRequest(requestSip, fileName.c_str(), host.c_str());
                    }
                    send(requestSip, gate("outDtbr"));
                } else {
                    error("Error with request");
                }
            }
            controlTu = (ControlTU *) createControlTu(answerSip,
                    answerSip->getAnswerType());
            if (controlTu != nullptr) {
                send(controlTu, gate("outTu"));
            } else {
                error("Error with controlTU");
            }
            return;
        }

    } else {
        EV_WARN << "Something went wrong, i cant find correct ID and Session.";
    }
}
void SipUAC::handleSelfMessage(cMessage* msg) {
    string n = msg->getName();
    typeRrb_MapUAC::iterator itRRB;
    typeS_MapUAC::iterator itSM;
    if (n == "TimerA") {
        itRRB = requestRetransmissionBuffor.find(msg);
        if (itRRB == requestRetransmissionBuffor.end()) {
            delete msg;
            error("Error timer A has not request to retransmitt in buffor.");
            return;
        }
        if (itRRB->second == nullptr) {
            delete msg;
            error("Error msg to retransmit is nullptr!");
            return;
        }
        n = itRRB->second->getCall_ID();
        itSM = stateMapUac.find(n);
        if (itSM == stateMapUac.end()) {
            delete msg;
            error("Error there is no session in stat table!");
            return;
        }
        if (itSM->second == nullptr) {
            delete msg;
            error("Error there is no session!");
            return;
        }
        if (itSM->second->getTimerA() != msg) {
            delete msg;
            error("Error");
            return;
        }
        int kind = itRRB->second->getRequestType();
        switch (kind) {
        case INVITE:
            sentInv++;
            retInv++;
            break;
        case ACK:
            sentAck++;
            retAck++;
            break;
        case BYE:
            sentBye++;
            retBye++;
            break;
        case CANCEL:
            sentCan++;
            retCan++;
            break;
        }
        RequestSIP* requestSip = itRRB->second->dup();
        if(write_logs){
        printRequest(requestSip, fileName.c_str(), host.c_str());
        }
        RequestSIPControlInfo * info = new RequestSIPControlInfo();
        info->setDestAddress(itSM->second->getToIpAddress());
        requestSip->setControlInfo(info);
        send(requestSip, gate("outDtbr"));
        if (itSM->second->getDeltaT() < T2.dbl()) {
            itSM->second->setDeltaT(2 * itSM->second->getDeltaT());
        }
        scheduleAt(simTime() + itSM->second->getDeltaT(), msg);
    } else {
        itSM = stateMapUac.find(n);
        if (itSM == stateMapUac.end()) {
            delete msg;
            error("Error");
            return;
        }
        if (itSM->second == nullptr) {
            delete msg;
            error("Error");
            return;
        }
        if (msg == itSM->second->getTimerB()
                or msg == itSM->second->getTimerK()) {
            if (msg == itSM->second->getTimerB()) {
                ControlTU* controlTu = (ControlTU*) createControlTu(
                        itSM->second, NOT_FOUNDansw);
                send(controlTu, gate("outTu"));
            }
            cancelRetransmissions(itSM->second);
            delete itSM->second;
            stateMapUac.erase(itSM);
        }
    }
}
void SipUAC::setRetransmission(RequestSIP* requestSip, bool isTimerB,
        StatusOfUac * sou) {

    if (sou->getTimerA() == nullptr) {
        sou->setTimerA(new cMessage("TimerA"));
    } else {
        if (sou->getTimerA()->isScheduled()) {
            cancelAndDelete(sou->getTimerA());
            sou->setTimerA(new cMessage("TimerA"));
        }
    }
    typeRrb_MapUAC::iterator itRRB = requestRetransmissionBuffor.find(
            sou->getTimerA());
    if (itRRB != requestRetransmissionBuffor.end()) {
        delete itRRB->second;
        requestRetransmissionBuffor.erase(itRRB);
    }
    requestRetransmissionBuffor[sou->getTimerA()] = requestSip->dup();
    sou->setDeltaT(par("T1").doubleValue());
    scheduleAt((simTime() + (sou->getDeltaT())), sou->getTimerA());
    if (isTimerB) {
        if (sou->getTimerB() != nullptr) {
            if (sou->getTimerB()->isScheduled()) {
                cancelAndDelete(sou->getTimerB());
                string s = sou->getCallId();
                sou->setTimerB(new cMessage(s.c_str()));
            }
        } else {
            string s = sou->getCallId();
            sou->setTimerB(new cMessage(s.c_str()));
        }
        scheduleAt((simTime() + (64 * T1)), sou->getTimerB());
    } else {
        if (sou->getTimerK() != nullptr) {
            if (sou->getTimerK()->isScheduled()) {
                cancelAndDelete(sou->getTimerK());
                string s = sou->getCallId();
                sou->setTimerK(new cMessage(s.c_str()));
            }
        } else {
            string s = sou->getCallId();
            sou->setTimerK(new cMessage(s.c_str()));
        }
        scheduleAt((simTime() + (64 * T1)), sou->getTimerB());
    }
}
void SipUAC::cancelRetransmissions(StatusOfUac * sou) {
    if (sou->getTimerA() != nullptr) {
        typeRrb_MapUAC::iterator itRRB = requestRetransmissionBuffor.find(
                sou->getTimerA());
        if (itRRB != requestRetransmissionBuffor.end()) {
            if (itRRB->second != nullptr) {
                delete itRRB->second;
                itRRB->second = nullptr;
            }
        }
        requestRetransmissionBuffor.erase(itRRB);
    }

    cancelAndDelete(sou->getTimerA());
    sou->setTimerA(nullptr);
    cancelAndDelete(sou->getTimerB());
    sou->setTimerB(nullptr);
    cancelAndDelete(sou->getTimerK());
    sou->setTimerK(nullptr);
}
bool SipUAC::checkAnswer(AnswerSIP *ans, StatusOfUac * sou) {
    if (ans->getAnswerType() == Ringing
            or ans->getAnswerType() == Not_Acceptable6
            or (ans->getAnswerType() == OK and sou->getToTag() == "")) {
        sou->setToTag(ans->getToTag());
        if (ans->getCall_ID() == sou->getCallId()
                and ans->getFromTag() == sou->getFromTag()) {
            return true;
        } else {
            error("Error checking Answer!");
            return false;
        }
    } else {
        if (ans->getCall_ID() == sou->getCallId()
                and ans->getToTag() == sou->getToTag()
                and ans->getFromTag() == sou->getFromTag()) {
            return true;
        } else {
            error("Error checking Answer!");
            return false;
        }
    }

}
void SipUAC::finish() {

    recordScalar("created INVITE", createdInv);
    recordScalar("sent INVITE", sentInv);
    recordScalar("retransmitted INVITE", retInv);
    recordScalar("created BYE", createdBye);
    recordScalar("sent BYE", sentBye);
    recordScalar("retransmitted BYE", retBye);
    recordScalar("created CANCEL", createdCan);
    recordScalar("sent CANCEL", sentCan);
    recordScalar("retransmitted CANCEL", retCan);
    recordScalar("created ACK", createdAck);
    recordScalar("sent ACK", sentAck);
    recordScalar("received 180 Ringing", rec180);
    recordScalar("received 200 OK", rec200);
    recordScalar("received 404 Not Found", rec404);
    recordScalar("received 406 Not Acceptable", rec406);
    recordScalar("received 486 Busy Here", rec486);
    recordScalar("received 606 Not Acceptable", rec606);

    TypeCid_Map::iterator itCIM = callIdMapUac.begin();
    while (itCIM != callIdMapUac.end()) {
        itCIM = callIdMapUac.erase(itCIM);
        itCIM = callIdMapUac.begin();
    }
    /* typeRrb_MapUAC::iterator itRRB = requestRetransmissionBuffor.begin();
     while (itRRB != requestRetransmissionBuffor.end()) {
     if (itRRB->second != nullptr) {
     delete itRRB->second;
     itRRB->second = nullptr;
     }
     itRRB = requestRetransmissionBuffor.erase(itRRB);
     itRRB = requestRetransmissionBuffor.begin();
     }*/
    typeS_MapUAC::iterator itSM = stateMapUac.begin();
    while (itSM != stateMapUac.end()) {
        if (itSM->second != nullptr) {
            delete itSM->second;
            itSM->second = nullptr;
        }
        itSM = stateMapUac.erase(itSM);
        itSM = stateMapUac.begin();
    }

}

void *createControlTu(StatusOfUac* ans, int type) {
    ControlTU* controlTu = new ControlTU();
    controlTu->setInfoCarried(type);
    controlTu->setDestOfMsg(Tu);
    controlTu->setCall2Address((L3Address) ans->getToIpAddress());
    controlTu->setCall2User(ans->getToUser().c_str());
    controlTu->setLocalUser(ans->getFromUser().c_str());
    return controlTu;
}
