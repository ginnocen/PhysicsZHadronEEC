

default: TestRunGenSelf TestRunPlot Deploy

TestRunGenSelf: Execute
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS0xDYLLS0.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true --SubEvent 0,0
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLxDYLL.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true --SubEvent -1,-1
	./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
		--IsPP false --IsReco false --Output DYLLxDYLLS1.root --HFShift 0 --OverSample 5 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true \
		--SubEvent -1,1 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
		--IsPP false --IsReco false --Output DYLLxDYLLS0.root --HFShift 0 --OverSample 5 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true \
		--SubEvent -1,0 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS1xDYLLS1.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true --Fraction 1 \
		--SubEvent 1,1 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
		--Output DYLLS1xDYLLS1_Different1.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ true \
		--IsPP false --IsReco false --SubEvent 1,1 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen2.root --Background Input/DYLLMC_Gen1.root --SelfMixing false \
		--Output DYLLS1xDYLLS1_Different2.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ true \
		--IsPP false --IsReco false --SubEvent 1,1 --CheckBackgroundZ true
	hadd -f DYLLS1xDYLLS1_Different.root DYLLS1xDYLLS1_Different1.root DYLLS1xDYLLS1_Different2.root

TestRunGen: Execute
	# ./Execute --Signal Input/DYLLMC_Gen.root --Background Input/MinBiasMC_Gen.root --SelfMixing false \
	# 	--IsPP false --IsReco false --Output DYLLxHydjet.root --HFShift 0 --OverSample 5 \
	# 	--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true \
	# 	--SubEvent -1,-1 --CheckBackgroundZ false
	./Execute --Signal Input/MinBiasMC_Gen.root --Background None --SelfMixing true --Output HydjetxHydjet.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ false --Fraction 1 \
		--SubEvent -1,-1 --CheckBackgroundZ false
	./Execute --Signal Input/MinBiasMC_Gen1.root --Background Input/MinBiasMC_Gen2.root --SelfMixing false \
		--Output HydjetxHydjet_Different1.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
		--IsPP false --IsReco false --SubEvent -1,-1 --CheckBackgroundZ false
	./Execute --Signal Input/MinBiasMC_Gen2.root --Background Input/MinBiasMC_Gen1.root --SelfMixing false \
		--Output HydjetxHydjet_Different2.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
		--IsPP false --IsReco false --SubEvent -1,-1 --CheckBackgroundZ false
	hadd -f HydjetxHydjet_Different.root HydjetxHydjet_Different1.root HydjetxHydjet_Different2.root

TestRunGenCentrality: Execute
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS0xDYLLS0_Centrality.root \
		--IsPP false --IsReco false --CheckSignalHiBin true --CheckSignalHF false --CheckZ true --SubEvent 0,0 \
		--SignalHFMin 20 --SignalHFMax 60
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLxDYLL_Centrality.root \
		--IsPP false --IsReco false --CheckSignalHiBin true --CheckSignalHF false --CheckZ true --SubEvent -1,-1 \
		--SignalHFMin 20 --SignalHFMax 60
	./Execute --Signal Input/DYLLMC_Gen.root --Background Input/MinBiasMC_Gen.root --SelfMixing false \
		--IsPP false --IsReco false --Output DYLLxHydjet_Centrality.root --HFShift 0 --OverSample 5 \
		--CheckSignalHiBin true --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true \
		--SubEvent -1,-1 --CheckBackgroundZ true --SignalHFMin 20 --SignalHFMax 60
	# ./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
	# 	--IsPP false --IsReco false --Output DYLLxDYLLS0_Centrality.root --HFShift 0 --OverSample 5 \
	# 	--CheckSignalHiBin true --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true \
	# 	--SubEvent -1,0 --CheckBackgroundZ true --SignalHFMin 20 --SignalHFMax 60
	# ./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS1xDYLLS1.root \
	# 	--IsPP false --IsReco false --CheckSignalHiBin true --CheckSignalHF false --CheckZ true --Fraction 1 \
	# 	--SubEvent 1,1 --CheckBackgroundZ true --SignalHFMin 20 --SignalHFMax 60
	# ./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
	# 	--Output DYLLS1xDYLLS1_Different1.root --CheckSignalHiBin false --CheckSignalHF false \
	# 	--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ true \
	# 	--IsPP false --IsReco false --SubEvent 1,1 --CheckBackgroundZ true
	# ./Execute --Signal Input/DYLLMC_Gen2.root --Background Input/DYLLMC_Gen1.root --SelfMixing false \
	# 	--Output DYLLS1xDYLLS1_Different2.root --CheckSignalHiBin false --CheckSignalHF false \
	# 	--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ true \
	# 	--IsPP false --IsReco false --SubEvent 1,1 --CheckBackgroundZ true
	hadd -f DYLLS1xDYLLS1_Different.root DYLLS1xDYLLS1_Different1.root DYLLS1xDYLLS1_Different2.root


Test: Execute

Execute: FillCorrelation.cpp
	g++ FillCorrelation.cpp -o Execute \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

