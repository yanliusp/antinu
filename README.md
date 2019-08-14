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

1. fitValid seemed to have a **HUGE** effect to remove background.

### Code logic

1. skim() will read in ntuples(data) and write useful events into a skimfile, stored under skim\_files.
	* useful events means events that passed triggerWord, dataCleaning and fitValid.

	* currently only one skimfile will be written -> when data gets big, it would make since to split iit to subfiles.

### Randomization

#### Obsoleted
1. 2019.Aug.14:
   does it make more sence to skim to a new TTree?
	* basically have another new ntuple to write event that passes triggerWord, dataCleaning and fitValid?
	* also added a branch that contains the time differences. 
