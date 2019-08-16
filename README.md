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

3c6e72c: This commit should mark the finish of construction of basic event selection framework. It is possible to merge all the functions (skim, scan, individual and coincidence) into one .cc/hh file, since it looks like it's going to be a streamline process.

More importantly, should start thinking about randomization now.

### General

### Code logic

1. skim() will read in ntuples(data) and write to skimfiles (skim\_s%.root), storing under skim\_files.

	* apply cuts: triggerWord, dataCleaning and fitValid.

	* calculate udotr.

	* skimfile will split into subfiles, 1,000,000 events per skimfile.

2. scan() will read in skimfiles and write to a scanfile, storing under scan\_files. 

	* apply cuts: [global cuts] FV, udotr, etc..

	> global cuts means cuts that both the prompt and delayed events need to satisfy.

	* calculate tickdiff50 and posdiff.

	* currently only storing into one scanfile -> don't expect to to have a very large sample at this point.

3. individual() applies individual cuts on prompt and delayed events, respectively.

	* event\_select() checks all events if they pass the criteria (energy and nhits), write to a branch named "prompt", and then save to the same file (scan.root)
	
4. coincidence() applies coincidence cuts (time and position difference) between prompt and delayed events.

	* validated coincidence cuts algorithm. It is working as expected. (2019.Aug.16)

	* store promptId, delayedId, position diff, time diff into a new root file (candidate.root). It's easy to add more branches if wanted.

### Randomization




### Obsoleted
1. 2019.Aug.14:
   does it make more sence to skim to a new TTree?
	* basically have another new ntuple to write event that passes triggerWord, dataCleaning and fitValid?
	* also added a branch that contains the time differences. 

2. 2019.Aug.15:
    fitValid seemed to have a **HUGE** effect to remove background.
	* this is becaues most the of events below 15 nhits are not fitted in this processing. Nothing strange here.
