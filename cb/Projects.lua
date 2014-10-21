-- A solution contains projects, and defines the available configurations
solution "CarlSim"
	configurations { "Debug", "Release" }
	
	project "CarlSim"
		language "C++"
		kind     "SharedLib"
		files  { "../src/config.h",
				 "../src/CUDAVersionControl.h",
				 "../src/errorCode.h",
				 "../src/gpu.h",
				 "../src/gpu_random.cu",
				 "../src/gpu_random.h",
				 "../src/mtrand.cpp",
				 "../src/mtrand.h",
				 "../src/printSNNInfo.cpp",
				 "../src/PropagatedSpikeBuffer.cpp",
				 "../src/PropagatedSpikeBuffer.h",
				 "../src/snn.h",
				 "../src/snn_cpu.cpp",
				 "../src/snn_gpu.cu",
				 "../src/v1ColorME.2.1.cu"}
		includedirs { "/usr/local/cuda/include", 
					  "/home/ubuntu/NVIDIA_CUDA-6.0_Samples/common/inc" }
		links { "dl", "cudart" }
	  
		configuration { "Debug", "linux" }
			defines { "_DEBUG", "CARLSIM_EXPORTS"	}
			flags   { "Symbols", "SEH" }
			targetdir ("Debug")
			targetname ("CarlSim_debug")
			postbuildcommands { "cp Debug/libCARLsim_debug.so ../lib" }
	 
		configuration { "Release", "linux" }
			defines { "NDEBUG", "CARLSIM_EXPORTS" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release")
			targetname ("CarlSim")
			postbuildcommands { "cp Debug/libCARLsim.so ../lib" }


	project "simple"
		language "C++"
		kind     "ConsoleApp"
		files  { "../examples/simple/*.h",
                 "../examples/simple/*.cpp"}
		includedirs { "/usr/local/cuda/include", 
					  "/home/ubuntu/NVIDIA_CUDA-6.0_Samples/common/inc",
					  "../src" }
		
		configuration { "Debug", "linux" }
			defines { "_DEBUG" }
			flags   { "Symbols", "SEH" }
			libdirs { "../../../bin" }
			links { "cudart", 
					"CarlSim_debug"}
			targetdir ("Debug")
			targetname ("simple_debug")				
	 
		configuration { "Release", "linux" }
			defines { "NDEBUG" }
			flags   { "Optimize", "SEH" }
			links { "cudart",
					"CarlSim"}					
			targetdir ("Release")
			targetname ("simple")				
					

											