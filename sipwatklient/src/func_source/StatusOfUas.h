/*
 * StatusOfUas.h
 *
 *  Created on: 7 sty 2018
 *      Author: Krzysztof Œwidrak
 */

#ifndef FUNC_SOURCE_STATUSOFUAS_H_
#define FUNC_SOURCE_STATUSOFUAS_H_
#include "Functions.h"
class StatusOfUas {
    int cSeq, stateUas;
    bool flagInvite;
    string toUser, fromUser, toTag, fromTag, callId, method, branch;
    L3Address toIpAddress, fromIpAddress;

    cMessage* timerA, *timerB;

    double deltaT;

public:
    StatusOfUas(int state, bool flag, RequestSIP* req) :
            stateUas(state) {

        toUser = req->getToUser();
        fromTag = req->getFromTag();
        fromUser = req->getFromUser();
        toTag = random_string(5);
        callId = req->getCall_ID();
        cSeq = req->getCSeq();
        branch = req->getBranch();
        method=req->getMethod();

        toIpAddress = L3Address(req->getToIpAddress());
        fromIpAddress = L3Address(req->getFromIpAddress());


        timerA = nullptr;
        timerB = nullptr;

    }
    ;
    void print(const char* host) {
        ofstream os;
        string x = host;
        string opens = "simlogs//" + x + "_StatusOfUas.txt";

        os.open(opens, ios::app);
        if (os.is_open()) {
            os << "\n$ UAC" << host << " at: " << simTime() << "sec.\n\n";
            os << "Status nr: " << this->stateUas << endl << "To user: "
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

    ~StatusOfUas() {

        /*if (timerA != nullptr) {
            delete timerA;
            timerA = nullptr;
        }
        if (timerB != nullptr) {
            delete timerB;
            timerB = nullptr;
        }//*/
    }
    const string& getCallId() const {
        return callId;
    }

    void setCallId(const string& callId) {
        this->callId = callId;
    }

    double getDeltaT() const {
        return deltaT;
    }

    void setDeltaT(double deltaT) {
        this->deltaT = deltaT;
    }

    bool isFlagInvite() const {
        return flagInvite;
    }

    void setFlagInvite(bool flagInvite) {
        this->flagInvite = flagInvite;
    }

    const L3Address& getFromIpAddress() const {
        return fromIpAddress;
    }

    void setFromIpAddress(const L3Address& fromIpAddress) {
        this->fromIpAddress = fromIpAddress;
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

    int getStateUas() const {
        return stateUas;
    }

    void setStateUas(int stateUas) {
        this->stateUas = stateUas;
    }

    cMessage * getTimerA() const {
        return timerA;
    }

    void setTimerA(cMessage* timerA) {
        this->timerA = timerA;
    }

    cMessage * getTimerB() const {
        return timerB;
    }

    void setTimerB(cMessage* timerB) {
        this->timerB = timerB;
    }

    const L3Address& getToIpAddress() const {
        return toIpAddress;
    }

    void setToIpAddress(const L3Address& toIpAddress) {
        this->toIpAddress = toIpAddress;
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

#endif /* FUNC_SOURCE_STATUSOFUAS_H_ */
