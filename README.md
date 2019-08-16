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

2019.Aug.16: RANDOMIZATION

The idea is, randomization can happen at any given point, and still won't affect the integrity of the method. For example, imagine if the detector had a higher threshold than what it actually does, the integrity of the method is not affected at all (You can't say the method only works if the detector has a threshold of 4, or 5, or 6.). Now the method also doesn't care about the threshold is a hardware one, or a software one. So all the global cuts, does not have an impact 'per say'.

However, the more cuts we applied, the long time interval between background events, and the better the effect of 'randomization'. In light of this, the randomization should be carried out after all global cuts are applied.

I think the same reasoning can apply to indivisual cuts as well. but I might need to think about it more.

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
	
4. coincidence() selct pairs of coincidence events and write to a new candidate.root file. It *DOES NOT* apply coincidence cuts (time and position difference) between prompt and delayed events.

	* validated coincidence cuts algorithm. It is working as expected. (2019.Aug.16)

	* store promptId, delayedId, position diff, time diff into a new root file (candidate.root). It's easy to add more branches if wanted.

5. randomize() randomize the time difference (if step is set to 0 then no randomization) and then (possibly) applies the coincidence cuts.

	* validated coincidence cuts algorithm. It is working as expected. (2019.Aug.16)

	* it's possible to write to a root file with a single branch that contains number of events that pass the cuts -> useful to draw the bg distribution. NOTE: One branch for a set of cuts. the root file could be contains multiple branches.

6. write() calls randomize() function to run the set of coincidence cuts over multiple random datasets.

	* two inputs: random\_step, and coincidence cuts
	* output: a root file that contains the number of events passed all cuts (see: ./plots/example\_2019.Aug.16.pdf)

### Randomization




### Obsoleted
1. 2019.Aug.14:
   does it make more sence to skim to a new TTree?
	* basically have another new ntuple to write event that passes triggerWord, dataCleaning and fitValid?
	* also added a branch that contains the time differences. 

2. 2019.Aug.15:
    fitValid seemed to have a **HUGE** effect to remove background.
	* this is becaues most the of events below 15 nhits are not fitted in this processing. Nothing strange here.
