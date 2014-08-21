#include <iostream>
#include <stack>
#include <list>
#include <algorithm>

using namespace std;

struct NODE
{
	char Key;

	NODE * Child;
	NODE * Sibling;

	bool bIsWord : 1;  //BitField
	char * pWord;  //contains a value only if bIsWord is set.
	NODE(char key) : Key(key), Child(nullptr), Sibling(nullptr), bIsWord(false), pWord(nullptr)
	{};

	~NODE()
	{
		if (pWord)
		{
			delete pWord;
		}
	}

};


struct Visitor
{
public:
	virtual void Visit(NODE * pNode) = 0;

};
class DeleteVisitor : public Visitor
{
private:

public:
	stack<NODE *> stack;

	void Visit(NODE * pNode) override
	{
		stack.push(pNode);
	}

};

class WordsListVisitor : public Visitor
{
private:

public:
	list<NODE *> list;

	void Visit(NODE * pNode) override
	{
		if (pNode->bIsWord)
		{
			list.push_back(pNode);
		}
	}

};

class Trie
{
private:
	NODE * Root;
	void EditsImpl(_In_opt_ NODE * pTrieRoot, _In_ char * pWord, _In_ list<char *> * pSuggestions, _In_ int editDistance);

public:
	Trie()
	{ Root = new NODE('\0'); };
	
	void Insert(_In_ char * pStr);

	void DeleteWord(_In_ char * pStr);

	bool IsWord(_In_ char * pStrn, _In_opt_ Visitor * pVisitor = nullptr);

	void TraverseTrie(_In_opt_ NODE* pNode, _In_opt_ Visitor *pVisitior = nullptr);

	list<char *> GetAllWords();

	list<char *> GetSuggestions(_In_ char * pWord, _In_ int nEditDistance = 1);


};


void Trie::Insert(_In_ char * pStr)
{
	if (pStr == nullptr) return;
	char * pWord = pStr;

	int len = strlen(pStr);

	NODE ** ppCurr = &Root;


	while (*pStr != '\0')
	{
		if ((*ppCurr)->Child == nullptr)
		{
			(*ppCurr)->Child = new NODE(*pStr);
			ppCurr = &((*ppCurr)->Child);
		}
		else
		{
			if ((*ppCurr)->Child->Key == *pStr)
			{
				ppCurr = &(*ppCurr)->Child;
			}
			else
			{
				NODE ** ppSibling = &((*ppCurr)->Child);
				while (*ppSibling != nullptr)
				{
					if ((*ppSibling)->Key == *pStr)
					{
						ppCurr = ppSibling;
						break;
					}
					else
						ppSibling = &((*ppSibling)->Sibling);

				}

				if (*ppSibling == nullptr)
				{
					*ppSibling = new NODE(*pStr);
					ppCurr = ppSibling;  //becomes new current.
				}

			}
		}


		pStr++;
	}
	(*ppCurr)->bIsWord = true;

	//store the entire word in the leaf word node.
	char* pBuff = new char[len + 1];
	pBuff[len] = '\0';
	strncpy_s(pBuff, len+1, pWord, len);
	(*ppCurr)->pWord = pBuff;
}

void Trie::DeleteWord(_In_ char * pStr)
{
	DeleteVisitor visitor;
	if (!IsWord(pStr, &visitor))
	{
		return;
	}

	stack<NODE *> stack = visitor.stack;

	NODE * pCurr = stack.top(); stack.pop();

	if (pCurr == this->Root)
	{
		return;
	}

	pCurr->bIsWord = false;  //set flag to false.

	if (pCurr->Child)
	{
		return;  // has children, so just unset flag and return.
	}

	NODE * pParent = nullptr, ** ppPrevChild = nullptr, ** ppChild = nullptr;
	
	while (pCurr != this->Root)
	{
		pParent = stack.top(); stack.pop();

		ppPrevChild = ppChild = &(pParent->Child);

		while (*ppChild != pCurr)
		{
			ppPrevChild = ppChild;
			ppChild = &((*ppChild)->Sibling);

		}

		//at this point ppChild is pointing to node to be deleted.
		*ppPrevChild = (*ppChild)->Sibling;
		delete(pCurr);

		if (pParent->Child || pParent->bIsWord)
		{
			break;  //the parent node has other children so we break the loop here.
		}
		else
		{
			pCurr = pParent; // else we walk up the stack.

		}
	}

	while (!stack.empty())
	{
		stack.top(); stack.pop();
	}
}

bool Trie::IsWord(_In_ char * pStr, _In_opt_ Visitor * pVisitor)
{

	if (pStr == nullptr)
	{
		return false;
	}

	if (Root->Child == nullptr)
	{
		return false;
	}

	NODE *pCurr = Root;
	if (pVisitor)
	{
		pVisitor->Visit(pCurr);
	}

	while (*pStr != '\0' && pCurr)
	{
		if (pCurr->Child == nullptr)
		{
			return false;
		}

		if (pCurr->Child->Key == *pStr)
		{
			pCurr = pCurr->Child;
			if (pVisitor)
			{
				pVisitor->Visit(pCurr);
			}
		}
		else
		{
			NODE * pSibling = pCurr->Child;
			while (pSibling)
			{
				if (pSibling->Key == *pStr)
				{
					pCurr = pSibling;
					if (pVisitor)
					{
						pVisitor->Visit(pCurr);
					}
					break;
				}
				else
				{
					pSibling = pSibling->Sibling;
				}
			}

			if (pSibling == nullptr)
			{
				return false;
			}
		}

		pStr++;
	}

	if (pCurr == nullptr)
	{
		return false;
	}

	if (*pStr == '\0' && pCurr->bIsWord)
	{
		return true;
	}
	else
		return false;

}

void Trie::TraverseTrie(_In_opt_ NODE* pNode, _In_opt_ Visitor * pVisitor)
{
	if (pNode == nullptr) return;

	if (pVisitor)
	{
		pVisitor->Visit(pNode);
	}

	TraverseTrie(pNode->Child, pVisitor);

	if (pNode->Child)
	{
		NODE * pCurr = pNode->Child->Sibling;
		while (pCurr)
		{
			TraverseTrie(pCurr, pVisitor);
			pCurr = pCurr->Sibling;
		}
	}
	
}

list<char *> Trie::GetAllWords()
{
	WordsListVisitor visitor;

	TraverseTrie(this->Root, &visitor);

	list<char*> wordsList;

	for (list<NODE*>::iterator it = visitor.list.begin(); it != visitor.list.end(); it++)
	{
		wordsList.push_back(((NODE*)*it)->pWord);
	}

	return wordsList;
}


void Trie::EditsImpl(_In_opt_ NODE * pTrieRoot, _In_ char * pWord, _In_ list<char *> * pSuggestions, _In_ int nEditDistance)
{
	if (pTrieRoot != nullptr && pTrieRoot->bIsWord && *pWord == '\0' && nEditDistance >= 0)
	{
		pSuggestions->push_back(pTrieRoot->pWord);
	}

	int nWordLen = strlen(pWord);
	if (nEditDistance >= 1)
	{

		//deletion
		if (nWordLen > 1)
		{
			EditsImpl(pTrieRoot, &pWord[1], pSuggestions, nEditDistance - 1);
		}
		else
		{
			EditsImpl(pTrieRoot, "", pSuggestions, nEditDistance - 1);
		}

		//Insertion and  substitution
		NODE * pCurr = pTrieRoot->Child;
		while (pCurr)
		{
			//Insertion
			EditsImpl(pCurr, pWord, pSuggestions, nEditDistance - 1);

			//Substitution
			EditsImpl(pCurr, &pWord[1], pSuggestions, nEditDistance - 1);

			pCurr = pCurr->Sibling;
		}

		//Transposition (swap first and second letters)
		char temp;
		if (nWordLen >= 2)
		{
			temp = pWord[0]; pWord[0] = pWord[1]; pWord[1] = temp;  //swap 0 and 1
			EditsImpl(pTrieRoot, pWord, pSuggestions, nEditDistance - 1);
			temp = pWord[0]; pWord[0] = pWord[1]; pWord[1] = temp;  //swap back

		}
	}

	// no edits have been made at this point. Advance to next char.
	if (nWordLen >= 1)
	{
		NODE * pCurr = pTrieRoot->Child;
		while (pCurr && pCurr->Key != *pWord)
		{
			pCurr = pCurr->Sibling;
		}

		if (pCurr)
		{
			if (nWordLen > 1)
			{
				EditsImpl(pCurr, &pWord[1], pSuggestions, nEditDistance);
			}
			else
			{
				EditsImpl(pCurr, "", pSuggestions, nEditDistance);
			}
		}
	}

}

