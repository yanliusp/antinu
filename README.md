# Antinu Search in Water

## underlining idea can found on [DocDB-6044](https://www.snolab.ca/snoplus/private/DocDB/cgi/ShowDocument?docid=6044)

## The analysis can be broke into 4 subpart:

1. Randomization of datasets
>
3. Calculate confidence level
>
2. Varying sets of cuts
>
4. Gaussian prior distribution
>

## Thoughts

### General

### Code logic

1. skim() will read in ntuples(data) and write to skimfiles (skim_s%.root), storing under skim\_files.

	* apply cuts: triggerWord, dataCleaning and fitValid.

	* calculate udotr.

	* skimfile will split into subfiles, 1,000,000 events per skimfile.

2. scan() will read in skimfiles and write to a scanfile, storing under scan\_files. 

	* apply cuts: [global cuts] FV, udotr, etc..

	> global cuts means cuts that both the prompt and delayed events need to satisfy.

	* calculate tickdiff50 and posdiff.

	* currently only storing into one scanfile -> don't expect to to have a very large sample at this point.

3. prompt() and delayed() is expect to apply individual cuts

	* maybe this should be written into the same .cc file

	* prompt\_select() checks all events if they pass the criteria (energy and nhits), write to a branch named "prompt", and then save to the same file (scan.root)
	

### Randomization


### Obsoleted
1. 2019.Aug.14:
   does it make more sence to skim to a new TTree?
	* basically have another new ntuple to write event that passes triggerWord, dataCleaning and fitValid?
	* also added a branch that contains the time differences. 

2. 2019.Aug.15:
    fitValid seemed to have a **HUGE** effect to remove background.
	* this is becaues most the of events below 15 nhits are not fitted in this processing. Nothing strange here.
