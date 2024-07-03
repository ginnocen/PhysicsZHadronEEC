

# default: TestRun TestRunPlot Deploy
default: TestRunKayaPlot Deploy

Execute: FillCorrelation.cpp
	g++ FillCorrelation.cpp -o Execute \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

TestRun: Execute
	bash RunReco.sh

TestRunKayaPlot: ExecutePlot
	./ExecutePlot --Input PPDataxPPData_Kaya.root,PPMCxPPMC_Kaya.root,PbPbDataxPbPbData_Centrality030_Kaya.root,PbPbDataxMinBiasData_Centrality030_Kaya.root \
		--Coefficients 0,0,1,-1 --Histograms HZHXi,HZHPhi --Output ZHPlotsKayaReproduction.pdf \
		--Rebin 12,12,12,12
	./ExecutePlot --Input PPDataxPPData_Kaya.root,PPMCxPPMC_Kaya.root,PbPbDataxPbPbData_Centrality030_Kaya.root,PbPbDataxMinBiasData_Centrality030_Kaya.root \
		--Coefficients 0,0,1,-1 --Histograms HZHPT --Output ZHPlotsKayaReproductionPT.pdf \
		--Rebin 1,1,1,1

TestRunShiftPlot: ExecutePlot
	./ExecutePlot --Input  \
		--Coefficients 0,0,1,-1 --Histograms HZHEta,HZHPhi,HZHPT --Output ZHPlotsPPSelfSubtraction.pdf \
		--SelfMixing 1,0,1,0 --Rebin 12,12,12,12

TestRunSelfSubtraction: ExecutePlot
	./ExecutePlot --Input PPMCxPPMC_Reco.root,PPMCxPPMC_Reco_Different.root,PPDataxPPData_Reco.root,PPDataxPPData_Reco_Different.root \
		--Coefficients 0,0,1,-1 --Histograms HZHEta,HZHPhi,HZHPT --Output ZHPlotsPPSelfSubtraction.pdf \
		--SelfMixing 1,0,1,0 --Rebin 12,12,12,12
	# ./ExecutePlot --Input PbPbMCxPbPbMC_Reco.root,PbPbMCxPbPbMC_Reco_Different1.root,PbPbMCxPbPbMC_Reco_Different2.root,PbPbDataxPbPbData_Reco.root,PbPbDataxPbPbData_Reco_Different1.root,PbPbDataxPbPbData_Reco_Different2.root \
	# 	--Coefficients 0,0,0,1,-0.5,-0.5 --Histograms HSignalHF,HMatchedHF,HSubEvent0HF,HSubEvent1HF,HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsPbPbSelfSubtraction.pdf \
	# 	--SelfMixing 1,0,0,1,0,0
	# ./ExecutePlot --Input PbPbMCxPbPbMC_Centrality030_Reco.root,PbPbMCxPbPbMC_Centrality030_Reco_Different.root,PbPbDataxPbPbData_Centrality030_Reco.root,PbPbDataxPbPbData_Centrality030_Reco_Different.root \
	# 	--Coefficients 0,0,1,-1 --Histograms HSignalHF,HMatchedHF,HSubEvent0HF,HSubEvent1HF,HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsPbPbSelfSubtractionCentrality030.pdf \
	# 	--SelfMixing 1,0,0,1,0,0
	./ExecutePlot --Input PPDataxPPData_Reco.root,PPDataxPPData_Reco_Different.root,PbPbDataxPbPbData_Centrality030_Reco.root,PbPbDataxPbPbData_Centrality030_Reco_Different.root \
		--Coefficients 0,0,1,-1 --Histograms HZHEta,HZHPhi,HZHPT,HCZHEta,HCZHPhi,HCZHPT,HPZHEta,HPZHPhi,HPZHPT --Output ZHPlotsSelfSubtractionCentrality030.pdf \
		--SelfMixing 1,0,0,1,0,0 --Rebin 12,12,12,12,12,12
	./ExecutePlot --Input PPDataxPPData_Reco.root,PPDataxPPData_Reco_Different.root,PbPbDataxPbPbData_Centrality3050_Reco.root,PbPbDataxPbPbData_Centrality3050_Reco_Different.root \
		--Coefficients 0,0,1,-1 --Histograms HZHEta,HZHPhi,HZHPT,HCZHEta,HCZHPhi,HCZHPT,HPZHEta,HPZHPhi,HPZHPT --Output ZHPlotsSelfSubtractionCentrality3050.pdf \
		--SelfMixing 1,0,0,1,0,0 --Rebin 12,12,12,12,12,12
	./ExecutePlot --Input PPDataxPPData_Reco.root,PPDataxPPData_Reco_Different.root,PbPbDataxPbPbData_Centrality5090_Reco.root,PbPbDataxPbPbData_Centrality5090_Reco_Different.root \
		--Coefficients 0,0,1,-1 --Histograms HZHEta,HZHPhi,HZHPT,HCZHEta,HCZHPhi,HCZHPT,HPZHEta,HPZHPhi,HPZHPT --Output ZHPlotsSelfSubtractionCentrality5090.pdf \
		--SelfMixing 1,0,0,1,0,0 --Rebin 12,12,12,12,12,12

