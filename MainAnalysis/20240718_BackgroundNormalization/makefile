default: TestRunNorm

TestRun: Execute
	./Execute --Input Input/MinBiasMC_Reco.root --MinPT 2 --MaxPT -1 --MaxAbsEta 2.4 --Output HFNormalization_MinBiasMC.root
	# ./Execute --Input Input/MinBiasData_Reco78.root,Input/MinBiasData_Reco9.root --MinPT 2 --MaxPT -1 --MaxAbsEta 2.4 --Output HFNormalization_MinBiasData.root
	# ./Execute --Input Input/MinBiasData_Reco9.root --MinPT 2 --MaxPT -1 --MaxAbsEta 2.4 --Output HFNormalization_MinBiasData.root
	./Execute --Input Input/MinBiasData2Inf_Reco1.root,Input/MinBiasData2Inf_Reco2.root \
		--MinPT 2 --MaxPT -1 --MaxAbsEta 2.4 --Output HFNormalization_MinBiasData.root
	./Execute --Input Input/PbPbData_Reco.root --MinPT 2 --MaxPT -1 --MaxAbsEta 2.4 \
		--Output HFNormalization_PbPbData.root --CheckZ true --MinZPT 40 \
		--DoShift true --ShiftFile SumHF/DataWeight_Centrality030.root
	./Execute --Input Input/PbPbData_Reco.root --MinPT 2 --MaxPT -1 --MaxAbsEta 2.4 \
		--Output HFNormalization_PbPbData_Corrected.root --CheckZ true --MinZPT 40 \
		--DoShift true --ShiftFile SumHF/DataWeight_Centrality030_Corrected.root
	./Execute --Input Input/PbPbData_Reco.root --MinPT 2 --MaxPT -1 --MaxAbsEta 2.4 \
		--Output HFNormalization_PbPbData_OverCorrected.root --CheckZ true --MinZPT 40 \
		--DoShift true --ShiftFile SumHF/DataWeight_Centrality030_OverCorrected.root

TestRunNorm: ExecuteNorm
	./ExecuteNorm --Input HFNormalization_MinBiasData.root --HF SumHF/DataWeight_Centrality030.root --Data HFNormalization_PbPbData.root
	./ExecuteNorm --Input HFNormalization_MinBiasData.root --HF SumHF/DataWeight_Centrality030_Corrected.root --Data HFNormalization_PbPbData_Corrected.root
	./ExecuteNorm --Input HFNormalization_MinBiasData.root --HF SumHF/DataWeight_Centrality030_OverCorrected.root --Data HFNormalization_PbPbData_OverCorrected.root

Execute: BuildNormalization.cpp
	g++ BuildNormalization.cpp -o Execute \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include/

ExecuteNorm: CalculateNormalizationEnsemble.cpp
	g++ CalculateNormalizationEnsemble.cpp -o ExecuteNorm \
		`root-config --cflags --libs` \
		$(ProjectBase)/CommonCode/library/*.o -I$(ProjectBase)/CommonCode/include/
