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

### Randomization

1. does it make more sence to output "useful" events to a new ntuple events?
> basically have another new ntuple to write event that passes triggerWord, dataCleaning and fitValid?