bool same_words(char * first, char * second)
{
	return (strncmp(first, second, _MAX_PATH) == 0);
}

bool sort_words(char * first, char * second)
{
	return (strncmp(first, second, _MAX_PATH) < 0 ? true : false);
}
list <char *> Trie::GetSuggestions(_In_ char * pWord, _In_ int nEditDistance)
{

	list<char *> suggestions;
	EditsImpl(this->Root, pWord, &suggestions, nEditDistance);

	suggestions.sort(sort_words);
	suggestions.unique(same_words);
	return suggestions;
}

void TestInsert()
{
	Trie* trie = new Trie();

	trie->Insert("Adrian");
	trie->Insert("Apple");
	trie->Insert("AdrianMa");
	trie->Insert("catapult");
	trie->Insert("cater");
	trie->Insert("cataract");
	trie->Insert("catamaran");
	trie->Insert("catamarans");

	_ASSERT(trie->IsWord("Adrian"));
	_ASSERT(!trie->IsWord("Ad"));
	_ASSERT(trie->IsWord("AdrianMa"));
	_ASSERT(trie->IsWord("Apple"));
	_ASSERT(trie->IsWord("catamaran"));
	_ASSERT(trie->IsWord("cater"));
	_ASSERT(!trie->IsWord("pupil"));



}

