# On keeping the polling of server data minimal

## The problem

From the standpoint of a normal session (a couple of hours each day), the data in the `relayInfo` schedule is fairly constant: important new data are unlikely to be added during a user's login time. It is therefore sufficient to poll the list once at login *.

*Note: A manual 'update' button will be made available though, and possibly an auto-update setting with a minimum delay of an hour or so.*

The other lists, however, are bound to change during a session, but must be monitored without the server getting flooded with queries. *

*Note: All such features (whose functionality is based on frequently sending commands to the server) must be considered a potential hazard. Bugs are likely to creep in and put the user in danger of being kicked for spamming. The less polling we can get away with is therefore the better.*

## The `relay` tourney list

The `relay` list can be expected to change at the dates given in the schedule list (plus minus half an hour or so to be on the safe side).

Polling it, however, is unnecessary, since the user will be receiving QTells if the relevant variable has been configured accordingly *.

*Note: I'm not sure whether this is entirely correct, since the helpfile ('`tell relay help notify`') says that it "will automatically notify the user of relayed tournaments when the user logs in", referring to the listing one gets at login time but perhaps also implying the later QTells. It certainly doesn't hurt to have this flag enabled though.*

To enable notifications, the user would '`tell relay notify`', which toggles the notification variable. I am unaware of a way to query the current setting except by toggling twice.

These are the possible outputs (for user `lipsticks`):

	:lipsticks will no longer be notified of relayed tournament at login.
	
	:lipsticks will be notified of relayed tournament at login.

At login, therefore, the plugin needs to query the relay info once ('`tell relay listTourney`') and futhermore ensure that notifications are enabled (by toggling either once or twice). Note also that, if notifications hadn't already been enabled by the user (i.e. if plugin had to toggle only once), we must ensure that notifications *are disabled again at logout*.

## The `mamer` tourney list

To get the initial list of joinable tourneys at logon, we do a '`tell mamer listTourneys -j`' (the list will specify whether a tourney has already started or not). To keep getting notifications, we do a '`tell mamer set tshout on`' *. To query, whether this has already been enabled by the user and need not be touched (i.e. enabled at login and disabled again at logout) '`tell mamer listPlayerVariables`' and parse for the line '`:TShout:          On`'.

*Note: I am not yet sure whether the relevant variable is indeed `TShout` or any one (or combination) of `TourneyUpdates`, `TourneyInfo`, and whether, say, variables `Censor` and `Silence` must be off and variables `Echo` and `SetDefault` on, and so on.*

For all parsing purposes **it is important** to take the `height` variable into consideration, which determines the number of output lines per page (browsable with '`tell mamer next`'). It makes sense to set it to at least '24' (the default) at login (and of course to restore its original value at logout).

## The `relay` games list

As soon as a tourney starts, we fetch its games list (with '`tell relay listGames`' plus the tourney number).

I am not sure whether `relay` is notifying the user of game results. If not, we would have to poll it in order to determine which games have been concluded yet, provided we wanted to include this info in our GUI *.

*Note: The alternative to having the games' status listed in our GUI would be to only show the pairings but require the user to double-click on one (i.e. to observe that game), in order to learn its status. I think this is the better option, but it could be left to the user to choose a particular behavior (with this, the server-friendlier one, being the default).*

To ease the burden on the server, we would derive a dynamic polling delay based on the games' time controls. For a 2 hour game, we would, say, poll after an hour, then after half an hour, and so on until some minimum is reached (e.g. 15 minutes) *.

*Note: Usually both player's time controls will be used up (4 hours) before a game concludes, but shorter games are possible.*

## The `mamer` games list

Unlike with `relay`, here we have the luxury of being notified about game results. Unfortunately this is only the case for tourneys in which we are participating.

Because having this list for all tourneys would involve significant polling traffic, and given that the utility of having a live view into a tourney which we aren't playing in is highly questionable, I have decided to leave this feature out. A games list will only be shown for the tourney we are playing in.

## Load-sensitive polling

Instead of fixing the polling delays to some default (adjustable by user in the preferences) we could consider making it dynamic, where the actual value would be contingent on the current server load (see '`ustat`'). Determining the server load is of course itself a polling operation (and would be self-adjusting).
