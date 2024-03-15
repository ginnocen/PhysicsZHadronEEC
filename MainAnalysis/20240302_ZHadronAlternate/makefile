

default: TestRunGen TestRunGenPlot

TestRun: Execute
	./Execute --Signal Input/ppMC.root --Background None --SelfMixingMode true --Output ppMCxppMC.root --IsPP true
	./Execute --Signal Input/DYLL.root --Background None --SelfMixingMode true --Output DYLLxDYLL.root
	./Execute --Signal Input/DYLL.root --Background Input/Hydjet.root --SelfMixing false \
		--Output DYLLxHydjet_Nominal.root --HFShift 767.2
	./Execute --Signal Input/DYLL.root --Background Input/Hydjet.root --SelfMixing false \
		--Output DYLLxHydjet_ShiftUp.root --HFShift 1150.8 
	./Execute --Signal Input/DYLL.root --Background Input/Hydjet.root --SelfMixing false \
		--Output DYLLxHydjet_ShiftDown.root --HFShift 383.6
	./Execute --Signal Input/DYLL.root --Background Input/Hydjet.root --SelfMixing false \
		--Output DYLLxHydjet_NoShift.root --HFShift 0

TestRunGen: Execute
	./Execute --Signal Input/ppMCGen.root --Background None --SelfMixingMode true --Output ppMCxppMC_Gen.root \
		--IsPP true
	./Execute --Signal Input/DYLLGen.root --Background None --SelfMixingMode true --Output DYLLxDYLL_Gen.root
	./Execute --Signal Input/DYLLGen.root --Background Input/HydjetGen.root --SelfMixing false \
		--Output DYLLxHydjet_Nominal_Gen.root --HFShift 817.4 --SignalHFMin 43688.527 --SignalHFMax 181342.2

Execute: FillCorrelation.cpp
	g++ FillCorrelation.cpp -o Execute \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

TestRunPlot: ExecutePlot
	./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,DYLLxHydjet_ShiftUp.root,DYLLxHydjet_ShiftDown.root,DYLLxHydjet_NoShift.root

TestRunGenPlot: ExecutePlot
	./ExecutePlot --Input ppMCxppMC_Gen.root,DYLLxDYLL_Gen.root,DYLLxHydjet_Nominal_Gen.root

ExecutePlot: QuickPlot.cpp
	g++ QuickPlot.cpp -o ExecutePlot \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include
