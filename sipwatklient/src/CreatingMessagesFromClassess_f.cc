/*
 * CreatingMessagesFromClassess_f.cc
 *
 *  Created on: 6 sty 2018
 *      Author: Krzysztof Œwidrak
 */
#include "SipUAS.h"
#include "SipUAC.h"
#include "SipTU.h"
#include "AnswerSIP_m.h"
#include "RequestSIP_m.h"

int SipUAC::countContentLength(RequestSIP* requestSip) {
//default SDP:
    /*
     * v=0
     * m=audio 53001 RTP 1
     * a=rtp 1 /myVoiceCodec/
     */
    string seq = requestSip->getMyVoiceCodec();
    int length = 34 + (int) strlen(seq.c_str());
    return length;
}
void* SipUAC::createIniRequest(ControlTU* controlTu, int type) {

    RequestSIP * requestSip = new RequestSIP();
    string seq;
    int i = 1;
    if (type == INVITE) {
        i = createdInv;
        seq = "INVITE";
    } else if (type == BYE) {
        i = createdBye;
        seq = "BYE";
    } else {
        error("Error in creating iniReques, no type!! (332UAC)");
    }
    requestSip->setName(seq.c_str());
    requestSip->setRequestType(type);
    requestSip->setMethod(seq.c_str());
    L3Address addr = controlTu->getCall2Address();
    requestSip->setToIpAddress(addr.str().c_str());
    requestSip->setToUser(controlTu->getCall2User());
    addr = L3AddressResolver().resolve(
            getParentModule()->getParentModule()->getParentModule()->getName(),
            L3AddressResolver::ADDR_IPv4);
    requestSip->setFromIpAddress(addr.str().c_str());
    requestSip->setFromUser(controlTu->getLocalUser());
    seq = random_string(5);
    requestSip->setFromTag(seq.c_str());
    seq = "";
    requestSip->setToTag(seq.c_str());
    seq = random_string(5);
    requestSip->setCall_ID(seq.c_str());
    seq = random_string(5);
    requestSip->setBranch(seq.c_str());
    requestSip->setCSeq(i);
    if (type == INVITE) {
        seq = "application/SDP";
        requestSip->setContent_Type(seq.c_str());
        //SDP
        /*
         * v=0
         * m=audio 53001 RTP 1
         * a=rtp 1 /myVoiceCodec/
         */
        /////
        seq = (string) controlTu->getMyVoiceCodec();
        requestSip->setMyVoiceCodec(controlTu->getMyVoiceCodec());
        requestSip->setContent_Length(countContentLength(requestSip));
    }
    RequestSIPControlInfo * info = new RequestSIPControlInfo();
    info->setDestAddress(controlTu->getCall2Address());
    requestSip->setControlInfo(info);
    requestSip->setByteLength(calculateLengthOfMsg(requestSip));
    return requestSip;

}
void* SipUAC::createFinRequest(int type, StatusOfUac* sou) {
    RequestSIP *requestSip = new RequestSIP();
    string str;
    int i;
    if (type == ACK) {
        str = "ACK";
        i = createdAck;
    } else if (type == CANCEL) {
        str = "CANCEL";
        i = createdCan;
    } else {
        error("Error in 372UAC");
        return nullptr;
    }
    requestSip->setName(str.c_str());
    requestSip->setRequestType(type);
    requestSip->setMethod(str.c_str());
    L3Address addr = sou->getToIpAddress();
    requestSip->setToIpAddress(addr.str().c_str());
    requestSip->setToUser(sou->getToUser().c_str());
    addr = L3AddressResolver().resolve(
            getParentModule()->getParentModule()->getParentModule()->getName(),
            L3AddressResolver::ADDR_IPv4);
    requestSip->setFromIpAddress(addr.str().c_str());
    requestSip->setFromUser(sou->getFromUser().c_str());
    requestSip->setFromTag(sou->getFromTag().c_str());
    requestSip->setToTag(sou->getToTag().c_str());
    requestSip->setCall_ID(sou->getCallId().c_str());
    str = random_string(5);
    requestSip->setBranch(str.c_str());
    requestSip->setCSeq(i);

    RequestSIPControlInfo * info = new RequestSIPControlInfo();
    info->setDestAddress(sou->getToIpAddress());
    requestSip->setControlInfo(info);
    requestSip->setByteLength(calculateLengthOfMsg(requestSip));
    return requestSip;
}
void* SipUAS::createAnswer(int type, StatusOfUas *sou) {

    AnswerSIP* answerSip = new AnswerSIP();
    string str;
    if (type == Ringing) {
        str = "180 Ringing";
        create180++;
    } else if (type == OK) {
        str = "200 OK";
        create200++;
    } else if (type == Not_Found) {
        str = "404 Not Found";
        create404++;
    } else if (type == Not_Acceptable4) {
        str = "406 Not Acceptable";
        create406++;
    } else if (type == Busy_Here) {
        str = "486 Busy Here";
        create486++;
    } else if (type == Not_Acceptable6) {
        str = "606 Not Acceptable";
        create606++;
    } else {
        error("Error in 178UAS");
    }
    answerSip->setName(str.c_str());
    answerSip->setMethod(sou->getMethod().c_str());
    answerSip->setAnswerType(type);
    answerSip->setFromUser(sou->getFromUser().c_str());
    answerSip->setFromIpAddress(sou->getFromIpAddress().str().c_str());
    answerSip->setToUser(sou->getToUser().c_str());
    answerSip->setToIpAddress(sou->getToIpAddress().str().c_str());
    answerSip->setToTag(sou->getToTag().c_str());
    answerSip->setFromTag(sou->getFromTag().c_str());
    answerSip->setBranch(sou->getBranch().c_str());
    answerSip->setCSeq(sou->getSeq());
    answerSip->setCall_ID(sou->getCallId().c_str());
    answerSip->setByteLength(calculateLengthOfMsg(answerSip));

    AnswerSIPControlInfo * info = new AnswerSIPControlInfo();
    info->setDestAddress(sou->getFromIpAddress());
    answerSip->setControlInfo(info);

    return answerSip;

}
