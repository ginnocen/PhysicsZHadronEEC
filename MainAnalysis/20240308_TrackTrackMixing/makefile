

default: TestRunGenSelf TestRunPlot Deploy

TestRun: Execute
	./Execute --Signal Input/ppMC.root --Background None --SelfMixing true --Output ppMCxppMC.root \
		--IsPP true
	./Execute --Signal Input/DYLL.root --Background None --SelfMixing true --Output DYLLxDYLL.root
	./Execute --Signal Input/DYLL.root --Background Input/Hydjet.root --SelfMixing false \
		--Output DYLLxHydjet_Nominal.root --HFShift 767.2 --OverSample 5 \
		--SignalHFMin 38220.226 --SignalHFMax 155015.70
	./Execute --Signal Input/Hydjet.root --Background None --SelfMixing true --Output HydjetxHydjet.root \
		--SignalHFMin 37453.026 --SignalHFMax 154248.5 --CheckSignalHiBin false --CheckSignalHF true --CheckZ false \
		--Fraction 0.1
	./Execute --Signal Input/Hydjet1.root --Background Input/Hydjet2.root --SelfMixing true \
		--Output HydjetxHydjet_Different.root --CheckSignalHiBin false --CheckSignalHF true \
		--SignalHFMin 37453.026 --SignalHFMax 154248.5 --HFShift 0 --OverSample 5 --CheckZ false \
		--Fraction 0.1

TestRunNoCentrality: Execute
	./Execute --Signal Input/PPMC_Reco.root --Background None --SelfMixing true --Output ppMCxppMC.root \
		--IsPP true --CheckZ true
	./Execute --Signal Input/DYLLMC_Reco.root --Background None --SelfMixing true --Output DYLLxDYLL.root \
		--CheckSignalHiBin false --CheckSignalHF false --CheckZ true
	./Execute --Signal Input/DYLLMC_Reco.root --Background Input/MinBiasMC_Reco.root --SelfMixing false \
	 	--Output DYLLxHydjet_Nominal.root --HFShift 0 --OverSample 5 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true
	./Execute --Signal Input/PPMC_Reco.root --Background Input/MinBiasMC_Reco.root --SelfMixing false \
	 	--Output ppMCxHydjet.root --HFShift 0 --OverSample 5 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true
	./Execute --Signal Input/MinBiasMC_Reco.root --Background None --SelfMixing true --Output HydjetxHydjet.root \
		--CheckSignalHiBin false --CheckSignalHF false --CheckZ false --Fraction 1
	./Execute --Signal Input/MinBiasMC_Reco1.root --Background Input/MinBiasMC_Reco2.root --SelfMixing false \
		--Output HydjetxHydjet_Different.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
		--Fraction 1
	# ./Execute --Signal Input/Hydjet2_Reco.root --Background Input/Hydjet1_Reco.root --SelfMixing false \
	# 	--Output HydjetxHydjet_DifferentSwap.root --CheckSignalHiBin false --CheckSignalHF false \
	# 	--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
	# 	--Fraction 1

TestRunGen: Execute
	./Execute --Signal Input/ppMCGen.root --Background None --SelfMixing true --Output ppMCxppMC_Gen.root \
		--IsPP true
	./Execute --Signal Input/DYLLGen.root --Background None --SelfMixing true --Output DYLLxDYLL_Gen.root
	./Execute --Signal Input/DYLLGen.root --Background Input/HydjetGen.root --SelfMixing false \
		--Output DYLLxHydjet_Nominal_Gen.root --HFShift 817.4 --SignalHFMin 43688.527 --SignalHFMax 181342.2

