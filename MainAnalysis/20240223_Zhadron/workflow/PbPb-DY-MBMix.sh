# Gen level 
./finalAnalysis.sh output/DY-Gen       0-20      --Input mergedSample/DY-gen-v6.root --MixFile mergedSample/hydjet-gen-v6.root --IsPP false --IsGenZ true --nMix 1 --IsSelfMixing false --MaxHiBin 20
./finalAnalysis.sh output/DY-Gen-RecoZ 0-20      --Input mergedSample/DY-gen-v6.root --MixFile mergedSample/hydjet-gen-v6.root --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --MaxHiBin 20

# Reco level 
#./finalAnalysis.sh output/DY     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 1 --IsSelfMixing false --Shift 696.5
./finalAnalysis.sh output/DY 		0-60     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 10 --IsSelfMixing false --Shift 746 --MaxHiBin 60
./finalAnalysis.sh output/DY680   	0-60     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 10 --IsSelfMixing false --Shift 680 --MaxHiBin 60
./finalAnalysis.sh output/DY800   	0-60     --Input mergedSample/DY-v6.root     --MixFile mergedSample/hydjet-v6.root     --IsPP false --IsGenZ false --nMix 10 --IsSelfMixing false --Shift 800 --MaxHiBin 60