void TestDelete()
{
	Trie* trie = new Trie();

	trie->Insert("Adrian");
	trie->Insert("Apple");
	trie->Insert("AdrianMa");
	trie->Insert("catapult");
	trie->Insert("cater");
	trie->Insert("cataract");
	trie->Insert("catamaran");
	trie->Insert("catamarans");


	_ASSERT(trie->IsWord("catamaran"));
	trie->DeleteWord("catamaran");  //should only unset flag
	_ASSERT(!trie->IsWord("catamaran"));
	_ASSERT(trie->IsWord("catamarans"));

	trie->Insert("catamaran");  //add flag back.
	trie->DeleteWord("catamarans");
	_ASSERT(!trie->IsWord("catamarans"));
	_ASSERT(trie->IsWord("catamaran"));

	trie->DeleteWord("catamar"); //should not delete since catamar is not a word.
	_ASSERT(trie->IsWord("catamaran"));
	trie->DeleteWord("catermaran");
	_ASSERT(trie->IsWord("cataract"));
	_ASSERT(trie->IsWord("catapult"));

	trie->Insert("catamaran");
	trie->DeleteWord("catapult");  //delete first child
	_ASSERT(trie->IsWord("catamaran")); //next sibling becomes first child
	_ASSERT(trie->IsWord("cataract"));

	trie->DeleteWord("cataract");  //delete last sibling
	_ASSERT(trie->IsWord("catamaran")); 

	
	trie->DeleteWord("catamaran");  //delete only child 
	_ASSERT(!trie->IsWord("catamaran"));
	_ASSERT(trie->IsWord("cater"));

	trie->DeleteWord("Adrian");
	trie->DeleteWord("AdrianMa");
	trie->DeleteWord("Apple");

	_ASSERT(trie->IsWord("cater"));



}
char * words[] = { "a", "abilities", "ability", "ability's", "able", "about", "above", "absence", "absolute", "absolutely", "abuse", "academic", "accept", "acceptable", "accepted", "accepting", "accepts", "access", "accessible", "accident", "accidental", "accidentally", "accord", "accorded", "according", "accordingly", "accords", "account", "account's", "accounts", "accuracy", "accurate", "achieve", "achieved", "achieves", "achieving", "acquire", "acquired", "acquires", "acquiring", "across", "act", "act's", "acted", "acting", "action", "action's", "actions", "active", "activities", "activity", "activity's", "acts", "actual", "actually", "add", "added", "adding", "addition", "additional", "address", "address's", "addressed", "addresses", "addressing", "adds", "adequate", "adjust", "administration", "admit", "admits", "admitted", "admittedly", "admitting", "adopt", "adopted", "adopting", "adopts", "advance", "advanced", "advances", "advancing", "advantage", "advantage's", "advantages", "advertise", "advertised", "advertises", "advertising", "advice", "advise", "advised", "advises", "advising", "affair", "affair's", "affairs", "affect", "affect's", "affected", "affecting", "affects", "afford", "afraid", "after", "afternoon", "afterward", "afterwards", "again", "against", "age", "age's", "agency", "ages", "ago", "agree", "agreed", "agreeing", "agreement", "agrees", "ahead", "aid", "aim", "aimed", "aiming", "aims", "air", "alarm", "album", "algorithm", "algorithm's", "algorithms", "alias", "alive", "all", "allow", "allowed", "allowing", "allows", "almost", "alone", "along", "already", "also", "alter", "altered", "altering", "alternate", "alternative", "alternative's", "alternatively", "alternatives", "alters", "although", "altogether", "always", "am", "ambiguous", "among", "amount", "amount's", "amounts", "amuse", "amused", "amuses", "amusing", "an", "analog", "analogue", "analysis", "ancient", "and", "angle", "angry", "animal", "announce", "announcement", "annoy", "annoyed", "annoying", "annoys", "annual", "anonymous", "another", "answer", "answer's", "answered", "answering", "answers", "any", "anybody", "anyone", "anyplace", "anything", "anyway", "anywhere", "apart", "apologies", "apologize", "apology", "apology's", "apparent", "apparently", "appeal", "appear", "appearance", "appeared", "appearing", "appears", "apple", "application", "application's", "applications", "applied", "applies", "apply", "applying", "appreciate", "appreciated", "appreciates", "appreciating", "approach", "appropriate", "approval", "approve", "approved", "approves", "approving", "arbitrary", "are", "area", "area's", "areas", "aren't", "argue", "argued", "argues", "arguing", "argument", "argument's", "arguments", "arise", "arises", "arithmetic", "arm", "army", "around", "arrange", "arranged", "arrangement", "arrangement's", "arrangements", "arranges", "arranging", "arrive", "arrived", "arrives", "arriving", "art", "article", "article's", "articles", "artificial", "artist", "as", "aside", "ask", "asked", "asking", "asks", "asleep", "aspect", "aspect's", "aspects", "assembler", "assembly", "assistant", "associate", "associated", "associates", "associating", "association", "assume", "assumed", "assumes", "assuming", "assumption", "assure", "assured", "assures", "assuring", "at", "ate", "atmosphere", "attach", "attached", "attaches", "attaching", "attack", "attempt", "attempt's", "attempted", "attempting", "attempts", "attend", "attended", "attending", "attends", "attention", "attitude", "attract", "attractive", "audience", "author", "author's", "authorities", "authority", "authority's", "authors", "automatic", "automatically", "automobile", "autumn", "autumn's", "available", "average", "avoid", "avoided", "avoiding", "avoids", "awake", "award", "aware", "away", "awful", "awkward", "back", "backed", "background", "backing", "backs", "backwards", "bad", "badly", "balance", "ball", "ban", "band", "bank", "bar", "bar's", "bars", "base", "based", "bases", "basic", "basically", "basing", "basis", "battery", "be", "bear", "bear's", "bearing", "bears", "beautiful", "became", "because", "become", "becomes", "becoming", "bed", "been", "before", "beforehand", "began", "begin", "beginning", "begins", "begun", "behalf", "behave", "behavior", "behind", "being", "believe", "believed", "believes", "believing", "belong", "belongs", "below", "benefit", "benefit's", "benefits", "besides", "best", "bet", "bets", "better", "betting", "between", "beyond", "bid", "bidding", "bids", "big", "bigger", "biggest", "bill", "binary", "bind", "binding", "binds", "biology", "bit", "bit's", "bite", "bites", "biting", "bits", "bitten", "bizarre", "black", "blame", "blank", "block", "blow", "blue", "board", "board's", "boards", "boat", "bodies", "body", "body's", "book", "book's", "books", "boot", "bore", "borne", "borrow", "borrowed", "borrowing", "borrows", "both", "bother", "bothered", "bothering", "bothers", "bottle", "bottom", "bought", "bound", "box", "box's", "boxes", "boy", "bracket", "bracket's", "brackets", "branch", "branch's", "branches", "brand", "breach", "break", "break's", "breaking", "breaks", "bridge", "brief", "briefly", "bright", "bring", "bringing", "brings", "broadcast", "broadcasting", "broadcasts", "broke", "broken", "brother", "brought", "brown", "bucket", "budget", "buffer", "bug", "bug's", "bugs", "build", "building", "building's", "buildings", "builds", "built", "bulk", "bulletin", "buried", "buries", "bury", "burying", "bus", "business", "busy", "but", "button", "buy", "buying", "buys", "by", "byte", "byte's", "bytes", "calculate", "calculation", "calculation's", "calculations", "call", "call's", "called", "calling", "calls", "came", "campaign", "can", "can't", "candidate", "cannot", "capable", "capacity", "capital", "captain", "car", "card", "card's", "cardboard", "cards", "care", "care's", "careful", "carefully", "cares", "carried", "carries", "carry", "carry's", "carrying", "case", "case's", "cases", "cassette", "cat", "catalog", "catch", "catches", "catching", "categories", "category", "category's", "caught", "cause", "cause's", "caused", "causes", "causing", "cease", "cell", "cent", "center", "central", "century", "certain", "certainly", "chain", "chair", "chairman", "chance", "chance's", "chances", "change", "change's", "changed", "changes", "changing", "channel", "channel's", "channels", "chaos", "chapter", "char", "char's", "character", "character's", "characters", "charge", "charge's", "charged", "charges", "charging", "chars", "cheap", "cheaper", "cheapest", "check", "check's", "checked", "checking", "checks", "chemical", "child", "child's", "children", "chip", "chip's", "chips", "choice", "choose", "chooses", "choosing", "chose", "chosen", "church", "circle", "circuit", "circulation", "circumstance", "circumstance's", "circumstances", "citizen", "city", "claim", "claim's", "claimed", "claiming", "claims", "clarify", "class", "class's", "classes", "clean", "clear", "cleared", "clearer", "clearest", "clearing", "clearly", "clears", "clever", "clock", "close", "closed", "closely", "closer", "closes", "closest", "closing", "club", "clue", "code", "code's", "coded", "codes", "coding", "coffee", "cold", "collapse", "collect", "collected", "collecting", "collection", "collects", "college", "college's", "colleges", "color", "color's", "colors", "column", "combination", "combination's", "combinations", "combine", "combined", "combines", "combining", "come", "comes", "coming", "command", "command's", "commands", "comment", "comment's", "commented", "commenting", "comments", "commercial", "commission", "commitment", "committee", "common", "commonly", "communicate", "communication", "communication's", "communications", "community", "company", "comparable", "comparatively", "compare", "compared", "compares", "comparing", "comparison", "compatibility", "compatible", "competition", "compiler", "complain", "complained", "complaining", "complains", "complaint", "complaint's", "complaints", "complete", "completed", "completely", "completes", "completing", "complex", "complexity", "complicate", "complicated", "complicates", "complicating", "component", "component's", "components", "compose", "composed", "composes", "composing", "composition", "comprehensive", "compromise", "compulsory", "compute", "computed", "computer", "computer's", "computers", "computes", "computing", "concept", "concern", "concern's", "concerned", "concerning", "concerns", "conclusion", "concrete", "condition", "condition's", "conditions", "conference", "confident", "confirm", "confirmed", "confirming", "confirms", "confuse", "confused", "confuses", "confusing", "confusion", "connect", "connected", "connecting", "connection", "connection's", "connections", "connects", "consequence", "consequence's", "consequences", "consequently", "consider", "considerable", "considerably", "consideration", "considered", "considering", "considers", "consist", "consistency", "consistent", "consists", "constant", "constraint", "constraint's", "constraints", "construct", "consumption", "contact", "contain", "contained", "containing", "contains", "content", "content's", "contents", "context", "continually", "continuation", "continue", "continued", "continues", "continuing", "continuous", "continuously", "contract", "contrary", "contrast", "contribute", "contribution", "contribution's", "contributions", "control", "control's", "controlled", "controlling", "controls", "convenient", "convention", "convention's", "conventional", "conventions", "conversation", "convert", "convince", "convinced", "convinces", "convincing", "cope", "copied", "copies", "copy", "copy's", "copying", "core", "corner", "corner's", "corners", "correct", "corrected", "correcting", "correction", "correctly", "corrects", "corrupt", "corrupted", "corrupting", "corrupts", "cost", "cost's", "costing", "costs", "could", "couldn't", "council", "count", "count's", "counted", "counter", "counting", "country", "counts", "county", "couple", "course", "course's", "courses", "court", "cover", "cover's", "covered", "covering", "covers", "crash", "crash's", "crashed", "crashes", "crashing", "crazy", "create", "created", "creates", "creating", "creation", "creature", "credit", "crisis", "crisp", "crisp's", "crisps", "critical", "criticism", "cross", "cry", "cs", "culture", "cum", "cumming", "cums", "cup", "cure", "curious", "current", "currently", "cursor", "customer", "cut", "cuts", "cutting", "cycle", "cycle's", "cycles", "daily", "damage", "damaged", "damages", "damaging", "danger", "dangerous", "dare", "dark", "data", "database", "date", "date's", "dated", "dates", "dating", "datum", "datum's", "day", "day's", "days", "dead", "dead's", "deal", "deal's", "dealing", "deals", "dealt", "dear", "death", "debate", "decade", "decent", "decide", "decided", "decides", "deciding", "decision", "decision's", "decisions", "declare", "declared", "declares", "declaring", "decrease", "dedicate", "dedicated", "dedicates", "dedicating", "deduce", "deem", "deemed", "deeming", "deems", "deep", "deeply", "default", "defense", "define", "defined", "defines", "defining", "definite", "definitely", "definition", "definition's", "definitions", "definitive", "degree", "degree's", "degrees", "delay", "delete", "deleted", "deletes", "deleting", "deliberate", "deliberately", "deliver", "delivered", "delivering", "delivers", "delivery", "demand", "demand's", "demands", "democratic", "demonstrate", "demonstration", "department", "depend", "depended", "depending", "depends", "depth", "derive", "derived", "derives", "deriving", "describe", "described", "describes", "describing", "description", "description's", "descriptions", "design", "designed", "designing", "designs", "desirable", "desire", "desired", "desires", "desiring", "desk", "desperate", "despite", "destroy", "destroyed", "destroying", "destroys", "detail", "detail's", "detailed", "detailing", "details", "detect", "detected", "detecting", "detects", "determine", "determined", "determines", "determining", "develop", "developed", "developing", "development", "develops", "device", "device's", "devices", "devote", "devoted", "devotes", "devoting", "dictionary", "did", "didn't", "die", "died", "dies", "differ", "difference", "difference's", "differences", "different", "differently", "difficult", "difficulties", "difficulty", "difficulty's", "digit", "digit's", "digital", "digits", "dinner", "direct", "directed", "directing", "direction", "direction's", "directions", "directly", "director", "directory", "directs", "dirty", "disadvantage", "disagree", "disappear", "disappeared", "disappearing", "disappears", "disaster", "disc", "disc's", "discipline", "discount", "discourage", "discouraged", "discourages", "discouraging", "discover", "discovered", "discovering", "discovers", "discs", "discuss", "discussed", "discusses", "discussing", "discussion", "discussion's", "discussions", "disk", "dislike", "display", "displayed", "displaying", "displays", "distance", "distant", "distinct", "distinction", "distinctly", "distinguish", "distribute", "distributed", "distributes", "distributing", "distribution", "district", "disturb", "disturbed", "disturbing", "disturbs", "ditto", "divide", "divided", "divides", "dividing", "division", "do", "document", "document's", "documentation", "documented", "documenting", "documents", "doe", "doe's", "does", "doesn't", "dog", "doing", "dollar", "domain", "don't", "done", "door", "door's", "doors", "double", "doubt", "doubtful", "down", "dozen", "dozen's", "dozens", "drastic", "draw", "draw's", "drawing", "drawn", "draws", "dream", "drew", "drink", "drive", "drive's", "driven", "driver", "driver's", "drivers", "drives", "driving", "drop", "dropped", "dropping", "drops", "drove", "dry", "dubious", "due", "dumb", "dump", "during", "duty", "dying", "each", "earlier", "earliest", "early", "earth", "ease", "easier", "easiest", "easily", "east", "easy", "eat", "eaten", "eating", "eats", "economic", "economy", "edge", "edit", "edited", "editing", "edition", "editor", "editor's", "editors", "edits", "education", "educational", "effect", "effect's", "effective", "effectively", "effects", "efficient", "effort", "effort's", "efforts", "eight", "either", "elect", "elected", "electing", "election", "electric", "electronic", "electronics", "elects", "element", "element's", "elements", "elevator", "else", "elsewhere", "embarrass", "embarrassed", "embarrasses", "embarrassing", "emergency", "emphasis", "employee", "empty", "enable", "enables", "encounter", "encountered", "encountering", "encounters", "encourage", "encouraged", "encourages", "encouraging", "end", "end's", "ended", "ending", "ends", "enemy", "engineer", "engineer's", "engineered", "engineering", "engineers", "enjoy", "enormous", "enough", "ensure", "ensured", "ensures", "ensuring", "enter", "entered", "entering", "enters", "entire", "entirely", "entitle", "entitled", "entitles", "entitling", "entity", "entrance", "entries", "entry", "entry's", "environment", "equal", "equally", "equipment", "equivalent", "eraser", "err", "error", "error's", "errors", "escape", "especially", "essential", "essentially", "establish", "established", "establishes", "establishing", "establishment", "estimate", "even", "evened", "evening", "evening's", "evenings", "evens", "event", "event's", "events", "eventually", "ever", "every", "everybody", "everyone", "everything", "everywhere", "evidence", "exact", "exactly", "examine", "examined", "examines", "examining", "example", "example's", "examples", "excellent", "except", "exception", "exception's", "exceptions", "excess", "excessive", "exchange", "exclude", "excluded", "excludes", "excluding", "exclusive", "excuse", "execute", "executed", "executes", "executing", "exercise", "exist", "existed", "existence", "existing", "exists", "expand", "expanded", "expanding", "expands", "expansion", "expect", "expected", "expecting", "expects", "expense", "expensive", "experience", "experience's", "experienced", "experiences", "experiencing", "experiment", "experiment's", "experimental", "experiments", "expert", "expert's", "experts", "explain", "explained", "explaining", "explains", "explanation", "explicit", "express", "expressed", "expresses", "expressing", "expression", "extend", "extended", "extending", "extends", "extension", "extensive", "extent", "external", "extra", "extract", "extreme", "extremely", "eye", "eye's", "eyes", "face", "facilities", "facility", "facility's", "fact", "fact's", "factor", "factor's", "factors", "facts", "fail", "fail's", "failed", "failing", "fails", "failure", "fair", "fairly", "faith", "fall", "fall's", "fallen", "falling", "falls", "false", "familiar", "family", "famous", "fan", "fancy", "far", "farm", "farther", "farthest", "fashion", "fast", "faster", "fastest", "fatal", "fate", "father", "fault", "fault's", "faults", "favor", "favorite", "fear", "feasible", "feature", "feature's", "features", "fed", "federal", "feed", "feedback", "feeding", "feeds", "feel", "feel's", "feeling", "feels", "feet", "fell", "felt", "few", "fewer", "fewest", "field", "field's", "fields", "fight", "figure", "figure's", "figures", "file", "file's", "filed", "files", "filing", "fill", "filled", "filling", "fills", "film", "final", "finally", "financial", "find", "find's", "finding", "finds", "fine", "finger", "finger's", "fingers", "finish", "finished", "finishes", "finishing", "finite", "fire", "firm", "firmly", "first", "firstly", "fiscal", "fish", "fish's", "fishes", "fit", "fit's", "fits", "fitted", "fitting", "five", "fix", "fixed", "fixes", "fixing", "flag", "flash", "flashed", "flashes", "flashing", "flat", "flavor", "flew", "flexible", "flied", "flies", "flight", "float", "floated", "floating", "floats", "floor", "flow", "flown", "fly", "fly's", "flying", "folk", "folk's", "folks", "follow", "followed", "following", "follows", "food", "foot", "foot's", "for", "force", "force's", "forced", "forces", "forcing", "foreign", "forever", "forget", "forgets", "forgetting", "forgot", "forgotten", "form", "form's", "formal", "format", "formed", "former", "forming", "forms", "forth", "forthcoming", "fortunately", "fortune", "forward", "found", "four", "fourth", "fraction", "frame", "free", "freedom", "freely", "french", "frequent", "frequently", "fresh", "friend", "friend's", "friendly", "friends", "fries", "from", "front", "fry", "fry's", "full", "fully", "fun", "function", "function's", "functions", "fund", "fund's", "fundamental", "fundamentally", "funds", "funny", "further", "furthest", "future", "gain", "gained", "gaining", "gains", "game", "game's", "games", "gap", "garbage", "garden", "gas", "gasoline", "gather", "gave", "general", "generally", "generate", "generated", "generates", "generating", "generation", "genuine", "get", "get's", "gets", "getting", "girl", "give", "given", "gives", "giving", "glad", "glass", "global", "go", "go's", "goes", "going", "gone", "good", "goods", "got", "gotten", "government", "governor", "gradually", "graduate", "grand", "grand's", "grands", "grant", "granted", "granting", "grants", "graph", "graphic", "graphic's", "graphics", "grateful", "grave", "gray", "great", "greater", "greatest", "greatly", "green", "grew", "grind", "grinding", "grinds", "gross", "gross's", "grosses", "ground", "ground's", "grounds", "group", "group's", "groups", "grow", "growing", "grown", "grows", "growth", "guarantee", "guaranteed", "guaranteeing", "guarantees", "guard", "guess", "guessed", "guesses", "guessing", "guide", "gun", "guy", "habit", "habit's", "habits", "hack", "had", "hair", "half", "hall", "hand", "hand's", "handed", "handing", "handle", "handled", "handles", "handling", "hands", "handy", "hang", "hanged", "hanging", "hangs", "happen", "happened", "happening", "happens", "happily", "happy", "hard", "harder", "hardest", "hardly", "hardware", "harm", "harmful", "harmless", "has", "hat", "hate", "have", "haven't", "having", "he", "he's", "head", "head's", "headed", "header", "heading", "heads", "health", "healthy", "hear", "heard", "hearing", "hears", "heart", "heat", "heavily", "heavy", "held", "hell", "hello", "help", "helped", "helpful", "helping", "helps", "hence", "her", "here", "hereby", "herself", "hes", "hid", "hidden", "hide", "hides", "hiding", "high", "higher", "highest", "highly", "hill", "him", "himself", "hint", "hint's", "hints", "his", "historical", "history", "hit", "hit's", "hits", "hitting", "hold", "hold's", "holding", "holds", "hole", "hole's", "holes", "holiday", "holiday's", "holidays", "home", "honest", "hope", "hoped", "hopefully", "hopes", "hoping", "horrible", "horse", "horse's", "horses", "hospital", "host", "hot", "hotel", "hour", "hour's", "hours", "house", "how", "however", "huge", "human", "hundred", "hundred's", "hundreds", "hung", "hunt", "hurry", "husband", "ice", "idea", "idea's", "ideal", "ideas", "identical", "identify", "identity", "if", "ignore", "ignored", "ignores", "ignoring", "ill", "illegal", "image", "image's", "images", "imagination", "imagine", "immediate", "immediately", "impact", "implement", "implemented", "implementing", "implements", "implication", "implication's", "implications", "implied", "implies", "imply", "implying", "importance", "important", "importantly", "impose", "imposed", "imposes", "imposing", "impossible", "impression", "improve", "improved", "improvement", "improvement's", "improvements", "improves", "improving", "in", "inability", "inadequate", "inch", "inch's", "inches", "incident", "incidentally", "incline", "inclined", "inclines", "inclining", "include", "included", "includes", "including", "income", "incompatible", "incomplete", "inconsistent", "inconvenience", "incorrect", "increase", "increase's", "increased", "increases", "increasing", "indeed", "independent", "independently", "index", "indicate", "indicates", "indication", "individual", "individual's", "individually", "individuals", "industrial", "industry", "inevitably", "inferior", "infinite", "influence", "info", "inform", "information", "informed", "informing", "informs", "initial", "initial's", "initially", "initials", "inner", "innocent", "input", "inputs", "inputted", "inputting", "insert", "inserted", "inserting", "inserts", "inside", "insist", "insisted", "insisting", "insists", "install", "installed", "installing", "installs", "instance", "instant", "instantly", "instead", "institution", "institution's", "institutions", "instruction", "instruction's", "instructions", "insurance", "integer", "integer's", "integers", "integral", "intelligence", "intelligent", "intend", "intended", "intending", "intends", "intention", "interact", "interest", "interest's", "interested", "interesting", "interests", "interface", "internal", "international", "interpret", "interpretation", "interpreted", "interpreting", "interprets", "interval", "interval's", "intervals", "intervention", "into", "introduce", "introduced", "introduces", "introducing", "introduction", "invalid", "invariably", "invent", "invented", "inventing", "invents", "investigate", "invisible", "invitation", "invite", "invited", "invites", "inviting", "involve", "involved", "involves", "involving", "irrelevant", "irritate", "irritated", "irritates", "irritating", "is", "isn't", "isolate", "isolated", "isolates", "isolating", "issue", "issue's", "issued", "issues", "issuing", "it", "it's", "item", "item's", "items", "its", "itself", "job", "job's", "jobs", "join", "joined", "joining", "joins", "joint", "joke", "joy", "judge", "judgment", "jump", "jump's", "jumps", "junk", "just", "justification", "justified", "justifies", "justify", "justifying", "keen", "keep", "keep's", "keeping", "keeps", "kept", "key", "key's", "keyboard", "keys", "kid", "kill", "killed", "killing", "kills", "kind", "kind's", "kindly", "kinds", "king", "knew", "knock", "knocked", "knocking", "knocks", "know", "knowing", "knowledge", "known", "knows", "label", "label's", "labeled", "labeling", "labels", "labor", "laboratory", "lack", "lacked", "lacking", "lacks", "ladies", "lady", "lady's", "lain", "land", "landed", "landing", "lands", "language", "language's", "languages", "large", "largely", "larger", "largest", "last", "last's", "lasts", "late", "later", "latest", "latter", "law", "law's", "laws", "lay", "layout", "lazy", "leach", "lead", "lead's", "leaded", "leader", "leading", "leads", "leaf", "leaf's", "learn", "learned", "learning", "learns", "least", "leave", "leave's", "leaved", "leaves", "leaving", "lecture", "lecture's", "lectures", "led", "left", "leg", "leg's", "legal", "legally", "legs", "lend", "length", "less", "less's", "lesser", "lesson", "lesson's", "lessons", "let", "let's", "lets", "letter", "letter's", "letters", "letting", "level", "level's", "levels", "liable", "libraries", "library", "library's", "lie", "lie's", "lied", "lies", "life", "life's", "lifetime", "lift", "light", "light's", "lights", "like", "like's", "liked", "likely", "likes", "likewise", "liking", "limit", "limit's", "limited", "limiting", "limits", "line", "line's", "linear", "lines", "link", "link's", "linked", "linking", "links", "list", "list's", "listed", "listen", "listing", "lists", "literally", "literature", "little", "live", "lived", "lives", "living", "load", "loaded", "loading", "loads", "loan", "local", "location", "location's", "locations", "lock", "locked", "locking", "locks", "log", "log's", "logged", "logging", "logic", "logical", "logs", "long", "longer", "longest", "look", "look's", "looked", "looking", "looks", "loop", "loose", "lorries", "lorry", "lorry's", "lose", "loses", "losing", "loss", "lost", "lot", "lot's", "lots", "loudly", "love", "low", "lower", "lowest", "luck", "lucky", "lunch", "lying", "machine", "machine's", "machines", "mad", "made", "magic", "magnetic", "magnitude", "mail", "main", "mainly", "maintain", "maintained", "maintaining", "maintains", "major", "majority", "make", "make's", "makes", "making", "man", "man's", "manage", "managed", "manager", "manages", "managing", "manipulation", "manner", "manual", "manual's", "manuals", "many", "map", "march", "mark", "mark's", "marked", "market", "marking", "marks", "marriage", "marry", "mass", "massive", "master", "match", "match's", "matches", "material", "material's", "materials", "mathematical", "mathematics", "matter", "matter's", "matters", "maximum", "may", "maybe", "me", "mean", "mean's", "meaning", "meaning's", "meaningful", "meaningless", "meanings", "means", "meant", "measure", "measure's", "measured", "measures", "measuring", "mechanic", "mechanic's", "mechanics", "mechanism", "media", "medical", "medium", "medium's", "mediums", "meet", "meet's", "meeting", "meeting's", "meetings", "meets", "member", "member's", "members", "membership", "memory", "men", "mention", "mention's", "mentioned", "mentioning", "mentions", "mere", "merely", "merit", "merit's", "merits", "mess", "message", "message's", "messages", "messy", "met", "metal", "method", "method's", "methods", "middle", "midnight", "might", "mile", "mile's", "miles", "military", "military's", "million", "million's", "millions", "mind", "mind's", "minded", "minding", "minds", "mine", "minimal", "minimum", "minor", "minority", "minute", "minute's", "minutes", "mislead", "misleading", "misleads", "misled", "miss", "missed", "misses", "missing", "mistake", "mistake's", "mistaken", "mistakes", "mistaking", "mistook", "misunderstand", "misunderstanding", "misunderstands", "misunderstood", "misuse", "mix", "mixed", "mixes", "mixing", "mod", "mode", "model", "model's", "models", "modern", "modified", "modifies", "modify", "modifying", "moment", "money", "monitor", "month", "month's", "months", "moral", "more", "morning", "morning's", "mornings", "most", "mostly", "mother", "motion", "mouth", "move", "move's", "moved", "movement", "movement's", "movements", "moves", "movie", "moving", "much", "multiple", "music", "must", "my", "myself", "mysterious", "naive", "name", "name's", "named", "namely", "names", "naming", "nasty", "nation", "national", "natural", "naturally", "nature", "naughty", "near", "nearby", "nearer", "nearest", "nearly", "necessarily", "necessary", "necessity", "neck", "need", "need's", "needed", "needing", "needs", "negative", "neither", "nervous", "net", "network", "network's", "networks", "never", "nevertheless", "new", "news", "next", "nice", "nicer", "nicest", "night", "nine", "no", "nobody", "noise", "noisy", "none", "nonsense", "nor", "normal", "normally", "north", "not", "note", "note's", "noted", "notes", "nothing", "notice", "notice's", "noticed", "notices", "noticing", "notify", "noting", "novel", "now", "nowadays", "nowhere", "numb", "number", "number's", "numbers", "numbest", "numerical", "numerous", "obey", "object", "object's", "objected", "objecting", "objection", "objection's", "objections", "objects", "obscure", "observation", "observe", "observed", "observes", "observing", "obtain", "obtained", "obtaining", "obtains", "obvious", "obviously", "occasion", "occasion's", "occasional", "occasionally", "occasions", "occupied", "occupies", "occupy", "occupying", "occur", "occurred", "occurring", "occurs", "odd", "odds", "of", "off", "offer", "offer's", "offered", "offering", "offers", "office", "office's", "officer", "offices", "official", "often", "oh", "oil", "old", "older", "oldest", "omit", "omits", "omitted", "omitting", "on", "once", "one", "one's", "ones", "only", "onto", "open", "open's", "opened", "opening", "opens", "operate", "operated", "operates", "operating", "operation", "operation's", "operations", "operator", "operator's", "operators", "opinion", "opinion's", "opinions", "opportunities", "opportunity", "opportunity's", "oppose", "opposed", "opposes", "opposing", "opposite", "opposition", "option", "option's", "optional", "options", "or", "order", "order's", "ordered", "ordering", "orders", "ordinary", "organization", "organize", "organized", "organizes", "organizing", "origin", "original", "originally", "other", "others", "otherwise", "ought", "our", "ours", "ourselves", "out", "outer", "output", "outside", "over", "overall", "owe", "owed", "owes", "owing", "own", "owner", "owner's", "owners", "pack", "package", "package's", "packages", "packet", "page", "page's", "pages", "paid", "pain", "painful", "pair", "pair's", "pairs", "paper", "paper's", "papers", "paragraph", "parallel", "parent", "park", "part", "part's", "partial", "partially", "particular", "particularly", "parties", "partly", "parts", "party", "party's", "pass", "passed", "passes", "passing", "past", "patch", "path", "patient", "pattern", "pattern's", "patterns", "pause", "pay", "payed", "paying", "pays", "peace", "peak", "peculiar", "pen", "people", "per", "perfect", "perfectly", "perform", "performance", "performed", "performing", "performs", "perhaps", "period", "permanent", "permanently", "permission", "permit", "permits", "permitted", "permitting", "person", "person's", "personal", "personally", "persons", "persuade", "persuaded", "persuades", "persuading", "petrol", "phase", "phenomenon", "philosophy", "phone", "phrase", "phrase's", "phrases", "physical", "pi", "pick", "picked", "picking", "picks", "picture", "picture's", "pictures", "piece", "piece's", "pieces", "pile", "pint", "pipe", "place", "place's", "placed", "places", "placing", "plain", "plan", "plan's", "plane", "planet", "planned", "planning", "plans", "plant", "plastic", "play", "played", "playing", "plays", "plea", "pleasant", "please", "pleased", "pleases", "pleasing", "plenty", "plot", "plot's", "plots", "plug", "plus", "pocket", "poem", "poet", "point", "point's", "pointed", "pointing", "pointless", "points", "police", "police's", "policies", "policy", "policy's", "political", "poll", "pool", "poor", "pop", "popular", "population", "port", "position", "position's", "positions", "positive", "possibilities", "possibility", "possibility's", "possible", "possibly", "post", "posted", "posting", "postmaster", "posts", "potential", "potentially", "pound", "pound's", "pounds", "power", "power's", "powerful", "powers", "practical", "practically", "practice", "precise", "precisely", "prefer", "preferable", "preferably", "preference", "preferred", "preferring", "prefers", "preparation", "prepare", "prepared", "prepares", "preparing", "presence", "present", "present's", "presented", "presenting", "presents", "preserve", "president", "press", "pressed", "presses", "pressing", "pressure", "presumably", "presume", "pretty", "prevent", "prevented", "preventing", "prevents", "previous", "previously", "price", "price's", "prices", "primary", "prime", "primitive", "principle", "principle's", "principles", "print", "printed", "printer", "printer's", "printers", "printing", "printout", "prints", "prior", "private", "probably", "problem", "problem's", "problems", "procedure", "process", "process's", "processed", "processes", "processing", "processor", "processor's", "processors", "produce", "produced", "produces", "producing", "product", "product's", "production", "products", "professional", "program", "program's", "programmed", "programmer", "programmer's", "programmers", "programming", "programs", "progress", "project", "project's", "projects", "promise", "promised", "promises", "promising", "prompt", "promptly", "prone", "proof", "proper", "properly", "properties", "property", "property's", "proportion", "proposal", "propose", "proposed", "proposes", "proposing", "prospect", "protect", "protected", "protecting", "protection", "protects", "protest", "prove", "proved", "proves", "provide", "provided", "provides", "providing", "proving", "public", "publication", "publicity", "publicly", "publish", "published", "publishes", "publishing", "pull", "pulled", "pulling", "pulls", "punctuation", "puncture", "purchase", "pure", "purely", "purpose", "purpose's", "purposes", "push", "pushed", "pushes", "pushing", "put", "put's", "puts", "putt", "putted", "putting", "putts", "qualified", "qualifies", "qualify", "qualifying", "quality", "quantities", "quantity", "quantity's", "quarter", "question", "question's", "questions", "queue", "quick", "quicker", "quickest", "quickly", "quiet", "quietly", "quit", "quite", "quits", "quitting", "quote", "quote's", "quoted", "quotes", "quoting", "race", "radio", "rain", "raise", "raised", "raises", "raising", "ran", "random", "randomly", "range", "rapid", "rapidly", "rare", "rarely", "rate", "rate's", "rates", "rather", "raw", "re", "reach", "reach's", "reached", "reaches", "reaching", "react", "reaction", "read", "read's", "readable", "reader", "reader's", "readers", "readily", "reading", "reads", "ready", "real", "reality", "realize", "realized", "realizes", "realizing", "really", "reason", "reason's", "reasonable", "reasonably", "reasons", "recall", "receive", "received", "receives", "receiving", "recent", "recently", "reception", "recognition", "recognize", "recognized", "recognizes", "recognizing", "recommend", "recommendation", "recommended", "recommending", "recommends", "record", "record's", "recorded", "recording", "records", "recover", "recovered", "recovering", "recovers", "red", "reduce", "reduced", "reduces", "reducing", "reduction", "redundant", "refer", "reference", "reference's", "references", "referred", "referring", "refers", "reflect", "reflected", "reflecting", "reflection", "reflects", "refuse", "refused", "refuses", "refusing", "regard", "regarded", "regarding", "regardless", "regards", "region", "register", "registered", "registering", "registers", "regret", "regular", "regularly", "regulation", "regulation's", "regulations", "reject", "rejected", "rejecting", "rejects", "relate", "related", "relates", "relating", "relation", "relationship", "relative", "relatively", "release", "release's", "released", "releases", "releasing", "relevance", "relevant", "reliable", "religion", "religious", "religious's", "reluctant", "rely", "remain", "remained", "remaining", "remains", "remark", "remark's", "remarks", "remember", "remembered", "remembering", "remembers", "remind", "reminded", "reminding", "reminds", "remote", "remotely", "removal", "remove", "removed", "removes", "removing", "repair", "repeat", "repeated", "repeatedly", "repeating", "repeats", "replace", "replaced", "replacement", "replaces", "replacing", "replied", "replies", "reply", "reply's", "replying", "report", "report's", "reported", "reporting", "reports", "represent", "representation", "representative", "represented", "representing", "represents", "reproduce", "request", "request's", "requested", "requesting", "requests", "require", "required", "requirement", "requirement's", "requirements", "requires", "requiring", "research", "reserve", "reserved", "reserves", "reserving", "resident", "resolution", "resort", "resource", "resource's", "resources", "respect", "respect's", "respectively", "respects", "respond", "response", "response's", "responses", "responsibility", "responsible", "rest", "restart", "restore", "restored", "restores", "restoring", "restrict", "restricted", "restricting", "restricts", "result", "result's", "resulted", "resulting", "results", "retain", "return", "return's", "returned", "returning", "returns", "reveal", "revealed", "revealing", "reveals", "reverse", "review", "rewrite", "rid", "ridding", "ride", "ridiculous", "rids", "right", "right's", "rights", "ring", "rise", "risk", "river", "road", "role", "roll", "room", "room's", "rooms", "root", "rough", "roughly", "round", "route", "routine", "row", "rubber", "rubbish", "rule", "rule's", "rules", "rumor", "run", "run's", "running", "runs", "rush", "sad", "sadly", "safe", "safely", "safer", "safest", "safety", "said", "saint", "sake", "sale", "sale's", "sales", "same", "sample", "sat", "satisfied", "satisfies", "satisfy", "satisfying", "save", "saved", "saves", "saving", "saw", "say", "saying", "says", "scale", "scan", "scene", "scheme", "school", "school's", "schools", "science", "science's", "sciences", "scientific", "score", "score's", "scores", "scrap", "scratch", "screen", "screen's", "screens", "script", "search", "searched", "searches", "searching", "season", "second", "second's", "secondary", "secondly", "seconds", "secret", "secretary", "section", "section's", "sections", "secure", "security", "see", "see's", "seeing", "seek", "seeking", "seeks", "seem", "seemed", "seeming", "seems", "seen", "sees", "select", "selected", "selecting", "selection", "selects", "self", "sell", "selling", "sells", "seminar", "send", "sending", "sends", "senior", "sense", "sensible", "sensibly", "sensitive", "sent", "sentence", "sentence's", "sentences", "separate", "separately", "sequence", "sequence's", "sequences", "serial", "series", "serious", "seriously", "serve", "serve's", "served", "server", "serves", "service", "service's", "services", "serving", "session", "session's", "sessions", "set", "set's", "sets", "setting", "settle", "settled", "settles", "settling", "seven", "several", "severe", "severely", "sex", "shall", "shame", "shape", "share", "share's", "shared", "shares", "sharing", "sharp", "she", "sheet", "shelf", "shell", "shift", "ship", "shoot", "shop", "shopped", "shopping", "shops", "short", "shortage", "shorter", "shortest", "shortly", "should", "show", "show's", "showed", "showing", "shown", "shows", "shut", "shuts", "shutting", "side", "side's", "sides", "sight", "sign", "sign's", "signal", "signal's", "signals", "signed", "significance", "significant", "significantly", "signing", "signs", "silly", "similar", "similarly", "simple", "simpler", "simplest", "simply", "simultaneous", "simultaneously", "since", "sincerely", "single", "sit", "site", "site's", "sites", "sits", "sitting", "situation", "situation's", "situations", "six", "size", "size's", "sizes", "skill", "skill's", "skills", "sleep", "slight", "slightly", "slip", "slow", "slower", "slowest", "slowly", "small", "smaller", "smallest", "smile", "smooth", "so", "social", "society", "soft", "software", "sold", "solely", "solid", "solution", "solution's", "solutions", "solve", "solved", "solves", "solving", "some", "somebody", "somehow", "someone", "someplace", "something", "sometime", "sometimes", "somewhat", "somewhere", "son", "soon", "sooner", "soonest", "sophisticate", "sophisticated", "sophisticates", "sophisticating", "sorry", "sort", "sort's", "sorted", "sorting", "sorts", "sought", "sound", "sound's", "sounded", "sounding", "sounds", "source", "source's", "sources", "south", "southern", "space", "space's", "spaces", "spare", "speak", "speaker", "speaker's", "speakers", "speaking", "speaks", "special", "specially", "specific", "specifically", "specified", "specifies", "specify", "specifying", "speech", "speed", "spell", "spelled", "spelling", "spells", "spend", "spending", "spends", "spent", "spirit", "spite", "split", "splits", "splitting", "spoke", "spoken", "spot", "spots", "spotted", "spotting", "spread", "spreading", "spreads", "spring", "square", "stable", "staff", "stage", "stage's", "stages", "stand", "stand's", "standard", "standard's", "standards", "standing", "stands", "start", "start's", "started", "starting", "starts", "state", "state's", "stated", "statement", "statement's", "statements", "states", "stating", "station", "station's", "stations", "statistic", "statistic's", "statistical", "statistics", "status", "stay", "stayed", "staying", "stays", "steal", "step", "stick", "sticking", "sticks", "still", "stock", "stone", "stone's", "stones", "stood", "stop", "stop's", "stopped", "stopping", "stops", "storage", "store", "stored", "stores", "storing", "story", "straight", "straightforward", "strange", "strategy", "stream", "street", "strength", "strict", "strictly", "strike", "strike's", "strikes", "striking", "string", "string's", "strings", "strong", "strongly", "struck", "structure", "structure's", "structures", "stuck", "student", "student's", "students", "studied", "studies", "study", "study's", "studying", "stuff", "stupid", "style", "subject", "subject's", "subjects", "submit", "submits", "submitted", "submitting", "subsequent", "subset", "substantial", "substitute", "subtle", "succeed", "success", "successful", "successfully", "such", "sudden", "suddenly", "suffer", "suffered", "suffering", "suffers", "suffice", "sufficient", "sufficiently", "sugar", "suggest", "suggested", "suggesting", "suggestion", "suggestion's", "suggestions", "suggests", "suit", "suitable", "suitably", "suited", "suiting", "suits", "sum", "summary", "summer", "sun", "superior", "supervisor", "supplied", "supplies", "supply", "supply's", "supplying", "support", "supported", "supporting", "supports", "suppose", "supposed", "supposedly", "supposes", "supposing", "sure", "surely", "surface", "surprise", "surprised", "surprises", "surprising", "survey", "survive", "survived", "survives", "surviving", "suspect", "suspected", "suspecting", "suspects", "suspend", "suspended", "suspending", "suspends", "suspicion", "switch", "switch's", "switched", "switches", "switching", "symbol", "symbol's", "symbols", "syntax", "system", "system's", "systems", "table", "table's", "tables", "take", "take's", "taken", "takes", "taking", "talk", "talk's", "talked", "talking", "talks", "tank", "tank's", "tanks", "tape", "tape's", "tapes", "target", "task", "task's", "tasks", "taste", "taught", "tax", "tea", "teach", "teacher", "teaches", "teaching", "team", "technical", "technique", "technique's", "techniques", "technology", "tedious", "teeth", "telephone", "television", "tell", "telling", "tells", "temperature", "temporarily", "temporary", "ten", "tend", "tendency", "tends", "term", "term's", "terminal", "terminal's", "terminals", "terminology", "terms", "terribly", "test", "test's", "tested", "testing", "tests", "text", "than", "thank", "thanks", "that", "that's", "the", "their", "them", "themselves", "then", "theoretical", "theory", "there", "there's", "thereby", "therefore", "these", "they", "they're", "thin", "thing", "thing's", "things", "think", "thinking", "thinks", "third", "this", "thoroughly", "those", "though", "thought", "thought's", "thoughts", "thousand", "thousand's", "thousands", "threat", "three", "threw", "through", "throughout", "throw", "throwing", "thrown", "throws", "thus", "ticket", "ticket's", "tickets", "tie", "tie's", "tied", "ties", "tight", "till", "time", "time's", "timed", "times", "timing", "tin", "title", "title's", "titles", "to", "today", "together", "token", "told", "tomorrow", "tonight", "too", "took", "tooth", "tooth's", "top", "topic", "topic's", "topics", "total", "totally", "touch", "touched", "touches", "touching", "toward", "towards", "town", "trace", "track", "track's", "tracks", "traditional", "traffic", "train", "trained", "training", "trains", "transfer", "transferred", "transferring", "transfers", "translate", "translated", "translates", "translating", "translation", "transport", "trap", "trapped", "trapping", "traps", "trash", "travel", "treat", "treated", "treating", "treatment", "treats", "tree", "tree's", "trees", "trial", "trick", "tried", "tries", "trip", "trivial", "trouble", "truck", "truck's", "trucks", "true", "truly", "trunk", "trust", "trusted", "trusting", "trusts", "truth", "try", "try's", "trying", "tune", "turn", "turn's", "turned", "turning", "turns", "twelve", "twenty", "twice", "two", "tying", "type", "type's", "typed", "types", "typical", "typing", "ugly", "ultimate", "ultimately", "unable", "unacceptable", "unaware", "uncertain", "unclear", "under", "undergraduate", "undergraduate's", "undergraduates", "underneath", "understand", "understanding", "understands", "understood", "unfortunate", "unfortunately", "unhappy", "uniform", "unique", "unit", "unit's", "unite", "units", "universal", "universities", "university", "university's", "unknown", "unless", "unlike", "unlikely", "unlimited", "unnecessarily", "unnecessary", "unpleasant", "unreasonable", "unsuitable", "until", "unusual", "unwanted", "up", "update", "updated", "updates", "updating", "upon", "upper", "upset", "upsets", "upsetting", "upwards", "us", "usage", "use", "use's", "used", "useful", "useless", "user", "user's", "users", "uses", "using", "usual", "usually", "utility", "utterly", "vacation", "vacation's", "vacations", "vague", "vaguely", "valid", "validity", "valuable", "value", "value's", "values", "van", "vans", "variable", "variable's", "variables", "variation", "varied", "varies", "variety", "various", "vary", "varying", "vast", "vastly", "vector", "version", "version's", "versions", "very", "via", "vice", "video", "view", "view's", "views", "virtually", "virtue", "visible", "vision", "visit", "vital", "voice", "volume", "vote", "vote's", "votes", "wait", "waited", "waiting", "waits", "walk", "walked", "walking", "walks", "wall", "wall's", "walls", "want", "want's", "wanted", "wanting", "wants", "war", "warm", "warn", "warned", "warning", "warns", "was", "wash", "wasn't", "waste", "wasted", "wastes", "wasting", "watch", "watched", "watches", "watching", "water", "way", "way's", "ways", "we", "weapon", "wear", "wearing", "wears", "weather", "week", "week's", "weekend", "weeks", "weight", "weird", "welcome", "welcomed", "welcomes", "welcoming", "well", "went", "were", "west", "western", "what", "whatever", "whatsoever", "wheel", "wheel's", "wheels", "when", "whenever", "where", "whereas", "whereby", "wherever", "whether", "which", "while", "whilst", "white", "who", "whoever", "whole", "whom", "whose", "why", "wide", "widely", "wider", "widespread", "widest", "wife", "wild", "will", "willed", "willing", "wills", "win", "wind", "window", "window's", "windows", "wine", "winning", "wins", "winter", "wire", "wise", "wish", "wish's", "wished", "wishes", "wishing", "with", "withdraw", "within", "without", "woman", "woman's", "women", "won", "won's", "won't", "wonder", "wondered", "wonderful", "wondering", "wonders", "wooden", "word", "word's", "worded", "wording", "words", "wore", "work", "work's", "worked", "worker", "worker's", "workers", "working", "works", "world", "worn", "worried", "worries", "worry", "worrying", "worse", "worst", "worth", "worthwhile", "worthy", "would", "wouldn't", "write", "writer", "writes", "writing", "written", "wrong", "wrote", "year", "year's", "years", "yellow", "yes", "yesterday", "yet", "you", "you're", "young", "young's", "your", "yours", "yourself", "zero" };

