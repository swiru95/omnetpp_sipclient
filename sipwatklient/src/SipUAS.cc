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

#include "SipUAS.h"

Define_Module(SipUAS);

void SipUAS::initialize(int stage) {
    // TODO - Generated method body
    ApplicationBase::initialize(stage);
    if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        //parameters
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
        if(write_logs){
        host =
                getParentModule()->getParentModule()->getParentModule()->getName();
        fileName += host + "_SipMsgLog.txt";
        }
        stateSignal = registerSignal("StateS");

        create180 = 0;
        create200 = 0;
        create404 = 0;
        create406 = 0;
        create486 = 0;
        create606 = 0;

        sent180 = 0;
        sent200 = 0;
        sent404 = 0;
        sent406 = 0;
        sent486 = 0;
        sent606 = 0;

        ret180 = 0;
        ret200 = 0;
        ret404 = 0;
        ret406 = 0;
        ret486 = 0;
        ret606 = 0;

        recInv = 0;
        recBye = 0;
        recCan = 0;
        recAck = 0;
    }

}

void SipUAS::handleMessageWhenUp(cMessage *msg) {
    // TODO - Generated method body
    if (msg->arrivedOn("inTu")) {
        ControlTU* controlTu = dynamic_cast<ControlTU *>(msg);
        if (controlTu == nullptr) {
            delete msg;
            error("Error thers not ControlTU");
            return;
        }
        if (controlTu->getDestOfMsg() != Uas) {

            delete msg;
            error("Error with controlTU");
            return;
        }
        handleMessageArrivedFromTu(controlTu);
        delete controlTu;
        controlTu=nullptr;
    } else if (msg->arrivedOn("inDtbr")) {
        RequestSIP* requestSip = dynamic_cast<RequestSIP *>(msg);
        if (requestSip == nullptr) {
            delete msg;
            return;
        }
        handleMessageArrivedFromDtbr(requestSip);
        delete requestSip;
        requestSip=nullptr;
    } else if (msg->isSelfMessage()) {
        handleSelfMessage(msg);
    } else {
        delete msg;
        return;
    }
}
void SipUAS::handleMessageArrivedFromDtbr(RequestSIP* requestSip) {
    ControlTU *controlTu = nullptr;
    typeS_MapUAS::iterator itSM = stateMapUas.find(requestSip->getCall_ID());
    if (itSM == stateMapUas.end()) {
        if (requestSip->getRequestType() == INVITE) {
            recInv++;
            stateMapUas[requestSip->getCall_ID()] = new StatusOfUas(PROCEEDING,
            TRUE, requestSip);
            emit(stateSignal, PROCEEDING);
            itSM = stateMapUas.find(requestSip->getCall_ID());
            if (itSM == stateMapUas.end()) {
                error("Error in 93UAS.");
            }
            callIdMap[requestSip->getFromUser()] = requestSip->getCall_ID();
            if(write_logs){
            itSM->second->print(host.c_str());
            }
            AnswerSIP* answerSip = (AnswerSIP *) createAnswer(Ringing,
                    itSM->second);
            if(write_logs){
            printAnswer(answerSip, fileName.c_str(), host.c_str());
            }
            sent180++;
            send(answerSip, gate("outDtbr"));
            controlTu = (ControlTU*) createControlTu(requestSip, CALL);
            controlTu->setName("RING!! RING!! RING!!");
            send(controlTu, gate("outTu"));
        } else if (requestSip->getRequestType() == BYE) {
            recBye++;
            ControlTU* controlTu = (ControlTU*) createControlTu(requestSip,
                    TERMINATE);
            stateMapUas[requestSip->getCall_ID()] = new StatusOfUas(TRYING,
            FALSE, requestSip);
            emit(stateSignal, TRYING);
            itSM = stateMapUas.find(requestSip->getCall_ID());
            if (itSM == stateMapUas.end()) {
                error("Error in 93UAS.");
            }
            callIdMap[requestSip->getFromUser()] = requestSip->getCall_ID();
            if(write_logs){
            itSM->second->print(host.c_str());
            }
            send(controlTu, gate("outTu"));
        } else {
            error("Error in 100UAS");
        }
    } else if (itSM != stateMapUas.end()) {
        if (!checkRequest(requestSip, itSM->second)) {
            delete requestSip;
            error("Error in 104UAS");
            return;
        }
        if (requestSip->getRequestType() == INVITE) {
            recInv++;
        } else if (requestSip->getRequestType() == CANCEL) {
            recCan++;
            itSM->second->setStateUas(TRYING);
            emit(stateSignal, TRYING);
            itSM->second->setMethod("CANCEL");
            if(write_logs){
            itSM->second->print(host.c_str());
            }
            ControlTU* controlTu = (ControlTU*) createControlTu(requestSip,
                    TERMINATE);
            send(controlTu, gate("outTu"));
        } else if (requestSip->getRequestType() == ACK) {
            recAck++;
            if (itSM->second->getStateUas() == PROCEEDING) {
                ControlTU* controlTu = (ControlTU*) createControlTu(requestSip,
                        CANTALK);
                send(controlTu, gate("outTu"));
            }
            if (itSM->second != nullptr) {
                cancelRetransmissions(itSM->second);
                delete itSM->second;
            }
            stateMapUas.erase(itSM);
        } else if (requestSip->getRequestType() == BYE) {
            ControlTU* controlTu = (ControlTU*) createControlTu(requestSip,
                               TERMINATE);
            send(controlTu, gate("outTu"));
            return;
        } else {
            error("Error in 126UAS");
        }
    }
}
void SipUAS::handleMessageArrivedFromTu(ControlTU* controlTu) {

    TypeCid_Map::iterator itCM = callIdMap.find(controlTu->getCall2User());
    if (itCM == callIdMap.end()) {
        delete controlTu;
        error("Error in UAS/no such session.");
        return;
    }
    typeS_MapUAS::iterator itSM = stateMapUas.find(itCM->second);
    if (itSM == stateMapUas.end()) {
        delete controlTu;
        error("Error in UAS!");
        return;
    }
    int infoCarried = controlTu->getInfoCarried();
    AnswerSIP* answerSip = nullptr;
    switch (infoCarried) {
    case OKansw: {
        if (itSM->second->getStateUas() == PROCEEDING) {
            answerSip = (AnswerSIP*) createAnswer(OK, itSM->second);
            if (answerSip == nullptr) {
                delete controlTu;
                error("Error with answerSip.");
                return;
            }
            if(write_logs){
            printAnswer(answerSip, fileName.c_str(), host.c_str());
            }
            send(answerSip, gate("outDtbr"));
            sent200++;
        } else if (itSM->second->getStateUas() == TRYING) {
            answerSip = (AnswerSIP*) createAnswer(OK, itSM->second);
            if (answerSip == nullptr) {
                delete controlTu;
                error("Error with answerSip.");
                return;
            }
            if(write_logs){
            printAnswer(answerSip, fileName.c_str(), host.c_str());
            }
            itSM->second->setStateUas(COMPLETE);
            emit(stateSignal, COMPLETE);
            if(write_logs){
            itSM->second->print(host.c_str());
            }
            if (strcmp(answerSip->getMethod(), "BYE")) {
                setRetransmission(answerSip, itSM->second);
            }
            send(answerSip, gate("outDtbr"));
            sent200++;
        }
        break;
    }
    case BUSYansw: {
        if (itSM->second->getStateUas() == TRYING
                or itSM->second->getStateUas() == PROCEEDING) {
            answerSip = (AnswerSIP*) createAnswer(Busy_Here, itSM->second);
            if (answerSip == nullptr) {
                delete controlTu;
                error("Error with answerSip.");
                return;
            }
            itSM->second->setStateUas(COMPLETE);
            emit(stateSignal, COMPLETE);
            if(write_logs){
            itSM->second->print(host.c_str());
            printAnswer(answerSip, fileName.c_str(), host.c_str());
            }
            setRetransmission(answerSip, itSM->second);
            sent486++;
            send(answerSip, gate("outDtbr"));
        } else {
            error("Error!");
        }
        break;
    }
    case NOT_ACCEPTABLE4answ: {
        if (itSM->second->getStateUas() == TRYING
                or itSM->second->getStateUas() == PROCEEDING) {
            answerSip = (AnswerSIP*) createAnswer(Not_Acceptable4,
                    itSM->second);
            if (answerSip == nullptr) {
                delete controlTu;
                error("Error with answerSip.");
                return;
            }
            itSM->second->setStateUas(COMPLETE);
            emit(stateSignal, COMPLETE);
            if(write_logs){
            itSM->second->print(host.c_str());
            printAnswer(answerSip, fileName.c_str(), host.c_str());
            }
            setRetransmission(answerSip, itSM->second);
            sent406++;
            send(answerSip, gate("outDtbr"));
        } else {
            error("Error!");
        }
        break;
    }
    case NOT_ACCEPTABLE6answ: {
        if (itSM->second->getStateUas() == TRYING
                or itSM->second->getStateUas() == PROCEEDING
                or itSM->second->getStateUas() == COMPLETE) {
            answerSip = (AnswerSIP*) createAnswer(Not_Acceptable6,
                    itSM->second);
            if (answerSip == nullptr) {
                delete controlTu;
                error("Error with answerSip.");
                return;
            }
            itSM->second->setStateUas(COMPLETE);
            emit(stateSignal, COMPLETE);
            if(write_logs){
            itSM->second->print(host.c_str());
            printAnswer(answerSip, fileName.c_str(), host.c_str());
            }
            if(itSM->second->getMethod()!="BYE"){
            setRetransmission(answerSip, itSM->second);
            }
            sent606++;
            send(answerSip, gate("outDtbr"));
        } else {
            error("Error!");
        }
        break;
    }
    case NOT_FOUNDansw: {
        if (itSM->second->getStateUas() == TRYING
                or itSM->second->getStateUas() == PROCEEDING) {
            answerSip = (AnswerSIP*) createAnswer(Not_Found, itSM->second);
            if (answerSip == nullptr) {
                delete controlTu;
                error("Error with answerSip.");
                return;
            }
            itSM->second->setStateUas(COMPLETE);
            emit(stateSignal, COMPLETE);
            if(write_logs){
            itSM->second->print(host.c_str());
            printAnswer(answerSip, fileName.c_str(), host.c_str());
            }
            setRetransmission(answerSip, itSM->second);
            sent404++;
            send(answerSip, gate("outDtbr"));
        } else {
            error("Error!");
        }
        break;
    }
    default: {
        delete controlTu;
        error("Error with ControlTU");
        return;
        break;
    }
        if (controlTu != nullptr) {
            delete controlTu;
        }
    }
}
bool SipUAS::checkRequest(RequestSIP * ans, StatusOfUas *sou) {
    if (ans->getRequestType() == CANCEL) {
        if (ans->getToTag() != sou->getToTag()) {
            error("Error checking Answer!");
            return false;
        }
    }
    if (ans->getCall_ID() == sou->getCallId()
            and ans->getFromTag() == sou->getFromTag()) {
        return true;
    } else {
        error("Error checking Answer!");
        return false;
    }
}

