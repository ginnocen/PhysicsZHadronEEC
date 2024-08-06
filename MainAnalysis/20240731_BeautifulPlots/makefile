


default: TestRunBasic

TestRunBasic: ExecuteBasic
	./ExecuteBasic --Input Curves/Data_PP_PT2.root,Curves/Data_Centrality030_PT2.root \
		--Systematics Systematics/Data_PP_PT2.root,Systematics/Data_Centrality030_PT2.root \
		--Label PP,"PbPb 0-30%" \
		--Output Result_Centrality030_PT2.pdf \
		--Color -1,-2 \
		--Histogram HDeltaREEC

ExecuteBasic: MakeBasicPlot.cpp
	g++ MakeBasicPlot.cpp -o ExecuteBasic \
		`root-config --cflags --libs` \
		-I$(ProjectBase)/CommonCode/include