void TestDictionary()
{
	Trie * trie = new Trie();

	int noOfWords = _countof(words);
	for (int i = 0; i < noOfWords; i++)
	{
		trie->Insert(words[i]);
	}

	_ASSERT(trie->IsWord("able"));
	_ASSERT(trie->IsWord("reference"));
	_ASSERT(trie->IsWord("yourself"));

	list<char *> wordsList = trie->GetAllWords();

	int i = 0;
	for (list<char*>::iterator it = wordsList.begin(); i < noOfWords && it != wordsList.end(); it++, i++)
	{
		_ASSERT(strncmp(*it, words[i], strlen(words[i])) == 0);
	}

	_ASSERT(i == wordsList.size());
}

void DisplayList(_In_ list<char*> * pList)
{
	for (list<char*>::iterator it = pList->begin(); it != pList->end(); it++)
	{
		printf_s("%s,", *it);
	}

	printf_s("\n");
}

void DisplaySuggestions(_In_ Trie * pTrie, _In_ char * pWord)
{
	list<char *> suggestions;

	printf_s("\nInput word: %s\n", pWord);

	printf_s("Suggestions with Edit Distance  = 1 : \n");
	suggestions = pTrie->GetSuggestions(pWord, 1);
	DisplayList(&suggestions);

	printf_s("Suggestions with Edit Distance  = 2 : \n");
	suggestions = pTrie->GetSuggestions(pWord, 2);
	DisplayList(&suggestions);

	printf_s("Suggestions with Edit Distance  = 3 : \n");
	suggestions = pTrie->GetSuggestions(pWord, 3);
	DisplayList(&suggestions);

}


void TestSpellingSuggestionsWithEditDistance()
{
	list<char *> suggestions;

	Trie * pTrie = new Trie();

	int noOfWords = _countof(words);
	for (int i = 0; i < noOfWords; i++)
	{
		pTrie->Insert(words[i]);
	}


	char w1[] = "that";
	DisplaySuggestions(pTrie, w1);
	char w2[] = "soonert";
	DisplaySuggestions(pTrie, w2);
	char w3[] = "htat"; // transposition
	DisplaySuggestions(pTrie, w3);

	char w4[] = "hat"; // insertion 'that' , and substitution 'hit'
	DisplaySuggestions(pTrie, w4);


}

int main()
{
	TestInsert();

	TestDelete();

	TestDictionary();

	TestSpellingSuggestionsWithEditDistance();


	printf_s("All Tests passed!");
	int n;
	cin >> n;

}