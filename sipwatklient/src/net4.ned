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
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.node.inet.Router;

import inet.node.ethernet.Eth1G;
import inet.node.inet.StandardHost;


//
// Generated network with random topology (30 nodes, 50 edges, seed=100).
//
network Net4
{
    @display("bgb=1031,664");
    submodules:
        node0: Router {
            @display("p=305,217");
        }
        node1: Router {
            @display("p=529,289");
        }
        node2: Router {
            @display("p=503,228");
        }
        node3: Router {
            @display("p=418,311");
        }
        node4: Router {
            @display("p=520,108");
        }
        node5: Router {
            @display("p=665,349");
        }
        node6: Router {
            @display("p=337,259");
        }
        node7: Router {
            @display("p=344,318");
        }
        node8: Router {
            @display("p=290,163");
        }
        node9: Router {
            @display("p=153,318");
        }
        node10: Router {
            @display("p=481,502");
        }
        node11: Router {
            @display("p=219,248");
        }
        node12: Router {
            @display("p=559,420");
        }
        node13: Router {
            @display("p=103,84");
        }
        node14: Router {
            @display("p=797,405");
        }
        node15: Router {
            @display("p=768,568");
        }
        node16: Router {
            @display("p=853,320");
        }
        node17: Router {
            @display("p=290,411");
        }
        node18: Router {
            @display("p=432,228");
        }
        node19: Router {
            @display("p=692,217");
        }
        node20: Router {
            @display("p=919,133");
        }
        node21: Router {
            @display("p=266,115");
        }
        node22: Router {
            @display("p=164,194");
        }
        node23: Router {
            @display("p=64,234");
        }
        node24: Router {
            @display("p=635,163");
        }
        node25: Router {
            @display("p=322,523");
        }
        node26: Router {
            @display("p=131,420");
        }
        node27: Router {
            @display("p=422,115");
        }
        node28: Router {
            @display("p=390,194");
        }
        node29: Router {
            @display("p=344,146");
        }
        host2: StandardHost {
            @display("p=166,37");
        }
        host1: StandardHost {
            @display("p=64,31");
        }
        host3: StandardHost {
            @display("p=46,128");
        }
        host4: StandardHost {
            @display("p=853,67");
        }
        host5: StandardHost {
            @display("p=953,56");
        }
        host7: StandardHost {
            @display("p=978,180");
        }
        host9: StandardHost {
            @display("p=895,547");
        }
        host10: StandardHost {
            @display("p=836,610");
        }
        host11: StandardHost {
            @display("p=672,610");
        }
        host12: StandardHost {
            @display("p=471,610");
        }
        host13: StandardHost {
            @display("p=180,610");
        }
        host6: StandardHost {
            @display("p=46,376");
        }
        host8: StandardHost {
            @display("p=939,283");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=466,35");
        }
        host14: StandardHost {
            @display("p=633.26404,29.392");
        }
    connections:
        node0.ethg++ <--> Eth1G <--> node1.ethg++;
        node0.ethg++ <--> Eth1G <--> node21.ethg++;
        node0.ethg++ <--> Eth1G <--> node6.ethg++;
        node0.ethg++ <--> Eth1G <--> node22.ethg++;
        node0.ethg++ <--> Eth1G <--> node11.ethg++;
        node0.ethg++ <--> Eth1G <--> node28.ethg++;
        node1.ethg++ <--> Eth1G <--> node16.ethg++;
        node1.ethg++ <--> Eth1G <--> node2.ethg++;
        node1.ethg++ <--> Eth1G <--> node3.ethg++;
        node1.ethg++ <--> Eth1G <--> node19.ethg++;
        node1.ethg++ <--> Eth1G <--> node5.ethg++;
        node1.ethg++ <--> Eth1G <--> node25.ethg++;
        node1.ethg++ <--> Eth1G <--> node11.ethg++;
        node2.ethg++ <--> Eth1G <--> node4.ethg++;
        node2.ethg++ <--> Eth1G <--> node8.ethg++;
        node3.ethg++ <--> Eth1G <--> node18.ethg++;
        node3.ethg++ <--> Eth1G <--> node7.ethg++;
        node3.ethg++ <--> Eth1G <--> node10.ethg++;
        node3.ethg++ <--> Eth1G <--> node12.ethg++;
        node4.ethg++ <--> Eth1G <--> node24.ethg++;
        node4.ethg++ <--> Eth1G <--> node27.ethg++;
        node5.ethg++ <--> Eth1G <--> node6.ethg++;
        node5.ethg++ <--> Eth1G <--> node12.ethg++;
        node5.ethg++ <--> Eth1G <--> node14.ethg++;
        node6.ethg++ <--> Eth1G <--> node18.ethg++;
        node6.ethg++ <--> Eth1G <--> node9.ethg++;
        node6.ethg++ <--> Eth1G <--> node28.ethg++;
        node6.ethg++ <--> Eth1G <--> node13.ethg++;
        node7.ethg++ <--> Eth1G <--> node17.ethg++;
        node7.ethg++ <--> Eth1G <--> node9.ethg++;
        node7.ethg++ <--> Eth1G <--> node26.ethg++;
        node7.ethg++ <--> Eth1G <--> node11.ethg++;
        node8.ethg++ <--> Eth1G <--> node21.ethg++;
        node8.ethg++ <--> Eth1G <--> node28.ethg++;
        node8.ethg++ <--> Eth1G <--> node29.ethg++;
        node9.ethg++ <--> Eth1G <--> node23.ethg++;
        node10.ethg++ <--> Eth1G <--> node14.ethg++;
        node11.ethg++ <--> Eth1G <--> node22.ethg++;
        node12.ethg++ <--> Eth1G <--> node14.ethg++;
        node12.ethg++ <--> Eth1G <--> node15.ethg++;
        node14.ethg++ <--> Eth1G <--> node16.ethg++;
        node17.ethg++ <--> Eth1G <--> node25.ethg++;
        node17.ethg++ <--> Eth1G <--> node26.ethg++;
        node18.ethg++ <--> Eth1G <--> node27.ethg++;
        node19.ethg++ <--> Eth1G <--> node20.ethg++;
        node19.ethg++ <--> Eth1G <--> node24.ethg++;
        node21.ethg++ <--> Eth1G <--> node29.ethg++;
        node22.ethg++ <--> Eth1G <--> node23.ethg++;
        node24.ethg++ <--> Eth1G <--> node28.ethg++;
        node27.ethg++ <--> Eth1G <--> node29.ethg++;
        host1.ethg++ <--> Eth10M <--> node13.ethg++;
        host2.ethg++ <--> Eth10M <--> node13.ethg++;
        host3.ethg++ <--> Eth10M <--> node13.ethg++;
        host6.ethg++ <--> Eth10M <--> node26.ethg++;
        host13.ethg++ <--> Eth10M <--> node25.ethg++;
        host12.ethg++ <--> Eth10M <--> node10.ethg++;
        host11.ethg++ <--> Eth10M <--> node15.ethg++;
        host10.ethg++ <--> Eth10M <--> node15.ethg++;
        host9.ethg++ <--> Eth10M <--> node15.ethg++;
        host8.ethg++ <--> Eth10M <--> node16.ethg++;
        host7.ethg++ <--> Eth10M <--> node20.ethg++;
        host5.ethg++ <--> Eth10M <--> node20.ethg++;
        host4.ethg++ <--> Eth10M <--> node20.ethg++;
        node4.ethg++ <--> Eth100M <--> host14.ethg++;
}
