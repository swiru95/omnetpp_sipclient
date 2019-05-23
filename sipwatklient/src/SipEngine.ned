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

import ned.IdealChannel;

module SipEngine
{
    //The form of modules name:
    // type typeDestinationCompoundModule, 
    // the rule is described in the SipClient compound module
    // (SipClient.ned)
    gates:
        ///////for UAC
        //to TU
        input inUacTu;
        output outUacTu;
        //to DTR
        input inUacDtbr;
        output outUacDtbr;
        ///////for UAS
        //to TU
        input inUasTu;
        output outUasTu;
        //to DTR
        input inUasDtbr;
        output outUasDtbr;


    types:
    submodules:
        sipUAC: SipUAC {
            @display("p=73,95");
        }
        sipUAS: SipUAS {
            @display("p=146,95");
        }
    connections:

        inUacDtbr  --> sipUAC.inDtbr;
        inUacTu  --> sipUAC.inTu;
        sipUAC.outDtbr  --> outUacDtbr;
        sipUAC.outTu  --> outUacTu;
        sipUAS.outDtbr  --> outUasDtbr;
        sipUAS.outTu  --> outUasTu;
        inUasDtbr --> sipUAS.inDtbr;
        inUasTu  --> sipUAS.inTu;
}
