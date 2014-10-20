-- A solution contains projects, and defines the available configurations
solution "CarlSim"
	configurations { "Debug", "Release" }
	
	project "CarlSim"
		language "C++"
		kind     "SharedLib"
		files  { "../*.h",
				 "../*.cpp",
				 "../*.cu"}
		includedirs { "/usr/local/cuda/include", 
					  "/home/ubuntu/NVIDIA_CUDA-6.0_Samples/common/inc" }
		links { "dl", "cudart" }
	  
		configuration { "Debug", "linux" }
			defines { "_DEBUG", "CARLSIM_EXPORTS"	}
			flags   { "Symbols", "SEH" }
			targetdir ("Debug")
			targetname ("CarlSim_debug")
	 
		configuration { "Release", "linux" }
			defines { "NDEBUG", "CARLSIM_EXPORTS" }
			flags   { "Optimize", "SEH" }
			targetdir ("Release")
			targetname ("CarlSim")
