/*
 * CountLengthSipMsgs_f.cc
 *
 *  Created on: 5 sty 2018
 *      Author: Krzysztof Œwidrak
 */

#include "Functions.h"

int calculateLengthOfMsg(RequestSIP* req) {

    int standard;
    if(req->getRequestType()==INVITE){
    standard = 172;
    } else {
    standard = 137;
    }

    //STANDARD:
    /*
     *
     _sip:@:5060_SIP/2.0
     Via:_SIP/2.0/UDP_:5060;_branch=
     CSeq:_1_
     Call-ID:_
     To:__<sip:@>_tag:
     From:__<sip:@>_tag:
     Max-Forwards:_10
     Content-Type:_
     Content-Length:_


     *
     */
    string str;
    string strFull = "";
    str = req->getToIpAddress();
    strFull += str;
    str = req->getToUser();
    strFull += str;
    str = req->getFromIpAddress();
    strFull += str;
    str = req->getFromUser();
    strFull += str;
    str = req->getToTag();
    strFull += str;
    str = req->getFromTag();
    strFull += str;
    str = req->getBranch();
    strFull += str;
    str = req->getCall_ID();
    strFull += str;
    str = req->getMethod();
    strFull += str;             //The beginning of header ex. INVITE
    strFull += str;             //The cSeq field ex. INVITE
    str = req->getContent_Type();
    strFull += str;
    int i = standard +((int)strlen(strFull.c_str()))
            + (req->getContent_Length());
    //////////////
    return i;
}
int calculateLengthOfMsg(AnswerSIP* ans) {

    const int standard = 101;
    //STANDARD:
    /*
     *
     SIP/2.0_200_
     Via:_SIP/2.0/UDP_:5060;_branch=
     CSeq:_1_
     Call-ID:_
     To:__<sip:@>
     From:__<sip:@>

     */
    int type;
    int typeOfMsg = ans->getAnswerType();
    if (typeOfMsg == 200) { //INVITE/CANCEL
        type = 2;
    }
    if (typeOfMsg == 404 or typeOfMsg == 486) { //BYE/ACK
        type = 9;
    }
    if (typeOfMsg == 406 or typeOfMsg == 606) {
        type = 14;
    }
    if (typeOfMsg == 180) {
        type = 7;
    }
    string str;
    string strFull = "";
    str = ans->getToIpAddress();
    strFull += str;
    str = ans->getToUser();
    strFull += str;
    str = ans->getFromIpAddress();
    strFull += str;
    str = ans->getFromUser();
    strFull += str;
    str = ans->getToTag();
    strFull += str;
    str = ans->getFromTag();
    strFull += str;
    str = ans->getBranch();
    strFull += str;
    str = ans->getMethod();
    strFull += str;
    str = ans->getCall_ID();
    strFull += str;
    int i = standard + type + ((int) (strlen(strFull.c_str())));
    //////////////
    return i;
}


