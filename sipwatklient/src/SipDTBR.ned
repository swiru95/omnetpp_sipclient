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

package sipwatklient;
//
// TODO auto-generated module
//
simple SipDTBR
{
    
    parameters:
        int destPort=default(5060);		//The port in which the SIP data is being sended
        int localPort=default(5060);	//The port which i use to bind the socket
        
    //The form of modules name:
    // type typeDestinationCompoundModule, 
    // the rule is described in the SipClient compound module
    // (SipClient.ned)
    gates:
        ///To SipEngine
        //To UAC
        input inUacSipEngine;
     	output outUacSipEngine;
     	//To UAS
     	input inUasSipEngine;
     	output outUasSipEngine;
     	///The way out
     	//To Client (compound module)
     	input inSipClient;
     	output outSipClient;  
        
}
