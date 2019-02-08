#include "pch.h"
#include "FileHandler.h"
#include "TreeStructure.h"
#include "Regex.h"
#include "DataStructures.h"
#include "Parser.h"
#include "Strings.h"
#include "BERCoder.h"
#include "BERDecoder.h"
#include "CheckValue.h"
#include "PDUPackage.h"
#include "Exceptions.h"
#include "Network.h"

//#include "../include/net-snmp-config.h"
//#include "../include/net-snmp-includes.h"


void showImportedObjects(vector<DataType> vDataType, vector<Index> vIndex, vector <Choice> vChoice, vector <Sequence> vSequence)
{
	DataType::showDataTypes(vDataType);
	Index::showIndexes(vIndex);
	Choice::showChoices(vChoice);
	Sequence::showSequences(vSequence);
	return;
}

void printOIDTree(Tree OIDTree)
{
	cout << "Drzewo OID" << endl;
	OIDTree.root->printTree("", true);
	return;
}

void findNodeByOID(string oid, Tree OIDTree)
{
	TreeNode * node;
	try
	{
		node = OIDTree.findOID(oid, OIDTree.root);
	}
	catch (Exceptions &e)
	{
		e.message();
		return;
	}
	cout << node->name << endl;
	return;
}

void findNodeByName(string name, Tree OIDTree)
{
	string oid;

	try
	{
		oid = OIDTree.findNodeWord(name, OIDTree.root, "");
	}
	catch (Exceptions &e)
	{
		e.message();
		return;
	}

	if (oid.size() == 0)
	{
		cout << "Brak wezla o podanej nazwie" << endl;
	}
	else
	{
		cout << "OID wezla o podanej nazwie: " << oid << endl;
	}
	return;
}

