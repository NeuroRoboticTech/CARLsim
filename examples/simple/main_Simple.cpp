#include "snn.h"
/*
int main()
{
	CpuSNN *sim2 = new CpuSNN("test");

	delete sim2;

	CpuSNN sim("My network");
	bool onGPU = true; // run network on GPU
	int ithGPU = 0; // run on 2nd device in a multi-GPU system

	// create topology
	int gIn=sim.createSpikeGeneratorGroup("input", 100, EXCITATORY_NEURON);
	int gEx=sim.createGroup("excitatory", 100, EXCITATORY_NEURON);
	sim.setNeuronParameters(gEx, 0.02f, 0.2f, -65.0f, 8.0f); // RS neurons
	sim.connect(gIn, gEx, "random", 1.0, 1.0, 0.10f, 1, 20, SYN_FIXED);

	// run in COBA mode
	sim.setConductances(ALL, true, 5.0, 150.0, 6.0, 150.0);

	// set log cycle to 1sec and print to stdout
	sim.setLogCycle(1, 1, stdout);

	// set up where to store the spikes
	sim.setSpikeMonitor(gEx, "C:/Projects/AnimatLabSDK/3rdParty/carlsim-2.2.0/examples/Example/spkEx.dat"); // write spike times to file

	// initialize the GPU / network
	sim.runNetwork(0,0, onGPU?GPU_MODE:CPU_MODE,ithGPU);

	// set Poisson rates for all neurons
	PoissonRate inSpikes(100);
	for (int i=0; i<100; i++)
		inSpikes.rates[i] = 50.0f;
	sim.setSpikeRate(gIn, &inSpikes);

	// run the network for 1 s and 0 ms
	sim.runNetwork(1,0, onGPU?GPU_MODE:CPU_MODE,ithGPU);

	// store the network state
	FILE* nid = fopen("C:/Projects/AnimatLabSDK/3rdParty/carlsim-2.2.0/examples/Example/network.dat","wb");
	sim.writeNetwork(nid);
	fclose(nid);

	//int Npre, Npost;
	//int gIdPre = 0, gIdPost = 0;
	//float* weights = sim.getWeights(gIdPre,gIdPost,Npre,Npost);
	//std::cout << "The weight of the synapse between presynaptic neuron 3 and postsynaptic neuron 5 is" << weights[5+Npre*3] << endl;
}
*/
int main()
{
	CpuSNN *sim = new CpuSNN("My network");
	bool onGPU = true; // run network on GPU
	int ithGPU = 0; // run on 2nd device in a multi-GPU system

	// create topology
	int gIn=sim->createSpikeGeneratorGroup("input", 100, EXCITATORY_NEURON);
	int gEx=sim->createGroup("excitatory", 100, EXCITATORY_NEURON);
	sim->setNeuronParameters(gEx, 0.02f, 0.2f, -65.0f, 8.0f); // RS neurons
	sim->connect(gIn, gEx, "random", 1.0, 1.0, 0.10f, 1, 20, SYN_FIXED);

	// run in COBA mode
	sim->setConductances(ALL, true, 5.0, 150.0, 6.0, 150.0);

	// set log cycle to 1sec and print to stdout
	sim->setLogCycle(1, 1, stdout);

	// set up where to store the spikes
	sim->setSpikeMonitor(gEx, "spkEx.dat"); // write spike times to file

	// initialize the GPU / network
	sim->runNetwork(0,0, onGPU?GPU_MODE:CPU_MODE,ithGPU);

	// set Poisson rates for all neurons
	PoissonRate inSpikes(100);
	for (int i=0; i<100; i++)
		inSpikes.rates[i] = 50.0f;
	sim->setSpikeRate(gIn, &inSpikes);

	// run the network for 1 s and 0 ms
	sim->runNetwork(2,0, onGPU?GPU_MODE:CPU_MODE,ithGPU);

	// store the network state
	FILE* nid = fopen("network.dat","wb");
	sim->writeNetwork(nid);
	fclose(nid);

	//int Npre, Npost;
	//int gIdPre = 0, gIdPost = 0;
	//float* weights = sim.getWeights(gIdPre,gIdPost,Npre,Npost);
	//std::cout << "The weight of the synapse between presynaptic neuron 3 and postsynaptic neuron 5 is" << weights[5+Npre*3] << endl;

	delete sim;
	sim = NULL;
}

