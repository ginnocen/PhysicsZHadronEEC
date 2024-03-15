

default: TestRunNoCentrality TestRunPlot

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
	# ./Execute --Signal Input/ppMC.root --Background None --SelfMixing true --Output ppMCxppMC.root \
	# 	--IsPP true
	# ./Execute --Signal Input/DYLL.root --Background None --SelfMixing true --Output DYLLxDYLL.root \
	# 	--CheckSignalHiBin false --CheckSignalHF false --CheckZ true
	# ./Execute --Signal Input/DYLL.root --Background Input/Hydjet.root --SelfMixing false \
	#  	--Output DYLLxHydjet_Nominal.root --HFShift 0 --OverSample 5 \
	# 	--CheckSignalHiBin false --CheckSignalHF false --SignalHFMin 0 --SignalHFMax 1000000 --CheckZ true
	./Execute --Signal Input/Hydjet.root --Background None --SelfMixing true --Output HydjetxHydjet.root \
		--CheckSignalHiBin false --CheckSignalHF false --CheckZ false --Fraction 1
	./Execute --Signal Input/Hydjet1.root --Background Input/Hydjet2.root --SelfMixing false \
		--Output HydjetxHydjet_Different.root --CheckSignalHiBin false --CheckSignalHF false \
		--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
		--Fraction 1
	# ./Execute --Signal Input/Hydjet2.root --Background Input/Hydjet1.root --SelfMixing false \
	# 	--Output HydjetxHydjet_DifferentSwap.root --CheckSignalHiBin false --CheckSignalHF false \
	# 	--SignalHFMin 0 --SignalHFMax 1000000 --HFShift 0 --OverSample 5 --CheckZ false \
	# 	--Fraction 1

TestRunGen: Execute
	./Execute --Signal Input/ppMCGen.root --Background None --SelfMixing true --Output ppMCxppMC_Gen.root \
		--IsPP true
	./Execute --Signal Input/DYLLGen.root --Background None --SelfMixing true --Output DYLLxDYLL_Gen.root
	./Execute --Signal Input/DYLLGen.root --Background Input/HydjetGen.root --SelfMixing false \
		--Output DYLLxHydjet_Nominal_Gen.root --HFShift 817.4 --SignalHFMin 43688.527 --SignalHFMax 181342.2

Execute: FillCorrelation.cpp
	g++ FillCorrelation.cpp -o Execute \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include

TestRunPlot: ExecutePlot
	./ExecutePlot --Input ppMCxppMC.root,DYLLxDYLL.root,DYLLxHydjet_Nominal.root,HydjetxHydjet.root,HydjetxHydjet_Different.root

TestRunGenPlot: ExecutePlot
	./ExecutePlot --Input ppMCxppMC_Gen.root,DYLLxDYLL_Gen.root,DYLLxHydjet_Nominal_Gen.root

ExecutePlot: QuickPlot.cpp
	g++ QuickPlot.cpp -o ExecutePlot \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include
