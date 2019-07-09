
	
	#ifndef __CPU_PRED_GSHARE_SPECULATIVE_PRED_HH__
	#define __CPU_PRED_GSHARE_SPECULATIVE_PRED_HH__
	
	#include <vector>
	#include "base/types.hh"
	#include "cpu/pred/bpred_unit.hh"
	#include "cpu/pred/sat_counter.hh"
	#include "params/Gshare_SpeculativeBP.hh"
	
	/*
	* Feel free to make any modifications, this is a skeleton code
	* to get you started.
	* Note: Do not change name of class
	*/
	class Gshare_SpeculativeBP : public BPredUnit
	{
	public:
	Gshare_SpeculativeBP(const Gshare_SpeculativeBPParams *params);
	void uncondBranch(ThreadID tid, Addr pc, void * &bp_history);
	void squash(ThreadID tid, void *bp_history);
	bool lookup(ThreadID tid, Addr branch_addr, void * &bp_history);
	void btbUpdate(ThreadID tid, Addr branch_addr, void * &bp_history);
	void update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory, bool squashed, const StaticInstPtr & inst, Addr corrTarget);
	void reset();
	
	private:
	void updateGlobalHistReg(bool taken);
	
	struct BPHistory {
	unsigned globalHistoryReg;
	/*
	the final taken/not-taken prediction
	true: predict taken
	false: predict not-taken
	*/
	bool finalPred;
	};
	
	/** Number of bits to shift the instruction over to get rid of the word
	* offset.
	*/
	unsigned instShiftAmt;
	
	//storing the bits of the global history register.
	unsigned globalHistoryReg;
	//indicates the size (length) of the global history register.
	unsigned globalHistoryBits;
	
	/** Mask to control how much history is stored. All of it might not be
	* used. */
	unsigned historyRegisterMask;
	
	/** Local counters, each element of localCtrs is a Saturating counter */
	std::vector<SatCounter> localCtrs;
	/** Number of counters in the local predictor. */
	unsigned localPredictorSize;
	/** Number of bits of the local predictor's counters. */
	unsigned localCtrBits;
	/** Mask to truncate values stored in the local history table. */
	unsigned localPredictorMask;
	
	/** Thresholds for the counter value; above the threshold is taken,
	* equal to or below the threshold is not taken.
	*/
	unsigned localThreshold;
	};
	
	#endif // __CPU_PRED_GSHARE_SPECULATIVE_PRED_HH__