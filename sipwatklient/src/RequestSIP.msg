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

//
// TODO generated message class
//
cplusplus
{{
     #include "inet/networklayer/common/L3Address.h"
}}    
class noncobject inet::L3Address;
namespace inet;
enum RequestType
    {
        INVITE=1;
        BYE=2;
        CANCEL=3;
        ACK=4;
    };

packet RequestSIP {
    //SIP Header
   		//SIP Header
   	int requestType @enum(RequestType);
    //IpAddress and Users are creating the SIP URI:  sip:/user/@/IPAddress/`
   	string toIpAddress; 		//where to call
    string fromIpAddress;  		//calleeAddress
    
    string toUser;				//User name To:
    string fromUser;  			//User name From:
      
      //Tags
    string toTag;
    string fromTag;
      
      //Other
    string branch;
    int cSeq;
    string method;
    int maxForwards=10;
    string call_ID;
   	int content_Length;
   	string content_Type;
   
    //SDP
    int vSdp=0;				//v=0
    //m=audio 53030 RTP 1
    string myVoiceCodec;	//a=rtp 1 ____

}
class RequestSIPControlInfo {
 
 L3Address destAddress;
    
}
