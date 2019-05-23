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

#include "SipTU.h"

Define_Module(SipTU);

void SipTU::initialize(int stage) {
    ApplicationBase::initialize(stage);
    if (stage == inet::INITSTAGE_LAST) {
        //statistics

        recCALL = 0;
        recTERM = 0;
        recCANT = 0;

        sentCALL = 0;
        sentTERM = 0;
        sentCANT = 0;

        rec200 = 0;
        rec404 = 0;
        rec406 = 0;
        rec486 = 0;
        rec606 = 0;

        sent200 = 0;
        sent404 = 0;
        sent406 = 0;
        sent486 = 0;
        sent606 = 0;

        stateSignal = registerSignal("StateS");
        //ready
        timerStart = nullptr;
        timerStop = nullptr;
        delayTimer = nullptr;
        localUser = par("localUser").stringValue();
        if (localUser == "") {
            error("The localUser has to be set!");
        }
        tuState = STANDBY;
        emit(stateSignal, tuState);
        if (!par("delayTime").isNumeric()) {
            error("DelayTime has to be numeric.");
        }
        myVoiceCodec = par("myVoiceCodec").stringValue();
        delayTime = par("delayTime").doubleValue();
        if (delayTime < 0.2) {
            error("Error delayTime value.");
        }
        string call2Users = par("call2Users").stringValue();
        if (!checkIsAddress(call2Users.c_str())) {
            error("Wrong addresses.");
        }
        if (call2Users != "") {
            tuState = READY2CALL;
            emit(stateSignal, tuState);
            if (!checkIsDouble(par("startCallingTimes").stringValue())) {
                error("Error with start times!");
            }
            if (!checkIsDouble(par("stopCallingTimes").stringValue())) {
                error("Error with stop times!");
            }
            vector<string> users =
                    cStringTokenizer(par("call2Users")).asVector();
            vector<double> startT =
                    cStringTokenizer(par("startCallingTimes")).asDoubleVector();
            vector<double> stopT =
                    cStringTokenizer(par("stopCallingTimes")).asDoubleVector();
            int cUS = users.size();
            int cStT = startT.size();
            int cSpT = stopT.size();
            if (cUS != cStT or cUS != cSpT or cStT != cSpT) {
                error("Error with user names or addresses.");
            }
            vector<CallUser> vecUser(cUS);
            int i = 0;
            while (i != cUS) {
                string tokenS = users[i];
                vector<string> users2 =
                        cStringTokenizer(tokenS.c_str(), "@").asVector();
                auto it = users2.begin();
                vecUser[i].name = *it;
                string addr = *(++it);
                vecUser[i].address = L3AddressResolver().resolve(addr.c_str(),
                        L3AddressResolver::ADDR_IPv4);
                vecUser[i].startTime = startT[i];
                if (vecUser[i].startTime < 0) {
                    error("Error startTime");
                }
                vecUser[i].stopTime = stopT[i];
                if ((vecUser[i].stopTime - vecUser[i].startTime) < 0.2) {
                    error("Error times");
                }
                usersList.insert(vecUser[i]);
                i++;
            }
            i = 0;
            set<CallUser>::const_iterator it = usersList.begin();
            auto it2 = usersList.begin();
            for (it = usersList.begin(); it != usersList.end(); ++it) {
                if (it != usersList.begin()) {
                    it2 = it;
                    if ((--it2)->stopTime > (it)->startTime) {
                        it = usersList.erase(it);
                        it = it2;
                    }
                }
            }
            it = usersList.begin();
            for (it = usersList.begin(); it != usersList.end(); ++it) {
                it->printUser();
            }
            EV << localUser << " Ready to call. (ONLINE-READY2CALL)" << endl;
            timerStart = new cMessage("startTimer");
            it = usersList.begin();
            scheduleAt(it->startTime, timerStart);
        }
    }
}
void SipTU::handleMessageWhenUp(cMessage *msg) {
    if (msg->isSelfMessage()) {
        handleSelfMessage(msg);
    } else {
        ControlTU* controlTu = dynamic_cast<ControlTU*>(msg);
        if (controlTu == nullptr) {
            delete msg;
            error("Error");
            return;
        }
        int dest = controlTu->getDestOfMsg();
        if (dest != Tu) {
            delete msg;
            error("Error");
            return;
        }
        if (msg->arrivedOn("inUasSipEngine")) {
            handleMessageArrivedFromUas(controlTu);
        } else if (msg->arrivedOn("inUacSipEngine")) {
            handleMessageArrivedFromUac(controlTu);
        }
    }
}
void SipTU::handleMessageArrivedFromUas(ControlTU* controlTu) {
    int info = controlTu->getInfoCarried();
    string calluser = controlTu->getCall2User(); //To
    string codec = controlTu->getMyVoiceCodec(); //codec
    string callinguser = controlTu->getLocalUser(); //From
    delete controlTu;
    if (info == CALL) {
        recCALL++;
        EV << "HERE CALL" << endl;
        if (tuState == STANDBY or tuState == READY2CALL) {
            if (strcmp(localUser.c_str(), calluser.c_str()) != 0) {
                controlTu = createControlTu(NOT_FOUNDansw, callinguser.c_str());
                sent404++;
                sendDelayed(controlTu, 0.005, gate("outUasSipEngine"));
                return;
            } else if (strcmp(codec.c_str(), myVoiceCodec.c_str()) != 0) {
                EV << myVoiceCodec << " and " << controlTu->getMyVoiceCodec()
                          << endl;
                controlTu = createControlTu(NOT_ACCEPTABLE4answ,
                        callinguser.c_str());
                sent406++;
                sendDelayed(controlTu, 0.005, gate("outUasSipEngine"));
                return;
            } else {
                EV << "HERE BUSY" << endl;
                tuState = BUSY;
                emit(stateSignal, tuState);
                delayTimer = new cMessage(callinguser.c_str());
                scheduleAt(simTime() + delayTime, delayTimer);
            }
        } else if (tuState == BUSY) {
            controlTu = createControlTu(BUSYansw, callinguser.c_str());
            sent486++;
            send(controlTu, gate("outUasSipEngine"));
            return;
        } else {
            error("Error");
        }
    } else if (info == TERMINATE) {
        recTERM++;
        if (tuState == STANDBY or tuState == READY2CALL) {
            controlTu = createControlTu(NOT_ACCEPTABLE6answ,
                    callinguser.c_str());
            sent606++;
            sendDelayed(controlTu, 0.005, gate("outUasSipEngine"));
        } else if (tuState == BUSY) {
            if (usersList.size() > 0) {
                tuState = READY2CALL;
                emit(stateSignal, tuState);
            } else {
                tuState = STANDBY;
                emit(stateSignal, tuState);
            }
            if (delayTimer != nullptr) {
                if (delayTimer->isScheduled()) {
                    cancelAndDelete(delayTimer);
                    delayTimer = nullptr;
                }
            }
            controlTu = createControlTu(OKansw, callinguser.c_str());
            sent200++;
            sendDelayed(controlTu, 0.005, gate("outUasSipEngine"));
        } else {
            error("Error");
        }
    } else if (info == CANTALK) {
        recCANT++;
        if (tuState == BUSY and !strcmp(calluser.c_str(), localUser.c_str())) {
            isTalking = true;
        } else {
            controlTu = createControlTu(NOT_ACCEPTABLE6answ,
                    callinguser.c_str());
            sent606++;
            sendDelayed(controlTu, 0.005, gate("outUasSipEngine"));
        }
    } else {
        error("Error");
    }
}
void SipTU::handleMessageArrivedFromUac(ControlTU * controlTu) {
    int info = controlTu->getInfoCarried();
    string callinguser = controlTu->getCall2User();     //To
    string codec = controlTu->getMyVoiceCodec();        //codec
    string calluser = controlTu->getLocalUser();        //From
    delete controlTu;
    if (info == RINGINGansw) {
        ////////////NothingToDo
    } else if (info == OKansw) {
        rec200++;
        ////////////NothingToDo
    } else if (info == BUSYansw or info == NOT_FOUNDansw
            or info == NOT_ACCEPTABLE4answ or info == NOT_ACCEPTABLE6answ) {
        switch (info) {
        case 486:
            rec486++;
            break;
        case 404:
            rec404++;
            break;
        case 406:
            rec406++;
            break;
        case 606:
            rec606++;
            break;
        }

        if (tuState == BUSY) {
            if (timerStart != nullptr) {
                if (timerStart->isScheduled()) {
                    cancelAndDelete(timerStart);
                }
            }
            if (timerStop != nullptr) {
                if (timerStop->isScheduled()) {
                    cancelAndDelete(timerStop);
                }
            }
            if (usersList.size() > 0) {
                auto it = usersList.begin();
                usersList.erase(it);
                it = usersList.begin();
                if (it != usersList.end()) {
                    timerStart = new cMessage("startTimer");
                    scheduleAt(it->startTime, timerStart);
                    tuState = READY2CALL;
                    emit(stateSignal, tuState);
                    EV << "Im READY2CALL now" << endl;
                } else {
                    tuState = STANDBY;
                    emit(stateSignal, tuState);
                }
            } else {
                tuState = STANDBY;
                emit(stateSignal, tuState);
            }
        }
    } else {
        error("Error");
    }
}
void SipTU::handleSelfMessage(cMessage *msg) {
    if (msg == timerStart) {
        auto it = usersList.begin();
        if (tuState == BUSY) {
            EV << "Sorry im talking" << endl;
            it = usersList.erase(it);
            it = usersList.begin();
            if (it != usersList.end()) {
                scheduleAt(it->startTime, msg);
            } else {
                cancelAndDelete(timerStart);
                timerStart = nullptr;
            }
            return;
        }
        cancelAndDelete(timerStart);
        timerStart = nullptr;
        timerStop = new cMessage("timerStop");
        scheduleAt(it->stopTime, timerStop);
        ControlTU* controlTu = createControlTu(it, CALL);
        sentCALL++;
        send(controlTu, gate("outUacSipEngine"));
        tuState = BUSY;
        emit(stateSignal, tuState);
        EV << "Im BUSY now" << endl;
    } else if (msg == timerStop) {
        cancelAndDelete(timerStop);
        timerStop = nullptr;
        auto it = usersList.begin();
        if (it == usersList.end()) {
            EV << "No calls to do." << endl;
            return;
        }
        ControlTU* controlTu = createControlTu(it, TERMINATE);
        sentTERM++;
        send(controlTu, gate("outUacSipEngine"));
        it = usersList.erase(it);
        it = usersList.begin();
        if (it != usersList.end()) {
            timerStart = new cMessage("timerStart");
            scheduleAt(it->startTime, timerStart);
            tuState = READY2CALL;
            emit(stateSignal, tuState);
            EV << "Im READY2CALL now" << endl;
        } else {
            tuState = STANDBY;
            emit(stateSignal, tuState);
            EV << "No more users" << endl;
        }
    } else if (msg == delayTimer) {
        ControlTU* controlTu = createControlTu(OKansw, msg->getName());
        sent200++;
        send(controlTu, gate("outUasSipEngine"));
        cancelAndDelete(delayTimer);
        delayTimer = nullptr;
    } else {
        delete msg;
        error("Error in TU");
    }
}
ControlTU* SipTU::createControlTu(set<CallUser>::iterator it, int infoC) {
    ControlTU* controlTu = new ControlTU();
    controlTu->setDestOfMsg(Uac);
    controlTu->setInfoCarried(infoC);
    controlTu->setLocalUser(localUser.c_str());
    controlTu->setMyVoiceCodec(myVoiceCodec.c_str());
    controlTu->setCall2User(it->name.c_str());
    controlTu->setCall2Address(it->address);
    return controlTu;
}
ControlTU* SipTU::createControlTu(int infoC, const char* call2user) {
    ControlTU* controlTu = new ControlTU();
    controlTu->setDestOfMsg(Uas);
    controlTu->setInfoCarried(infoC);
    controlTu->setLocalUser(localUser.c_str());
    controlTu->setMyVoiceCodec(myVoiceCodec.c_str());
    controlTu->setCall2User(call2user);
    return controlTu;
}