TestRunPlot: ExecutePlot
	./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Reco.root,DYLLxHydjet_Reco.root,PPMCxPPMC.root \
		--Coefficients 0,0,1,-1,0 --Histograms HSignalHF,HMatchedHF,HSubEvent0HF,HSubEvent1HF,HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsReco.pdf \
		--SelfMixing 1,1,1,0,1
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL_Reco.root,DYLLxHydjet_Reco.root,HydjetxHydjet_Reco.root,HydjetxHydjet_Reco_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HDeltaR,HDeltaREEC --Output FullPlotsReco.pdf --SelfMixing 1,1,0,1,0

TestRunRecoPlot: ExecutePlot
	./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality1030_Reco.root,DYLLxHydjet_Centrality1030_Reco.root \
		--Coefficients 0,0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
		--Output ZHPlotsCentrality1030Reco.pdf \
		--SelfMixing 1,1,1,0
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality1030_Reco.root,DYLLxHydjet_Centrality1030_Reco.root \
	# 	--Coefficients 0,0,1,-1.004 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
	# 	--Output ZHPlotsCentrality1030RecoAdjusted.pdf \
	# 	--SelfMixing 1,1,1,0
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality1030_Reco.root,DYLLxHydjet_Centrality1030_Reco_ExtraShift2.root \
	# 	--Coefficients 0,0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
	# 	--Output ZHPlotsCentrality1030RecoShifted.pdf \
	# 	--SelfMixing 1,1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality010_Reco.root,DYLLxHydjet_Centrality010_Reco.root \
		--Coefficients 0,0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
		--Output ZHPlotsCentrality010Reco.pdf \
		--SelfMixing 1,1,1,0
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality010_Reco.root,DYLLxHydjet_Centrality010_Reco.root \
	# 	--Coefficients 0,0,1,-1.002 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
	# 	--Output ZHPlotsCentrality010RecoAdjusted.pdf \
	# 	--SelfMixing 1,1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality030_Reco.root,DYLLxHydjet_Centrality030_Reco.root \
		--Coefficients 0,0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
		--Output ZHPlotsCentrality030Reco.pdf \
		--SelfMixing 1,1,1,0
	./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL_Centrality030_Reco.root,DYLLxHydjet_Centrality030_Reco.root,HydjetxHydjet_Centrality030_Reco.root,HydjetxHydjet_Centrality030_Reco_Different.root --Coefficients 0,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HDeltaR,HDeltaREEC --Output FullPlotsCentrality030Reco.pdf --SelfMixing 1,1,0,1,0
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality030_Reco.root,DYLLxHydjet_Centrality030_Reco1.root,DYLLxHydjet_Centrality030_Reco2.root \
	# 	--Coefficients 0,0,1,-0.5,-0.5 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
	# 	--Output ZHPlotsCentrality030Reco.pdf \
	# 	--SelfMixing 1,1,1,0,0
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,PPMCxPPMC_Reco.root,DYLLxDYLL_Centrality030_Reco.root,DYLLxHydjet_Centrality030_Reco1.root,DYLLxHydjet_Centrality030_Reco2.root \
	# 	--Coefficients 0,0,1,-0.51,-0.51 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT \
	# 	--Output ZHPlotsCentrality030RecoAdjusted.pdf \
	# 	--SelfMixing 1,1,1,0,0

TestRunGenPlot: ExecutePlot
	# ./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,HydjetxHydjet.root,HydjetxHydjet_Different.root --Coefficients -1,1,-2,-1,2 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass --Output FullPlots.pdf --SelfMixing 1,1,0,1,0
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,HydjetxHydjet.root,HydjetxHydjet_Different.root --Coefficients -1,1,-2,-1,2 --Histograms HZY,HZPT,HZMass,HPhi,HEta,HPT1,HPT2 --Output FullPlotsAlternate.pdf --SelfMixing 1,1,0,1,0
	# ./ExecutePlot --Input ppMCxHydjet.root,DYLLxHydjet_Nominal.root,HydjetxHydjet_Different.root --Coefficients -1,1,-1 --Histograms HPhi,HEta,HPT1,HPT2,HZY,HZPT,HZMass --Output PartialPlots.pdf --SelfMixing 0,0,0
	# ./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,DYLLS0xDYLLS0.root --Coefficients -1,1,-1,0 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlots.pdf --SelfMixing 1,1,0,1
	# ./ExecutePlot --Input ppMCxppMC_Reweight.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,DYLLS0xDYLLS0.root --Coefficients 0,1,-1,0 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsReweight.pdf --SelfMixing 1,1,0,1
	# ./ExecutePlot --Input DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root --Coefficients -1,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsAlternate.pdf --SelfMixing 1,1,0
	##########
	./ExecutePlot --Input DYLLxHydjet.root,DYLLS0xDYLLS0.root,DYLLxDYLL.root,DYLLxDYLLS1.root --Coefficients 0,0,1,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsSelf.pdf --SelfMixing 0,1,1,0
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

TestRunGenPlotTemp: ExecutePlot
	./ExecutePlot --Input ppMCxppMC_Gen.root,DYLLxDYLL_Gen.root,DYLLxHydjet_Nominal_Gen.root --Histograms HPhi

TestRunGenWeightPlot: ExecutePlot
	./ExecutePlot --Input DYLLS0xDYLLS0.root,ppMCxppMC.root,ppMCxppMC_Reweight.root --Coefficients 1,0,-1 --Histograms HZY,HZPT,HZMass,HZHEta,HZHPhi,HZHPT --Output ZHPlotsWeighting.pdf

ExecutePlot: QuickPlot.cpp
	g++ QuickPlot.cpp -o ExecutePlot \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

Deploy:
	cp *pdf ~/Dropbox/Temp/