TestRunPlot: ExecutePlot
	# ./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,HydjetxHydjet.root,HydjetxHydjet_Different.root --Coefficients -1,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass --Output FullPlots.pdf --SelfMixing 1,1,0,1,0
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,HydjetxHydjet.root,HydjetxHydjet_Different.root --Coefficients -1,1,-2,-1,2 --Histograms HZY,HZPT,HZMass,HPhi,HEta,HPT1,HPT2 --Output FullPlotsAlternate.pdf --SelfMixing 1,1,0,1,0
	# ./ExecutePlot --Input ppMCxHydjet.root,DYLLxHydjet_Nominal.root,HydjetxHydjet_Different.root --Coefficients -1,1,-1 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass --Output PartialPlots.pdf --SelfMixing 0,0,0
	# ./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,DYLLS0xDYLLS0.root --Coefficients -1,1,-1,0 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlots.pdf --SelfMixing 1,1,0,1
	# ./ExecutePlot --Input ppMCxppMC_Reweight.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,DYLLS0xDYLLS0.root --Coefficients 0,1,-1,0 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsReweight.pdf --SelfMixing 1,1,0,1
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root --Coefficients -1,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsAlternate.pdf --SelfMixing 1,1,0
	##########
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxDYLLS1.root --Coefficients 0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsSelf.pdf --SelfMixing 1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxDYLLS1.root,DYLLS1xDYLLS1.root,DYLLS1xDYLLS1_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsSelf.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxDYLLS1.root,DYLLS1xDYLLS1.root,DYLLS1xDYLLS1_Different1.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsSelf1.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxDYLLS1.root,DYLLS1xDYLLS1.root,DYLLS1xDYLLS1_Different2.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsSelf2.pdf --SelfMixing 1,1,0,1,0
	###########
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet.root --Coefficients 0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsAlmostSelf.pdf --SelfMixing 1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet.root,HydjetxHydjet.root,HydjetxHydjet_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsAlmostSelf.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet.root,HydjetxHydjet.root,HydjetxHydjet_Different1.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsAlmostSelf1.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet.root,HydjetxHydjet.root,HydjetxHydjet_Different2.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsAlmostSelf2.pdf --SelfMixing 1,1,0,1,0
	###########
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality010.root,DYLLxDYLL_Centrality010.root,DYLLxHydjet_Centrality010.root --Coefficients 0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsCentrality010.pdf --SelfMixing 1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality010.root,DYLLxDYLL_Centrality010.root,DYLLxHydjet_Centrality010.root,HydjetxHydjet_Centrality010.root,HydjetxHydjet_Centrality010_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality010.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality010.root,DYLLxDYLL_Centrality010.root,DYLLxHydjet_Centrality010.root,HydjetxHydjet_Centrality010.root,HydjetxHydjet_Centrality010_Different1.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality0101.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality010.root,DYLLxDYLL_Centrality010.root,DYLLxHydjet_Centrality010.root,HydjetxHydjet_Centrality010.root,HydjetxHydjet_Centrality010_Different2.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality0102.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality1030.root,DYLLxDYLL_Centrality1030.root,DYLLxHydjet_Centrality1030.root --Coefficients 0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsCentrality1030.pdf --SelfMixing 1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality1030.root,DYLLxDYLL_Centrality1030.root,DYLLxHydjet_Centrality1030.root,HydjetxHydjet_Centrality1030.root,HydjetxHydjet_Centrality1030_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality1030.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality1030.root,DYLLxDYLL_Centrality1030.root,DYLLxHydjet_Centrality1030.root,HydjetxHydjet_Centrality1030.root,HydjetxHydjet_Centrality1030_Different1.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality10301.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality1030.root,DYLLxDYLL_Centrality1030.root,DYLLxHydjet_Centrality1030.root,HydjetxHydjet_Centrality1030.root,HydjetxHydjet_Centrality1030_Different2.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality10302.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality3050.root,DYLLxDYLL_Centrality3050.root,DYLLxHydjet_Centrality3050.root --Coefficients 0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsCentrality3050.pdf --SelfMixing 1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality3050.root,DYLLxDYLL_Centrality3050.root,DYLLxHydjet_Centrality3050.root,HydjetxHydjet_Centrality3050.root,HydjetxHydjet_Centrality3050_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality3050.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality3050.root,DYLLxDYLL_Centrality3050.root,DYLLxHydjet_Centrality3050.root,HydjetxHydjet_Centrality3050.root,HydjetxHydjet_Centrality3050_Different1.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality30501.pdf --SelfMixing 1,1,0,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0_Centrality3050.root,DYLLxDYLL_Centrality3050.root,DYLLxHydjet_Centrality3050.root,HydjetxHydjet_Centrality3050.root,HydjetxHydjet_Centrality3050_Different2.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsCentrality30502.pdf --SelfMixing 1,1,0,1,0

TestRunGenPlot: ExecutePlot
	./ExecutePlot --Input ppMCxppMC_Gen.root,DYLLxDYLL_Gen.root,DYLLxHydjet_Nominal_Gen.root --Histograms HPhi

TestRunGenWeightPlot: ExecutePlot
	./ExecutePlot --Input DYLLS0xDYLLS0.root,ppMCxppMC.root,ppMCxppMC_Reweight.root --Coefficients 1,0,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsWeighting.pdf

ExecutePlot: QuickPlot.cpp
	g++ QuickPlot.cpp -o ExecutePlot \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

Deploy:
	cp *pdf ~/Dropbox/Temp/
