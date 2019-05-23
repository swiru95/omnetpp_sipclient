/*
 * StatusOfUac.h
 *
 *  Created on: 7 sty 2018
 *      Author: Krzysztof Œwidrak
 */

#ifndef FUNC_SOURCE_STATUSOFUAC_H_
#define FUNC_SOURCE_STATUSOFUAC_H_
#include "Functions.h"
class StatusOfUac {
    int stateUac, cSeq;
    bool flagInvite; //if 0 then there is no invite else there is invite

    string toUser, fromUser, toTag, fromTag, callId, method, branch;
    L3Address toIpAddress, fromIpAddress;

    cMessage* timerA, *timerB, *timerK;

    double deltaT;

public:
    void print(const char* host) {
        ofstream os;
        string x = host;
        string opens = "simlogs//" + x + "_StatusOfUac.txt";

        os.open(opens, ios::app);
        if (os.is_open()) {
            os << "\n$ UAC" << host << " at: " << simTime() << "sec.\n\n";
            os << "Status nr: " << this->stateUac << endl << "To user: "
                    << this->getToUser() << ":" << this->getToIpAddress() << "|"
                    << this->getToTag() << endl << "FromUser: "
                    << this->getFromUser() << ":" << this->getFromIpAddress()
                    << "|" << this->getFromTag() << endl << "CallID: "
                    << this->getCallId();
            os << endl << endl << "$";
            os.close();
        } else {
            return;
        }
    }

    StatusOfUac(int state, bool flag, RequestSIP* req) :
            stateUac(state), flagInvite(flag) {

        toUser = req->getToUser();
        fromTag = req->getFromTag();
        fromUser = req->getFromUser();
        toTag = "";
        callId = req->getCall_ID();
        cSeq = req->getCSeq();
        branch = req->getBranch();
        method = req->getMethod();

        toIpAddress = L3Address(req->getToIpAddress());
        fromIpAddress = L3Address(req->getFromIpAddress());

        timerA = nullptr;
        timerB = nullptr;
        timerK = nullptr;

    }
    virtual ~StatusOfUac() {
        /*
         if (timerA != nullptr) {
         delete timerA;
         timerA = nullptr;
         }
         if (timerB != nullptr) {
         delete timerB;
         timerB = nullptr;
         }
         if (timerK != nullptr) {
         delete timerK;
         timerK = nullptr;
         }//*/
    }

    const string& getCallId() const {
        return callId;
    }

    void setCallId(const string& callId) {
        this->callId = callId;
    }

    bool isFlagInvite() const {
        return flagInvite;
    }

    void setFlagInvite(bool flagInvite) {
        this->flagInvite = flagInvite;
    }

    const string& getFromTag() const {
        return fromTag;
    }

    void setFromTag(const string& fromTag) {
        this->fromTag = fromTag;
    }

    const string& getFromUser() const {
        return fromUser;
    }

    void setFromUser(const string& fromUser) {
        this->fromUser = fromUser;
    }

    int getStateUac() const {
        return stateUac;
    }

    void setStateUac(int stateUac) {

        this->stateUac = stateUac;
    }

    const string& getToTag() const {
        return toTag;
    }

    void setToTag(const string& toTag) {
        this->toTag = toTag;
    }

    const string& getToUser() const {
        return toUser;
    }

    void setToUser(const string& toUser) {
        this->toUser = toUser;
    }

    const L3Address& getFromIpAddress() const {
        if (toIpAddress.isUnspecified()) {
            EV_WARN << "Unspecified Address.";
        }
        return fromIpAddress;
    }

    void setFromIpAddress(const L3Address& fromIpAddress) {
        this->fromIpAddress = fromIpAddress;
    }

    cMessage* getTimerA() const {
        return timerA;
    }

    void setTimerA(cMessage* timerA) {
        this->timerA = timerA;
    }

    cMessage* getTimerB() const {
        return timerB;
    }

    void setTimerB(cMessage* timerB) {
        this->timerB = timerB;
    }

    cMessage* getTimerK() const {
        return timerK;
    }

    void setTimerK(cMessage* timerK) {
        this->timerK = timerK;
    }

    const L3Address& getToIpAddress() const {
        if (toIpAddress.isUnspecified()) {
            EV_WARN << "Unspecified Address.";
        }
        return toIpAddress;
    }

    void setToIpAddress(const L3Address& toIpAddress) {
        this->toIpAddress = toIpAddress;
    }

    double getDeltaT() const {
        return deltaT;
    }

    void setDeltaT(double deltaT) {
        this->deltaT = deltaT;
    }

    int getSeq() const {
        return cSeq;
    }

    void setSeq(int seq) {
        cSeq = seq;
    }

    const string& getMethod() const {
        return method;
    }

    void setMethod(const string& method) {
        this->method = method;
    }

    const string& getBranch() const {
        return branch;
    }

    void setBranch(const string& branch) {
        this->branch = branch;
    }
};

#endif /* FUNC_SOURCE_STATUSOFUAC_H_ */