TestRunGenNoCentrality: Execute
	./Execute --Signal Input/PPMC_Gen.root --Background None --SelfMixing true --Output ppMCxppMC.root \
		--IsPP true --IsReco false --CheckZ true --CheckSignalHiBin false --CheckSignalHF false
	./Execute --Signal Input/PPMC_Gen.root --Background None --SelfMixing true --Output ppMCxppMC_Reweight.root \
		--IsPP true --IsReco false --CheckZ true --CheckSignalHiBin false --CheckSignalHF false --DoZReweight true
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLxDYLL.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS0xDYLLS0.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true --SubEvent 0
	./Execute --Signal Input/DYLLMC_Gen.root --Background Input/MinBiasMC_Gen.root --SelfMixing false \
	 	--IsPP false --IsReco false --Output DYLLxHydjet_Nominal.root --HFShift 0 --OverSample 5 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true
	./Execute --Signal Input/PPMC_Gen.root --Background Input/MinBiasMC_Gen.root --SelfMixing false \
	 	--IsPP false --IsReco false --Output ppMCxHydjet.root --HFShift 0 --OverSample 5 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true
	./Execute --Signal Input/MinBiasMC_Gen.root --Background None --SelfMixing true --Output HydjetxHydjet.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ false --Fraction 1
	./Execute --Signal Input/MinBiasMC_Gen1.root --Background Input/MinBiasMC_Gen2.root --SelfMixing false \
		--Output HydjetxHydjet_Different.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
		--IsPP false --IsReco false --Fraction 1
	# ./Execute --Signal Input/Hydjet2_Gen.root --Background Input/Hydjet1_Gen.root --SelfMixing false \
	# 	--Output HydjetxHydjet_DifferentSwap.root --CheckSignalHiBin false --CheckSignalHF false \
	# 	--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
	# 	--Fraction 1

TestRunGenSelf: Execute
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS0xDYLLS0.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true --SubEvent 0,0
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLxDYLL.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true --SubEvent -1,-1
	./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
		--IsPP false --IsReco false --Output DYLLxDYLLS1.root --HFShift 0 --OverSample 1 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true \
		--SubEvent -1,1 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
		--IsPP false --IsReco false --Output DYLLxDYLLS0.root --HFShift 0 --OverSample 1 \
		--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true \
		--SubEvent -1,0 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS1xDYLLS1.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ true --Fraction 1 \
		--SubEvent 1,1 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen1.root --Background Input/DYLLMC_Gen2.root --SelfMixing false \
		--Output DYLLS1xDYLLS1_Different1.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 1 --CheckZ true \
		--IsPP false --IsReco false --SubEvent 1,1 --CheckBackgroundZ true
	./Execute --Signal Input/DYLLMC_Gen2.root --Background Input/DYLLMC_Gen1.root --SelfMixing false \
		--Output DYLLS1xDYLLS1_Different2.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 1 --CheckZ true \
		--IsPP false --IsReco false --SubEvent 1,1 --CheckBackgroundZ true
	hadd -f DYLLS1xDYLLS1_Different.root DYLLS1xDYLLS1_Different1.root DYLLS1xDYLLS1_Different2.root

Test: Execute
	./Execute --Signal Input/DYLLMC_Gen.root --Background None --SelfMixing true --Output DYLLS1xDYLLS1.root \
		--IsPP false --IsReco false --CheckSignalHiBin false --CheckSignalHF false --CheckZ false --Fraction 1 \
		--SubEvent 1,1

Execute: FillCorrelation.cpp
	g++ FillCorrelation.cpp -o Execute \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

TestRunPlot: ExecutePlot
	./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,HydjetxHydjet.root,HydjetxHydjet_Different.root --Coefficients -1,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass --Output FullPlots.pdf
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,HydjetxHydjet.root,HydjetxHydjet_Different.root --Coefficients -1,1,-2,-1,2 --Histograms HZY,HZPT,HZMass,HPhi,HEta,HPT1,HPT2 --Output FullPlotsAlternate.pdf
	./ExecutePlot --Input ppMCxHydjet.root,DYLLxHydjet_Nominal.root,HydjetxHydjet_Different.root --Coefficients -1,1,-1 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass --Output PartialPlots.pdf
	./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,DYLLS0xDYLLS0.root --Coefficients -1,1,-1,0 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlots.pdf
	./ExecutePlot --Input ppMCxppMC_Reweight.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,DYLLS0xDYLLS0.root --Coefficients -1,1,-1,0 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsReweight.pdf
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root --Coefficients -1,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsAlternate.pdf
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxDYLLS1.root --Coefficients 0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsSelf.pdf
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxDYLLS1.root,DYLLS1xDYLLS1.root,DYLLS1xDYLLS1_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass,HDeltaR,HDeltaREEC --Output FullPlotsSelf.pdf

TestRunGenPlot: ExecutePlot
	./ExecutePlot --Input ppMCxppMC_Gen.root,DYLLxDYLL_Gen.root,DYLLxHydjet_Nominal_Gen.root --Histograms HPhi

ExecutePlot: QuickPlot.cpp
	g++ QuickPlot.cpp -o ExecutePlot \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

Deploy:
	cp *pdf ~/Dropbox/Temp/