string encode(BERCoder coder, Tree OIDTree, vector<DataType> vDataType, vector<Index> vIndex, vector <Choice> vChoice, vector <Sequence> vSequence, vector < ObjectTypeSize> vObjectTypeSize)
{
	int encodingType = 0;
	string encodedValue, encodedValue1;

	cout << "Wybierz typ kodowania:" << endl << "1 - kodowanie liscia drzewa" << endl << "2 - kodowanie dowolnej wartosci" << endl;
	cin >> encodingType;

	if (encodingType == 1)
	{
		encodedValue = coder.treeNodeEncoding("", "", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
		cout << endl << "Zakodowana wartosc:" << endl;
		cout << encodedValue << endl;
	}
	else if (encodingType == 2)
	{
		encodedValue1 = "", encodedValue = coder.anyValueEncoding(encodedValue1, false);
		cout << endl << "Zakodowana wartosc:" << endl;
		cout << encodedValue << endl;
	}
	else
	{
		cout << "Podano zly typ" << endl;
	}
	return encodedValue;
}

void decode(string encodedValue, BERDecoder decoder, TreeBER BERTree)
{
	decoder.decode(encodedValue, 0, BERTree, nullptr);
	cout << endl << "Odkodowana wartosc:" << endl;
	BERTree.root->printTree("", true);
	cout << endl;
}

void pduAnalyzePacket(PDUPackage pdu, string packet, TreeBER BERTree)
{
	//"30 2c 02 01 00 04 07 70 72 69 76 61 74 65 a0 1e 02 01 01 02 01 00 02 01 00 30 13 30 11 06 0d 2b 06 01 04 01 94 78 01 02 07 03 02 00 05 00"
	pdu.analyzePacket(packet);
	BERTree.root->printTree("", true);
	return;
}

void pduGenerateResponse(string oid, string value, int requestID, int errorStatus, int errorIndex, PDUPackage pdu)
{
	map<string, string> varBindList;
	//oid "1.3.6.1.2.1.1.1.0"
	//value "05 00"
	varBindList.insert(pair<string, string>(oid, value));
	string responsePacket = pdu.generateResponsePacket(varBindList, requestID, errorStatus, errorIndex, "public");
	cout << "Odpowiedz: " << responsePacket << endl;
	return;
}

void pduGenerateResponseFromPacket(PDUPackage pdu, string packet, Tree OIDTree, vector<DataType> vDataType, vector<Index> vIndex, vector <Choice> vChoice, vector <Sequence> vSequence, vector < ObjectTypeSize> vObjectTypeSize)
{
	//packet "30 26 02 01 00 04 06 70 75 62 6c 69 63 a0 19 02 01 01 02 01 00 02 01 00 30 0e 30 0c 06 08 2b 06 01 02 01 01 01 00 05 00"
	string responsePacket = pdu.packetHandler(packet, OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
	cout << "Odpowiedz: " << responsePacket << endl;
	return;
}

void runServer(Network net)
{
	net.setServer();
}

int main()
{
	Tree OIDTree;
	TreeBER BERTree;
	Parser parser;
	BERCoder coder;
	BERDecoder decoder;
	PDUPackage pduPackage;
	Network net;

	vector <DataType> vDataType;
	vector <Index> vIndex;
	vector <Choice> vChoice;
	vector <Sequence> vSequence;
	vector <ObjectTypeSize> vObjectTypeSize;

	/*
	//---------------------------------------------
	//snmp server
	//---------------------------------------------

	struct snmp_session session, *ss;
	netsnmp_pdu *pdu;
	netsnmp_pdu *response;

	oid anOID[MAX_OID_LEN];
	size_t anOID_len;

	netsnmp_variable_list *vars;
	int status;
	int count = 1;


	 //Initialize the SNMP library

	init_snmp("snmpdemoapp");


	//Initialize a "session" that defines who we're going to talk to

	snmp_sess_init(&session);                   //set up defaults
	session.peername = (char*)"test.net-snmp.org";

	//set up the authentication parameters for talking to the server

	// set the SNMP version number
	session.version = SNMP_VERSION_1;

	// set the SNMPv1 community name used for authentication
	session.community = (u_char*)"demopublic";
	session.community_len = strlen((const char*)session.community);



	 //Open the session

	SOCK_STARTUP;
	ss = snmp_open(&session);                     //establish the session

	if (!ss) {
		snmp_perror("ack");
		exit(2);
	}


	 //Create the PDU for the data for our request.
	 //1) We're going to GET the system.sysDescr.0 node.

	pdu = snmp_pdu_create(SNMP_MSG_GET);

	read_objid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len);
	//get_node("sysDescr.0", anOID, &anOID_len);
	//read_objid("system.sysDescr.0", anOID, &anOID_len);

	snmp_add_null_var(pdu, anOID, anOID_len);


	//Send the Request out.

	status = snmp_synch_response(ss, pdu, &response);


	//Process the response.

	if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {

		 //SUCCESS: Print the result variables


		for (vars = response->variables; vars; vars = vars->next_variable)
			print_variable(vars->name, vars->name_length, vars);

		//manipulate the information ourselves
		for (vars = response->variables; vars; vars = vars->next_variable) {
			int count = 1;
			if (vars->type == ASN_OCTET_STR) {
				char *sp = malloc(1 + vars->val_len);
				memcpy(sp, vars->val.string, vars->val_len);
				sp[vars->val_len] = '\0';
				printf("value #%d is a string: %s\n", count++, sp);
				free(sp);
			}
			else
				printf("value #%d is NOT a string! Ack!\n", count++);
		}
	}
	else {

		//FAILURE: print what went wrong!


		if (status == STAT_SUCCESS)
			fprintf(stderr, "Error in packet\nReason: %s\n",
				snmp_errstring(response->errstat));
		else
			snmp_sess_perror("snmpget", ss);

	}


	 //Clean up:
	 //1) free the response.
	 //2) close the session.

	if (response)
		snmp_free_pdu(response);
	snmp_close(ss);

	//windows32 specific cleanup (is a noop on unix)
	SOCK_CLEANUP;

	//---------------------------------------------------------------------------------------------------
	*/
	try
	{
		//parser.wholeFileParse("MIBS/RFC-1213-MIB.txt", OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
		//string e = encode(coder, OIDTree, vDataType, vIndex, vChoice, vSequence, vObjectTypeSize);
		//decode(e, decoder, BERTree);
		runServer(net);
	}
	catch (Exceptions &e)
	{
		e.message();
		return -1;
	}

	//podawanie wartosci sekwencji w formacie {w1,w2,w3,...,wn}
	//podawanie wartosci object identifier w formacie {1.2.3.4.5}

	return(0);
}