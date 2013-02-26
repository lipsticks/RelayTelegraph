import re

S = [
	"Reykjavik Open",
	"German Bundesliga",
	"Karkonosze Open Group A",
	"Cen.Serbia Champ's Men",
	"Romania Champ's Women",
	]
	
R = [
	"Reykjavik Open 2013",
	"German Bundesliga",
	"6th Karkonosze Group A",
	"Central Serbia Championhips",
	"Romania Champs",
	]

def adjustCaseForComparison(S, R, sensitive=False):
	"""Returns two new lists with titles adjusted for either case-sensitive
		or case-insensitive comparison."""
	if not sensitive:
		# case doesn't matter, so force all case to lower
		S = [s.lower() for s in S]
		R = [r.lower() for r in R]
	return (S, R)

def strictMatch(a, b):
	"""True if the two strings are identical."""
	return a == b

def sparseMatch(a, b):
	"""True if the shorter string's words are included in the longer in
		the same order (but possibly with extra characters in-between)."""
	l, r = (a, b) if len(a) < len(b) else (b, a)
	L = re.split(r"[^0-9A-Za-z]+", l)
	l = ".*?" + ".*?".join(L) + ".*?"
	return re.match(l, r, re.I)

def getCommonPrefixLength(a, b):
	"""Returns the number of left-most matching characters."""
	assert len(a) > 0 or len(b) > 0, "two empty strings would match 100% but still return a zero weight"
	count = 0
	for i in xrange(min(len(a), len(b))):
		if a[i] != b[i]: break
		count += 1
	return count

def getWordSimilarityWeight(a, b):
	"""Returns a simple similarity weight. Does this by comparing each word in 'a'
		with each word in 'b', looking for those pairs having the longest common prefix
		and returning the sum of these prefixes' lengths."""
	A = re.split(r"[ \t]+", a)
	B = re.split(r"[ \t]+", b)
	totalBestWeight = 0
	for a in A:
		bestWeight = 0
		for b in B:
			bestWeight = max(bestWeight, getCommonPrefixLength(a, b))
		#TODO: What would happen if we removed each best match from B? Is it necessary?
		#	Leaving an already best-matched word in the list could lead to it being
		#	partially matched again, accumulating meaningless weight (noise).
		totalBestWeight += bestWeight
	return totalBestWeight

def weightedLoop(r, S, weighingMethod):
	"""A more complex looper, used for fuzzy search. The result of 'weighingMethod' is
		an integer: the higher the better. The looper runs through the whole list of
		titles, returning the title with the highest weight."""
	bestMatch = None
	highestWeight = 0
	for i, s in enumerate(S):
		weight = weighingMethod(r, s)
		if weight > highestWeight:
			highestWeight = weight
			bestMatch = i
	return bestMatch

def independentLoop(r, S, compareMethod):
	"""A simple looper which treates each title independently. Used for strict matching:
		the first match is returned."""
	for i, s in enumerate(S):
		if compareMethod(r, s):
			return i
	return None

def run(S, R, looper, method):
	"""Determine the correlation between titles in 'S' and 'R', return a list of tuples
		of indices into these lists (the 'mapping'). The 'looper' callback is a function
		that compares one title from 'S' against all titles in 'R' and returns the one
		that 'method' determines to be the closest match."""
	mapping = []
	for i, r in enumerate(R):
		mapping.append((i, looper(r, S, method)))
	return mapping

def dump(S, R, mapping, name=None):
	"""Print a listing of correlated titles as defined in 'mapping'."""
	print "\n" + name
	for i, j in mapping:
		s = S[i] if None != i else ""
		r = R[j] if None != j else ""
		print "'%-30s' -> '%s'" % (s, r)

if "__main__" == __name__:
	_S, _R = adjustCaseForComparison(S, R, sensitive=False)
	dump(S, R, run(_S, _R, independentLoop, strictMatch), "STRICT MATCHING (identity)")
	dump(S, R, run(_S, _R, independentLoop, sparseMatch), "STRICT MATCHING (sparse)")
	dump(S, R, run(_S, _R, weightedLoop, getCommonPrefixLength), "WEIGHTED MATCHING (full prefix)")
	dump(S, R, run(_S, _R, weightedLoop, getWordSimilarityWeight), "WEIGHTED MATCHING (word prefix)")
	