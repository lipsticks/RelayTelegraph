# On the need for fuzzy comparison of `TourneyId` objects

## The problem

We have two kinds of tourney listing:

* the one returned by "tell relayInfo show" (S-type *) and
* the one returned by "tell relay listTourney" (R-type).
	
*Note: The 'S' in S-type stands for 'scheduled'; the 'R' in R-type for 'relayed'.*

The lists will have slightly differing titles for the same tourney.

In order to correlate entries in-between lists, we need to devise a fuzzy `operator==()` for `TourneyId` objects.

## Are there regularities?

The first question is whether there are some regularities in the transformation function that generated the R-type titles from the S-type ones.

Here are some observed examples *:

*Note: It is dawning on me why a plugin like this hasn't already been written until now.*

	S-type					|	R-type
	------------------------+-------------------------------
	Reykjavik Open          |   Reykjavik Open 2013
	German Bundesliga       |   German Bundesliga
	Karkonosze Open Group A |   6th Karkonosze Group A
	Cen.Serbia Champ's Men  |   Central Serbia Championhips
	Romania Champ's Women   |   Romania Champs

*Note: At the same day was scheduled a 'Romania Champ's Men' tourney; they might have even been merged under the single '`Romania Champs`' title, I didn't check this then, but a custom like this would certainly make things more difficult correlation-wise.*

So we see that any one of these possibilites could be the case:

* the titles are identical (as in the '`Bundesliga`' case) or
* the shorter title is a prefix of the longer one ('`Reykjavik`')
* some words might be contracted or spelled wrong ('`Champs`')
* the words are mostly there but in the wrong order ('`Karkonsze`')

It is especially because of this last possibility (i.e. where words might be in the wrong order) that we can't use a simple prefix matching option which would have worked even for contracted or slightly misspelled words (as in the '`Romania`' case).

*Note: It seems to me that there is, in fact, no automatic translation going on here after all, but some human quick and dirty typing on the fly. This however would mean that the above cases of translation error are *definitely* not exhaustive.*

## A simple approach

Given all this, it might be worth a shot to have the fuzzy comparator first try to match the exact string, and if that fails try a prefix match, and failing that try a strict word-inclusion match * (WIM) first and a prefix WIM (pWIM) last.

*Note: A strict WIM will split both titles into word lists, then go through each word in the shorter list and check whether this word is present in the other list. A pWIM will do analogously but each time looking for the longest prefix match, and summing up their char counts.*

A working mockup in executable pseudocode (i.e. python) should serve to [illustrate what I have in mind](pseudocode/wim.py).