void SipUAS::handleSelfMessage(cMessage* msg) {
    string n = msg->getName();
    typeRrb_MapUAS::iterator itRRB;
    typeS_MapUAS::iterator itSM;
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
        itSM = stateMapUas.find(n);
        if (itSM == stateMapUas.end()) {
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
        AnswerSIP* answerSip = itRRB->second->dup();
        AnswerSIPControlInfo * info = new AnswerSIPControlInfo();
        info->setDestAddress(itSM->second->getToIpAddress());
        answerSip->setControlInfo(info);
        if(write_logs){
        printAnswer(answerSip, fileName.c_str(), host.c_str());
        }
        switch (answerSip->getAnswerType()) {
        case Ringing:
            ret180++;
            sent180++;
            break;
        case Not_Found:
            ret404++;
            sent404++;
            break;
        case Not_Acceptable4:
            ret406++;
            sent406++;
            break;
        case Busy_Here:
            ret486++;
            sent486++;
            break;
        case Not_Acceptable6:
            ret606++;
            sent606++;
            break;
        }
        send(answerSip, gate("outDtbr"));
        if (itSM->second->getDeltaT() < T2.dbl()) {
            itSM->second->setDeltaT(2 * itSM->second->getDeltaT());
        }
        scheduleAt(simTime() + itSM->second->getDeltaT(), msg);
    } else {
        itSM = stateMapUas.find(n);
        if (itSM == stateMapUas.end()) {
            delete msg;
            error("Error");
            return;
        }
        if (itSM->second == nullptr) {
            delete msg;
            error("Error");
            return;
        }
        if (msg == itSM->second->getTimerB()) {
            cancelRetransmissions(itSM->second);
            delete itSM->second;
            stateMapUas.erase(itSM);
        }
    }
}

