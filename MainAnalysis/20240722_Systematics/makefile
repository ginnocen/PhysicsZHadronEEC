
default: TestRun TestRunPlot

TestRun: Execute
	bash RunData030.sh

TestRunPlot: ExecutePlot
	./ExecutePlot --Input Data_Centrality030.root --Output Data_Centrality030.pdf \
		--Histogram HDeltaREEC,HDeltaREECLog --Item GroupExtraZ,Corrected,OverCorrected \
		--Label ExtraZ,HF60,HF120
	./ExecutePlot --Input Data_Centrality030.root --Output Data_Centrality030_ExtraZ.pdf \
		--Histogram HDeltaREEC,HDeltaREECLog --Item ExtraZ0,ExtraZ1,ExtraZ2,ExtraZ3,ExtraZ4,ExtraZ5,ExtraZ6,ExtraZ7,ExtraZ8,ExtraZ9,ExtraZ10,ExtraZ11 \
		--Label Z0,Z1,Z2,Z3,Z4,Z5,Z6,Z7,Z8,Z9,Z10,Z11
	cp Data_Centrality030*.pdf ~/Dropbox/Temp/

Execute: Harvest.cpp
	g++ Harvest.cpp -o Execute \
		-I$(ProjectBase)/CommonCode/include \
		`root-config --cflags --libs`

ExecutePlot: MakeSystematicsPlot.cpp
	g++ MakeSystematicsPlot.cpp -o ExecutePlot \
		-I$(ProjectBase)/CommonCode/include \
		`root-config --cflags --libs`

