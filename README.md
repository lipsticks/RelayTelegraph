# RelayTelegraph

## Introduction

This is supposed to become a little plugin for the [BabasChess](http://www.babaschess.net "BabasChess Home") client to be run on [FICS](http://www.freechess.org "The Free Internet Chess Server"). Its foremost purpose being to move the interaction with the **relay bot** away from the console and into the Info Window.

### Prerequisites

The makefiles ware written under the assumption of a working [MinGW](http://www.mingw.org "Minimalist GNU for Windows") environment in place.

### Updates

#### 2013-02-23

The plugin now shows the list of scheduled tournaments. The next step is to add a toolbar (and maybe a menu) that will let the user switch between a couple of lists:

* scheduled tourneys (this one),
* currently relayed,
* mamer tourneys, and a separate one for the 
* games in the currently selected tourney (in one of the other lists).

The toughest part, as far as I can see, will be devising a [reliable method for correlating items](docs/fuzzycomp.md) between the schedule (relayInfo) and relay lists, since the titles are not guaranteed to be identical (and in fact are likely to differ in creative ways). So we'll have to look for regularities in the ways the titles might mismatch and have our comparison function try to exploit those. (It's fortunate that we can compare the round numbers explicity, which should get rid of a substantial subset of candidates.) Additionally, before running the costly comparison, we'll be shortening the schedule list by running it through a sanity filter, removing tourneys that have the wrong date (i.e. which could not have started yet) even though this might open us up to another error source (i.e. when a relay occasionally starts earlier than advertised).

#### 2013-02-18

I've added a simple test tab to the InfoWindow now. It should draw a little interactive box.

**Note**: If the new tab doesn't show, you will want to enable it. Right click the InfoWindow and open its preferences where you must put a tick next to the "RelayTelegraph".
