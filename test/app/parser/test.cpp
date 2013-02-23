#include "../../../modules/app/parser/parser.h"
#include "../../../modules/lib/parser/parser.h"
#include "../../input/replies.h"


void testScheduleParser() {
	app::parser::ScheduleParser p;
	app::parser::ScheduleTableBorderParser e;
	lib::parser::Splitter s(relayInfoShow);
	for(int i=0; i<s.count(); ++i) {
		printf("%s\n", s[i]);
		if(e.match(s[i]))
			printf("### SCHEDULE BORDER\n");
		if(p.match(s[i]))
			for(int j=0; j<p.count()-1; ++j)
				printf("[%d] '%s'\n", j, p[j]);
	}
}


void testRelayTourneyListParser() {
	app::parser::RelayTourneyListParser p;
	lib::parser::Splitter s(relayListTourney);
	for(int i=0; i<s.count(); ++i) {
		printf("%s\n", s[i]);
		if(p.match(s[i]))
			for(int j=0; j<p.count(); ++j)
				printf("[%d] '%s'\n", j, p[j]);
	}
}


void testServerDate() {
	app::parser::ServerDate d("2013-02-16", "06:30");
	printf("%d-%d-%d %d:%d\n", d.Y, d.M, d.D, d.h, d.m);
}


void testScheduleParser2() {
	app::parser::ScheduleParser p;
	app::parser::ScheduleTableBorderParser e;
	app::parser::ScheduleTableEndParser f;
	lib::parser::Splitter s(relayInfoShow2);
	for(int i=0; i<s.count(); ++i) {
		//printf("%s\n", s[i]);
		if(e.match(s[i]))
			printf("### SCHEDULE BORDER\n");
		if(f.match(s[i]))
			printf("### END\n");
		if(p.match(s[i])) {
			//for(int j=0; j<p.count()-1; ++j)
			//	printf("[%d] '%s'\n", j, p[j]);
			
			app::parser::ServerDate d;
			d.load(p);
			d.print();
			printf("\n");
			
			app::parser::TourneyId tid;
			tid.load(p);
			tid.print();
			printf("\n");
		}
		else
			printf("%s\n", s[i]);
	}
}


void testRelayTourneyListParser2() {
	app::parser::RelayTourneyListParser p;
	lib::parser::Splitter s(relayListTourney3);
	for(int i=0; i<s.count(); ++i) {
		//printf("%s\n", s[i]);
		if(p.match(s[i])) {
			//for(int j=0; j<p.count(); ++j)
			//	printf("[%d] '%s'\n", j, p[j]);
			
			app::parser::TourneyId tid;
			tid.load(p);
			tid.print();
			printf("\n");
			
			app::parser::TourneyStatus ts;
			ts.load(p);
			ts.print();
			printf("\n\n");
		}
		else
			printf("%s\n", s[i]);
	}
}


int main() {
	//testScheduleParser();
	//testRelayTourneyListParser();
	//testServerDate();
	//testScheduleParser2();
	testRelayTourneyListParser2();
	return 0;
}
