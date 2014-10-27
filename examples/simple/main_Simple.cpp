#include "snn.h"

int main()
{
	CpuSNN *sim = new CpuSNN("My network");
	bool onGPU = false; // run network on GPU
	int ithGPU = 0; // run on 2nd device in a multi-GPU system

	// create topology
	int gIn1=sim->createSpikeGeneratorGroup("input1", 1, EXCITATORY_NEURON);
	int gIn2=sim->createSpikeGeneratorGroup("input2", 1, EXCITATORY_NEURON);
	//int gEx=sim->createGroup("excitatory", 100, EXCITATORY_NEURON);
	int gEx1=sim->createGroup("A", 1, INHIBITORY_NEURON);
	int gEx2=sim->createGroup("B", 1, EXCITATORY_NEURON);

	sim->setNeuronParameters(gEx1, 0.1f, 0.2f, -65.0f, 2.0f); // RS neurons
	sim->setNeuronParameters(gEx2, 0.02f, 0.2f, -65.0f, 8.0f); // RS neurons

	sim->connect(gIn1, gEx1, "one-to-one", 0.1, 0.1, 1, 1, 20, SYN_FIXED);
	sim->connect(gIn2, gEx2, "one-to-one", 0.1, 0.1, 1, 1, 20, SYN_FIXED);
	sim->connect(gEx1, gEx2, "one-to-one", -1, -1, 1, 1, 20, SYN_FIXED);

	// run in COBA mode
	sim->setConductances(ALL, true, 5.0, 150.0, 6.0, 150.0);

	// set log cycle to 1sec and print to stdout
	sim->setLogCycle(1, 1, stdout);

	// set up where to store the spikes
	sim->setSpikeMonitor(gEx1, "spkEx1.dat"); // write spike times to file
	sim->setSpikeMonitor(gEx2, "spkEx2.dat"); // write spike times to file

	// initialize the GPU / network
	sim->runNetwork(0,0, onGPU?GPU_MODE:CPU_MODE,ithGPU);

	// set Poisson rates for all neurons
	PoissonRate in1Spikes(1);
	PoissonRate in2Spikes(1);
	in1Spikes.rates[0] = 0.0f;
	in2Spikes.rates[0] = 50.0f;
	sim->setSpikeRate(gIn1, &in1Spikes);
	sim->setSpikeRate(gIn2, &in2Spikes);

	// run the network for 1 s and 0 ms
	sim->runNetwork(1,0, onGPU?GPU_MODE:CPU_MODE,ithGPU);

	in1Spikes.rates[0] = 50.0f;
	sim->setSpikeRate(gIn1, &in1Spikes);

	// run the network for 1 s and 0 ms
	sim->runNetwork(1,0, onGPU?GPU_MODE:CPU_MODE,ithGPU);

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

