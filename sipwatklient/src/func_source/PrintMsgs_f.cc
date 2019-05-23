/*
 * PrintMsgs_f.cc
 *
 *  Created on: 5 sty 2018
 *      Author: Krzysztof Œwidrak
 */

#include "Functions.h"

/*std::ofstream& operator<<(std::ofstream& os, RequestSIP* req) {

 if (req->getRequestType() == INVITE) {
 os << req->getMethod() << " sip:" << req->getToUser() << "@"
 << req->getToIpAddress() << ":5060 SIP/2.0" << endl
 << "Via: SIP/2.0/UDP " << req->getFromIpAddress()
 << ":5060;branch=" << req->getBranch() << endl << "CSeq: "
 << req->getCSeq() << " " << req->getMethod() << endl
 << "Call-ID: " << req->getCall_ID() << endl << "To: <sip:"
 << req->getToUser() << "@" << req->getToIpAddress() << ">"
 << endl << "From: <sip:" << req->getToUser() << "@"
 << req->getToIpAddress() << ">" << endl << "Max-Forwards: "
 << req->getMaxForwards() << endl << "Content-Type: "
 << req->getContent_Type() << endl << "Content-Length: "
 << req->getContent_Length() << endl << endl
 << "v=0\nm=audio 53001 RTP 1\na=rtp: 1 "
 << req->getMyVoiceCodec();
 }
 else
 {
 os << req->getMethod() << " sip:" << req->getToUser() << "@"
 << req->getToIpAddress() << ":5060 SIP/2.0" << endl
 << "Via: SIP/2.0/UDP " << req->getFromIpAddress()
 << ":5060;branch=" << req->getBranch() << endl << "CSeq: "
 << req->getCSeq() << " " << req->getMethod() << endl
 << "Call-ID: " << req->getCall_ID() << endl << "To: <sip:"
 << req->getToUser() << "@" << req->getToIpAddress() << ">"
 << endl << "From: <sip:" << req->getToUser() << "@"
 << req->getToIpAddress() << ">" << endl << "Max-Forwards: "
 << req->getMaxForwards() << endl;
 }

 return os;
 }*/
void printRequest(RequestSIP* req, const char* file, const char* host) {

    ofstream os;
    string x = file;
    string opens = "simlogs//" + x;

    os.open(opens, ios::app);
    if (os.is_open()) {
        os << "\n$" << host << " at: " << simTime() << "sec.\n\n";
        if (req->getRequestType() == INVITE) {
            os << req->getMethod() << " sip:" << req->getToUser() << "@"
                    << req->getToIpAddress() << ":5060 SIP/2.0" << endl
                    << "Via: SIP/2.0/UDP " << req->getFromIpAddress()
                    << ":5060;branch=" << req->getBranch() << endl << "CSeq: "
                    << req->getCSeq() << " " << req->getMethod() << endl
                    << "Call-ID: " << req->getCall_ID() << endl << "To: <sip:"
                    << req->getToUser() << "@" << req->getToIpAddress() << ">"
                    << req->getToTag() << endl << "From: <sip:"
                    << req->getFromUser() << "@" << req->getFromIpAddress()
                    << ">; tag=" << req->getFromTag() << endl
                    << "Max-Forwards: " << req->getMaxForwards() << endl
                    << "Content-Type: " << req->getContent_Type() << endl
                    << "Content-Length: " << req->getContent_Length() << endl
                    << endl << "v=0\nm=audio 53001 RTP 1\na=rtp: 1 "
                    << req->getMyVoiceCodec();

        } else {
            os << req->getMethod() << " sip:" << req->getToUser() << "@"
                    << req->getToIpAddress() << ":5060 SIP/2.0" << endl
                    << "Via: SIP/2.0/UDP " << req->getFromIpAddress()
                    << ":5060;branch=" << req->getBranch() << endl << "CSeq: "
                    << req->getCSeq() << " " << req->getMethod() << endl
                    << "Call-ID: " << req->getCall_ID() << endl << "To: <sip:"
                    << req->getToUser() << "@" << req->getToIpAddress()
                    << ">; tag=" << req->getToTag() << endl << "From: <sip:"
                    << req->getFromUser() << "@" << req->getFromIpAddress()
                    << ">; tag=" << req->getFromTag() << endl
                    << "Max-Forwards: " << req->getMaxForwards() << endl;
        }
        os << endl << endl << "$";
        os.close();
    } else {
        return;
    }
}
void printAnswer(AnswerSIP* req, const char* file, const char* host) {

    ofstream os;
    string x = file;
    string opens = "simlogs//" + x;

    string ansType;
    int i = req->getAnswerType();
    switch (i) {
    case 180:
        ansType = "Ringing";
        break;
    case 200:
        ansType = "OK";
        break;
    case 404:
        ansType = "Not Found";
        break;
    case 406:
        ansType = "Not Acceptable";
        break;
    case 486:
        ansType = "Busy Here";
        break;
    case 606:
        ansType = "Not Acceptable";
        break;
    }

    os.open(opens, ios::app);
    if (os.is_open()) {
        os << "\n$" << host << " at: " << simTime() << "sec.\n\n";
        os << "SIP/2.0 "<<req->getAnswerType() <<" "<< ansType << endl
                << "Via: SIP/2.0/UDP "
                << req->getFromIpAddress() << ":5060;branch="
                << req->getBranch() << endl << "CSeq: " << req->getCSeq() << " "
                << req->getMethod() << endl << "Call-ID: " << req->getCall_ID()
                << endl << "To: <sip:" << req->getToUser() << "@"
                << req->getToIpAddress() << ">; tag=" << req->getToTag() << endl
                << "From: <sip:" << req->getFromUser() << "@"
                << req->getFromIpAddress() << ">; tag=" << req->getFromTag();

        os << endl << endl << "$";
        os.close();
    } else {
        return;
    }
}
