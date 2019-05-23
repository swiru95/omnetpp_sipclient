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

enum AnswerType
    {
        OK=200;
        Busy_Here=486;
        Ringing=180;
        Not_Acceptable4=406;
        Not_Acceptable6=606;
        Not_Found=404;
    };



packet AnswerSIP {
   	
   	
   	
   	//SIP Header
   	int answerType @enum(AnswerType);
   	//IpAddress and Users are creating the SIP URI:  sip:/user/@/IPAddress/
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
    string call_ID;
    	
    
}
class AnswerSIPControlInfo {
 
 L3Address destAddress;
    
}
