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
simple SipUAS
{
    parameters:
        double T1 @unit(s)=default(500ms);
      	double T2 @unit(s)=default(4s);
      	double T4 @unit(s)=default(5s);
      	//statistics
      	@signal[StateS](type=long);
        @statistic[StateT](source=StateS; record=vector);
    
    //The form of modules name:
    // type typeDestinationCompoundModule, 
    // the rule is described in the SipClient compound module
    // (SipClient.ned)
    gates:
        //to TU
    	input inTu;
    	output outTu;
    	//to DTBR
    	input inDtbr;
    	output outDtbr;
        
}