void SipTU::finish() {

    recordScalar("Receive CALLs", recCALL);
    recordScalar("Receive TERMINATEs", recTERM);
    recordScalar("Receive CANTALKs", recCANT);

    recordScalar("Sent CALLs", recCALL);
    recordScalar("Sent TERMINATEs", sentTERM);
    recordScalar("Sent CANTALKs", sentCANT);

    recordScalar("Sent OK answers", sent200);
    recordScalar("Sent Not Found answers", sent404);
    recordScalar("Sent Not Acceptable answers", sent406);
    recordScalar("Sent Busy Here answers", sent486);
    recordScalar("Sent Not Acceptable answers", sent606);

    recordScalar("received OK", rec200);
    recordScalar("received Not Found", rec404);
    recordScalar("received Not Acceptable", rec406);
    recordScalar("received Busy Here", rec486);
    recordScalar("received Not Acceptable", rec606);

    if (timerStart != nullptr) {
        if (timerStart->isScheduled()) {
            cancelAndDelete(timerStart);
        }
        timerStart = nullptr;
    }
    if (timerStop != nullptr) {
        if (timerStop->isScheduled()) {
            cancelAndDelete(timerStart);
        }
        timerStop = nullptr;
    }
    if (delayTimer != nullptr) {
        if (delayTimer->isScheduled()) {
            cancelAndDelete(timerStart);
        }
        delayTimer = nullptr;
    }

}
