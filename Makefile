all: JavaXServ XServ testclients

JavaXServ:
	cd ./JavaXServ && $(MAKE) all

XServ:
	cd ./www/XServ && $(MAKE) all

testclients:
	cd ./www/testClients && $(MAKE) all


