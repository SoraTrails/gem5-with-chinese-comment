#!/bin/bash
#./build/ARM/gem5.opt ./configs/example/se.py --cpu-type=MinorCPU --mem-size=1024MB --caches --l2cache --bp-type=LocalBP -c ../dhrystone-2.1/gcc_dry2
set -e
BTBEntries=(8 16 32 64 128 256 512 1024 2048 4096)
BTBTagSize=(1 2 4 8 16)
# BP=Gshare_SpeculativeBP

#作业2
# for i in ${BTBEntries[@]};do
#     for j in ${BTBTagSize[@]};do
#         sed -i "s/BTBEntries = Param\.Unsigned([0-9]*, \"Number of BTB entries\")/BTBEntries = Param\.Unsigned(${i}, \"Number of BTB entries\")/" src/cpu/pred/BranchPredictor.py
#         sed -i "s/BTBTagSize = Param\.Unsigned([0-9]*, \"Size of the BTB tags, in bits\")/BTBTagSize = Param.Unsigned(${j}, \"Size of the BTB tags, in bits\")/" src/cpu/pred/BranchPredictor.py
#         scons build/ARM/gem5.opt
#         ./build/ARM/gem5.opt -d m5out/${BP}_${i}_${j} ./configs/example/se.py --cpu-type=MinorCPU --mem-size=1024MB --caches --l2cache --bp-type=${BP} -c ../dhrystone-2.1/gcc_dry2_100000
#     done
# done

#作业2 结果分析
# for i in ${BTBEntries[@]};do
#     for j in ${BTBTagSize[@]};do
#         echo -n "${i} ${j} " >> res.txt
#         in=$(grep 'condIncorrect' m5out/${BP}_${i}_${j}/stats.txt | awk '{print $2}')
#         sum=$(grep 'condPredicted' m5out/${BP}_${i}_${j}/stats.txt | awk '{print $2}')
#         mis=$(echo "${in}/${sum}" | bc -l)
#         echo -n "0${mis} " >> res.txt
#         cpi=$(grep 'cpi' m5out/${BP}_${i}_${j}/stats.txt | awk '{print $2}')
#         echo "${cpi}" >> res.txt
#     done
# done

BP=(LocalBP Gshare_SpeculativeBP)
PHT_entry=(32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144) 

# # 作业3
# for i in ${PHT_entry[@]};do
#     sed -i "s/Unsigned([0-9]*, \"Size of local predictor \"/Unsigned(${i}, \"Size of local predictor \"/" src/cpu/pred/BranchPredictor.py
#     scons build/ARM/gem5.opt -j2
#     for j in ${BP[@]}; do
#         ./build/ARM/gem5.opt -d m5out/PHT_${j}_${i} ./configs/example/se.py --cpu-type=MinorCPU --mem-size=1024MB --caches --l2cache --bp-type=${j} -c ../dhrystone-2.1/gcc_dry2_100000 &
#     done
#     wait
# done

# 作业3 结果分析
for i in ${PHT_entry[@]};do
    for j in ${BP[@]}; do
        echo -n "${i} " >> res2_${j}.txt
        in=$(grep 'condIncorrect' m5out/PHT_${j}_${i}/stats.txt | awk '{print $2}')
        sum=$(grep 'condPredicted' m5out/PHT_${j}_${i}/stats.txt | awk '{print $2}')
        mis=$(echo "${in}/${sum}" | bc -l)
        echo -n "0${mis} " >> res2_${j}.txt
        cpi=$(grep 'cpi' m5out/PHT_${j}_${i}/stats.txt | awk '{print $2}')
        echo "${cpi}" >> res2_${j}.txt
    done
done
