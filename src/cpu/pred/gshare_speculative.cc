

#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "cpu/pred/gshare_speculative.hh"

/*
 * Constructor for gshare BP
 */
Gshare_SpeculativeBP::Gshare_SpeculativeBP(const Gshare_SpeculativeBPParams *params)
    : BPredUnit(params), instShiftAmt(params->instShiftAmt),
      globalHistoryReg(0), //initilize the global History registor to 0
      globalHistoryBits(ceilLog2(params->localPredictorSize/params->localCtrBits)),  //initilize the size of the global history register to be log2(localPredictorSize)
      localPredictorSize(params->localPredictorSize/params->localCtrBits),
      localCtrBits(params->localCtrBits)
{
	if (!isPowerOf2(localPredictorSize))
		fatal("Invalid local predictor size.\n");

	//set the mask of the global history register, to ensure the bits above globalHistoryBits are 0s.
	this->historyRegisterMask = mask(this->globalHistoryBits);
	//initilize the so-called localCtrs
	this->localCtrs.resize(this->localPredictorSize);

	for(uint32_t count_ctr = 0;count_ctr < this->localPredictorSize;count_ctr++)
	{
		this->localCtrs[count_ctr].setBits(this->localCtrBits);
	}

	//setting the threshold for the value in local counter to indicates a taken branch
	// This is equivalent to (2^(Ctr))/2 - 1
    localThreshold  = (unsigned) (ULL(1) << (this->localCtrBits  - 1)) - 1;

    //this->localThreshold  = (ULL(1) << (this->localCtrBits  - 2)) - 1;
    //printf("localCtrBits is %u\n",this->localCtrBits);
    //printf("localThreshold is %08x\n",this->localThreshold);

}

/*
 * Reset Data Structures
 */
void
Gshare_SpeculativeBP::reset()
{
	//reset the global history register
	this->globalHistoryReg = 0;

	//reset the localCtrs
	for(uint32_t count_ctr = 0;count_ctr < this->localPredictorSize;count_ctr++)
	{
		this->localCtrs[count_ctr].setBits(localCtrBits);
	}

}

/*
 * Actions for an unconditional branch
 	1. create new record of bpHistory, and return it via bpHistory
 	2. update the record of global history register.
 */
void
Gshare_SpeculativeBP::uncondBranch(ThreadID tid, Addr pc, void * &bpHistory)
{
	BPHistory *history = new BPHistory;
	//store the current global history register to the returning history
	history->globalHistoryReg = this->globalHistoryReg;
	//treat unconditional branch as a predict-to-take branch
	history->finalPred = true;
	//return the content in history to bpHistory
	bpHistory = static_cast<void*>(history);
	updateGlobalHistReg(true);
	return ;
}

/*
 * Lookup the actual branch prediction.
 */
bool
Gshare_SpeculativeBP::lookup(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
	//hash the branchAddr with the global history register to get the index into the table of counter.
	unsigned localCtrsIdx = ((branchAddr >> this->instShiftAmt) ^ this->globalHistoryReg) & this->historyRegisterMask;
    assert(localCtrsIdx < this->localPredictorSize);
    
    //read the value from the local counters, and assign the judgement into the final_prediction
    bool final_prediction = (this->localCtrs[localCtrsIdx].read() > this->localThreshold);

    //update the bpHistory
    BPHistory *history = new BPHistory;
    history->finalPred = final_prediction;
    history->globalHistoryReg = this->globalHistoryReg;
    bpHistory = static_cast<void*>(history);

    //speculatively update the global history register.
    updateGlobalHistReg(final_prediction);

    return final_prediction;
}

/*
 * BTB Update actions, called when a BTB miss happen
 */
void
Gshare_SpeculativeBP::btbUpdate(ThreadID tid, Addr branchAddr, void * &bpHistory)
{
	//force set the last prediction made to be 0
	this->globalHistoryReg &= (this->historyRegisterMask & ~ULL(1));
}

/*
 * Update data structures after getting actual decison 
 */
void
Gshare_SpeculativeBP::update(ThreadID tid, Addr branchAddr, bool taken, void *bpHistory, bool squashed, const StaticInstPtr & inst, Addr corrTarget)
{
	if(bpHistory)
	{
		//case that the branch history is not null
		BPHistory *history = static_cast<BPHistory *>(bpHistory);
		//1. get the index to the local counter for that branch address at that bpHistory time
		unsigned localCtrsIdx = ((branchAddr >> this->instShiftAmt) ^ history->globalHistoryReg) & this->historyRegisterMask;
		assert(localCtrsIdx < localPredictorSize);

		//2. update the local counter by the acutal judgement of the conditional branch
		if(taken)
		{
			this->localCtrs[localCtrsIdx].increment();
		}
		else
		{
			this->localCtrs[localCtrsIdx].decrement();
		}

		//if the branch is mis-predicted
		if(squashed)
		{
			if(taken)
				this->globalHistoryReg = (history->globalHistoryReg << 1) | 1;
			else
				this->globalHistoryReg = (history->globalHistoryReg << 1);
			this->globalHistoryReg &= this->historyRegisterMask;
		}
		else
		{
			//the globalHistoryReg is already updated when lookup() is called.
			delete history;
		}
	}
	//otherwise do nothing
}

/*
 * Global History Registor Update 
 */
void
Gshare_SpeculativeBP::updateGlobalHistReg(bool taken)
{
	//shift the register and insert the new value.
	this->globalHistoryReg = taken ? (globalHistoryReg << 1) | 1 :
								(globalHistoryReg << 1);
	this->globalHistoryReg &= this->historyRegisterMask;
}

/*
 * Actions for squash
 */
void
Gshare_SpeculativeBP::squash(ThreadID tid, void *bpHistory) {
	//retrieve the data from the bpHistory
	BPHistory *history = static_cast<BPHistory*>(bpHistory);
	this->globalHistoryReg = history->globalHistoryReg;
	//release the memory
	delete history;
}

Gshare_SpeculativeBP*
Gshare_SpeculativeBPParams::create()
{
	return new Gshare_SpeculativeBP(this);
}