void SipUAS::setRetransmission(AnswerSIP* answerSip, StatusOfUas * sou) {
    if (sou->getTimerA() == nullptr) {
        sou->setTimerA(new cMessage("TimerA"));
    } else {
        if (sou->getTimerA()->isScheduled()) {
            cancelAndDelete(sou->getTimerA());
            sou->setTimerA(new cMessage("TimerA"));
        }
    }
    typeRrb_MapUAS::iterator itRRB = requestRetransmissionBuffor.find(
            sou->getTimerA());
    if (itRRB != requestRetransmissionBuffor.end()) {
        delete itRRB->second;
        requestRetransmissionBuffor.erase(itRRB);
    }
    requestRetransmissionBuffor[sou->getTimerA()] = answerSip->dup();
    sou->setDeltaT(par("T1").doubleValue());
    scheduleAt((simTime() + (sou->getDeltaT())), sou->getTimerA());
    if (sou->getTimerB() == nullptr) {
        string s = sou->getCallId();
        sou->setTimerB(new cMessage(s.c_str()));
    }
    scheduleAt((simTime() + (64 * T1)), sou->getTimerB());

}

void SipUAS::cancelRetransmissions(StatusOfUas * sou) {
    if (sou->getTimerA() != nullptr) {
        typeRrb_MapUAS::iterator itRRB = requestRetransmissionBuffor.find(
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
}

void SipUAS::finish() {

    recordScalar("Created 180 Ringing", create180);
    recordScalar("Created 200 OK", create200);
    recordScalar("Created 404 Not Found", create404);
    recordScalar("Created 406 Not Acceptable", create406);
    recordScalar("Created 486 Busy Here", create486);
    recordScalar("Created 606 Not Acceptable", create606);

    recordScalar("Sent 180 Ringing", sent180);
    recordScalar("Sent 200 OK", sent200);
    recordScalar("Sent 404 Not Found", sent404);
    recordScalar("Sent 406 Not Acceptable", sent406);
    recordScalar("Sent 486 Busy Here", sent486);
    recordScalar("Sent 606 Not Acceptable", sent606);

    recordScalar("Retransmitted 180 Ringing", ret180);
    recordScalar("Retransmitted 200 OK", ret200);
    recordScalar("Retransmitted 404 Not Found", ret404);
    recordScalar("Retransmitted 406 Not Acceptable", ret406);
    recordScalar("Retransmitted 486 Busy Here", ret486);
    recordScalar("Retransmitted 606 Not Acceptable", ret606);

    recordScalar("Received INVITE", recInv);
    recordScalar("Received ACK", recAck);
    recordScalar("Received BYE", recBye);
    recordScalar("Received CANCEL", recCan);

    TypeCid_Map::iterator itCIM = callIdMap.begin();
    while (itCIM != callIdMap.end()) {
        itCIM = callIdMap.erase(itCIM);
        itCIM = callIdMap.begin();
    }
    /* typeRrb_MapUAS::iterator itRRB = requestRetransmissionBuffor.begin();
     while (itRRB != requestRetransmissionBuffor.end()) {
     if (itRRB->second != nullptr) {
     delete itRRB->second;
     itRRB->second = nullptr;
     }
     itRRB = requestRetransmissionBuffor.erase(itRRB);
     itRRB = requestRetransmissionBuffor.begin();
     }
     requestRetransmissionBuffor.erase(itRRB);*/
    typeS_MapUAS::iterator itSM = stateMapUas.begin();
    while (itSM != stateMapUas.end()) {
        if (itSM->second != nullptr) {
            delete itSM->second;
            itSM->second = nullptr;
        }
        itSM = stateMapUas.erase(itSM);
        itSM = stateMapUas.begin();
    }
}
void *createControlTu(StatusOfUas* ans, int type) {
    ControlTU* controlTu = new ControlTU();
    controlTu->setInfoCarried(type);
    controlTu->setDestOfMsg(Tu);
    controlTu->setCall2Address((L3Address) ans->getToIpAddress());
    controlTu->setCall2User(ans->getToUser().c_str());
    controlTu->setLocalUser(ans->getFromUser().c_str());
    return controlTu;
}
