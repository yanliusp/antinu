# Antinu Search in Water

## Underlining idea can found on [DocDB-6044](https://www.snolab.ca/snoplus/private/DocDB/cgi/ShowDocument?docid=6044)

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

2019.Sep.7: THINGS TO DO

Rather than using cuts -> now using a cumulative prob. calculation (see slides on Sep. 5 for Queen's group meeting). Things still need to be done:

	* test that the method works for other cuts. Specifically, when relieving the nhit maximum cut on delayed events
	> it does not affect results by relaxing this cut.
	* fit the distance distribution with more polynomials
	> Done. Can be well fitted by adding an additional 4th polynomial term and floating other three parameters
	* change to a likelihood analysis

2019.Aug.16: EVENT SELECTION CRITERIA

need to think about what are the possible criteria can will be applied:

	* triggerWord, dataCleaning, fitValid
	* global criteria:
		* FV, udotr, beta14, ITR 
	* individual criteria: (applies only on either prompt or delayed)
		* energy, nhits
	* coincidence criteria:
		* time, position

What else?

### Obsoleted

2019.Aug.16: OVERVIEW OF RESULTS AND COMPARISON

need to think about how the organization looks like to compare results (results mean confidence levels here). Save everything into a root file? Would that be too dangerous (of overwrite)? Maybe not if I do it the right way.

ANSWER: all results from different sets of cuts are stored in one root file (currently named count.root); each set of cuts are assigned to a unique branch, named with the full cuts; each branch can store an arbitrary number of random dataset results, where the result from the real dataset is stored in the first entry. 

NOTE: When doing grid search, as the grid inteval gets smaller, it's possible to increase number of random datasets.

-------------------------------------------------

2019.Aug.16: RANDOMIZATION

The idea is, randomization can happen at any given point, and still won't affect the integrity of the method. For example, imagine if the detector had a higher threshold than what it actually does, the integrity of the method is not affected at all (You can't say the method only works if the detector has a threshold of 4, or 5, or 6.). Now the method also doesn't care about the threshold is a hardware one, or a software one. So all the global cuts, does not have an impact 'per say'.

However, the more cuts we applied, the long time interval between background events, and the better the effect of 'randomization'. In light of this, the randomization should be carried out after all global cuts are applied.

I think the same reasoning can apply to indivisual cuts as well. but I might need to think about it more.

--------------------------------------------------

3c6e72c: This commit should mark the finish of construction of basic event selection framework. It is possible to merge all the functions (skim, scan, individual and coincidence) into one .cc/hh file, since it looks like it's going to be a streamline process.

More importantly, should start thinking about randomization now.

-------------------------------------------------

2019.Aug.15: fitValid seemed to have a **HUGE** effect to remove background.

	* this is becaues most the of events below 15 nhits are not fitted in this processing. Nothing strange here.

-------------------------------------------------

2019.Aug.14: does it make more sence to skim to a new TTree?

	* basically have another new ntuple to write event that passes triggerWord, dataCleaning and fitValid?
	* also added a branch that contains the time differences. 

## General

## Code logic

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
	* output: this root file can have multiple branches corresponding to different sets of cuts. The first entry will save the result from cuts on the real dataset.
