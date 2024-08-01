
TestRun: Execute
	./Execute --Input Input/PbPbDataxPbPbData_Centrality030_Reco2inf.root,Input/PbPbDataxMinBiasData_Centrality030_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030_Reco2inf_Different.root --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog --Output Data_Centrality030.root --NormalizationState Data_Centrality030 --NormalizationKey None,SBDiff,BBSame,BBDiff

Execute: CombineGraphs.cpp
	g++ CombineGraphs.cpp -o Execute \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/*.o \
		`root-config --cflags --libs`
